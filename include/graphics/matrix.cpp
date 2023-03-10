#include "matrix.h"

namespace solstice {
    Matrix::Matrix() {
        mRows = 0;
        mColumns = 0;
        mMatrixArr = NULL;
    }
    Matrix::Matrix(unsigned int size, int value) {
        for(size_t i = 0; i < size; i++) {
            mMatrix.push_back(SolVector(size, value));
        }
        mRows = size;
        mColumns = size;
        mMatrixArr = NULL;
    }
    Matrix::Matrix(unsigned int rows, unsigned int columns, int value) {
        Init(rows, columns, value);
    }
    Matrix::Matrix(vector<SolVector> matrix) {
        for(size_t i = 0; i < matrix.size(); i++) {
            mMatrix.push_back(matrix[i]);
        }
        mRows = matrix.size();
        mColumns = mMatrix.size();
        mMatrixArr = NULL;
    }
    Matrix::Matrix(unsigned int size, SolVector initialValues) {
        for(size_t i = 0; i < size; i++) {
            mMatrix.push_back(initialValues);
        }
        mRows = initialValues.Size();
        mColumns = size;
        mMatrixArr = NULL;
    }
    Matrix::Matrix(float* matrix, int m, int n) {
        Init(matrix, m, n);
        mMatrixArr = NULL;
    }
    Matrix::Matrix(const Matrix& m) {
        if(this != &m) {
            mMatrixArr = NULL;
        }
        *this = m;
    }
    Matrix::~Matrix() {
        Clear();
    }
    void Matrix::Clear() {
        if(!mMatrix.empty()) {
            for(size_t i = 0; i < ColumnCount(); i++) {
                mMatrix[i].Clear();
            }
        }
        mMatrix.clear();
        DeleteMatrixArr();
        mRows = 0;
        mColumns = 0;
    }
    Matrix& Matrix::operator=(const Matrix& m) {
        if(this != &m) {
            Clear();
            Resize(m.RowCount(), m.ColumnCount());
            for(size_t i = 0; i < m.ColumnCount(); i++) {
                this->SetColumn(i, m[i]);
            }
        }
        return *this;
    }
    Matrix& Matrix::operator=(const SolVector& v) {
        Clear();
        mRows = v.Size();
        mColumns = 1;
        mMatrix.push_back(v);
        return *this;
    }
    Matrix& Matrix::operator=(const vector<SolVector>& v) {
        Clear();
        for(size_t i = 0; i < v.size(); i++) {
            mMatrix.push_back(v[i]);
        }
		return *this;
    }
    Matrix Matrix::GetInverse() {
        Matrix m = *this;
        m.Invert();
        return m;
    }
    Matrix& Matrix::Invert() {
        float det = GetDeterminant();
        if(det == 0) {
            return *this;
        }
        float determinant = 1 / det;
        if(mRows == 2 && mColumns == 2) {
            SetElement(0, 0, determinant * GetElement(1, 1));
            SetElement(0, 1, determinant * -GetElement(1, 0));
            SetElement(1, 0, determinant * -GetElement(0, 1));
            SetElement(1, 1, determinant * GetElement(0, 0));
        }
        return *this;
    }

    Matrix Matrix::GetTranspose() {
        Matrix m = *this;
        m.Transpose();
        return m;
    }
    Matrix& Matrix::Transpose() {
        Matrix m;
        for(size_t i = 0; i < ColumnCount(); i++) {
            m.AddRow(m.GetColumn(i));
        }
        return *this;
    }

    float Matrix::GetDeterminant() {
        if(mRows == 2 && mColumns == 2) {
            return GetElement(0, 0) * GetElement(1, 1) - GetElement(0, 1) * GetElement(1, 0);
        }
    }
    void Matrix::Init(float* matrix, int m, int n) {
        mMatrixArr = NULL;
        mColumns = n;
        mRows =  m;
        Clear();
        Resize(m, n);

        for(size_t i = 0; i < ColumnCount(); i++) {
            vector<float> v;
            for(int j = 0; j < RowCount(); j++) {
                v.push_back(matrix[i + j * ColumnCount()]);
            }
            SetColumn(i, v);
        }
    }
    void Matrix::Init(float value) {
        for(size_t i = 0; i < ColumnCount(); i++) {
            mMatrix[i].Init(value);
        }
    }
    void Matrix::Init(unsigned int rows, unsigned int columns, int value) {
        for(size_t i = 0; i < rows; i++) {
            mMatrix.push_back(SolVector(columns, value));
        }
        mRows = rows;
        mColumns = columns;
        mMatrixArr = NULL;
    }
    void Matrix::InitDiagonal(SolVector v) {
        Clear();
        Resize(v.Size());
        for(size_t i = 0; i < ColumnCount(); i++) {
            SetElement(i, i, v[i]);
        }
    }
    void Matrix::InitIdentity() {
        if(mRows != mColumns) {
            return;
        }
        else {
            for(size_t i = 0; i < mRows; i++) {
                for(int j = 0; j < mColumns; i++) {
                    SetElement(i, j, i == j ? 1 : 0);
                }
            }
        }
    }
    Matrix Matrix::GetIdentity() {
        Matrix m = *this;
        m.InitIdentity();
        return m;
    }
    void Matrix::InitShaderMatrix(float x, float y, float z) {
        vector<float> values;
        values.push_back(x);
        values.push_back(y);
        values.push_back(z);
        values.push_back(1);
        SolVector v(values);
        InitDiagonal(v);
    }
    void Matrix::InitShaderMatrix(float x, float y, float z, float w) {
        vector<float> values;
        values.push_back(x);
        values.push_back(y);
        values.push_back(z);
        values.push_back(w);
        SolVector v(values);
        InitDiagonal(v);
    }
    size_t Matrix::Size() const {return mRows * mColumns;}
    size_t Matrix::RowCount() const {return mRows;}
    size_t Matrix::ColumnCount() const {return mColumns;}
    bool Matrix::Empty() const {return mMatrix.empty();}
    void Matrix::Resize(int size) {
        mMatrix.resize(size);
        DeleteMatrixArr();
        for(size_t i = 0; i < size; i++) {
            mMatrix[i].Resize(size);
            mMatrix[i].Init(0);
        }
        mRows = size;
        mColumns = size;
    }
    void Matrix::Resize(int m, int n) {
        mMatrix.resize(n);
        DeleteMatrixArr();
        for(size_t i = 0; i < n; i++) {
            mMatrix[i].Resize(m);
        }
        mColumns = n;
        mRows = m;
    }
    void Matrix::DeleteMatrixArr() {
        if(mMatrixArr != NULL) {
            delete[] mMatrixArr;
            mMatrixArr = NULL;
        }
    }
    float* Matrix::GetMatrix() {
        int size = mRows * mColumns;
        if(mMatrixArr == NULL) {
            mMatrixArr = new float[size];
            for(size_t i = 0; i < RowCount(); i++) {
                for(int j = 0; j < ColumnCount(); j++) {
                    mMatrixArr[i * RowCount() + j] = mMatrix[j].GetElement(i);
                }
            }
        }
        return mMatrixArr;
    }

    SolVector Matrix::GetColumn(size_t i) const {
        if(Empty()) {
            return -1;
        }
        return mMatrix[i];
    }
    SolVector Matrix::GetRow(size_t i) const {
        if(Empty()) {
            return -1;
        }
        vector<float> v;
        for(int j = 0; j < ColumnCount(); j++) {
            v.push_back(mMatrix[j].GetElement(i));
        }
        return SolVector(v);
    }
    void Matrix::SetColumn(int col, float value) {
        SetColumn(col, SolVector(RowCount(), value));
    }
    void Matrix::SetRow(int row, float value) {
        SetRow(row, SolVector(ColumnCount(), value));
    }
    void Matrix::SetColumn(size_t i, SolVector v) {
        if(Empty()) {
            return;
        }
        else if(v.Size() != RowCount()) {
            return;
        }
        else {
            mMatrix[i] = v;
        }
    }
    void Matrix::SetRow(size_t i, SolVector v) {
        if(Empty()) {
            return;
        }
        else if(v.Size() != ColumnCount()) {
            return;
        }
        else {
            for(int j = 0; j < ColumnCount(); j++) {
                mMatrix[j].SetElement(i, v[j]);
            }
        }
    }

    SolVector Matrix::operator[](size_t i) const {return mMatrix[i];}
    Matrix Matrix::operator*(float f) {
        Matrix m = *this;
        for(size_t i = 0; i < ColumnCount(); i++) {
            m[i] = mMatrix[i] * f;
        }
        return m;
    }
    void Matrix::SetElement(int row, int column, float value) {
        mMatrix[column].SetElement(row, value);
    }
    float Matrix::GetElement(int row, int column) {
        return mMatrix[column].GetElement(row);
    }
    void Matrix::AddRow(vector<float> row) {
        for(size_t i = 0; i < mMatrix.size(); i++ ) {
            mMatrix[i].AddElement(row[i]);
        }
    }
    void Matrix::AddRow(SolVector row) {
        for(size_t i = 0; i < mMatrix.size(); i++ ) {
            mMatrix[i].AddElement(row[i]);
        }
    }
    void Matrix::AddColumn(vector<float> col) {
        mMatrix.push_back(col);
    }
    void Matrix::AddColumn(SolVector col) {
        mMatrix.push_back(col);
    }
    Matrix& Matrix::operator*=(float f) {
        *this = *this * f;
        return *this;
    }
    Matrix& Matrix::operator*=(const Matrix& m) {
        *this = *this * m;
        return *this;
    }
    Matrix Matrix::operator*(const Matrix& m) {
        if(m.ColumnCount() != RowCount()) {
            return Matrix();
        }
        else {
            int dimension = m.ColumnCount() * RowCount();
            float* matrix = new float[dimension];
            int rows = RowCount();
            int columns = m.ColumnCount();
            for(size_t i = 0; i < rows; i++) {
                for(int j  = 0; j < columns; j++) {
                    float value = GetRow(i) * m.GetColumn(j);
                    matrix[i * rows + j] = value;
                }
            }
			delete[] matrix;
			Matrix m(matrix, rows, columns);
			return m;
        }
    }

    Matrix& Matrix::operator*=(const SolVector& v) {
        *this = *this * v;
		return *this;
    }
    Matrix Matrix::operator*(const SolVector& v) {
        Matrix m;
        m = v;
        return *this;
    }

    Matrix Matrix::operator+=(const Matrix& m) {
        *this = *this + m;
        return *this;
    }
    Matrix Matrix::operator+(const Matrix& m) {
        if(m.RowCount() != this->RowCount() || m.ColumnCount() != this->ColumnCount()) {
            return Matrix();
        }
        else {
            float* matrix = new float[m.RowCount() * m.ColumnCount()];
            for(size_t i = 0; i < m.ColumnCount(); i++) {
                for(int j = 0; j < m.RowCount(); j++) {
                    float value = (*this)[i].GetElement(j) + m[i].GetElement(j);
                    matrix[i + j * RowCount()] = value;
                }
            }
			Matrix m(matrix, RowCount(), ColumnCount());
			delete[] matrix;
			return m;
        }
    }
    Matrix Matrix::operator-(const Matrix& m) {
        if(m.RowCount() != this->RowCount() || m.ColumnCount() != this->ColumnCount()) {
            return Matrix();
        }
        else {
            float* matrix = new float[m.RowCount() * m.ColumnCount()];
            for(size_t i = 0; i < m.ColumnCount(); i++) {
                for(int j = 0; j < m.RowCount(); j++) {
                    float value = (*this)[i].GetElement(j) - m[i].GetElement(j);
                    matrix[i + j * RowCount()] = value;
                }
            }
			Matrix m(matrix, RowCount(), ColumnCount());
			delete[] matrix;
			return m;
        }
    }
    void Matrix::Print() {
        if(mMatrix.empty()) {
            return;
        }
        for(size_t i = 0; i < RowCount(); i++) {
            cout << "[";
            for(int j = 0; j < ColumnCount(); j++) {
                cout << mMatrix[j].GetElement(i) << " ";
            }
            cout << "]\n";
        }
    }

};
