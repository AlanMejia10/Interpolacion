#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <algorithm>
#include <cstdlib>
#include "Matrix.h"

bool OrdernarPorValor(const std::pair<float, float> &a, const std::pair<float, float> &b){
    return (a.first < b.first);
}

Matrix Spline(std::vector<std::pair<float, float>> &data, std::vector<float>& anchuraH, std::vector<float>& fx);
void SetupTable(std::vector<std::pair<float, float>> &data);
void clearscr();
void Portada();
void LeerTabla(std::vector<std::pair<float, float>> &data);
void ModificarPosicion(std::vector<std::pair<float, float>> &data, int position);
void ImprimirTabla(std::vector<std::pair<float, float>> &data);
void SortData(std::vector<std::pair<float, float>> &data);
void Menu();
bool EstaEnRango(std::vector<std::pair<float, float>> &data, float numInterpolar);
void InterpolacionDiferenciasDivididas(std::vector<std::pair<float, float>> &data, float numInterpolar, int grado);

int main()
{

    std::cout << std::setprecision(6);
    std::vector<std::pair<float, float>> data;
    char opc;
    int opcMenu;

    Portada();
    getchar();
    clearscr();

    do
    {
        clearscr();
        std::cout << "Metodos de interpolacion" << std::endl;
        Menu();
        std::cin >> opcMenu;

        switch (opcMenu){
        case 1:

            float numInterpolar;
            int grado, elementoPosicion;

            do{
                SetupTable(data);
                do{
                    SortData(data);
                    std::cout << "Ingresa el punto a interpolar: ";
                    std::cin >> numInterpolar;
                    if (EstaEnRango(data, numInterpolar))
                    {
                        std::cout << "Ingresa grado del polinomio que se interpolara: ";
                        std::cin >> grado;

                        for (int i = 0; i < data.size() - 1; i++)
                        {
                            if (data[i].first < numInterpolar && numInterpolar < data[i + 1].first)
                                elementoPosicion = i;
                        }

                        if ((grado + 1) > data.size()){
                            std::cout << "Puntos insuficientes" << std::endl;
                        }
                        else{
                            InterpolacionDiferenciasDivididas(data, numInterpolar, grado);
                        }
                    }
                    else{
                        std::cout << "El punto no esta en el rango de la tabla" << std::endl;
                    }
                    std::cout << "Deseas interpolar otro punto con la misma tabla? (s/n): ";
                    std::cin >> opc;
                } while (opc == 's');
                data.clear();
                std::cout << "Te gustaria cambiar la tabla? (s/n): ";
                std::cin >> opc;
            } while (opc == 's');
            break;
        case 2:
        {
            // Spline cubico
            std::vector<float> anchuraH;
            std::vector<float> fx;
            char ajusteOpc;

            do{
                clearscr();
                SetupTable(data);
                SortData(data);

                Matrix matCoeficientes = Spline(data, anchuraH, fx);
                float **matCoeficientesPtr = matCoeficientes.GetMatrixPtr();

                for (int i = 0; i < data.size() - 1; i++){
                    std::cout << "g" << i << "(x)=" << std::showpos << matCoeficientesPtr[i][0] << "(x-xi)^3 " << matCoeficientesPtr[i][1] << "(x-xi)^2 " << matCoeficientesPtr[i][2] << "(x-xi) " << matCoeficientesPtr[i][3] << std::endl;
                    std::cout << data[i].first << " <= x <= " << data[i + 1].first << "\n"<< std::endl;
                    std::cout << std::noshowpos;
                }

                data.clear();
                anchuraH.clear();
                fx.clear();
                std::cout << "¿Desea realizar otro ajuste con otra tabla? (s/n): ";
                std::cin >> ajusteOpc;
            } while (ajusteOpc == 's');
        }
        break;
        case 3:
            break;
        default:
            std::cout << "Opcion incorrecta" << std::endl;
        }
        std::cout << "Te gustaria ver nuevamente el submenu? (s/n):";
        std::cin >> opc;
        if (opc == 's')
            data.clear();
    } while (opc == 's');
}

void Portada()
{
    std::cout << "Metodos Numericos II \n"
              << "Metodos de interpolacion (Diferencias divididas, Spline cubico)\n"
              << "Profesora: Carrillo Ramirez Teresa\n"
              << "Integrantes:\n"
              << "Diaz Lopez Alan Fernando\n"
              << "Mejia Espinosa Ruben Alan\n"
              << "Grupo: 2401" << std::endl;
}

void SetupTable(std::vector<std::pair<float, float>> &data)
{
    char opc;

    LeerTabla(data);
    clearscr();
    ImprimirTabla(data);
    std::cout << "Los datos de la tabla son correctos: (s/n): ";
    std::cin >> opc;

    if (opc == 's')
        return;

    while (opc != 's')
    {
        int position;
        std::cout << "Indica la posicion que se modificara: ";
        std::cin >> position;
        ModificarPosicion(data, position);
        ImprimirTabla(data);
        std::cout << "Los datos de la tabla son correctos: (s/n): ";
        std::cin >> opc;
    }
}

void LeerTabla(std::vector<std::pair<float, float>> &data)
{
    int numPuntos;
    float x, fx;
    std::cout << "Ingresa el numero de puntos: ";
    std::cin >> numPuntos;

    for (int i = 0; i < numPuntos; ++i)
    {
        std::cout << "Ingresa el valor de x" << i << ": ";
        std::cin >> x;
        std::cout << "Ingresa el valor de f" << i << ": ";
        std::cin >> fx;
        data.push_back(std::make_pair(x, fx));
    }
}

void ModificarPosicion(std::vector<std::pair<float, float>> &data, int position)
{
    float x, fx;
    std::cout << "Ingresa el valor de x" << position << ": ";
    std::cin >> x;
    std::cout << "Ingresa el valor de f" << position << ": ";
    std::cin >> fx;
    data.at(position) = std::make_pair(x, fx);
}

void ImprimirTabla(std::vector<std::pair<float, float>> &data)
{
    std::cout << "i \tx \tf(x) " << std::endl;
    for (int i = 0; i < data.size(); i++)
        std::cout << i << "\t" << data[i].first << "    " << data[i].second << std::endl;
}

void SortData(std::vector<std::pair<float, float>> &data)
{
    sort(data.begin(), data.end(), OrdernarPorValor);
}

void Menu()
{
    std::cout << "1) Interpolacion por diferencias divididas\n"
              << "2) Ajuste de curvas (Spline cubico)\n"
              << "3) Salir\n"
              << "Ingresa la opcion deseada: " << std::endl;
}

bool EstaEnRango(std::vector<std::pair<float, float>> &data, float numInterpolar)
{
    for (int i = 0; i < data.size() - 1; i++)
    {
        if (data[i].first < numInterpolar && numInterpolar < data[i + 1].first)
            return true;
    }

    return false;
}

void InterpolacionDiferenciasDivididas(std::vector<std::pair<float, float>> &data, float numInterpolar, int gradoPolinomio)
{
    std::vector<std::vector<float>> interpolacion(gradoPolinomio);
    std::vector<std::pair<float, float>> dataTable = data;
    int numElementos = gradoPolinomio;
    int grado = 1;

    for (int i = 0; i < gradoPolinomio; i++)
    {
        std::vector<std::pair<float, float>> dat;
        for (int j = 0; j < numElementos; j++)
        {
            float fx = dataTable[j + 1].second - dataTable[j].second;
            float x = data[j + grado].first - data[j].first;
            float result = fx / x;
            interpolacion[i].push_back(result);
            dat.push_back(std::make_pair(x, result));
        }
        grado++;
        numElementos--;
        dataTable = dat;
    }

    float valorInterpolacion = data[0].second;
    float mult = 1;
    for (int i = 0; i < gradoPolinomio; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            mult *= (numInterpolar - data[j].first);
        }
        valorInterpolacion += interpolacion[i][0] * mult;
        mult = 1;
    }

    std::cout << "Valor de interpolacion para " << numInterpolar << " es: " << valorInterpolacion << std::endl;
}

Matrix Spline(std::vector<std::pair<float, float>> &data, std::vector<float>& anchuraH, std::vector<float>& fx)
{
    for (int i = 0; i < data.size() - 1; i++)
    {
        float deltaH = data[i + 1].first - data[i].first;
        float func = (data[i + 1].second - data[i].second) / deltaH;
        anchuraH.push_back(deltaH);
        fx.push_back(func);
    }

    Matrix matA(data.size() - 2, data.size() - 2);
    float **matAPtr = matA.GetMatrixPtr();

    for (int i = 0; i < matA.GetRows(); ++i)
    {
        for (int j = 0; j < matA.GetColumns(); ++j)
        {
            if (j == i - 1)
                matAPtr[i][j] = anchuraH[i];
            else if (i == j)
                matAPtr[i][j] = 2 * (anchuraH[i] + anchuraH[i + 1]);
            else if (j == i + 1)
                matAPtr[i][j] = anchuraH[i + 1];
            else
                matAPtr[i][j] = 0;
        }
    }

    Matrix matB(data.size() - 2, 1);
    float **matBPtr = matB.GetMatrixPtr();

    for (int i = 0; i < matB.GetRows(); i++)
        matBPtr[i][0] = 6 * (fx[i + 1] - fx[i]);

    Matrix matInv = Matrix::Inverse(matA);
    Matrix matResult = Matrix::Mult(matInv, matB);
    float **matResultPtr = matResult.GetMatrixPtr();

    std::deque<float> sCoeficientes;
    for (int i = 0; i < matResult.GetRows(); i++)
    {
        sCoeficientes.push_back(matResultPtr[i][0]);
    }
    sCoeficientes.push_front(0);
    sCoeficientes.push_back(0);

    Matrix matCoeficientes(anchuraH.size(), 4);
    float **matCoeficientesPtr = matCoeficientes.GetMatrixPtr();

    for (int i = 0; i < matCoeficientes.GetRows(); i++)
    {
        for (int j = 0; j < matCoeficientes.GetColumns(); j++)
        {
            if (j == 0)
                matCoeficientesPtr[i][j] = (sCoeficientes[i + 1] - sCoeficientes[i]) / (6 * anchuraH[i]);
            else if (j == 1)
                matCoeficientesPtr[i][j] = sCoeficientes[i] / 2.0;
            else if (j == 2)
                matCoeficientesPtr[i][j] = ((data[i + 1].second - data[i].second) / anchuraH[i]) - anchuraH[i] * ((sCoeficientes[i + 1] + 2 * sCoeficientes[i]) / 6.0);
            else if (j == 3)
                matCoeficientesPtr[i][j] = data[i].second;
        }
    }
    return matCoeficientes;
}

void clearscr()
{
#ifdef _WIN32
    system("cls");
#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
    system("clear");
//add some other OSes here if needed
#else
#error "OS not supported."
    //you can also throw an exception indicating the function can't be used
#endif
}
//365 lines