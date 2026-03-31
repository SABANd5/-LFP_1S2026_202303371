#include "LexicalAnalyzer.h"
#include <iostream>

LexicalAnalyzer::LexicalAnalyzer(const std::string &source)
{
    sourceCode = source;
    currentPos = 0;
    currentLine = 1;
    currentCol = 1;
}

ErrorManager LexicalAnalyzer::getErrorManager() const
{
    return errorManager;
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

    if (lexema == "CARDIOLOGIA" || lexema == "NEUROLOGIA" || lexema == "PEDIATRIA" ||
        lexema == "CIRUGIA" || lexema == "MEDICINA_GENERAL" || lexema == "ONCOLOGIA")
    {
        return TokenType::ESPECIALIDAD;
    }

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
        case 0:
            if (c == '"')
            {
                estado = 1;
                lexema += c;
                currentCol++;
                currentPos++;
            }
            else if (isDigit(c))
            {
                estado = 2;
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

                if (!isLetter(next_c) && !isDigit(next_c) && next_c != '_' && next_c != '-')
                {
                    if (lexema.find('-') != std::string::npos)
                    {
                        return Token(lexema, TokenType::CODIGO_ID, startLine, startCol);
                    }
                    else
                    {
                        TokenType tipo = clasificarPalabra(lexema);
                        if (tipo == TokenType::ERROR_LEXICO)
                        {
                            // REGISTRAMOS EL ERROR: Palabra no reconocida
                            errorManager.agregarError(lexema, "Palabra no reconocida", "El lexema no pertenece a las palabras reservadas o enumeraciones.", startLine, startCol, "ERROR");
                        }
                        return Token(lexema, tipo, startLine, startCol);
                    }
                }
            }
            else
            {
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
                else
                {
                    // REGISTRAMOS EL ERROR: Carácter ilegal (ej. @, $, %)
                    errorManager.agregarError(lexema, "Caracter Ilegal", "El caracter introducido no es valido en MedLang.", startLine, startCol, "ERROR");
                }

                currentCol++;
                currentPos++;
                return Token(lexema, tipo, startLine, startCol);
            }
            break;

        case 1:
            lexema += c;
            currentCol++;
            currentPos++;

            if (c == '"')
            {
                std::string valorStr = lexema.substr(1, lexema.length() - 2);

                // 1. Validar Tipo de Sangre correcto
                if (valorStr == "A+" || valorStr == "A-" || valorStr == "B+" || valorStr == "B-" ||
                    valorStr == "O+" || valorStr == "O-" || valorStr == "AB+" || valorStr == "AB-")
                {
                    return Token(lexema, TokenType::TIPO_SANGRE, startLine, startCol);
                }
                // --- AQUÍ ENTRA LA MODIFICACIÓN 1 (Tipos de sangre inválidos como "Z+") ---
                else if (valorStr.length() >= 1 && valorStr.length() <= 3 &&
                         (valorStr.back() == '+' || valorStr.back() == '-'))
                {
                    errorManager.agregarError(lexema, "Tipo de sangre invalido", "No es un tipo de sangre reconocido. Use A+, O-, etc.", startLine, startCol, "ERROR");
                    return Token(lexema, TokenType::ERROR_LEXICO, startLine, startCol);
                }
                // --------------------------------------------------------------------------

                // 2. Validar Código ID
                if (valorStr.length() >= 5 && valorStr[3] == '-')
                {
                    bool formatoValido = true;
                    for (int i = 0; i < 3; i++)
                    {
                        if (!isLetter(valorStr[i]))
                            formatoValido = false;
                    }
                    for (size_t i = 4; i < valorStr.length(); i++)
                    {
                        if (!isDigit(valorStr[i]))
                            formatoValido = false;
                    }

                    if (formatoValido)
                        return Token(lexema, TokenType::CODIGO_ID, startLine, startCol);
                }

                return Token(lexema, TokenType::CADENA, startLine, startCol);
            }
            else if (c == '\n')
            {
                // REGISTRAMOS EL ERROR CRÍTICO: Cadena sin cerrar antes del salto de línea
                errorManager.agregarError(lexema, "Cadena sin cerrar", "Se encontro un salto de linea antes de cerrar la cadena.", startLine, startCol, "CRITICO");
                currentLine++;
                currentCol = 1;
                return Token(lexema, TokenType::ERROR_LEXICO, startLine, startCol);
            }
            break;

        case 2:
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
                estado = 6;
                lexema += next_c;
                currentCol++;
                currentPos++;
            }
            else if (next_c == ':')
            {
                estado = 7;
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
                // VALIDACIÓN ESTRICTA DE FECHA
                bool formatoValido = (lexema.length() == 10 && lexema[4] == '-' && lexema[7] == '-');
                if (formatoValido)
                {
                    try
                    {
                        int mes = std::stoi(lexema.substr(5, 2));
                        int dia = std::stoi(lexema.substr(8, 2));

                        if (mes >= 1 && mes <= 12 && dia >= 1 && dia <= 31)
                        {
                            return Token(lexema, TokenType::FECHA, startLine, startCol);
                        }
                        else
                        {
                            errorManager.agregarError(lexema, "Fecha con mes/dia invalido", "El mes (01-12) o dia (01-31) esta fuera del rango. Formato: AAAA-MM-DD.", startLine, startCol, "ERROR");
                            return Token(lexema, TokenType::ERROR_LEXICO, startLine, startCol);
                        }
                    }
                    catch (...)
                    {
                        formatoValido = false;
                    }
                }

                // Si no cumple la longitud o falla el stoi
                if (!formatoValido)
                {
                    errorManager.agregarError(lexema, "Formato de fecha invalido", "Se esperaba el formato AAAA-MM-DD.", startLine, startCol, "ERROR");
                    return Token(lexema, TokenType::ERROR_LEXICO, startLine, startCol);
                }
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
                // VALIDACIÓN ESTRICTA DE HORA
                bool formatoValido = (lexema.length() == 5 && lexema[2] == ':');
                if (formatoValido)
                {
                    try
                    {
                        int hh = std::stoi(lexema.substr(0, 2));
                        int mm = std::stoi(lexema.substr(3, 2));

                        if (hh >= 0 && hh <= 23 && mm >= 0 && mm <= 59)
                        {
                            return Token(lexema, TokenType::HORA, startLine, startCol);
                        }
                        else
                        {
                            errorManager.agregarError(lexema, "Hora fuera de rango", "La hora esta fuera del rango permitido (00:00 - 23:59).", startLine, startCol, "ERROR");
                            return Token(lexema, TokenType::ERROR_LEXICO, startLine, startCol);
                        }
                    }
                    catch (...)
                    {
                        formatoValido = false;
                    }
                }

                // Si no cumple la longitud o falla el stoi
                if (!formatoValido)
                {
                    errorManager.agregarError(lexema, "Formato de hora invalido", "Se esperaba el formato HH:MM.", startLine, startCol, "ERROR");
                    return Token(lexema, TokenType::ERROR_LEXICO, startLine, startCol);
                }
            }
        }
        break;
        } // Fin del switch
    } // Fin del while

    // --- MODIFICACIÓN 2: EOF inesperado dejando cadena abierta ---
    if (estado == 1) {
        errorManager.agregarError(lexema, "Cadena sin cerrar", "Fin de archivo inesperado antes de cerrar la cadena.", startLine, startCol, "CRITICO");
        return Token(lexema, TokenType::ERROR_LEXICO, startLine, startCol);
    }
    // -------------------------------------------------------------

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