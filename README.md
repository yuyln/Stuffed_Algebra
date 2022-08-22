# Place Holder
## Introduction
This library was made with studying purposes only.
Since I personally avoid using modern C++ stuff, such as `std::vector`, and really like using old C stuff, aka `malloc`,
`free`, etc, I decided to do my implementation of `my::Vec` (which is conceptually different from `std::vector`), and
some other things (`my::Array`, `my::Matrix`).

## Examples
There is a file with a full set of examples using all structs here. Compile with:
```console
$ g++ -o examples examples.cpp
```
It`s just that, there is no weird stuff on compilation.

## Specifications
## `my::Vec`
`my::Vec` is a templated struct, defined as `my::Vec<std::size_t dimensions, data type>`. From here becomes clear that
this is totally different from `std::vector`, since it have a fixed length (aka dimensions). This vector is more related
to the physical vector, which has a direction and length. However, some operations implementated here doesn't exists in
real vector math.
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
```c++
vector.dim(): returns the vector dimension.
vector.forEach(function(index, element)): change each element to the functions returning value. Returns the vector itself.
```

### Vector Expressions
`my::Vec` uses expression templates to accept an easier typing without any overhead. The following expressions and their definitions are available:
```c++
vector_1+vector_2: the trivial sum of two vector, sum each element of vector_1 with the respective element on vector_2.
vector_1 - vector_2: same thing as the sum, but with subtraction.
vector_1 / vector_2: divides each element of vector_1 by the respective element on vector_2.
vector_1 * vector_2: multiples element wise the vector.
vector_1 ^ vector_2: returns the dot product of vectors: vector_1[0] * vector_2[0] + vector_1[1] * vector_2[1]+....
scalar * vector: multiples each element of vector by scalar.
vector / scalar: divides each element of vector by scalar.
scalar / vector: divides scalar by each element of vector.
```
  
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
`my::Array` is defined to be similar to `my::Vec`, but with changing size. It is templated with the data type
`my::Array<type>`. Here you can insert and remove elements from the array, changing its corresponding size.
For low overhead by calling `realloc`, this structs caches its elements on an array.
Every time an element is inserted, the struct checks if inserting an element will be off the allocated memory
region, if yes, then it allocates a new memory block region adding the size of 64 elements on the cached array.
### Constructors
```c++
Array()
Array(std::size_t n)
Array(std::size_t n, function(index))
Array(const Array &other)
Array(Array &&other)
Array(const ArrayExpression &expr)
```
The first one creates and empty array, but initializes the cached region.
The second zero initilize n elements of the array, also, sets the cached region to be n size.
Third initilize n elements of the array using the passed function.
Next two are the usual copy and move constructors.
Last is the constructor using `ArrayExpression`.

### Functions
The following functions are available:
```c++
array.len(): returns the array length (aka number of elements inserted).
array.insert(obj): inserts obj into array. Returns the array itself.
array.pop(): removes the last element inserted. Returns this element.
array.forEach(function(index, element)): change each array element following the function. Returns the array itself.
```

### Array Expressions
The same way vector have templated expressions, arrays also have it. It has the exact same operations.
```c++
array_1 + array_2: sum of two arrays element wise.
array_1 - array_2: subtraction of two arrays element wise.
array_1 / array_2: dividions of two arrays element wise.
array_1 * array_2: multiplication of two arrays element wise.
array_1 ^ array_2: dot product between array_1 & array_2, defined as array_1[0] * array_2[0] + array_1[1] * array_2[1]+...
scalar * array: multiplies each element of array by scalar.
array / scalar: divides each element of array by scalar.
scalar / array: divides scalar by each element of array.
```

## `my::Matrix`
## `my::Complex`