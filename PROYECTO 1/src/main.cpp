#include <iostream>
#include <vector>
#include "Token.h"
#include "LexicalAnalyzer.h"

int main() {
    // Simulamos una entrada compleja con todos los tipos de tokens requeridos
    std::string codigoPrueba = "cita: \"Carlos\" con \"Dr. Solis\" [fecha: 2025-04-10, hora: 11:30, especialidad: NEUROLOGIA, codigo: \"MED-002\", tipo_sangre: \"O-\"]";
    
    LexicalAnalyzer lexer(codigoPrueba);
    std::vector<Token> tokens = lexer.analyzeAll();
    
    std::cout << "--- TABLA DE TOKENS MEDLANG ---" << std::endl;
    for (const Token& t : tokens) {
        std::cout << t.toString() << " -> Tipo (Enum): " << static_cast<int>(t.getTipo()) << std::endl;
    }
    
    return 0;
}