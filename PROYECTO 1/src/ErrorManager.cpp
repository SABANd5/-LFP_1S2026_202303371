#include "ErrorManager.h"

ErrorManager::ErrorManager()
{
    contadorErrores = 1; // El identificador incremental inicia en 1
}

void ErrorManager::agregarError(std::string lexema, std::string tipo, std::string descripcion, int linea, int col, std::string gravedad)
{
    ErrorLexico nuevoError = {contadorErrores++, lexema, tipo, descripcion, linea, col, gravedad};
    listaErrores.push_back(nuevoError);
}

std::vector<ErrorLexico> ErrorManager::getErrores() const
{
    return listaErrores;
}

bool ErrorManager::hayErrores() const
{
    return !listaErrores.empty();
}

void ErrorManager::imprimirErrores() const
{
    if (listaErrores.empty())
    {
        std::cout << "No se detectaron errores lexicos." << std::endl;
        return;
    }

    std::cout << "\n--- TABLA DE ERRORES LEXICOS ---" << std::endl;
    for (const auto &error : listaErrores)
    {
        std::cout << "No. " << error.numero
                  << " | Lexema: '" << error.lexema
                  << "' | Tipo: " << error.tipoError
                  << " | Desc: " << error.descripcion
                  << " | Linea: " << error.linea
                  << " | Col: " << error.columna
                  << " | Gravedad: " << error.gravedad << std::endl;
    }
}