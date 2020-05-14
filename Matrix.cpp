#include <iostream>
#include "Matrix.h"

Matrix::Matrix(int m, int n) : m_rows(m), m_cols(n) {
	CreateMatrix();
}

Matrix::Matrix(const Matrix& other) : m_rows(other.m_rows), m_cols(other.m_cols) {
    /* Assigning dynamic memory */
	m_matrix = new float*[m_rows];

	for(int i = 0; i < m_rows; i++)
		m_matrix[i] = new float[m_cols];

	/* Copying all the values */
	for(int i=0; i<m_rows; i++)
		for(int j=0; j<m_cols; j++)
			m_matrix[i][j] = other.m_matrix[i][j];
}

Matrix::~Matrix() {
	for(int i = 0; i < m_rows; ++i)
		delete[] m_matrix[i];

	delete[] m_matrix;
}

float** Matrix::GetMatrixPtr()const {
	return m_matrix;
}

int Matrix::GetRows()const {
	return m_rows;
}

int Matrix::GetColumns()const {
	return m_cols;
}

std::ostream& operator<<(std::ostream& out, const Matrix& mat) {
	for(int i = 0; i < mat.m_rows; ++i) {
		for(int j = 0; j < mat.m_cols; ++j)
			out<<mat.m_matrix[i][j]<<"\t  ";

		out<<"\n";
	}
	return out;
}

std::istream& operator>>(std::istream& in, Matrix& mat) {
	for(int i = 0; i < mat.m_rows; ++i)
		for(int j = 0; j < mat.m_cols; ++j) {
			std::cout<<"matrix["<<i+1<<"]["<<j+1<<"]: ";
			in>>mat.m_matrix[i][j];
		}
	return in;
}

Matrix operator*(float scalar, Matrix& mat){
    return mat * scalar;
}

const Matrix& Matrix::operator=(const Matrix& other) {
    // self-assignment check
	if(&other != this) {
        /* Assigning new values */
		if(m_rows == other.m_rows && m_cols == other.m_cols) {
			for(int i=0; i<m_rows; i++)
				for(int j=0; j<m_cols; j++)
					m_matrix[i][j] = other.m_matrix[i][j];
		} else {
			/* Resizing the matrix and assigning new values */
			for(int i = 0; i < m_rows; ++i)
				delete[] m_matrix[i];

			delete[] m_matrix;

			m_rows = other.m_rows;
			m_cols = other.m_cols;
			CreateMatrix();

			for(int i=0; i<m_rows; i++)
				for(int j=0; j<m_cols; j++)
					m_matrix[i][j] = other.m_matrix[i][j];
		}
	}
	return *this;
}

Matrix Matrix::operator+(const Matrix& other){
    //if(other.m_rows == m_rows && other.m_cols == m_cols){
    Matrix additionMat(m_rows, m_cols);
    float** additionMatPtr = additionMat.GetMatrixPtr();

    for(int i=0; i<additionMat.m_rows; ++i)
		for(int j=0; j<additionMat.m_cols; ++j)
			additionMatPtr[i][j]= m_matrix[i][j] + other.m_matrix[i][j];

    return additionMat;
    //}
}

Matrix Matrix::operator*(float scalar){
    Matrix scalarMult(m_rows, m_cols);
    float** scalarMultPtr = scalarMult.GetMatrixPtr();

    for(int i=0; i<scalarMult.m_rows; i++)
        for(int j=0; j<scalarMult.m_cols; j++)
            scalarMultPtr[i][j] = scalar * m_matrix[i][j];

    return scalarMult;
}

Matrix Matrix::operator-(const Matrix& other){
    Matrix substractMat(m_rows, m_cols);
    float** substractMatPtr = substractMat.GetMatrixPtr();

    for(int i=0; i<substractMat.m_rows; ++i)
		for(int j=0; j<substractMat.m_cols; ++j)
			substractMatPtr[i][j]= m_matrix[i][j] - other.m_matrix[i][j];

    return substractMat;
}

float Matrix::Determinant(const Matrix& mat) {
	float det=0;
	for(int j = 0; j < mat.m_cols; ++j)
		det += ((0+j) % 2 == 0 ? 1 : -1) * mat.m_matrix[0][j] * Minor(mat, 0, j); // solving with minors and cofactors

	return det;
}

float Matrix::Minor(const Matrix& mat, int m, int n) {
	int k = 0, l = 0;
	Matrix minorDet(mat.m_rows-1, mat.m_cols-1);
	float** minorDetPtr = minorDet.GetMatrixPtr();

    /* the algorithm finds the minor at m, n and solves it */
	for(int i = 0; i < mat.m_rows; ++i) {
		for(int j = 0; j < mat.m_cols; ++j) {
			if(i != m && j != n) {
				if(l < minorDet.m_cols) {
					minorDetPtr[k][l] = mat.m_matrix[i][j];
					++l;
				} else {
					++k; l=0;
					minorDetPtr[k][l] = mat.m_matrix[i][j];
					++l;
				}
			}
		}
	}

	if(minorDet.m_rows == 1)
		return minorDetPtr[0][0];
	else
		return Determinant(minorDet);
}

Matrix Matrix::Inverse(const Matrix& mat) {

    /* finds the inverse using the adjugate matrix  A^-1 = (1/det(matA)) * adj(matA)
        Note: the adjugate matrix is the transpose of the cofactor matrix*/

	Matrix cofactorMat(mat.m_rows, mat.m_cols);
	float** cofactorMatPtr = cofactorMat.GetMatrixPtr();

	for(int i=0; i<cofactorMat.m_rows; ++i)
		for(int j=0; j < cofactorMat.m_cols; ++j)
			cofactorMatPtr[i][j] = ((i+j) % 2 == 0 ? 1 : -1) * Minor(mat, i, j);

	Matrix adjugateMat = Matrix::Transpose(cofactorMat);
	//float** adjugateMatPtr = adjugateMat.GetMatrixPtr();

	float detMat= 1/Matrix::Determinant(mat);

	Matrix inverseMat = detMat * adjugateMat;
	//float** inverseMatPtr = inverseMat.GetMatrixPtr();


//	for(int i=0; i<inverseMat.m_rows; ++i)
//		for(int j =0; j<inverseMat.m_cols; ++j)
//			inverseMatPtr[i][j] = detMat * adjugateMatPtr[i][j];

	return inverseMat;

}

Matrix Matrix::Transpose(const Matrix& mat) {
	Matrix trnMat(mat.m_rows, mat.m_cols);
	float** trnMatPtr = trnMat.GetMatrixPtr();

	for(int i=0; i<trnMat.m_rows; ++i)
		for(int j=0; j<trnMat.m_cols; j++)
			trnMatPtr[i][j] = mat.m_matrix[j][i];

	return trnMat;

}

Matrix Matrix::Mult(const Matrix& matA, const Matrix& matB) {

	Matrix multMat(matA.m_rows, matB.m_cols);
	float** multPtr = multMat.GetMatrixPtr();

	for(int i=0; i<matA.m_rows; ++i) {
		for(int j=0; j<matB.m_cols; ++j) {
			multPtr[i][j]=0;
			for(int k=0; k < matA.m_cols; ++k) {
				multPtr[i][j]=multPtr[i][j] + matA.m_matrix[i][k]*matB.m_matrix[k][j];
			}
		}
	}

	return multMat;
}

void Matrix::CreateMatrix() {
	m_matrix = new float*[m_rows];

	for(int i = 0; i < m_rows; i++)
		m_matrix[i] = new float[m_cols];

	/* initializing matrix to 0 */
	for(int i=0; i<m_rows; i++)
		for(int j=0; j<m_cols; j++)
			m_matrix[i][j] = 0;
}
