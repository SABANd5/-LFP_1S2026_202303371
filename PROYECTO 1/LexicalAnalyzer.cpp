#include "LexicalAnalyzer.h"
#include <iostream>

LexicalAnalyzer::LexicalAnalyzer(const std::string &source)
{
    sourceCode = source;
    currentPos = 0;
    currentLine = 1;
    currentCol = 1;
}

bool LexicalAnalyzer::isLetter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool LexicalAnalyzer::isDigit(char c)
{
    return (c >= '0' && c <= '9');
}

bool LexicalAnalyzer::isSpace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

// Catálogo completo de palabras reservadas y enumeraciones de MedLang
TokenType LexicalAnalyzer::clasificarPalabra(const std::string &lexema)
{
    if (lexema == "HOSPITAL")
        return TokenType::PR_HOSPITAL;
    if (lexema == "PACIENTES")
        return TokenType::PR_PACIENTES;
    if (lexema == "MEDICOS")
        return TokenType::PR_MEDICOS;
    if (lexema == "CITAS")
        return TokenType::PR_CITAS;
    if (lexema == "DIAGNOSTICOS")
        return TokenType::PR_DIAGNOSTICOS;

    if (lexema == "paciente")
        return TokenType::PR_PACIENTE_ELEM;
    if (lexema == "medico")
        return TokenType::PR_MEDICO_ELEM;
    if (lexema == "cita")
        return TokenType::PR_CITA_ELEM;
    if (lexema == "diagnostico")
        return TokenType::PR_DIAGNOSTICO_ELEM;
    if (lexema == "con")
        return TokenType::CON;

    // Enumeraciones de Especialidades
    if (lexema == "CARDIOLOGIA" || lexema == "NEUROLOGIA" || lexema == "PEDIATRIA" ||
        lexema == "CIRUGIA" || lexema == "MEDICINA_GENERAL" || lexema == "ONCOLOGIA")
    {
        return TokenType::ESPECIALIDAD;
    }

    // Enumeraciones de Dosis
    if (lexema == "DIARIA" || lexema == "CADA_8_HORAS" || lexema == "CADA_12_HORAS" || lexema == "SEMANAL")
    {
        return TokenType::FRECUENCIA_DOSIS;
    }
    if (lexema == "edad" || lexema == "tipo_sangre" || lexema == "habitacion" ||
        lexema == "especialidad" || lexema == "codigo" || lexema == "fecha" ||
        lexema == "hora" || lexema == "condicion" || lexema == "medicamento" ||
        lexema == "dosis")
    {
        return TokenType::ATRIBUTO;
    }
    return TokenType::ERROR_LEXICO;
}

Token LexicalAnalyzer::nextToken()
{
    int estado = 0;
    std::string lexema = "";
    int startLine = currentLine;
    int startCol = currentCol;

    while (currentPos < sourceCode.length())
    {
        char c = sourceCode[currentPos];

        if (estado == 0 && isSpace(c))
        {
            if (c == '\n')
            {
                currentLine++;
                currentCol = 1;
            }
            else
            {
                currentCol++;
            }
            currentPos++;
            startLine = currentLine;
            startCol = currentCol;
            continue;
        }

        switch (estado)
        {
        case 0: // ESTADO INICIAL (S0)
            if (c == '"')
            {
                estado = 1; // Transición a Cadena
                lexema += c;
                currentCol++;
                currentPos++;
            }
            else if (isDigit(c))
            {
                estado = 2; // Transición a Números (S2 de tu tabla)
                lexema += c;
                currentCol++;
                currentPos++;
            }
            else if (isLetter(c))
            {
                lexema += c;
                currentCol++;
                currentPos++;
                char next_c = (currentPos < sourceCode.length()) ? sourceCode[currentPos] : ' ';

                // Acumulamos letras, números y guiones para formar palabras o Códigos ID
                if (!isLetter(next_c) && !isDigit(next_c) && next_c != '_' && next_c != '-')
                {
                    if (lexema.find('-') != std::string::npos)
                    {
                        return Token(lexema, TokenType::CODIGO_ID, startLine, startCol);
                    }
                    else
                    {
                        return Token(lexema, clasificarPalabra(lexema), startLine, startCol);
                    }
                }
            }
            else
            {
                // Delimitadores estructurales requeridos
                lexema += c;
                TokenType tipo = TokenType::ERROR_LEXICO;
                if (c == '{')
                    tipo = TokenType::LLAVE_ABRE;
                else if (c == '}')
                    tipo = TokenType::LLAVE_CIERRA;
                else if (c == '[')
                    tipo = TokenType::CORCHETE_ABRE;
                else if (c == ']')
                    tipo = TokenType::CORCHETE_CIERRA;
                else if (c == ':')
                    tipo = TokenType::DOS_PUNTOS;
                else if (c == ',')
                    tipo = TokenType::COMA;
                else if (c == ';')
                    tipo = TokenType::PUNTO_COMA;

                currentCol++;
                currentPos++;
                return Token(lexema, tipo, startLine, startCol);
            }
            break;

        case 1: // CADENAS, TIPO DE SANGRE Y CÓDIGOS ID (S1)
            lexema += c;
            currentCol++;
            currentPos++;

            if (c == '"')
            {
                std::string valorStr = lexema.substr(1, lexema.length() - 2);
                
                // 1. Validamos si la cadena es un tipo de sangre restringido
                if (valorStr == "A+" || valorStr == "A-" || valorStr == "B+" || valorStr == "B-" ||
                    valorStr == "O+" || valorStr == "O-" || valorStr == "AB+" || valorStr == "AB-")
                {
                    return Token(lexema, TokenType::TIPO_SANGRE, startLine, startCol);
                }
                
                // --- AQUÍ PEGAMOS LA MODIFICACIÓN ---
                // 2. Validar Código ID (Formato: 3 letras + guión + dígitos)
                if (valorStr.length() >= 5 && valorStr[3] == '-') {
                    bool formatoValido = true;
                    // Validar 3 letras iniciales
                    for(int i = 0; i < 3; i++) {
                        if(!isLetter(valorStr[i])) formatoValido = false;
                    }
                    // Validar dígitos después del guión
                    for(size_t i = 4; i < valorStr.length(); i++) {
                        if(!isDigit(valorStr[i])) formatoValido = false;
                    }
                    
                    if (formatoValido) {
                        return Token(lexema, TokenType::CODIGO_ID, startLine, startCol);
                    }
                }
                // --- FIN DE LA MODIFICACIÓN ---

                // Si no fue sangre ni código, es una cadena genérica
                return Token(lexema, TokenType::CADENA, startLine, startCol);
            }
            else if (c == '\n')
            {
                currentLine++;
                currentCol = 1;
                return Token(lexema, TokenType::ERROR_LEXICO, startLine, startCol); // Error Crítico
            }
            break;

        case 2: // NÚMEROS ENTEROS (S2)
        {
            char next_c = (currentPos < sourceCode.length()) ? sourceCode[currentPos] : ' ';
            if (isDigit(next_c))
            {
                lexema += next_c;
                currentCol++;
                currentPos++;
            }
            else if (next_c == '-')
            {
                estado = 6; // Desviación hacia Fecha
                lexema += next_c;
                currentCol++;
                currentPos++;
            }
            else if (next_c == ':')
            {
                estado = 7; // Desviación hacia Hora
                lexema += next_c;
                currentCol++;
                currentPos++;
            }
            else
            {
                return Token(lexema, TokenType::ENTERO, startLine, startCol);
            }
        }
        break;

        case 6: // FECHAS AAAA-MM-DD (S6)
        {
            char next_c = (currentPos < sourceCode.length()) ? sourceCode[currentPos] : ' ';
            if (isDigit(next_c) || next_c == '-')
            {
                lexema += next_c;
                currentCol++;
                currentPos++;
            }
            else
            {
                return Token(lexema, TokenType::FECHA, startLine, startCol);
            }
        }
        break;

        case 7: // HORAS HH:MM (S7)
        {
            char next_c = (currentPos < sourceCode.length()) ? sourceCode[currentPos] : ' ';
            if (isDigit(next_c))
            {
                lexema += next_c;
                currentCol++;
                currentPos++;
            }
            else
            {
                return Token(lexema, TokenType::HORA, startLine, startCol);
            }
        }
        break;
        }
    }

    return Token("EOF", TokenType::FIN_ARCHIVO, currentLine, currentCol);
}

std::vector<Token> LexicalAnalyzer::analyzeAll()
{
    std::vector<Token> tokens;
    Token t = nextToken();
    while (t.getTipo() != TokenType::FIN_ARCHIVO)
    {
        tokens.push_back(t);
        t = nextToken();
    }
    return tokens;
}