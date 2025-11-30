// raii-test.cpp
// Demo simple de RAII: manejo seguro de archivo con FileGuard

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

class FileGuard {
public:
    explicit FileGuard(const std::string& path)
        : path_(path),
        stream_(path, std::ios::out | std::ios::app)
    {
        if (!stream_.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo: " + path_);
        }

        std::cout << "[FileGuard] Abriendo archivo: " << path_ << '\n';
    }

    // No copiamos (un archivo, un dueño)
    FileGuard(const FileGuard&) = delete;
    FileGuard& operator=(const FileGuard&) = delete;

    // Movimiento permitido (opcional, pero correcto)
    FileGuard(FileGuard&&) = default;
    FileGuard& operator=(FileGuard&&) = default;

    std::ofstream& stream() {
        return stream_;
    }

    ~FileGuard() {
        std::cout << "[FileGuard] Cerrando archivo: " << path_ << '\n';
        // std::ofstream se cierra solo en su destructor
    }

private:
    std::string   path_;
    std::ofstream stream_;
};

// Función que usa RAII y lanza una excepción a propósito
void procesar() {
    std::cout << "Entrando a procesar()\n";

    FileGuard file("raii-log.txt");
    file.stream() << "Escribiendo en el archivo antes de lanzar excepcion\n";

    std::cout << "Voy a lanzar una excepcion...\n";
    throw std::runtime_error("Algo salio mal en procesar()");
}

int main() {
    try {
        procesar();
    }
    catch (const std::exception& ex) {
        std::cout << "[main] Capturada excepcion: " << ex.what() << '\n';
    }

    std::cout << "[main] El programa sigue vivo y el archivo se cerro correctamente.\n";
    return 0;
}
