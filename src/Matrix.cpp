#include "Matrix.hpp"
#include <vector>
#include <iostream>
#include <stack>
#include <cmath>
Matrix::Matrix()
{
    loadIdentity();
}
void Matrix::loadIdentity()
{
    currentMatrix = {1, 0, 0, 0,
                     0, 1, 0, 0,
                     0, 0, 1, 0,
                     0, 0, 0, 1};
}
void Matrix::translate(float x, float y, float z) {
    std::cout << "TRANSLATE: (" << x << ", " << y << ", " << z << ")" << std::endl;
    Matrix translationMatrix;
    translationMatrix.currentMatrix = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1  // Colonne 3 (indices 12,13,14)
    };
    multiply(translationMatrix);
    std::cout << "New Transformation Matrix: ";
    for (float value : currentMatrix) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

void Matrix::rotate(float angle, float x, float y, float z) {
    float c = cos(angle);
    float s = sin(angle);
    float norm = sqrt(x * x + y * y + z * z);
    x /= norm; y /= norm; z /= norm;

    Matrix rotationMatrix;
    rotationMatrix.currentMatrix = { x*x*(1-c)+c, x*y*(1-c)-z*s, x*z*(1-c)+y*s, 0,
                                     y*x*(1-c)+z*s, y*y*(1-c)+c, y*z*(1-c)-x*s, 0,
                                     z*x*(1-c)-y*s, z*y*(1-c)+x*s, z*z*(1-c)+c, 0,
                                     0, 0, 0, 1 };
    multiply(rotationMatrix);
}
void Matrix::scale(float x, float y, float z) {
    Matrix scaleMatrix;
    scaleMatrix.currentMatrix = {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    };
    multiply(scaleMatrix);
}
void Matrix::multiply(const Matrix& other) {
    std::vector<float> result(16, 0);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            for (int k = 0; k < 4; ++k)
                result[i * 4 + j] += currentMatrix[i * 4 + k] * other.currentMatrix[k * 4 + j];
    currentMatrix = result;
    std::cout << "Matrix Multiplication Applied" << std::endl;
}
void Matrix::push()
{
    stack.push_back(currentMatrix);
    std::cout << "PUSH: Stack size = " << stack.size() << std::endl;
}
void Matrix::pop()
{
    if (!stack.empty())
    {
        currentMatrix = stack.back();
        stack.pop_back();
        std::cout << "POP: Stack size = " << stack.size() << std::endl;
    }
    else
    {
        std::cerr << "Error: pop() called on an empty stack" << std::endl; 
    }
}
const std::vector<float> &Matrix::getData() const
{
    return currentMatrix;
}
Matrix::~Matrix() {}