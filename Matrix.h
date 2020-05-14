#ifndef MATRIX_H
#define MATRIX_H

class iostream;

class Matrix {

	friend std::ostream& operator<<(std::ostream& out, const Matrix& mat);
	friend std::istream& operator>>(std::istream& in, Matrix& mat);
	friend Matrix operator*(float scalar, Matrix& mat); // scalar multiplication

public:
	Matrix(int m, int n);
	Matrix(const Matrix& other); // copy constructor
	~Matrix();

	float** GetMatrixPtr()const;
	int GetRows()const;
	int GetColumns()const;

	const Matrix& operator=(const Matrix& other);
	Matrix operator+(const Matrix& other);
	Matrix operator*(float scalar);
	Matrix operator-(const Matrix& other);

	static float Determinant(const Matrix& mat);
	static float Minor(const Matrix& mat, int m, int n);
	static Matrix Inverse(const Matrix& mat);
	static Matrix Transpose(const Matrix& mat);

	static Matrix Mult(const Matrix& matA, const Matrix& matB);

private:
	//Helper functions
	void CreateMatrix();
	//Size of the matrix
	int m_rows;
	int m_cols;
	float** m_matrix;
};

#endif
