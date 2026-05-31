#include "Kociemba.h"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <cadena_54_caracteres>" << std::endl;
        return 1;
    }

    std::string facelets = argv[1];
    
    try {
        std::cout << "Inicializando tablas..." << std::endl;
        KociembaSolver::initTables();
        std::cout << "Tablas inicializadas. Resolviendo..." << std::endl;
        std::vector<std::string> solution = KociembaSolver::solve(facelets);
        
        std::cout << "[";
        for (size_t i = 0; i < solution.size(); i++) {
            std::cout << "\"" << solution[i] << "\"";
            if (i < solution.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
        
    } catch (const InvalidCubeStateException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error inesperado: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
