#ifndef TOKEN_H
#define TOKEN_H

#include <string>

// Enumeración con todos los tipos de tokens requeridos por MedLang
enum class TokenType
{
    // Palabras reservadas estructurales
    PR_HOSPITAL,
    PR_PACIENTES,
    PR_MEDICOS,
    PR_CITAS,
    PR_DIAGNOSTICOS,

    // Palabras reservadas de elemento
    PR_PACIENTE_ELEM,
    PR_MEDICO_ELEM,
    PR_CITA_ELEM,
    PR_DIAGNOSTICO_ELEM,

    // Identificadores y Literales
    CODIGO_ID, // Ej: PAC-001, MED-023
    FECHA,     // AAAA-MM-DD
    HORA,      // HH:MM
    ENTERO,    // Números positivos
    CADENA,    // Texto entre comillas

    // Enumeraciones Médicas (se pueden manejar como un solo tipo o separados)
    ESPECIALIDAD,
    FRECUENCIA_DOSIS,
    TIPO_SANGRE,

    // Delimitadores y Símbolos
    LLAVE_ABRE,      // {
    LLAVE_CIERRA,    // }
    CORCHETE_ABRE,   // [
    CORCHETE_CIERRA, // ]
    DOS_PUNTOS,      // :
    COMA,            // ,
    PUNTO_COMA,      // ;
    CON,             // Palabra "con" usada en citas

    ATRIBUTO, // Nuevo: para edad, fecha, codigo, habitacion, etc.

    // Control
    ERROR_LEXICO,
    FIN_ARCHIVO // EOF
};

class Token
{
private:
    std::string lexema;
    TokenType tipo;
    int linea;
    int columna;

public:
    // Constructor
    Token(std::string lexema, TokenType tipo, int linea, int columna);

    // Getters
    std::string getLexema() const;
    TokenType getTipo() const;
    int getLinea() const;
    int getColumna() const;

    // Método auxiliar para imprimir (útil para depurar en consola)
    std::string toString() const;
};

#endif // TOKEN_H