#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <string>
#include <vector>
#include "Token.h"
#include "ErrorManager.h"

class ReportGenerator {
public:
    // Reportes de la Fase 2 (Depuración)
    static void generarReporteTokens(const std::vector<Token>& tokens, const std::string& rutaSalida);
    static void generarReporteErrores(const ErrorManager& errorManager, const std::string& rutaSalida);

    // NUEVOS REPORTES MÉDICOS (Fase Final del PDF)
    static void generarReportePacientes(const std::vector<Token>& tokens, const std::string& rutaSalida);
    static void generarReporteMedicos(const std::vector<Token>& tokens, const std::string& rutaSalida);
    static void generarReporteCitas(const std::vector<Token>& tokens, const std::string& rutaSalida);
    static void generarReporteEstadistico(const std::vector<Token>& tokens, const std::string& rutaSalida);
    static void generarDiagramaGraphviz(const std::vector<Token>& tokens, const std::string& rutaSalida);

private:
    static std::string obtenerNombreTipoToken(TokenType tipo);
};

#endif // REPORTGENERATOR_H