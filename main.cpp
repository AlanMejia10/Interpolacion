#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <deque>
#include <algorithm>
#include "Matrix.h"
#include "NewtonRaphson.h"

void Portada();
void clearScreen();

void Menu();
void MenuInterpolacion();

bool OrdernarPorValor(const std::pair<float, float> &a, const std::pair<float, float> &b) {
	return (a.first < b.first);
}

Matrix Spline(std::vector<float>& anchuraH, std::vector<float>& fx);
void SetupTable();
void LeerTabla();
void ModificarPosicion(int position);
void ImprimirTabla();
void SortData();
bool EstaEnRango(float numInterpolar);
void InterpolacionDiferenciasDivididas(float numInterpolar, int grado);

void Newton();
void MetodosInterpolacion();
void Diferencias();
void SplineCubico();

std::vector<std::pair<float, float>> data;

int main() {
	int opc;
	char decision;

	Portada();
	getchar();
	clearScreen();
	std::cout << std::setprecision(6);
	do {
		clearScreen();
		Menu();
		std::cout<<"Selecciona una opcion: ";
		std::cin>>opc;

		switch (opc) {
		case 1:
			Newton();
			break;
		case 2:
			MetodosInterpolacion();
			break;
		case 3:
			break;
		default:
			std::cout<<"Opcion incorrecta"<<std::endl;
		}
		std::cout<<"¿Te gustaria resolver otro sistema? (s/n)";
		std::cin>>decision;
	} while(decision == 's');
	return 0;
}

void Portada() {
	std::cout << "Metodos Numericos II \n"
	          << "Paquete de programas\n"
	          << "Profesora: Carrillo Ramirez Teresa\n"
	          << "Integrantes:\n"
	          << "Diaz Lopez Alan Fernando\n"
	          << "Mejia Espinosa Ruben Alan\n"
	          << "Grupo: 2401" << std::endl;
}

void clearScreen() {
#ifdef _WIN32
	system("cls");
#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
	system("clear");
#else
#error "OS not supported."
#endif
}

void Menu() {
	std::cout<<"Metodos Numericos\n"
	         <<"1) Newton Raphson (Sistemas de ecuaciones no lineales)\n"
	         <<"2) Metodos de interpolacion\n"
	         <<"3) Metodos de integracion y diferenciacion"<<std::endl;
}

void Newton() {
	int opc;
	clearScreen();
	NewtonRaphson::Menu();
	std::cout<<"Selecciona una opcion: ";
	std::cin>>opc;

	switch(opc) {
	case 1:
		NewtonRaphson::SistemaEcuaciones1();
		break;
	case 2:
		NewtonRaphson::SistemaEcuaciones2();
		break;
	case 3:
		NewtonRaphson::SistemaEcuaciones3();
		break;
	case 4:
		NewtonRaphson::SistemaEcuaciones4();
		break;
	default:
		std::cout<<"Opcion incorrecta"<<std::endl;
		break;
	}
}

void MetodosInterpolacion() {
	int opc;
	clearScreen();
	MenuInterpolacion();
	std::cout<<"Selecciona una opcion: ";
	std::cin>>opc;

	switch(opc) {
	case 1:
		Diferencias();
		break;
	case 2:
		SplineCubico();
		break;
	default:
		std::cout<<"Opcion incorrecta"<<std::endl;
		break;
	}
}

void Diferencias() {
	char opc;
	float numInterpolar;
	int grado, elementoPosicion;
	do {
		SetupTable();
		do {
			SortData();
			std::cout << "Ingresa el punto a interpolar: ";
			std::cin >> numInterpolar;
			if (EstaEnRango(numInterpolar)) {
				std::cout << "Ingresa grado del polinomio que se interpolara: ";
				std::cin >> grado;

				for (int i = 0; i < data.size() - 1; i++) {
					if (data[i].first < numInterpolar && numInterpolar < data[i + 1].first)
						elementoPosicion = i;
				}

				if ((grado + 1) > data.size()) {
					std::cout << "Puntos insuficientes" << std::endl;
				} else {
					InterpolacionDiferenciasDivididas(numInterpolar, grado);
				}
			} else {
				std::cout << "El punto no esta en el rango de la tabla" << std::endl;
			}
			std::cout << "Deseas interpolar otro punto con la misma tabla? (s/n): ";
			std::cin >> opc;
		} while (opc == 's');
		data.clear();
		std::cout << "Te gustaria cambiar la tabla? (s/n): ";
		std::cin >> opc;
	} while (opc == 's');
}

void SplineCubico() {
	std::vector<float> anchuraH;
	std::vector<float> fx;
	char ajusteOpc;

	do {
		clearScreen();
		SetupTable();
		SortData();

		Matrix matCoeficientes = Spline(anchuraH, fx);
		float **matCoeficientesPtr = matCoeficientes.GetMatrixPtr();

		for (int i = 0; i < data.size() - 1; i++) {
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

void SetupTable() {
	char opc;

	LeerTabla();
	clearScreen();
	ImprimirTabla();
	std::cout << "Los datos de la tabla son correctos: (s/n): ";
	std::cin >> opc;

	if (opc == 's')
		return;

	while (opc != 's') {
		int position;
		std::cout << "Indica la posicion que se modificara: ";
		std::cin >> position;
		ModificarPosicion(position);
		ImprimirTabla();
		std::cout << "Los datos de la tabla son correctos: (s/n): ";
		std::cin >> opc;
	}
}

void LeerTabla() {
	int numPuntos;
	float x, fx;
	std::cout << "Ingresa el numero de puntos: ";
	std::cin >> numPuntos;

	for (int i = 0; i < numPuntos; ++i) {
		std::cout << "Ingresa el valor de x" << i << ": ";
		std::cin >> x;
		std::cout << "Ingresa el valor de f" << i << ": ";
		std::cin >> fx;
		data.push_back(std::make_pair(x, fx));
	}
}

void ModificarPosicion(int position) {
	float x, fx;
	std::cout << "Ingresa el valor de x" << position << ": ";
	std::cin >> x;
	std::cout << "Ingresa el valor de f" << position << ": ";
	std::cin >> fx;
	data.at(position) = std::make_pair(x, fx);
}

void ImprimirTabla() {
	std::cout << "i \tx \tf(x) " << std::endl;
	for (int i = 0; i < data.size(); i++)
		std::cout << i << "\t" << data[i].first << "    " << data[i].second << std::endl;
}

void SortData() {
	sort(data.begin(), data.end(), OrdernarPorValor);
}

bool EstaEnRango(float numInterpolar) {
	for (int i = 0; i < data.size() - 1; i++) {
		if (data[i].first < numInterpolar && numInterpolar < data[i + 1].first)
			return true;
	}

	return false;
}

void InterpolacionDiferenciasDivididas(float numInterpolar, int gradoPolinomio) {
	std::vector<std::vector<float>> interpolacion(gradoPolinomio);
	std::vector<std::pair<float, float>> dataTable = data;
	int numElementos = gradoPolinomio;
	int grado = 1;

	for (int i = 0; i < gradoPolinomio; i++) {
		std::vector<std::pair<float, float>> dat;
		for (int j = 0; j < numElementos; j++) {
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
	for (int i = 0; i < gradoPolinomio; i++) {
		for (int j = 0; j <= i; j++) {
			mult *= (numInterpolar - data[j].first);
		}
		valorInterpolacion += interpolacion[i][0] * mult;
		mult = 1;
	}

	std::cout << "Valor de interpolacion para " << numInterpolar << " es: " << valorInterpolacion << std::endl;
}

Matrix Spline(std::vector<float>& anchuraH, std::vector<float>& fx) {
	for (int i = 0; i < data.size() - 1; i++) {
		float deltaH = data[i + 1].first - data[i].first;
		float func = (data[i + 1].second - data[i].second) / deltaH;
		anchuraH.push_back(deltaH);
		fx.push_back(func);
	}

	Matrix matA(data.size() - 2, data.size() - 2);
	float **matAPtr = matA.GetMatrixPtr();

	for (int i = 0; i < matA.GetRows(); ++i) {
		for (int j = 0; j < matA.GetColumns(); ++j) {
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
	for (int i = 0; i < matResult.GetRows(); i++) {
		sCoeficientes.push_back(matResultPtr[i][0]);
	}
	sCoeficientes.push_front(0);
	sCoeficientes.push_back(0);

	Matrix matCoeficientes(anchuraH.size(), 4);
	float **matCoeficientesPtr = matCoeficientes.GetMatrixPtr();

	for (int i = 0; i < matCoeficientes.GetRows(); i++) {
		for (int j = 0; j < matCoeficientes.GetColumns(); j++) {
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

void MenuInterpolacion() {
	std::cout<<"Metodos de interpolacion\n"
	         << "1) Interpolacion por diferencias divididas\n"
	         << "2) Ajuste de curvas (Spline cubico)\n"
	         << "Ingresa la opcion deseada: " << std::endl;
}
