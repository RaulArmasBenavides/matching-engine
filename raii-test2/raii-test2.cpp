#include <iostream>
using namespace std;

struct prenda {
    int cod;
    char tipo;
    double prec, desc, pfin;
};

int main() {
    int n;
    cout << "Cantidad de prendas realmente usadas (n): ";
    cin >> n;

    // SIN control fino: arreglo fijo en el stack
    prenda fijo[1234];

    // CON control fino: arreglo dinamico en el heap
    prenda* dinamico = new prenda[n];

    size_t bytesPrenda = sizeof(prenda);
    size_t bytesFijo = sizeof(fijo);             // 20 * sizeof(prenda)
    size_t bytesDinam = bytesPrenda * n;          // n * sizeof(prenda)

    cout << "\n=== INFO DE MEMORIA ===\n";
    cout << "sizeof(prenda) = " << bytesPrenda << " bytes\n\n";

    cout << "Arreglo FIJO (sin control fino)\n";
    cout << "  Capacidad fija       = 1234 prendas\n";
    cout << "  Bytes reservados     = " << bytesFijo << " bytes\n";
    cout << "  Prendas realmente n  = " << n << "\n";
    if (n <= 1234) {
        size_t usados = bytesPrenda * n;
        size_t desperdicio = bytesFijo - usados;
        cout << "  Bytes usados         = " << usados << " bytes\n";
        cout << "  Bytes desperdiciados = " << desperdicio << " bytes\n";
    }
    else {
        cout << "  OJO: n > 1234, te saldrias del arreglo fijo (overflow)\n";
    }

    cout << "\nArreglo DINAMICO (con control fino)\n";
    cout << "  Capacidad dinamica   = " << n << " prendas\n";
    cout << "  Bytes reservados     = " << bytesDinam << " bytes\n";
    cout << "  Bytes desperdiciados = 0 (se Reserva exactamente n)\n\n";

    // Opcional: mostrar direcciones para ver que son contiguas
    if (n >= 2) {
        cout << "=== DIRECCIONES (contigüidad) ===\n";
        cout << "&fijo[0]     = " << static_cast<void*>(&fijo[0]) << "\n";
        cout << "&fijo[1]     = " << static_cast<void*>(&fijo[1]) << "\n";
        cout << "&dinamico[0] = " << static_cast<void*>(&dinamico[0]) << "\n";
        cout << "&dinamico[1] = " << static_cast<void*>(&dinamico[1]) << "\n";
    }

    delete[] dinamico;
    return 0;
}
