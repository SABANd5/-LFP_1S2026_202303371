#include "ReportGenerator.h"
#include <fstream>
#include <iostream>

std::string ReportGenerator::obtenerNombreTipoToken(TokenType tipo) {
    switch (tipo) {
        case TokenType::PR_HOSPITAL: return "PR_HOSPITAL";
        case TokenType::PR_PACIENTES: return "PR_PACIENTES";
        case TokenType::PR_MEDICOS: return "PR_MEDICOS";
        case TokenType::PR_CITAS: return "PR_CITAS";
        case TokenType::PR_DIAGNOSTICOS: return "PR_DIAGNOSTICOS";
        case TokenType::PR_PACIENTE_ELEM: return "PR_PACIENTE_ELEM";
        case TokenType::PR_MEDICO_ELEM: return "PR_MEDICO_ELEM";
        case TokenType::PR_CITA_ELEM: return "PR_CITA_ELEM";
        case TokenType::PR_DIAGNOSTICO_ELEM: return "PR_DIAGNOSTICO_ELEM";
        case TokenType::CODIGO_ID: return "CODIGO_ID";
        case TokenType::FECHA: return "FECHA";
        case TokenType::HORA: return "HORA";
        case TokenType::ENTERO: return "ENTERO";
        case TokenType::CADENA: return "CADENA";
        case TokenType::ESPECIALIDAD: return "ESPECIALIDAD";
        case TokenType::FRECUENCIA_DOSIS: return "FRECUENCIA_DOSIS";
        case TokenType::TIPO_SANGRE: return "TIPO_SANGRE";
        case TokenType::LLAVE_ABRE: return "LLAVE_ABRE";
        case TokenType::LLAVE_CIERRA: return "LLAVE_CIERRA";
        case TokenType::CORCHETE_ABRE: return "CORCHETE_ABRE";
        case TokenType::CORCHETE_CIERRA: return "CORCHETE_CIERRA";
        case TokenType::DOS_PUNTOS: return "DOS_PUNTOS";
        case TokenType::COMA: return "COMA";
        case TokenType::PUNTO_COMA: return "PUNTO_COMA";
        case TokenType::CON: return "CON";
        case TokenType::ATRIBUTO: return "ATRIBUTO";
        case TokenType::FIN_ARCHIVO: return "FIN_ARCHIVO";
        default: return "ERROR_LEXICO";
    }
}

void ReportGenerator::generarReporteTokens(const std::vector<Token>& tokens, const std::string& rutaSalida) {
    std::ofstream archivo(rutaSalida);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo crear el reporte de tokens en " << rutaSalida << std::endl;
        return;
    }

    archivo << "<!DOCTYPE html>\n<html lang=\"es\">\n<head>\n"
            << "<meta charset=\"UTF-8\">\n<title>Reporte de Tokens</title>\n"
            << "<style>\n"
            << "body { font-family: Arial, sans-serif; background-color: #f4f7f6; margin: 20px; }\n"
            << "h1 { color: #2c3e50; text-align: center; }\n"
            << "table { width: 80%; margin: 0 auto; border-collapse: collapse; background: #fff; box-shadow: 0 0 10px rgba(0,0,0,0.1); }\n"
            << "th, td { padding: 12px; border: 1px solid #ddd; text-align: center; }\n"
            << "th { background-color: #3498db; color: white; }\n"
            << "tr:nth-child(even) { background-color: #f2f2f2; }\n"
            << "tr:hover { background-color: #e8f4f8; }\n"
            << "</style>\n</head>\n<body>\n"
            << "<h1>Reporte de Tokens - MedLang</h1>\n"
            << "<table>\n"
            << "<tr><th>No.</th><th>Lexema</th><th>Tipo</th><th>Línea</th><th>Columna</th></tr>\n";

    int contador = 1;
    for (const auto& t : tokens) {
        // Ignoramos los errores y el fin de archivo para este reporte
        if (t.getTipo() != TokenType::ERROR_LEXICO && t.getTipo() != TokenType::FIN_ARCHIVO) {
            archivo << "<tr>"
                    << "<td>" << contador++ << "</td>"
                    << "<td>" << t.getLexema() << "</td>"
                    << "<td>" << obtenerNombreTipoToken(t.getTipo()) << "</td>"
                    << "<td>" << t.getLinea() << "</td>"
                    << "<td>" << t.getColumna() << "</td>"
                    << "</tr>\n";
        }
    }

    archivo << "</table>\n</body>\n</html>";
    archivo.close();
    std::cout << "Reporte de tokens generado exitosamente: " << rutaSalida << std::endl;
}

void ReportGenerator::generarReporteErrores(const ErrorManager& errorManager, const std::string& rutaSalida) {
    std::ofstream archivo(rutaSalida);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo crear el reporte de errores en " << rutaSalida << std::endl;
        return;
    }

    archivo << "<!DOCTYPE html>\n<html lang=\"es\">\n<head>\n"
            << "<meta charset=\"UTF-8\">\n<title>Reporte de Errores Léxicos</title>\n"
            << "<style>\n"
            << "body { font-family: Arial, sans-serif; background-color: #fef9f9; margin: 20px; }\n"
            << "h1 { color: #c0392b; text-align: center; }\n"
            << "table { width: 90%; margin: 0 auto; border-collapse: collapse; background: #fff; box-shadow: 0 0 10px rgba(0,0,0,0.1); }\n"
            << "th, td { padding: 12px; border: 1px solid #ddd; text-align: center; }\n"
            << "th { background-color: #e74c3c; color: white; }\n"
            << "tr:nth-child(even) { background-color: #fdf2f2; }\n"
            << ".critico { font-weight: bold; color: #c0392b; }\n"
            << "</style>\n</head>\n<body>\n"
            << "<h1>Reporte de Errores Léxicos - MedLang</h1>\n";

    if (!errorManager.hayErrores()) {
        archivo << "<h3 style='text-align:center; color:#27ae60;'>No se detectaron errores en el archivo analizado.</h3>\n";
    } else {
        archivo << "<table>\n"
                << "<tr><th>No.</th><th>Lexema</th><th>Tipo de Error</th><th>Descripción</th><th>Línea</th><th>Columna</th><th>Gravedad</th></tr>\n";

        std::vector<ErrorLexico> errores = errorManager.getErrores();
        for (const auto& err : errores) {
            std::string claseCSS = (err.gravedad == "CRITICO") ? "class='critico'" : "";
            archivo << "<tr " << claseCSS << ">"
                    << "<td>" << err.numero << "</td>"
                    << "<td>" << err.lexema << "</td>"
                    << "<td>" << err.tipoError << "</td>"
                    << "<td>" << err.descripcion << "</td>"
                    << "<td>" << err.linea << "</td>"
                    << "<td>" << err.columna << "</td>"
                    << "<td>" << err.gravedad << "</td>"
                    << "</tr>\n";
        }
        archivo << "</table>\n";
    }

    archivo << "</body>\n</html>";
    archivo.close();
    std::cout << "Reporte de errores generado exitosamente: " << rutaSalida << std::endl;
}