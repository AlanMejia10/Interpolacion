#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <algorithm>
#include <cstdlib>
#include "Matrix.h"

bool OrdernarPorValor(const std::pair<float, float> &a, const std::pair<float, float> &b)
{
    return (a.first < b.first);
}

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
    // setting 6 numbers precision
    std::cout << std::setprecision(6);
    
    std::vector<std::pair<float, float>> data;
    char opc;
    int opcNum;

    Portada();
    system("pause");
    clearscr();
    std::cout << "Interpolacion" << std::endl;

    do
    {
        clearscr();
        std::cout << "Metodo de interpolacion" << std::endl;
        Menu();
        std::cin >> opcNum;

        switch (opcNum)
        {
        case 1:

            float numInterpolar;
            int grado, elementoPosicion;
            do
            {

                LeerTabla(data);
                clearscr();
                ImprimirTabla(data);
                std::cout << "Los datos de la tabla son correctos: (s/n): ";
                std::cin >> opc;
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

                do
                {
                    SortData(data);
                    std::cout << "Ingresa el punto a interpolar: ";
                    std::cin >> numInterpolar;
                    if (EstaEnRango(data, numInterpolar))
                    {
                        std::cout << "Ingresa grado del polinomio que se interpolara: ";
                        std::cin >> grado;
                        /**modificacion */

                        for (int i = 0; i < data.size() - 1; i++)
                        {
                            if (data[i].first < numInterpolar && numInterpolar < data[i + 1].first)
                                elementoPosicion = i;
                        }

                        if ((grado + 1) > data.size())
                        {
                            std::cout << "Puntos insuficientes" << std::endl;
                        }
                        else
                        {
                            InterpolacionDiferenciasDivididas(data, numInterpolar, grado);
                        }
                    }
                    else
                    {
                        std::cout << "El punto no esta en el rango de la tabla" << std::endl;
                    }
                    std::cout << "Deseas interpolar otro punto con la misma tabla? (s/n): ";
                    std::cin >> opc;
                } while (opc == 's');
                std::cout << "Te gustaria cambiar la tabla? (s/n): ";
                std::cin >> opc;
                if (opc == 's')
                {
                    data.clear();
                }
            } while (opc == 's');

            break;
        case 2:
        {
            // Spline cubico
            std::vector<float> anchuraH;
            std::vector<float> fx;
            char ajusteOpc;

            do{
                std::cout<<std::noshowpos;
                clearscr();

            LeerTabla(data);
            clearscr();
            ImprimirTabla(data);
            std::cout << "Los datos de la tabla son correctos: (s/n): ";
            std::cin >> opc;
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
            
            SortData(data);

            for(int i=0; i<data.size()-1; i++){
                float deltaH = data[i+1].first - data[i].first;
                float func = (data[i+1].second - data[i].second)/deltaH;
                anchuraH.push_back(deltaH);
                fx.push_back(func);
            }

            Matrix matA(data.size()-2, data.size()-2);
            float** matAPtr = matA.GetMatrixPtr();

            for(int i=0; i<matA.GetRows(); ++i){
                for(int j=0; j<matA.GetColumns(); ++j){
                    if(j == i-1)
                        matAPtr[i][j] = anchuraH[i];
                    else if(i==j)
                        matAPtr[i][j] = 2*(anchuraH[i] + anchuraH[i+1]);
                    else if(j == i+1)
                        matAPtr[i][j] = anchuraH[i+1];
                    else
                        matAPtr[i][j] = 0;                    
                }
            }

            Matrix matB(data.size()-2,1);
            float** matBPtr = matB.GetMatrixPtr(); 
            for(int i=0; i<matB.GetRows(); i++)
                matBPtr[i][0] = 6*(fx[i+1] - fx[i]);

            Matrix matInv = Matrix::Inverse(matA);
            Matrix matResult = Matrix::Mult(matInv, matB);
            float** matResultPtr = matResult.GetMatrixPtr();

            std::deque<float> sCoeficientes; 
            for(int i=0; i<matResult.GetRows(); i++){
                sCoeficientes.push_back(matResultPtr[i][0]);
            }

            sCoeficientes.push_front(0);
            sCoeficientes.push_back(0);

            Matrix matCoeficientes(anchuraH.size(), 4);
            float** matCoeficientesPtr = matCoeficientes.GetMatrixPtr();            
            
            for(int i=0; i<matCoeficientes.GetRows(); i++){
                for(int j=0; j<matCoeficientes.GetColumns(); j++){
                    if(j==0)
                        matCoeficientesPtr[i][j] = (sCoeficientes[i+1] - sCoeficientes[i])/(6* anchuraH[i]);
                    else if(j==1)
                        matCoeficientesPtr[i][j] = sCoeficientes[i]/2.0;
                    else if(j ==2)
                        matCoeficientesPtr[i][j] = ((data[i+1].second - data[i].second)/anchuraH[i]) - anchuraH[i]*((sCoeficientes[i+1] + 2*sCoeficientes[i])/6.0);
                    else if(j==3)
                        matCoeficientesPtr[i][j] = data[i].second;    
                }
            }           
            
            std::cout<< std::showpos;
            for(int i=0; i<data.size()-1; i++){
                //printf("g%i(x)=%f(x-xi)^3 %f(x-xi)^2 %f(x-xi) %f\t %f <= x <= %f", matCoeficientesPtr[i][0], matCoeficientesPtr[i][1], matCoeficientesPtr[i][2], matCoeficientesPtr[i][3], data[i].first, data[i+1].first);
                std::cout<<"g"<<i<<"(x)="<<matCoeficientesPtr[i][0]<<"(x-xi)^3 "<<matCoeficientesPtr[i][1]<<"(x-xi)^2 "<<matCoeficientesPtr[i][2]<<"(x-xi) "<<matCoeficientesPtr[i][3]<<std::endl;
                std::cout<<data[i].first<<" <= x <= "<<data[i+1].first<<"\n"<<std::endl;
            }
            // int grado = 3;
            // for(int i=0; i<matCoeficientes.GetRows(); i++){
            //     std::cout<<"g"<<i<<"(x)=";
            //     for(int j=0; j<matCoeficientes.GetColumns(); j++){
            //         if(grado>1)
            //             std::cout<<matCoeficientesPtr[i][j]<<"(x-xi)^"<<grado;
            //         else
            //             std::cout<<matCoeficientesPtr[i][j]<<std::endl;
                    
            //         --grado;
            //     }
            //     grado = 3;
            // }
            data.clear();
            anchuraH.clear();
            fx.clear();
            std::cout<<"¿Desea realizar otro ajuste con otra tabla? (s/n): ";
            std::cin>>ajusteOpc;
            }while(ajusteOpc == 's');
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
    std::cout << "Metodos Numericos II" << std::endl;
    std::cout << "Metodo de interpolacion (diferencias divididas)" << std::endl;
    std::cout << "Profesora: Carrillo Ramirez Teresa" << std::endl;
    std::cout << "Integrantes:" << std::endl;
    std::cout << "Diaz Lopez Alan Fernando" << std::endl;
    std::cout << "Mejia Espinosa Ruben Alan" << std::endl;
    std::cout << "Grupo: 2401\n"
              << std::endl;
}

void LeerTabla(std::vector<std::pair<float, float>> &data)
{
    bool isCorrect = false;
    char opc;
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
    data[position] = std::make_pair(x, fx);
}

void ImprimirTabla(std::vector<std::pair<float, float>> &data)
{
    std::cout << "pos \tx \tf(x) " << std::endl;
    for (int i = 0; i < data.size(); i++)
        std::cout << i << "\t" << data[i].first << "    " << data[i].second << std::endl;
}

void SortData(std::vector<std::pair<float, float>> &data)
{
    sort(data.begin(), data.end(), OrdernarPorValor);
}

void Menu()
{
    std::cout << "1) Interpolacion por diferencias divididas" << std::endl;
    std::cout << "2) Ajuste de curvas" << std::endl;
    std::cout << "Ingresa la opcion deseada: ";
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
