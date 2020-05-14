#include <iostream>
#include <math.h>
#include "Matrix.h"
#include "NewtonRaphson.h"

void NewtonRaphson::Menu(){
    std::cout<<"Metodo de Newton Raphson\n"<<std::endl;
    std::cout<<"1) f1(x, y) = x^2+xy-10=0\n   f2(x, y) = y+3xy^2-50=0\n\n"
    <<"2) f1(x, y) = x^2+y^2-9=0\n   f2(x, y) = -e^x-2y-3=0\n\n"
    <<"3) f1(x, y, z) = 2x^2-4x+y^2+3z^2+6z+2=0\n   f2(x, y, z) = x^2+y^2-2y+2z^2-5=0\n   f3(x, y, z) = 3x^2-12x+y^2-3z^2+8=0\n\n"
    <<"4) f1(x, y, z) = x^2-4x+y^2=0\n   f2(x, y, z) = x^2-x-12y+1=0\n   f3(x, y, z) = 3x^2-12x+y^2-3z^2+8=0\n\n";
}

void NewtonRaphson::SistemaEcuaciones1(){
    Matrix xVector(2,1);
    int iterations;
    float tolerance;

    std::cout<<"Ingresa el valor del vector incial x: "<<std::endl;
    std::cin>>xVector;
    float** xVectorPtr = xVector.GetMatrixPtr();

    std::cout<<"Ingresa el numero de iteraciones: ";
    std::cin>>iterations;

    std::cout<<"Ingresa el valor de la tolerancia: ";
    std::cin>>tolerance;

    Matrix functionMat(2,1);
    float** functionMatPtr= functionMat.GetMatrixPtr();

    Matrix jacobianMat(2,2);
    float** jacobianMatPtr = jacobianMat.GetMatrixPtr();

    for(int i=0; i<iterations; ++i){
        // Evaluating Functions
        functionMatPtr[0][0]= pow(xVectorPtr[0][0],2) + xVectorPtr[0][0]*xVectorPtr[1][0]-10;
        functionMatPtr[1][0] = xVectorPtr[1][0] + 3*xVectorPtr[0][0]*pow(xVectorPtr[1][0],2)-50;

        // Evaluating jacobian matrix (partial derivatives)
        jacobianMatPtr[0][0] = 2*xVectorPtr[0][0]+xVectorPtr[1][0];
        jacobianMatPtr[0][1] = xVectorPtr[0][0];
        jacobianMatPtr[1][0] = 3*pow(xVectorPtr[1][0],2);
        jacobianMatPtr[1][1] = 6*xVectorPtr[0][0]*xVectorPtr[1][0]+1;

        // Newton Raphson's algorithm
        Matrix inverseJacobianMat = Matrix::Inverse(jacobianMat);
        Matrix inverseJacobianTimesFunction = Matrix::Mult(inverseJacobianMat, functionMat);
        xVector =  xVector - inverseJacobianTimesFunction; //Matrix::Subtract(x, jInvFunc);

        if(Tolerance(functionMat, tolerance) || i==iterations-1){
            if(Tolerance(functionMat, tolerance))
                std::cout<<"Tolerancia alcanzada en iteracion: "<<i<<std::endl;
            std::cout<<"Vector solucion: "<<std::endl;
            std::cout<<xVector<<std::endl;
            break;
        }
    }
}

void NewtonRaphson::SistemaEcuaciones2() {
	Matrix xVector(2,1);
	int iterations;
	float tolerance;

	std::cout<<"Ingresa el valor del vector incial x: "<<std::endl;
	std::cin>>xVector;
	float** xVectorPtr = xVector.GetMatrixPtr();

	std::cout<<"Ingresa el numero de iteraciones: ";
	std::cin>>iterations;

	std::cout<<"Ingresa el valor de la tolerancia: ";
	std::cin>>tolerance;

	Matrix functionMat(2,1);
	float** functionMatPtr= functionMat.GetMatrixPtr();

	Matrix jacobianMat(2,2);
	float** jacobianMatPtr = jacobianMat.GetMatrixPtr();

	for(int i=0; i<iterations; ++i) {
		// Evaluating Functions
		functionMatPtr[0][0]= pow(xVectorPtr[0][0],2) + pow(xVectorPtr[1][0],2) - 9;
        functionMatPtr[1][0] = -exp(xVectorPtr[0][0])-2*xVectorPtr[1][0]-3;

		// Evaluating jacobian matrix (partial derivatives)
		jacobianMatPtr[0][0] = 2*xVectorPtr[0][0];
        jacobianMatPtr[0][1] = 2*xVectorPtr[1][0];
        jacobianMatPtr[1][0] = -exp(xVectorPtr[0][0]);
        jacobianMatPtr[1][1] = -2;

		// Newton Raphson's algorithm
		Matrix inverseJacobianMat = Matrix::Inverse(jacobianMat);
		Matrix inverseJacobianTimesFunction = Matrix::Mult(inverseJacobianMat, functionMat);
		xVector =  xVector - inverseJacobianTimesFunction; //Matrix::Subtract(x, jInvFunc);

		if(Tolerance(functionMat, tolerance) || i==iterations-1) {
			if(Tolerance(functionMat, tolerance))
				std::cout<<"Tolerancia alcanzada en iteracion: "<<i<<std::endl;
			std::cout<<"Vector solucion: "<<std::endl;
			std::cout<<xVector<<std::endl;
			break;
		}
	}
}

void NewtonRaphson::SistemaEcuaciones3() {
	Matrix xVector(3,1);
	int iterations;
	float tolerance;

	std::cout<<"Ingresa el valor del vector incial x: "<<std::endl;
	std::cin>>xVector;
	float** xVectorPtr = xVector.GetMatrixPtr();

	std::cout<<"Ingresa el numero de iteraciones: ";
	std::cin>>iterations;

	std::cout<<"Ingresa el valor de la tolerancia: ";
	std::cin>>tolerance;

	Matrix functionMat(3,1);
	float** functionMatPtr= functionMat.GetMatrixPtr();

	Matrix jacobianMat(3,3);
	float** jacobianMatPtr = jacobianMat.GetMatrixPtr();

	for(int i=0; i<iterations; ++i) {
		// Evaluating Functions
		functionMatPtr[0][0]= 2*pow(xVectorPtr[0][0],2)-4*xVectorPtr[0][0]+pow(xVectorPtr[1][0],2)+3*pow(xVectorPtr[2][0],2)+6*xVectorPtr[2][0]+2;
        functionMatPtr[1][0] = pow(xVectorPtr[0][0],2) + pow(xVectorPtr[1][0],2) -2*xVectorPtr[1][0] + 2*pow(xVectorPtr[2][0],2)-5;
        functionMatPtr[2][0] = 3*pow(xVectorPtr[0][0],2)-12*xVectorPtr[0][0]+pow(xVectorPtr[1][0],2)-3*pow(xVectorPtr[2][0],2)+8;

		// Evaluating jacobian matrix (partial derivatives)
		jacobianMatPtr[0][0] = 4*xVectorPtr[0][0]-4;
        jacobianMatPtr[0][1] = 2*xVectorPtr[1][0];
        jacobianMatPtr[0][2] = 6*xVectorPtr[2][0]+6;
        jacobianMatPtr[1][0] = 2*xVectorPtr[0][0];
        jacobianMatPtr[1][1] = 2*xVectorPtr[1][0]-2;
        jacobianMatPtr[1][2] = 4*xVectorPtr[2][0];
        jacobianMatPtr[2][0] = 6*xVectorPtr[0][0]-12;
        jacobianMatPtr[2][1] = 2*xVectorPtr[1][0];
        jacobianMatPtr[2][2] = -6*xVectorPtr[2][0];

		// Newton Raphson's algorithm
		Matrix inverseJacobianMat = Matrix::Inverse(jacobianMat);
		Matrix inverseJacobianTimesFunction = Matrix::Mult(inverseJacobianMat, functionMat);
		xVector =  xVector - inverseJacobianTimesFunction; //Matrix::Subtract(x, jInvFunc);

		if(Tolerance(functionMat, tolerance) || i==iterations-1) {
			if(Tolerance(functionMat, tolerance))
				std::cout<<"Tolerancia alcanzada en iteracion: "<<i<<std::endl;
			std::cout<<"Vector solucion: "<<std::endl;
			std::cout<<xVector<<std::endl;
			break;
		}
	}
}

void NewtonRaphson::SistemaEcuaciones4() {
	Matrix xVector(3,1);
	int iterations;
	float tolerance;

	std::cout<<"Ingresa el valor del vector incial x: "<<std::endl;
	std::cin>>xVector;
	float** xVectorPtr = xVector.GetMatrixPtr();

	std::cout<<"Ingresa el numero de iteraciones: ";
	std::cin>>iterations;

	std::cout<<"Ingresa el valor de la tolerancia: ";
	std::cin>>tolerance;

	Matrix functionMat(3,1);
	float** functionMatPtr= functionMat.GetMatrixPtr();

	Matrix jacobianMat(3,3);
	float** jacobianMatPtr = jacobianMat.GetMatrixPtr();

	for(int i=0; i<iterations; ++i) {
		// Evaluating Functions
		functionMatPtr[0][0]= pow(xVectorPtr[0][0],2)-4*xVectorPtr[0][0]+pow(xVectorPtr[1][0],2);
        functionMatPtr[1][0] = pow(xVectorPtr[0][0],2)-xVectorPtr[0][0]-12*xVectorPtr[1][0]+1;
        functionMatPtr[2][0] = 3*pow(xVectorPtr[0][0],2)-12*xVectorPtr[0][0]+pow(xVectorPtr[1][0],2)-3*pow(xVectorPtr[2][0],2)+8;

		// Evaluating jacobian matrix (partial derivatives)
		jacobianMatPtr[0][0] = 2*xVectorPtr[0][0]-4;
        jacobianMatPtr[0][1] = 2*xVectorPtr[1][0];
        jacobianMatPtr[0][2] = 0;
        jacobianMatPtr[1][0] = 2*xVectorPtr[0][0]-1;
        jacobianMatPtr[1][1] = -12;
        jacobianMatPtr[1][2] = 0;
        jacobianMatPtr[2][0] = 6*xVectorPtr[0][0]-12;
        jacobianMatPtr[2][1] = 2*xVectorPtr[1][0];
        jacobianMatPtr[2][2] = -6*xVectorPtr[2][0];

		// Newton Raphson's algorithm
		Matrix inverseJacobianMat = Matrix::Inverse(jacobianMat);
		Matrix inverseJacobianTimesFunction = Matrix::Mult(inverseJacobianMat, functionMat);
		xVector =  xVector - inverseJacobianTimesFunction;

		if(Tolerance(functionMat, tolerance) || i==iterations-1) {
			if(Tolerance(functionMat, tolerance))
				std::cout<<"Tolerancia alcanzada en iteracion: "<<i<<std::endl;
			std::cout<<"Vector solucion: "<<std::endl;
			std::cout<<xVector<<std::endl;
			break;
		}
	}
}

bool NewtonRaphson::Tolerance(const Matrix& mat, float tol){
    float** matPtr = mat.GetMatrixPtr();
    for(int i=0; i<mat.GetRows(); ++i)
        if(fabs(matPtr[i][0]) > tol) return false;

    return true;
}
