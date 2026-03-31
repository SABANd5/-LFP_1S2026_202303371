#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <string>
#include <vector>
#include "Token.h"
#include "ErrorManager.h"

class ReportGenerator {
public:
    // Métodos estáticos para no tener que instanciar la clase
    static void generarReporteTokens(const std::vector<Token>& tokens, const std::string& rutaSalida);
    static void generarReporteErrores(const ErrorManager& errorManager, const std::string& rutaSalida);

private:
    // Método auxiliar para convertir el Enum a un texto legible para la tabla
    static std::string obtenerNombreTipoToken(TokenType tipo);
};

#endif // REPORTGENERATOR_H