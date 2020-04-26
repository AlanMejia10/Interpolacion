#ifndef MATRIX_H
#define MATRIX_H

class Matrix{
public:
    Matrix(int m, int n);
    Matrix(const Matrix& mat);
    ~Matrix();
    void Init();
    void CreateMatrix();
    float** GetMatrixPtr()const;
    int GetRows()const;
    int GetColumns()const;
    void PrintMatrix();
    static float Determinant(const Matrix& mat);
    static float Cofactor(const Matrix& mat, int m, int n);
    static Matrix Inverse(const Matrix& mat);
    static Matrix Transpose(const Matrix& mat);
    static Matrix Mult(const Matrix& matA, const Matrix& matB);
    static Matrix Add(const Matrix& matA, const Matrix& matB);
    static Matrix Subtract(const Matrix& matA, const Matrix& matB);

private:
    //Helper functions       
    void FillMatrix();
    //Size of the matrix
    int m_rows;
    int m_cols;
    float** m_matrix;
};

#endif
