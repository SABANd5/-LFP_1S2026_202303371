#include "Token.h"

Token::Token(std::string lexema, TokenType tipo, int linea, int columna)
    : lexema(lexema), tipo(tipo), linea(linea), columna(columna) {}

std::string Token::getLexema() const { return lexema; }
TokenType Token::getTipo() const { return tipo; }
int Token::getLinea() const { return linea; }
int Token::getColumna() const { return columna; }

std::string Token::toString() const
{
    return "Token[" + lexema + " | Linea: " + std::to_string(linea) +
           " | Col: " + std::to_string(columna) + "]";
}