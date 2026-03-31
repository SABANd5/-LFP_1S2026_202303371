#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H

#include <string>
#include <vector>
#include <iostream>

// Estructura que cumple con todos los campos de la Sección 3.5 del manual
struct ErrorLexico {
    int numero;
    std::string lexema;
    std::string tipoError;
    std::string descripcion;
    int linea;
    int columna;
    std::string gravedad;
};

class ErrorManager {
private:
    std::vector<ErrorLexico> listaErrores;
    int contadorErrores;

public:
    // Constructor
    ErrorManager();

    // Método para registrar un nuevo error sin detener el análisis
    void agregarError(std::string lexema, std::string tipo, std::string descripcion, int linea, int col, std::string gravedad);

    // Getters y utilidades
    std::vector<ErrorLexico> getErrores() const;
    void imprimirErrores() const;
    bool hayErrores() const;
};

#endif // ERRORMANAGER_H