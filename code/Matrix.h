#pragma once
#pragma once
#include <vector>
#include <iostream>
using namespace std;


typedef vector<double> Vector;
typedef vector<Vector> Matrix;
typedef vector<int> Vint;
Vector operator*(const Matrix& C, const Vector& V);
Vector operator*(const double& a, const Vector& V);
Vector operator*(const Vector& V, const Vector& W);
Vector operator+(const double& a, const Vector& V);
Vector operator+(const Vector& V, const Vector& W);
Vector operator-(const double& a, const Vector& V);
Vector operator-(const Vector& V, const Vector& W);
Vector operator+=(Vector& V, const Vector& W);
Vector operator-=(Vector& V, const Vector& W);
Vector exp(const Vector& V);
ostream& operator<<(ostream& out, Vint& V);
//double operator+=(const Vector& V);
double operator^(const Vector& V, const Vector& W); // scalar operator
Vector operator/(const Vector& C, const Vector& V);
ostream& operator<<(ostream& out, Vector& V); // Overload cout for Vector
ostream& operator<<(ostream& out, Matrix& W); // Overload cout for Matrix
