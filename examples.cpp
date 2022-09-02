#ifdef NO_ASSERTIONS
#define NO_VEC_ASSERTION
#define NO_ARRAY_ASSERTION
#define NO_MATRIX_ASSERTION
#endif

#include <iostream>
#include <fstream>
#include "all.hpp"

//g++ -o examples examples.cpp

void exampleVecs(std::ostream &out=std::cout)
{
    stuff::Vec<4, double> v1(1.0, 2.0, 3.0, 4.0);
    stuff::Vec<4, double> v2(5.0, 6.0, 7.0, 8.0);
    out << "----------------------------------" << std::endl;
    out << "Vector example: " << std::endl << std::endl;

    out << "Vector 1 (v1): " << v1 << std::endl;
    out << "Vector 2 (v2): " << v2 << std::endl << std::endl;
    out << "v1 + v2 = " << v1 + v2 << std::endl;
    out << "v1 - v2 = " << v1 - v2 << std::endl;
    out << "v1 * v2 = " << v1 * v2 << std::endl;
    out << "v1 / v2 = " << v1 / v2 << std::endl;
    out << "v1 ^ v2 = " << (v1 ^ v2) << std::endl;
    out << "2.2 * v1 = " << 2.2 * v1 << std::endl;
    out << "v1 / 1.5 = " << v1 / 1.5 << std::endl;
    out << "1.4 / v1 = " << 1.4 / v1 << std::endl;
    out << "-v1 = " << -v1 << std::endl;
    out << "----------------------------------" << std::endl;

}

void exampleArrays(std::ostream &out=std::cout)
{
    out << "----------------------------------" << std::endl;
    out << "Array example: " << std::endl << std::endl;
    stuff::Array<double> array1, array2;
    out << "Length at instantiation: " << array1.len() << std::endl;
    out << "Inserting elements: " << std::endl;
    out << "array1.insert(1.0): " << array1.insert(1.0) << std::endl;
    out << "array1.insert(2.0): " << array1.insert(2.0) << std::endl;
    out << "array1.insert(5.0): " << array1.insert(5.0) << std::endl;
    out << "Final Length: " << array1.len() << std::endl;
    array2.insert(2.0); array2.insert(3.0); array2.insert(3.0);
    out << "array1: " << array1 << std::endl;
    out << "array2: " << array2 << std::endl;

    out << "array1 + array2: " << array1 + array2 << std::endl;
    out << "array1 - array2: " << array1 - array2 << std::endl;
    out << "array1 * array2: " << array1 * array2 << std::endl;
    out << "array1 / array2: " << array1 / array2 << std::endl;
    out << "array1 ^ array2: " << (array1 ^ array2) << std::endl;
    out << "2.3 * array1: " << 2.3 * array1 << std::endl;
    out << "array1 / 2.0: " << array1 / 2.0 << std::endl;
    out << "2.0 / array1: " << 2.0 / array1 << std::endl;
    out << "-array1: " << -array1 << std::endl;
    out << "array1.pop(): " << array1.pop() << std::endl;
    out << "array1 after pop(): " << array1 << std::endl;
    out << "array1.forEach([](std::size_t i, double elem) { return elem * elem }): " 
              << array1.forEach([](std::size_t, double elem) { return elem * elem; }) 
              << std::endl;
    out << "----------------------------------" << std::endl;

}

void exampleMatrix(std::ostream &out=std::cout)
{
    out << "----------------------------------" << std::endl;
    out << "Matrix example: " << std::endl << std::endl;
    stuff::Matrix<double> m1(2, 2, [](std::size_t i, std::size_t j) { return (double)(i != j) + 1; }),
                       m2(2, 2, [](std::size_t i, std::size_t j) { return i + 2 * j + 1; });
    out << "Matrix m1: " << std::endl;
    out << m1 << std::endl << std::endl;
    out << "Matrix m2: " << std::endl;
    out << m2 << std::endl << std::endl;

    out << "m1 + m2: " << std::endl;
    out << m1 + m2 << std::endl << std::endl;

    out << "m1 - m2: " << std::endl;
    out << m1 - m2 << std::endl << std::endl;

    out << "m1 * m2: " << std::endl;
    out << m1 * m2 << std::endl << std::endl;

    out << "m1 & m2: " << std::endl;
    out << (m1 & m2) << std::endl << std::endl;

    out << "m1 / m2: " << std::endl;
    out << m1 / m2 << std::endl << std::endl;

    out << "m1 % m2: " << std::endl;
    out << m1 % m2 << std::endl << std::endl;

    out << "m1 * 2.0: " << std::endl;
    out << m1 * 2.0 << std::endl << std::endl;

    out << "m1 / 2.0: " << std::endl;
    out << m1 / 2.0 << std::endl << std::endl;

    out << "2.0 / m1: " << std::endl;
    out << 2.0 / m1 << std::endl << std::endl;

    out << "m1 + 1.0: " << std::endl;
    out << m1 + 1.0 << std::endl << std::endl;

    out << "m1 - 1.0: " << std::endl;
    out << m1 - 1.0 << std::endl << std::endl;

    out << "m1.insertRow(stuff::Vec<2, double>(1.0, 2.0)): " << std::endl;
    out << m1.insertRow(stuff::Vec<2, double>(1.0, 2.0)) << std::endl << std::endl;

    out << "m1.insertCol(stuff::Vec<3, double>(1.0, 3.0, 4.0)): " << std::endl;
    out << m1.insertCol(stuff::Vec<3, double>(1.0, 3.0, 4.0)) << std::endl << std::endl;

    out << "m1.expand(5, 5): " << std::endl;
    out << m1.expand(5, 5) << std::endl << std::endl;

    out << "m1.expand(2, 2): " << std::endl;
    out << m1.expand(2, 2) << std::endl << std::endl;

    out << "!m1: " << std::endl;
    out << !m1 << std::endl << std::endl;

    out << "-m1: " << std::endl;
    out << -m1 << std::endl << std::endl;

    out << "m1.forEach([](std::size_t i, std::size_t j, double elem) { return i == j; }): " << std::endl;
    out << m1.forEach([](std::size_t i, std::size_t j, double ) { return i == j; }) << std::endl << std::endl;
    out << "----------------------------------" << std::endl;
}

void exampleComplex(std::ostream &out=std::cout)
{
    stuff::Complex<double> c1(1.0, 2.0), c2(2.0, -1.0);
    out << "----------------------------------" << std::endl;
    out << "Complex example: " << std::endl << std::endl;
    out << "c1: " << c1 << std::endl;
    out << "c2: " << c2 << std::endl;
    out << "c1 + c2: " << c1 + c2 << std::endl;
    out << "c1 - c2: " << c1 - c2 << std::endl;
    out << "c1 * c2: " << c1 * c2 << std::endl;
    out << "c1 / c2: " << c1 / c2 << std::endl;
    out << "c1 / 2.0: " << c1 / 2.0 << std::endl;
    out << "2.0 / c1: " << 2.0 / c1 << std::endl;
    out << "2.0 + c1: " << 2.0 + c1 << std::endl;
    out << "c1 - 2.0: " << c1 - 2.0 << std::endl;
    out << "2.0 - c1: " << 2.0 - c1 << std::endl;
    out << "-c1: " << -c1 << std::endl;
    out << "!c1: " << !c1 << std::endl;
    out << "c1 * !c1: " << c1 * !c1 << std::endl;
    out << "stuff::complexExp(M_PI): " << stuff::complexExp(M_PI) << std::endl;
    out << "----------------------------------" << std::endl;
}

int main()
{
    exampleComplex();
    exampleVecs();
    exampleArrays();
    exampleMatrix();

    std::fstream file_examples("output_examples", std::fstream::out);
    exampleComplex(file_examples);
    exampleVecs(file_examples);
    exampleArrays(file_examples);
    exampleMatrix(file_examples);
    file_examples.close();

    return 0;
}
