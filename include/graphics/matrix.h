#ifndef MATRIX4_H_INCLUDED
#define MATRIX4_H_INCLUDED

#include <vector>
#include <iostream>
#include "solvector.h"
using std::vector;
using std::cout;

namespace solstice {
    class Matrix {
        private:
            vector<SolVector> mMatrix;
            size_t mRows, mColumns;
            float* mMatrixArr;
        public:
            Matrix();
            Matrix(unsigned int size, int value = 0);
            Matrix(unsigned int rows, unsigned int columns, int value);
            Matrix(vector<SolVector> matrix);
            Matrix(unsigned int size, SolVector initialValues);
            Matrix(float* matrix, int m, int n);
            Matrix(const Matrix& m);
            ~Matrix();

            Matrix& operator=(const Matrix& m);
            Matrix& operator=(const vector<SolVector>& v);
            Matrix& operator=(const SolVector& v);

            Matrix& operator*=(float f);
            Matrix operator*(float f);

            Matrix& operator*=(const Matrix& m);
            Matrix operator*(const Matrix& m);

            Matrix& operator*=(const SolVector& v);
            Matrix operator*(const SolVector& v);

            Matrix operator+=(const Matrix& m);
            Matrix operator+(const Matrix& m);
            Matrix operator-(const Matrix& m);

            SolVector operator[](size_t i) const;

            Matrix GetInverse();
            Matrix& Invert();

            Matrix GetTranspose();
            Matrix& Transpose();

            float GetDeterminant();
            // TODO (Stephen#1#): Implement GetCofactor() and GetSubMatrix() for the matrix class. This i used for determintants/inverting matrices.
            //float GetCofactor();
            //Matrix GetSubMatrix(unsigned int m, unsigned int n)

            void Clear();
            size_t Size() const;
            size_t RowCount() const;
            size_t ColumnCount() const;
            bool Empty() const;
            void Resize(int size);
            void Resize(int m, int n);
            void DeleteMatrixArr();

            float* GetMatrix();

            SolVector GetColumn(size_t i) const;
            SolVector GetRow(size_t i) const;
            void SetColumn(size_t i, SolVector v);
            void SetRow(size_t i, SolVector v);
            void SetColumn(int col, float value);
            void SetRow(int row, float value);
            void SetElement(int row, int column, float value);
            float GetElement(int row, int column);

            void AddRow(vector<float> row);
            void AddColumn(vector<float> col);
            void AddRow(SolVector row);
            void AddColumn(SolVector col);

            void Print();
            void Init(float value);
            void Init(float* matrix, int m, int n);
            void Init(unsigned int rows, unsigned int columns, int value);
            void InitDiagonal(SolVector v);
            void InitIdentity();
            Matrix GetIdentity();

            void InitShaderMatrix(float x, float y, float z, float w);
            void InitShaderMatrix(float x, float y, float z);
    };
};


#endif // UMATRIX4_H_INCLUDED
