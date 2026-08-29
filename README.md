*This project has been created as part of the 42 curriculum by kong.*

## Description
### The Goal

**CPP02** introduces class-based **operator overloading** through a `Fixed` class that implements **fixed-point arithmetic** — representing real numbers using only integers, the way older CPUs and embedded systems without a floating-point unit (FPU) do decimal math. The last exercise builds a small `Point`/triangle geometry example on top of `Fixed` to put the operators to real use.

The module is split into four progressive exercises:

| Exercise | Adds |
|---|---|
| `ex00` | The `Fixed` class skeleton — orthodox canonical form, raw-bits accessors |
| `ex01` | Int/float constructors, `toInt()`/`toFloat()`, stream insertion (`<<`) |
| `ex02` | Comparison operators, arithmetic operators, `min`/`max`, increment/decrement |
| `ex03` | A `Point` class (immutable `Fixed` coordinates) and a point-in-triangle test built on it |

By the end, `Fixed` behaves like a numeric type: it can be constructed from an `int` or `float`, compared, added/subtracted/multiplied/divided, incremented, and printed — all while internally storing just a single `int`. `Point` then shows what an *immutable* value type built on top of it looks like.

## Instructions
### Compilation

Each exercise has its own `Makefile`. From inside an exercise directory:

```bash
cd ex02
make
```

This produces the executable:

```bash
./fixed
```

Other targets:

```bash
make clean   # remove object files
make fclean  # remove object files and the executable
make re      # fclean + all
```

### Execution

```bash
./fixed
```

`main.cpp` in each exercise runs a set of demonstrations and prints the result to stdout — no arguments needed.

## Technical Choices

### Fixed-point representation

- **Fixed-point number** — a way to represent fractional values using only an integer, by reserving some of its bits for the fractional part instead of dedicating all of them to the whole number. Contrast with **floating-point** (`float`/`double`), where the position of the "point" moves depending on an exponent; in fixed-point, the point's position is constant — hence the name.
- **Scale factor** — the number you multiply a real value by before storing it as an integer, and divide by to read it back. `Fixed` uses `2^frac` where `frac = 8`, so:
  ```cpp
  fp = round(real_value * 256);   // storing
  real_value = fp / 256.0f;       // reading back
  ```
- **Q notation** — the standard naming convention for this scheme, written `Qm.n` (`m` integer bits, `n` fractional bits). `Fixed` is a **Q23.8** format: 1 sign bit + 23 integer bits + 8 fractional bits, packed into a 32-bit `int`.
- **Resolution** — the smallest step between two representable values, here `1/256 ≈ 0.0039`. Anything finer than that gets rounded to the nearest representable step at construction time — a real value like `1.01` becomes whichever of `1.0078125` (`258/256`) or `1.01171875` (`259/256`) is closer, and from then on is stored *exactly* as that value, with no further precision loss.

  | fp (raw) | fp / 256 |
  |---|---|
  | 0 | 0.00000000 |
  | 1 | 0.00390625 |
  | 2 | 0.00781250 |
  | ... | ... |
  | 255 | 0.99609375 |

- **Static storage duration** — `frac` is declared `static const int` and defined once in `Fixed.cpp` (`const int Fixed::frac = 8;`). This line runs **exactly once**, before `main()` even starts — it is not re-run per object, and creating any number of `Fixed` instances never touches it again. This is what lets every instance share one copy of `frac` without it taking up space inside each individual object.
- **Integer division truncation** — `toFloat()` must cast to `float` *before* dividing, or the fractional part is destroyed before it has a chance to exist:
  ```cpp
  float Fixed::toFloat( void ) const
  {
      return (static_cast<float>(fp) / (1 << frac));   // cast first
      // return (fp / (1 << frac));   // bug: int / int truncates, then converts to float
  }
  ```

### Why multiplication and division need extra scaling, but addition/subtraction don't

The core gotcha of this project: every stored `fp` is already scaled by `256`. Adding two of them is fine as-is — the scale factors just add linearly, same as `550 cents + 200 cents = 750 cents`. But **multiplying** two scaled integers multiplies their scale factors too:

```
(5.50 × 100) × (2.00 × 100) = 5.50 × 2.00 × 100²
```

so the raw product is scaled by `100²`, not `100` — you have to divide out one factor of the scale to get back to a sane result. **Division** has the opposite problem: dividing two equally-scaled values cancels the scale out entirely, undershooting by a factor of `100`, so the numerator needs to be pre-multiplied by the scale before dividing. The simplest way to sidestep this bookkeeping is to convert to `float`, do the arithmetic there, then let the existing float constructor rescale on the way back in:

```cpp
Fixed Fixed::operator*(const Fixed& other) const
{
    float temp = this->toFloat() * other.toFloat();
    return (Fixed(temp));   // the float constructor already knows how to rescale
}
```

### Reference parameters and dangling references

The subject specifies `min`/`max` must take **references**, not copies — this isn't a style preference, it's required for correctness. A first attempt using by-value parameters fails two different ways:

```cpp
static Fixed& min(Fixed a, Fixed b)         // by value
{
    if (a < b)
        return (a);   // dangling reference — a is a local copy!
    return (b);
}

static Fixed& min(const Fixed a, const Fixed b)   // "overload" attempt
{ ... }
```

- **Dangling reference** — `a` and `b` are local copies that are destroyed the instant the function returns. Returning `Fixed&` bound to one of them hands the caller a reference to memory that no longer exists — undefined behavior, even though it may appear to "work" by accident.
- **Redefinition, not overloading** — top-level `const` on a *by-value* parameter (`const Fixed a` vs `Fixed a`) is not part of a function's signature for overload-resolution purposes, since the caller can't observe it from outside. The compiler treats the two declarations above as the exact same function and rejects the second as a duplicate definition.

The fix is reference parameters — now `a`/`b` genuinely alias the caller's own objects (which outlive the call), so returning a reference to either is safe, and reference-ness/constness on a reference parameter *does* participate in overload resolution, making the const/non-const pair legitimate overloads:

```cpp
static Fixed&       min(Fixed& a, Fixed& b);
static const Fixed& min(const Fixed& a, const Fixed& b);
```

### Const-correctness

- **`const` member function** — a method that promises not to modify `*this`, declared by writing `const` *after* the parameter list (`bool operator<(const Fixed& other) const;`). This is a compile-time contract, not a body-content check — the compiler enforces it purely from the signature.
- **Why it matters here** — a `const` object can only call `const`-qualified methods. Since several exercises declare `const Fixed` variables, every method that doesn't need to mutate the object (all comparisons, all arithmetic, `toFloat`/`toInt`/`getRawBits`) must be marked `const`, or code like this fails to compile:
  ```cpp
  const Fixed a(1.0f), b(2.0f);
  a < b;   // needs `operator<` to be const-qualified
  ```
- **Why `min`/`max` need both a const and non-const overload** — the constness of what you get back should match the constness of what you passed in. A single `const Fixed&` version would compile for everything, but would also force a `const` result even when both inputs were mutable, permanently blocking any further modification through the returned reference. Providing both lets the compiler pick automatically:
  ```cpp
  static Fixed&       min(Fixed& a, Fixed& b);
  static const Fixed& min(const Fixed& a, const Fixed& b);
  ```

### The stream insertion operator (`operator<<`)

`std::cout` is just an object — an instance of `std::ostream` — and `<<` is an ordinary overloaded operator that the standard library repurposed from "bitwise left shift" to mean "insert into this stream." To make `std::cout << myFixed` compile, `Fixed` needs to teach the compiler what that means.

This **cannot be a member function** of `Fixed`: for `a << b` as a member call, `a` would need to be the owning class's type, but here the left operand is `std::ostream` (which `Fixed` doesn't own). It must instead be a free function taking the stream first:

```cpp
std::ostream& operator<<(std::ostream& out, const Fixed& value)
{
    out << value.toFloat();
    return (out);   // returned so `std::cout << a << b;` can chain
}
```

Declared outside the class body (but still in `Fixed.hpp`), defined without the `Fixed::` qualifier in `Fixed.cpp`.

### Orthodox canonical form and delegation

Every `Fixed` implements the four canonical functions: default constructor, copy constructor, copy assignment operator, destructor. The copy constructor delegates to `operator=`:

```cpp
Fixed::Fixed(const Fixed& other)
{
	// from copy ctor
    *this = other;  // this activates assignment ctor
}
```

This is a deliberate, common pattern to avoid duplicating the copy logic — but it comes with a real caveat for classes managing actual resources (pointers, file handles, etc.): `operator=` typically assumes `*this` is already validly constructed (e.g. it may `delete` an old buffer before allocating a new one). If the constructor delegates to it, `operator=` runs on a not-yet-initialized object. `Fixed` only holds a plain `int`, so this specific class is safe either way — but it's why the more general C++ idiom goes the *other* direction (implement `operator=` via copy-and-swap, never construct-via-assign) once a class owns a resource.

### Increment/decrement operators

```cpp
Fixed& operator++();       // pre-increment:  ++a
Fixed  operator++(int);    // post-increment: a++
Fixed& operator--();       // pre-decrement:  --a
Fixed  operator--(int);    // post-decrement: a--
```

- The unused `int` parameter on the postfix versions exists purely so the compiler can distinguish "prefix" from "postfix" overloads — there's no other syntactic way to tell them apart, and the argument is never actually supplied by the caller (the compiler silently passes `0`).
- **Prefix** mutates first, then returns a reference to the now-updated object — cheap, no copy needed.
- **Postfix** must snapshot the value *before* mutating, then return that snapshot **by value** (never by reference — the snapshot is a local variable, and returning a reference to it would dangle the moment the function exits):
  ```cpp
  Fixed Fixed::operator++(int)
  {
      Fixed old(*this);
      fp += 1;
      return (old);
  }
  ```

### Divide by zero

`Fixed::operator/` converts both operands to `float` and divides. IEEE 754 float division by zero is well-defined (it produces `+inf`, `-inf`, or `NaN` — no crash), but the *result* then gets fed into the float constructor, which converts it back to `int`:

```cpp
fp = roundf(n * (1 << frac));   // n here can be inf or NaN
```

Converting an `inf`/`NaN` float to `int` is undefined behavior in C++. In practice, on x86, this silently produces `INT_MIN` rather than crashing — a good reminder that "no error thrown" doesn't mean "no bug."

The subject explicitly says *"if you ever do a division by 0, it is acceptable that the program crashes"* — the operative word being **acceptable**, not *guaranteed*. Relying on the UB above isn't a real crash, it's an unspecified outcome that happens to look like one on this platform; a different compiler or optimization level could do something else entirely. A deliberate, portable way to satisfy the spec is an explicit check that forces a controlled crash:

```cpp
#include <cassert>

Fixed Fixed::operator/(const Fixed& other) const
{
    assert(other.getRawBits() != 0 && "Fixed: division by zero");
    float temp = this->toFloat() / other.toFloat();
    return (Fixed(temp));
}
```

`assert` prints the failing condition, file, and line, then calls `abort()` — deterministic and debuggable, instead of leaving the outcome to chance. (Pure-integer division would also crash naturally, since integer division by zero is a hardware trap on essentially every CPU — unlike float division, which has defined bit patterns for `inf`/`NaN` and simply computes a wrong-but-valid answer instead of faulting.)

### Const data members and why `operator=` becomes a no-op

`ex03`'s `Point` stores its coordinates as `Fixed const x, y` — once a `Point` is built, it can never move. This has real consequences for the canonical form:

- **Copy constructor** — a `const` member can only ever be given a value *once*, at construction, through the **member initializer list**. Assigning to it in the constructor's body fails to compile, because by then the (implicit) construction phase has already finished and `x`/`y` are already fixed:
  ```cpp
  Point::Point(const Point& other)
  {
      this->x = other.x;   // fails: x is already const by this point
  }

  Point::Point(const Point& other) : x(other.x), y(other.y) {}   // correct
  ```
- **Copy assignment operator** — this one is more fundamental than a syntax fix. `operator=` runs on an object that's *already fully constructed*; there is no initializer-list phase available at that point, ever. So there is genuinely no way to write a `Point::operator=` that changes `x` or `y` — not a workaround problem, a logical one. (If you don't write one yourself, the compiler's implicitly-generated copy assignment operator is deleted for exactly this reason.) The subject still requires the overload to exist, so the correct implementation is an honest no-op:
  ```cpp
  Point& Point::operator=(const Point& other)
  {
      (void)other;
      return (*this);   // nothing CAN change — x and y are immutable for life
  }
  ```
  This isn't a placeholder to fill in later — it's the intended, complete answer: a `Point`'s coordinates are meant to never move once set, and `operator=` existing at all is purely to satisfy the canonical-form requirement.

### The 2D cross product and the point-in-triangle test

- **2D cross product** — for two vectors `u = (ux, uy)` and `v = (vx, vy)`, `cross(u, v) = ux·vy − uy·vx`. Geometrically, its *sign* tells you which side of `u`'s direction `v` points to (positive = counter-clockwise turn, negative = clockwise), which is exactly what's needed to ask "which side of this edge is this point on?"
- **Same-side test (`bsp`)** — walk the triangle's three edges in order (`a→b`, `b→c`, `c→a`), and for each edge, cross the edge's direction against the direction from that edge's start point to the query point:
  ```cpp
  Fixed z1 = cross(b - a, point - a);
  Fixed z2 = cross(c - b, point - b);
  Fixed z3 = cross(a - c, point - c);
  ```
  A point is **inside** the triangle exactly when `z1`, `z2`, `z3` all share the same strict sign (all positive, or all negative) — meaning it's consistently on the same side of every edge:

  | Condition | Meaning |
  |---|---|
  | all three `> 0` | inside, walking the triangle counter-clockwise |
  | all three `< 0` | inside, walking the triangle clockwise |
  | mixed signs | outside |
  | any one exactly `0` | on that edge's line — **not** counted as inside |

  Using strict `<`/`>` (never `<=`/`>=`) is what makes the boundary case work correctly: if a point sits exactly on an edge or on a vertex, at least one `z` is exactly `0`, so it can satisfy neither "all positive" nor "all negative" — the function naturally reports it as outside, matching the subject's requirement that edge points don't count as inside, with no special-case code needed.

## Resources
### Documentation & References
- [cppreference — Operator overloading](https://en.cppreference.com/w/cpp/language/operators)
- [Wikipedia — Q (number format)](https://en.wikipedia.org/wiki/Q_(number_format))
- [Wikipedia — Fixed-point arithmetic](https://en.wikipedia.org/wiki/Fixed-point_arithmetic)
- 42's own `en.subject.pdf` for this module
