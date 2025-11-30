#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <locale>
#include <limits>
#include "overload-test1.h"
using namespace std;
/* Sobrecargar una funcion enigma, que cuando reciba como parámetro
 un numero entero convierta un numero entero a un numero romano
 y cuando reciba como parámetro el nombre de una persona diga el numero de la
 suerte, que se calcula como el modulo de dicho numero entre 9
*/

// Definición de funciones
void Enigma(int n)
{
	int entero, res;
	cout << "Ingrese un numero" << endl;
	cin >> n;

	entero = n / 100;
	res = n % 100;

	switch (entero)
	{
		case 1: cout << "C";   break;
		case 2: cout << "CC";  break;
		case 3: cout << "CCC"; break;
		case 4: cout << "CD";  break;
		case 5: cout << "D";   break;
		case 6: cout << "DC";  break;
		case 7: cout << "DCC"; break;
		case 8: cout << "DCCC";break;
		case 9: cout << "CM";  break;
	}

	entero = res / 10;
	res = res % 10;

	switch (entero)
	{
		case 1: cout << "X";    break;
		case 2: cout << "XX";   break;
		case 3: cout << "XXX";  break;
		case 4: cout << "XL";   break;
		case 5: cout << "L";    break;
		case 6: cout << "LX";   break;
		case 7: cout << "LXX";  break;
		case 8: cout << "LXXX"; break;
		case 9: cout << "XC";   break;
	}

	switch (res)
	{
		case 1: cout << "I";    break;
		case 2: cout << "II";   break;
		case 3: cout << "III";  break;
		case 4: cout << "IV";   break;
		case 5: cout << "V";    break;
		case 6: cout << "VI";   break;
		case 7: cout << "VII";  break;
		case 8: cout << "VIII"; break;
		case 9: cout << "IX";   break;
	}

	cout << endl;
}

void Enigma(char x[], size_t size)
{
	int nro;
	cout << "Ingrese su nombre (1 solo nombre)" << endl;

	// Limpiamos el buffer antes de leer una línea
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.getline(x, static_cast<std::streamsize>(size));

	if (strlen(x) != 0)
	{
		nro = rand() % 9;
		cout << "El numero de suerte es " << nro << endl;
	}
	else
	{
		cout << "No hay nombre" << endl;
	}
}

void FuncionObjetivo(int nt[], int n)
{
	int aux = 0;
	for (int i = 0;i < n;i++)
	{
		nt[i] = 0 + rand() % 800;
		cout << nt[i];
		cout << endl;
	}
	cout << endl;
}

void FuncionObjetivo(int x[], int n, int impares[], int& nimpares, int& minimpar, bool ok)
{
	minimpar = 801;
	nimpares = 0;
	int i = 0;
	while (i < n)
	{
		if (x[i] % 2 == 0)
		{
			//son pares
			//b[n2]=x[i];
			//n2++;
		}
		else
		{
			//vectores
			impares[nimpares] = x[i];
			nimpares++;
		}
		i++;
	}

	minimpar = impares[0];
	//vector impares
	cout << "Hay numeros impares" << nimpares << endl;
	for (int j = 0;j < nimpares;j++)
	{
		if (minimpar > impares[j])
			minimpar = impares[j];

	}

	cout << "Este es el menor número impar " << minimpar << endl;
}

void FuncionObjetivo(int impares[], int& nimpares, int minimpar)
{
	int distancia = 0;
	//vector impares
	cout << endl;
	cout << "Reporte de distancias del vector de impartes " << endl;
	for (int j = 0;j < nimpares;j++)
	{
		distancia = impares[j] - minimpar;
		if (distancia != 0)
			cout << "Distancia " << distancia << endl;
		else
			cout << "Este es el menor número impar " << impares[j] << endl;

	}

}

void mostrarVector(int a[50], int n)
{
	cout << "Reporte de datos del vector" << endl;
	for (int i = 0;i < n;i++)
		cout << a[i] << endl;
}


float fibo(int n)
{
	if (n == 0 || n == 1)  
		return n;
	else
		return fibo(n - 1) + fibo(n - 2);
}

// Función principal
int main()
{
	setlocale(LC_CTYPE, "Spanish");
	srand(static_cast<unsigned>(time(nullptr)));

	int m = 10;
	char nom[20];

	Enigma(m);
	Enigma(nom, sizeof(nom));

	int nt[100];
	int impares[100];
	int n;
	int nimpares;
	int minimpar;


	n = 10;
	//invocacion de funciones
	FuncionObjetivo(nt, n);
	FuncionObjetivo(nt, n, impares, nimpares, minimpar, true);
	FuncionObjetivo(impares, nimpares, minimpar);
	return 0;
}
