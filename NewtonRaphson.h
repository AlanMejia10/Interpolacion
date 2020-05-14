#ifndef NEWTONRAPHSON_H
#define NEWTONRAPHSON_H

//class Matrix;

class NewtonRaphson{
public:
    static void Menu();
    static void SistemaEcuaciones1();
    static void SistemaEcuaciones2();
    static void SistemaEcuaciones3();
    static void SistemaEcuaciones4();
    static bool Tolerance(const Matrix& mat, float tol);

private:
    NewtonRaphson(){}
};

#endif
