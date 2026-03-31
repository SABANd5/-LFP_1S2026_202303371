#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <string>
#include <vector>
#include "Token.h"
#include "ErrorManager.h"

class LexicalAnalyzer {
private:
    std::string sourceCode;
    int currentPos;
    int currentLine;
    int currentCol;
    
    ErrorManager errorManager; // Nuestro administrador de errores

    // Métodos auxiliares
    bool isLetter(char c);
    bool isDigit(char c);
    bool isSpace(char c);
    TokenType clasificarPalabra(const std::string& lexema);

public:
    LexicalAnalyzer(const std::string& source);
    Token nextToken();
    std::vector<Token> analyzeAll();
    
    // Método para extraer los errores después de analizar todo
    ErrorManager getErrorManager() const;
};

#endif // LEXICALANALYZER_H