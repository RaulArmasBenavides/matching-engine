#include <iostream>
#include <limits>   // numeric_limits
using namespace std;

// NODO DOBLEMENTE ENLAZADO
class Nodo {
public:
    int dato;
    Nodo* sig;
    Nodo* ant;

    Nodo(int d) : dato(d), sig(nullptr), ant(nullptr) {}
};

// LISTA DOBLEMENTE ENLAZADA
class Lista {
private:
    Nodo* h;  // head
    Nodo* t;  // tail

public:
    Lista() : h(nullptr), t(nullptr) {}

    void Inicializa();
    bool Vacia();
    int Tamano();
    void Mostrar();
    void InsertarInicio(int dato);
    void InsertarFinal(int dato);
    void InsertarPosicion(int dato, int posicion);
    void Eliminar(int posicion);
    void Anular();
    int Recupera(int posicion);
    int Localiza(int dato);
    void Ordena();
};

void Lista::Inicializa() {
    Anular();
    h = nullptr;
    t = nullptr;
}

bool Lista::Vacia() {
    return (h == nullptr && t == nullptr);
}

int Lista::Tamano() {
    int cont = 0;
    Nodo* aux = h;
    while (aux != nullptr) {
        cont++;
        aux = aux->sig;
    }
    return cont;
}

void Lista::Mostrar() {
    Nodo* aux = h;
    while (aux != nullptr) {
        cout << aux->dato << " ";
        aux = aux->sig;
    }
    cout << endl;
}

void Lista::InsertarInicio(int dato) {
    Nodo* tmp = new Nodo(dato);

    if (Vacia()) {
        h = t = tmp;
    }
    else {
        tmp->sig = h;
        h->ant = tmp;
        h = tmp;
    }
}

void Lista::InsertarFinal(int dato) {
    Nodo* tmp = new Nodo(dato);

    if (Vacia()) {
        h = t = tmp;
    }
    else {
        t->sig = tmp;
        tmp->ant = t;
        t = tmp;
    }
}

void Lista::InsertarPosicion(int dato, int posicion) {
    int n = Tamano();

    if (posicion < 1 || posicion > n + 1) {
        cout << "POSICION INVALIDA" << endl;
        return;
    }

    if (posicion == 1) {
        InsertarInicio(dato);
        return;
    }

    if (posicion == n + 1) {
        InsertarFinal(dato);
        return;
    }

    // Insertar en medio: antes del nodo que está en 'posicion'
    Nodo* aux = h;
    for (int i = 1; i < posicion; i++) {
        aux = aux->sig;         // al final aux apunta al nodo en 'posicion'
    }

    Nodo* tmp = new Nodo(dato);
    Nodo* previo = aux->ant;

    // Enlazar
    previo->sig = tmp;
    tmp->ant = previo;
    tmp->sig = aux;
    aux->ant = tmp;
}

void Lista::Eliminar(int posicion) {
    int n = Tamano();
    if (posicion < 1 || posicion > n) {
        cout << "Error, posicion invalida" << endl;
        return;
    }

    if (posicion == 1) {
        Nodo* aux = h;
        h = h->sig;
        if (h != nullptr) {
            h->ant = nullptr;
        }
        else {
            // la lista quedo vacia
            t = nullptr;
        }
        delete aux;
    }
    else if (posicion == n) {
        Nodo* aux = t;
        t = t->ant;
        if (t != nullptr) {
            t->sig = nullptr;
        }
        else {
            // la lista quedo vacia
            h = nullptr;
        }
        delete aux;
    }
    else {
        Nodo* aux = h;
        for (int i = 1; i < posicion; i++) {
            aux = aux->sig;
        }
        // aux apunta al nodo a eliminar
        Nodo* previo = aux->ant;
        Nodo* siguiente = aux->sig;

        previo->sig = siguiente;
        siguiente->ant = previo;

        delete aux;
    }
}

void Lista::Anular() {
    Nodo* aux;
    while (h != nullptr) {
        aux = h;
        h = h->sig;
        delete aux;
    }
    t = nullptr;
}

int Lista::Recupera(int posicion) {
    int n = Tamano();
    if (posicion < 1 || posicion > n) {
        return -1;
    }

    Nodo* aux = h;
    for (int i = 1; i < posicion; i++) {
        aux = aux->sig;
    }
    return aux->dato;
}

int Lista::Localiza(int dato) {
    int cont = 1;
    Nodo* aux = h;
    while (aux != nullptr) {
        if (aux->dato == dato) {
            return cont;
        }
        cont++;
        aux = aux->sig;
    }
    return -1;
}

void Lista::Ordena() {
    int n = Tamano();
    if (n < 2) return;

    // Bubble sort sobre la lista (solo intercambiamos datos)
    bool cambiado;
    do {
        cambiado = false;
        Nodo* aux = h;
        while (aux != nullptr && aux->sig != nullptr) {
            if (aux->dato > aux->sig->dato) {
                int temp = aux->dato;
                aux->dato = aux->sig->dato;
                aux->sig->dato = temp;
                cambiado = true;
            }
            aux = aux->sig;
        }
    } while (cambiado);
}

// FUNCION PRINCIPAL (MENÚ)
int main() {
    Lista l;
    l.Inicializa();
    int opc;

    do {
        cout << "1. Ingresar al principio" << endl;
        cout << "2. Ingresar al final" << endl;
        cout << "3. Ingresar en posicion" << endl;
        cout << "4. Mostrar" << endl;
        cout << "5. Recuperar" << endl;
        cout << "6. Localizar y Eliminar" << endl;
        cout << "7. Ordenar" << endl;
        cout << "8. Eliminar en posicion" << endl;
        cout << "9. Anular" << endl;
        cout << "10. Salir" << endl;
        cout << "Elija una opcion: ";
        cin >> opc;

        switch (opc) {
        case 1: {
            int num;
            cout << "Ingrese un numero: ";
            cin >> num;
            l.InsertarInicio(num);
            break;
        }
        case 2: {
            int num;
            cout << "Ingrese un numero: ";
            cin >> num;
            l.InsertarFinal(num);
            break;
        }
        case 3: {
            int num, pos;
            cout << "Ingrese un numero: ";
            cin >> num;
            cout << "Ingrese la posicion: ";
            cin >> pos;
            l.InsertarPosicion(num, pos);
            break;
        }
        case 4: {
            if (l.Vacia()) {
                cout << "La lista esta vacia" << endl;
            }
            else {
                cout << "Lista: ";
                l.Mostrar();
            }
            break;
        }
        case 5: {
            if (l.Vacia()) {
                cout << "La lista esta vacia" << endl;
            }
            else {
                int pos;
                cout << "Ingrese la posicion: ";
                cin >> pos;
                int valor = l.Recupera(pos);
                if (valor == -1) {
                    cout << "Posicion invalida" << endl;
                }
                else {
                    cout << "Elemento en posicion " << pos << ": " << valor << endl;
                }
            }
            break;
        }
        case 6: {
            if (l.Vacia()) {
                cout << "La lista esta vacia" << endl;
            }
            else {
                int num;
                cout << "Ingrese el elemento a buscar: ";
                cin >> num;
                int pos = l.Localiza(num);
                if (pos == -1) {
                    cout << "Ese elemento no existe" << endl;
                }
                else {
                    cout << "Elemento encontrado en posicion: " << pos << endl;
                    cout << "Eliminando elemento..." << endl;
                    l.Eliminar(pos);
                }
            }
            break;
        }
        case 7: {
            if (l.Vacia()) {
                cout << "La lista esta vacia" << endl;
            }
            else {
                l.Ordena();
                cout << "Elementos ordenados" << endl;
            }
            break;
        }
        case 8: {
            if (l.Vacia()) {
                cout << "La lista esta vacia" << endl;
            }
            else {
                int pos;
                cout << "Ingrese la posicion a eliminar: ";
                cin >> pos;
                l.Eliminar(pos);
            }
            break;
        }
        case 9: {
            if (l.Vacia()) {
                cout << "La lista esta vacia" << endl;
            }
            else {
                l.Anular();
                cout << "Todos los elementos han sido eliminados" << endl;
            }
            break;
        }
        case 10:
            cout << "Saliendo..." << endl;
            break;
        default:
            cout << "Opcion invalida" << endl;
            break;
        }

        // Limpiar buffer para evitar problemas si luego quieres pausar
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << endl;

    } while (opc != 10);

    return 0;
}
