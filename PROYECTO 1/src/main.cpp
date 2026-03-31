#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Token.h"
#include "LexicalAnalyzer.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Uso: ./MedLexer <ruta_archivo.med>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filePath << std::endl;
        return 1;
    }

    // Leer todo el contenido del archivo
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    LexicalAnalyzer lexer(content);
    std::vector<Token> tokens = lexer.analyzeAll();

    std::cout << "--- ANALISIS DE: " << filePath << " ---" << std::endl;
    std::cout << "--- TOKENS ENCONTRADOS ---" << std::endl;
    for (const auto& t : tokens) {
        if (t.getTipo() != TokenType::ERROR_LEXICO) {
            std::cout << t.toString() << std::endl;
        }
    }

    lexer.getErrorManager().imprimirErrores();

    return 0;
}
