#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <string>
#include <vector>
#include "Token.h"

class LexicalAnalyzer {
private:
    std::string sourceCode;
    int currentPos;
    int currentLine;
    int currentCol;

    // Métodos auxiliares
    bool isLetter(char c);
    bool isDigit(char c);
    bool isSpace(char c);
    
    // Nuevo: Nos ayudará a clasificar las palabras acumuladas en S0
    TokenType clasificarPalabra(const std::string& lexema);

public:
    LexicalAnalyzer(const std::string& source);
    Token nextToken();
    std::vector<Token> analyzeAll();
};

#endif // LEXICALANALYZER_H