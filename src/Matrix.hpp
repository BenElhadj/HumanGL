#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <vector>
#include <cmath>
class Matrix
{
public:
    Matrix();
    Matrix(const Matrix &other);
    Matrix &operator=(const Matrix &other);
    ~Matrix();
    void loadIdentity();
    void translate(float x, float y, float z);
    void rotate(float angle, float x, float y, float z);
    void scale(float x, float y, float z);
    void multiply(const Matrix &other);
    void push();
    void pop();
    const std::vector<float> &getData() const;

private:
    std::vector<float> currentMatrix;
    std::vector<std::vector<float>> stack;
};
#endif