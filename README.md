# Place Holder
## Introduction
This library was made with studying purposes only.
Since I personally avoid using modern C++ stuff, such as `std::vector`, and really like using old C stuff, aka `malloc`, `free`, etc, I decided to do my implementation of `my::Vec` (which is conceptually different from `std::vector`), and some other things (`my::Array`, `my::Matrix`).

## Examples
There is a file with a full set of examples using all structs here. Compile with:
```console
$ g++ -o examples examples.cpp
```
It`s just that, there is no weird stuff on compilation.

## Specifications
## `my::Vec`
`my::Vec` is a templated struct, defined as `my::Vec<std::size_t dimensions, data type>`. From here becomes clear that this is totally different from `std::vector`, since it have a fixed length (aka dimensions). This vector is more related to the physical vector, which has a direction and length. However, some operations implementated here doesn't exists in real vector math.
### Constructors
`my::Vec` takes 5 constructors:
```c++
Vec()
Vec(type i0, type i1, type i2, ...)
Vec(const Vec& other)
Vec(Vec&& other)
Vec(const VecExpression& expr)
```
  
The first one will zero initialize a vector with the specified dimensions.
The second creates the vector and their elements are initialized by the arguments.
Third and fourth are the ordinary copy and move constructors.
Last takes an vector expression and converts it to a vector.
### Functions
The following functions are available:
- `vector.dim()`: returns the vector dimension.
- `vector.forEach(function(std::size_t index, type element))`: change each element to the functions returning value.

### Vector Expressions
`my::Vec` uses expression templates to accept an easier typing without any overhead. The following expressions and their definitions are available:
- `vector_1+vector_2`: the trivial sum of two vector, sum each element of `vector_1` with the respective element on `vector_2`.
- `vector_1 - vector_2`: same thing as the sum, but with subtraction.
- `vector_1 / vector_2`: divides each element of `vector_1` by the respective element on `vector_2`.
- `vector_1 * vector_2`: multiples element wise the vector.
- `scalar * vector`: multiples each element of `vector` by `scalar`.
- `vector / scalar`: divides each element of `vector` by `scalar`.
- `scalar / vector`: divides scalar by each element of `vector`.
- `vector_1 ^ vector_2`: returns the dot product of vectors: `vector_1[0] * vector_2[0] + vector_1[1] * vector_2[1]+...`.
  
### Common types
Usually vector with a similar struct have well known uses. So, for simplicity some shorther definitions are available: `my::Vec2d`, `my::Vec2f`, `my::Vec2i`, `my::Vec3d`, `my::Vec3f`, `my::Vec3i`, which have the following definitions:
```c++
my::Vec2d = my::Vec<2, double>
my::Vec2f = my::Vec<2, float>
my::Vec2i = my::Vec<2, int>
my::Vec3d = my::Vec<3, double>
my::Vec3f = my::Vec<3, float>
my::Vec3i = my::Vec<3, int>
```

## `my::Array`