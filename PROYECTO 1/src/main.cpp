#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Token.h"
#include "LexicalAnalyzer.h"
#include "ReportGenerator.h" // NUEVO INCLUDE

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

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    LexicalAnalyzer lexer(content);
    std::vector<Token> tokens = lexer.analyzeAll();

    std::cout << "--- ANALISIS DE: " << filePath << " ---" << std::endl;
    
    // Imprimir en consola
    for (const auto& t : tokens) {
        if (t.getTipo() != TokenType::ERROR_LEXICO && t.getTipo() != TokenType::FIN_ARCHIVO) {
            std::cout << t.toString() << std::endl;
        }
    }
    lexer.getErrorManager().imprimirErrores();

    // NUEVO: Generar Reportes HTML en la carpeta del ejecutable
    std::cout << "\n--- GENERANDO REPORTES HTML ---" << std::endl;
    ReportGenerator::generarReporteTokens(tokens, "reporte_tokens.html");
    ReportGenerator::generarReporteErrores(lexer.getErrorManager(), "reporte_errores.html");

    return 0;
}