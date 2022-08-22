#ifdef NO_ASSERTIONS
#define NO_VEC_ASSERTION
#define NO_ARRAY_ASSERTION
#define NO_MATRIX_ASSERTION
#endif

#include <iostream>
#include <fstream>
#include "all.hpp"

//g++ -o examples examples.cpp

void exampleVecs()
{
    my::Vec<4, double> v1(1.0, 2.0, 3.0, 4.0);
    my::Vec<4, double> v2(5.0, 6.0, 7.0, 8.0);
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Vector example: " << std::endl << std::endl;

    std::cout << "Vector 1 (v1): " << v1 << std::endl;
    std::cout << "Vector 2 (v2): " << v2 << std::endl << std::endl;
    std::cout << "v1 + v2 = " << v1 + v2 << std::endl;
    std::cout << "v1 - v2 = " << v1 - v2 << std::endl;
    std::cout << "v1 * v2 = " << v1 * v2 << std::endl;
    std::cout << "v1 / v2 = " << v1 / v2 << std::endl;
    std::cout << "v1 ^ v2 = " << (v1 ^ v2) << std::endl;
    std::cout << "2.2 * v1 = " << 2.2 * v1 << std::endl;
    std::cout << "v1 / 1.5 = " << v1 / 1.5 << std::endl;
    std::cout << "1.4 / v1 = " << 1.4 / v1 << std::endl;
    std::cout << "-v1 = " << -v1 << std::endl;
    std::cout << "----------------------------------" << std::endl << std::endl;

}

void exampleArrays()
{
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Array example: " << std::endl << std::endl;
    my::Array<double> array1, array2;
    std::cout << "Length at instantiation: " << array1.len() << std::endl;
    std::cout << "Inserting elements: " << std::endl;
    std::cout << "array1.insert(1.0): " << array1.insert(1.0) << std::endl;
    std::cout << "array1.insert(2.0): " << array1.insert(2.0) << std::endl;
    std::cout << "array1.insert(5.0): " << array1.insert(5.0) << std::endl;
    std::cout << "Final Length: " << array1.len() << std::endl;
    array2.insert(2.0); array2.insert(3.0); array2.insert(3.0);
    std::cout << "array1: " << array1 << std::endl;
    std::cout << "array2: " << array2 << std::endl;

    std::cout << "array1 + array2: " << array1 + array2 << std::endl;
    std::cout << "array1 - array2: " << array1 - array2 << std::endl;
    std::cout << "array1 * array2: " << array1 * array2 << std::endl;
    std::cout << "array1 / array2: " << array1 / array2 << std::endl;
    std::cout << "2.3 * array1: " << 2.3 * array1 << std::endl;
    std::cout << "array1 / 2.0: " << array1 / 2.0 << std::endl;
    std::cout << "2.0 / array1: " << 2.0 / array1 << std::endl;
    std::cout << "-array1: " << -array1 << std::endl;
    std::cout << "array1.pop(): " << array1.pop() << std::endl;
    std::cout << "array1 after pop(): " << array1 << std::endl;
    std::cout << "array1.forEach([](std::size_t i, double elem) { return elem * elem }): " 
              << array1.forEach([](std::size_t, double elem) { return elem * elem; }) 
              << std::endl;
    std::cout << "----------------------------------" << std::endl << std::endl;

}

void exampleMatrix()
{
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Matrix example: " << std::endl << std::endl;
    my::Matrix<double> m1(2, 2, [](std::size_t i, std::size_t j) { return (double)(i != j) + 1; }),
                       m2(2, 2, [](std::size_t i, std::size_t j) { return i + 2 * j + 1; });
    std::cout << "Matrix m1: " << std::endl;
    std::cout << m1 << std::endl << std::endl;
    std::cout << "Matrix m2: " << std::endl;
    std::cout << m2 << std::endl << std::endl;

    std::cout << "m1 + m2: " << std::endl;
    std::cout << m1 + m2 << std::endl << std::endl;

    std::cout << "m1 - m2: " << std::endl;
    std::cout << m1 - m2 << std::endl << std::endl;

    std::cout << "m1 * m2: " << std::endl;
    std::cout << m1 * m2 << std::endl << std::endl;

    std::cout << "m1 & m2: " << std::endl;
    std::cout << (m1 & m2) << std::endl << std::endl;

    std::cout << "m1 / m2: " << std::endl;
    std::cout << m1 / m2 << std::endl << std::endl;

    std::cout << "m1 % m2: " << std::endl;
    std::cout << m1 % m2 << std::endl << std::endl;

    std::cout << "m1 * 2.0: " << std::endl;
    std::cout << m1 * 2.0 << std::endl << std::endl;

    std::cout << "m1 / 2.0: " << std::endl;
    std::cout << m1 / 2.0 << std::endl << std::endl;

    std::cout << "2.0 / m1: " << std::endl;
    std::cout << 2.0 / m1 << std::endl << std::endl;

    std::cout << "m1 + 1.0: " << std::endl;
    std::cout << m1 + 1.0 << std::endl << std::endl;

    std::cout << "m1 - 1.0: " << std::endl;
    std::cout << m1 - 1.0 << std::endl << std::endl;

    std::cout << "m1.insertRow(my::Vec<2, double>(1.0, 2.0)): " << std::endl;
    std::cout << m1.insertRow(my::Vec<2, double>(1.0, 2.0)) << std::endl << std::endl;

    std::cout << "m1.insertCol(my::Vec<3, double>(1.0, 3.0, 4.0)): " << std::endl;
    std::cout << m1.insertCol(my::Vec<3, double>(1.0, 3.0, 4.0)) << std::endl << std::endl;

    std::cout << "m1.expand(5, 5): " << std::endl;
    std::cout << m1.expand(5, 5) << std::endl << std::endl;

    std::cout << "!m1: " << std::endl;
    std::cout << !m1 << std::endl << std::endl;

    std::cout << "-m1: " << std::endl;
    std::cout << -m1 << std::endl << std::endl;

    std::cout << "m1.forEach([](std::size_t i, std::size_t j, double elem) { return i == j; }): " << std::endl;
    std::cout << m1.forEach([](std::size_t i, std::size_t j, double ) { return i == j; }) << std::endl << std::endl;
}

int main()
{
    exampleVecs();
    exampleArrays();
    exampleMatrix();

    return 0;
}
