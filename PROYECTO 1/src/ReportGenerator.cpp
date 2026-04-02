#include "ReportGenerator.h"
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <ctime> // Para timestamp en reportes estadísticos

std::string ReportGenerator::obtenerNombreTipoToken(TokenType tipo)
{
    switch (tipo)
    {
    case TokenType::PR_HOSPITAL:
        return "PR_HOSPITAL";
    case TokenType::PR_PACIENTES:
        return "PR_PACIENTES";
    case TokenType::PR_MEDICOS:
        return "PR_MEDICOS";
    case TokenType::PR_CITAS:
        return "PR_CITAS";
    case TokenType::PR_DIAGNOSTICOS:
        return "PR_DIAGNOSTICOS";
    case TokenType::PR_PACIENTE_ELEM:
        return "PR_PACIENTE_ELEM";
    case TokenType::PR_MEDICO_ELEM:
        return "PR_MEDICO_ELEM";
    case TokenType::PR_CITA_ELEM:
        return "PR_CITA_ELEM";
    case TokenType::PR_DIAGNOSTICO_ELEM:
        return "PR_DIAGNOSTICO_ELEM";
    case TokenType::CODIGO_ID:
        return "CODIGO_ID";
    case TokenType::FECHA:
        return "FECHA";
    case TokenType::HORA:
        return "HORA";
    case TokenType::ENTERO:
        return "ENTERO";
    case TokenType::CADENA:
        return "CADENA";
    case TokenType::ESPECIALIDAD:
        return "ESPECIALIDAD";
    case TokenType::FRECUENCIA_DOSIS:
        return "FRECUENCIA_DOSIS";
    case TokenType::TIPO_SANGRE:
        return "TIPO_SANGRE";
    case TokenType::LLAVE_ABRE:
        return "LLAVE_ABRE";
    case TokenType::LLAVE_CIERRA:
        return "LLAVE_CIERRA";
    case TokenType::CORCHETE_ABRE:
        return "CORCHETE_ABRE";
    case TokenType::CORCHETE_CIERRA:
        return "CORCHETE_CIERRA";
    case TokenType::DOS_PUNTOS:
        return "DOS_PUNTOS";
    case TokenType::COMA:
        return "COMA";
    case TokenType::PUNTO_COMA:
        return "PUNTO_COMA";
    case TokenType::CON:
        return "CON";
    case TokenType::ATRIBUTO:
        return "ATRIBUTO";
    case TokenType::FIN_ARCHIVO:
        return "FIN_ARCHIVO";
    default:
        return "ERROR_LEXICO";
    }
}

void ReportGenerator::generarReporteTokens(const std::vector<Token> &tokens, const std::string &rutaSalida)
{
    std::ofstream archivo(rutaSalida);
    if (!archivo.is_open())
    {
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
    for (const auto &t : tokens)
    {
        // Ignoramos los errores y el fin de archivo para este reporte
        if (t.getTipo() != TokenType::ERROR_LEXICO && t.getTipo() != TokenType::FIN_ARCHIVO)
        {
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

void ReportGenerator::generarReporteErrores(const ErrorManager &errorManager, const std::string &rutaSalida)
{
    std::ofstream archivo(rutaSalida);
    if (!archivo.is_open())
    {
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

    if (!errorManager.hayErrores())
    {
        archivo << "<h3 style='text-align:center; color:#27ae60;'>No se detectaron errores en el archivo analizado.</h3>\n";
    }
    else
    {
        archivo << "<table>\n"
                << "<tr><th>No.</th><th>Lexema</th><th>Tipo de Error</th><th>Descripción</th><th>Línea</th><th>Columna</th><th>Gravedad</th></tr>\n";

        std::vector<ErrorLexico> errores = errorManager.getErrores();
        for (const auto &err : errores)
        {
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
// ==========================================
// REPORTE 1: Historial de Pacientes
// ==========================================

// Estructuras auxiliares para organizar la información
struct DatosPaciente
{
    std::string nombre;
    std::string edad;
    std::string tipoSangre;
    std::string condicion = "Sin diagnóstico registrado";
    std::string medicamento = "-";
    int cantidadDiagnosticos = 0;
};

void ReportGenerator::generarReportePacientes(const std::vector<Token> &tokens, const std::string &rutaSalida)
{
    std::vector<DatosPaciente> listaPacientes;

    // 1. Extraer datos recorriendo los tokens
    for (size_t i = 0; i < tokens.size(); i++)
    {
        // Buscar pacientes
        if (tokens[i].getTipo() == TokenType::PR_PACIENTE_ELEM)
        {
            DatosPaciente p;
            // El nombre siempre está 2 posiciones adelante: paciente : "Nombre"
            if (i + 2 < tokens.size() && tokens[i + 2].getTipo() == TokenType::CADENA)
            {
                p.nombre = tokens[i + 2].getLexema();
                p.nombre = p.nombre.substr(1, p.nombre.length() - 2); // Quitar comillas
            }

            // Buscar edad y sangre dentro de los corchetes
            size_t j = i + 2;
            while (j < tokens.size() && tokens[j].getTipo() != TokenType::CORCHETE_CIERRA)
            {
                if (tokens[j].getLexema() == "edad" && j + 2 < tokens.size())
                {
                    p.edad = tokens[j + 2].getLexema();
                }
                else if (tokens[j].getLexema() == "tipo_sangre" && j + 2 < tokens.size())
                {
                    p.tipoSangre = tokens[j + 2].getLexema();
                    p.tipoSangre = p.tipoSangre.substr(1, p.tipoSangre.length() - 2);
                }
                j++;
            }
            listaPacientes.push_back(p);
        }

        // Buscar diagnósticos para enlazarlos con los pacientes
        if (tokens[i].getTipo() == TokenType::PR_DIAGNOSTICO_ELEM)
        {
            std::string nombrePacienteDiag = "";
            if (i + 2 < tokens.size() && tokens[i + 2].getTipo() == TokenType::CADENA)
            {
                nombrePacienteDiag = tokens[i + 2].getLexema();
                nombrePacienteDiag = nombrePacienteDiag.substr(1, nombrePacienteDiag.length() - 2);
            }

            std::string condicion = "", medicamento = "", dosis = "";
            size_t j = i + 2;
            while (j < tokens.size() && tokens[j].getTipo() != TokenType::CORCHETE_CIERRA)
            {
                if (tokens[j].getLexema() == "condicion" && j + 2 < tokens.size())
                {
                    condicion = tokens[j + 2].getLexema();
                    condicion = condicion.substr(1, condicion.length() - 2);
                }
                else if (tokens[j].getLexema() == "medicamento" && j + 2 < tokens.size())
                {
                    medicamento = tokens[j + 2].getLexema();
                    medicamento = medicamento.substr(1, medicamento.length() - 2);
                }
                else if (tokens[j].getLexema() == "dosis" && j + 2 < tokens.size())
                {
                    dosis = tokens[j + 2].getLexema();
                }
                j++;
            }

            // Enlazar diagnóstico con el paciente correspondiente
            for (auto &p : listaPacientes)
            {
                if (p.nombre == nombrePacienteDiag)
                {
                    p.condicion = condicion;
                    p.medicamento = medicamento + " / " + dosis;
                    p.cantidadDiagnosticos++;
                    break;
                }
            }
        }
    }

    // 2. Generar el HTML
    std::ofstream archivo(rutaSalida);
    if (!archivo.is_open())
        return;

    archivo << "<!DOCTYPE html>\n<html lang=\"es\">\n<head>\n<meta charset=\"UTF-8\">\n"
            << "<title>Historial de Pacientes</title>\n"
            << "<style>\n"
            << "body { font-family: 'Segoe UI', Arial, sans-serif; background-color: #f8f9fa; padding: 20px; }\n"
            << "h1 { color: #2c3e50; text-align: center; border-bottom: 2px solid #2E7D52; padding-bottom: 10px; }\n"
            << "table { width: 90%; margin: 20px auto; border-collapse: collapse; background: white; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }\n"
            << "th, td { padding: 12px 15px; border: 1px solid #dee2e6; text-align: center; }\n"
            << "th { background-color: #2E7D52; color: white; font-weight: bold; }\n"
            << "tr:nth-child(even) { background-color: #f2f2f2; }\n"
            << ".estado-activo { background-color: #d4edda; color: #155724; font-weight: bold; border-radius: 5px; padding: 5px; }\n"
            << ".estado-sindiag { background-color: #fff3cd; color: #856404; font-weight: bold; border-radius: 5px; padding: 5px; }\n"
            << ".estado-critico { background-color: #f8d7da; color: #721c24; font-weight: bold; border-radius: 5px; padding: 5px; }\n"
            << "</style>\n</head>\n<body>\n"
            << "<h1>Reporte 1: Historial de Pacientes</h1>\n"
            << "<table>\n"
            << "<tr><th>Paciente</th><th>Edad</th><th>Sangre</th><th>Diagnóstico Activo</th><th>Medicamento / Dosis</th><th>Estado</th></tr>\n";

    for (const auto &p : listaPacientes)
    {
        std::string claseEstado = "estado-sindiag";
        std::string textoEstado = "SIN DIAG.";

        if (p.cantidadDiagnosticos > 1)
        {
            claseEstado = "estado-critico";
            textoEstado = "CRÍTICO";
        }
        else if (p.cantidadDiagnosticos == 1)
        {
            claseEstado = "estado-activo";
            textoEstado = "ACTIVO";
        }

        archivo << "<tr>"
                << "<td>" << p.nombre << "</td>"
                << "<td>" << p.edad << "</td>"
                << "<td>" << p.tipoSangre << "</td>"
                << "<td>" << p.condicion << "</td>"
                << "<td>" << p.medicamento << "</td>"
                << "<td><span class='" << claseEstado << "'>" << textoEstado << "</span></td>"
                << "</tr>\n";
    }

    archivo << "</table>\n</body>\n</html>";
    archivo.close();
}

// ==========================================
// REPORTE 2: CARGA DE MÉDICOS
// ==========================================
struct DatosMedico
{
    std::string nombre;
    std::string codigo;
    std::string especialidad;
    int citasProgramadas = 0;
    std::vector<std::string> pacientesAtendidos;
};

void ReportGenerator::generarReporteMedicos(const std::vector<Token> &tokens, const std::string &rutaSalida)
{
    std::vector<DatosMedico> listaMedicos;

    // 1. Extraer Médicos
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].getTipo() == TokenType::PR_MEDICO_ELEM)
        {
            DatosMedico m;
            if (i + 2 < tokens.size() && tokens[i + 2].getTipo() == TokenType::CADENA)
            {
                m.nombre = tokens[i + 2].getLexema();
                m.nombre = m.nombre.substr(1, m.nombre.length() - 2);
            }
            size_t j = i + 2;
            while (j < tokens.size() && tokens[j].getTipo() != TokenType::CORCHETE_CIERRA)
            {
                if (tokens[j].getLexema() == "especialidad" && j + 2 < tokens.size())
                {
                    m.especialidad = tokens[j + 2].getLexema();
                }
                else if (tokens[j].getLexema() == "codigo" && j + 2 < tokens.size())
                {
                    m.codigo = tokens[j + 2].getLexema();
                    m.codigo = m.codigo.substr(1, m.codigo.length() - 2);
                }
                j++;
            }
            listaMedicos.push_back(m);
        }
    }

    // 2. Contar Citas y Pacientes por Médico
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].getTipo() == TokenType::PR_CITA_ELEM)
        {
            std::string pacienteCita = "", medicoCita = "";
            if (i + 2 < tokens.size() && tokens[i + 2].getTipo() == TokenType::CADENA)
            {
                pacienteCita = tokens[i + 2].getLexema();
                pacienteCita = pacienteCita.substr(1, pacienteCita.length() - 2);
            }
            if (i + 4 < tokens.size() && tokens[i + 4].getTipo() == TokenType::CADENA)
            {
                medicoCita = tokens[i + 4].getLexema();
                medicoCita = medicoCita.substr(1, medicoCita.length() - 2);
            }

            for (auto &m : listaMedicos)
            {
                if (m.nombre == medicoCita)
                {
                    m.citasProgramadas++;
                    // Agregar paciente solo si no está en la lista (para contar pacientes únicos)
                    bool existe = false;
                    for (const auto &p : m.pacientesAtendidos)
                    {
                        if (p == pacienteCita)
                        {
                            existe = true;
                            break;
                        }
                    }
                    if (!existe)
                        m.pacientesAtendidos.push_back(pacienteCita);
                    break;
                }
            }
        }
    }

    // 3. Generar HTML (Usaremos un color Morado para diferenciarlo)
    std::ofstream archivo(rutaSalida);
    if (!archivo.is_open())
        return;

    archivo << "<!DOCTYPE html>\n<html lang=\"es\">\n<head>\n<meta charset=\"UTF-8\">\n"
            << "<title>Carga de Médicos</title>\n"
            << "<style>\n"
            << "body { font-family: 'Segoe UI', Arial, sans-serif; background-color: #f8f9fa; padding: 20px; }\n"
            << "h1 { color: #2c3e50; text-align: center; border-bottom: 2px solid #8E44AD; padding-bottom: 10px; }\n"
            << "table { width: 90%; margin: 20px auto; border-collapse: collapse; background: white; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }\n"
            << "th, td { padding: 12px 15px; border: 1px solid #dee2e6; text-align: center; }\n"
            << "th { background-color: #8E44AD; color: white; font-weight: bold; }\n"
            << "tr:nth-child(even) { background-color: #f2f2f2; }\n"
            << ".carga-baja { background-color: #d1ecf1; color: #0c5460; font-weight: bold; padding: 5px; border-radius: 5px; }\n"
            << ".carga-normal { background-color: #d4edda; color: #155724; font-weight: bold; padding: 5px; border-radius: 5px; }\n"
            << ".carga-alta { background-color: #fff3cd; color: #856404; font-weight: bold; padding: 5px; border-radius: 5px; }\n"
            << ".carga-saturada { background-color: #f8d7da; color: #721c24; font-weight: bold; padding: 5px; border-radius: 5px; }\n"
            << "</style>\n</head>\n<body>\n"
            << "<h1>Reporte 2: Carga de Médicos por Especialidad</h1>\n"
            << "<table>\n"
            << "<tr><th>Médico</th><th>Código</th><th>Especialidad</th><th>Citas Prog.</th><th>Pacientes</th><th>Nivel de Carga</th></tr>\n";

    for (const auto &m : listaMedicos)
    {
        std::string claseCarga = "carga-baja";
        std::string textoCarga = "BAJA";

        if (m.citasProgramadas >= 9)
        {
            claseCarga = "carga-saturada";
            textoCarga = "SATURADA";
        }
        else if (m.citasProgramadas >= 7)
        {
            claseCarga = "carga-alta";
            textoCarga = "ALTA";
        }
        else if (m.citasProgramadas >= 4)
        {
            claseCarga = "carga-normal";
            textoCarga = "NORMAL";
        }

        archivo << "<tr>"
                << "<td>" << m.nombre << "</td>"
                << "<td>" << m.codigo << "</td>"
                << "<td>" << m.especialidad << "</td>"
                << "<td>" << m.citasProgramadas << "</td>"
                << "<td>" << m.pacientesAtendidos.size() << "</td>"
                << "<td><span class='" << claseCarga << "'>" << textoCarga << "</span></td>"
                << "</tr>\n";
    }
    archivo << "</table>\n</body>\n</html>";
    archivo.close();
}

// ==========================================
// REPORTE 3: AGENDA DE CITAS (Y CONFLICTOS)
// ==========================================
struct DatosCita
{
    std::string paciente;
    std::string medico;
    std::string fecha;
    std::string hora;
    bool tieneConflicto = false;
};

#include <ctime> // Incluido aquí para asegurar que funcione la fecha

void ReportGenerator::generarReporteCitas(const std::vector<Token> &tokens, const std::string &rutaSalida)
{
    std::vector<DatosCita> listaCitas;

    // 1. Extraer Citas
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].getTipo() == TokenType::PR_CITA_ELEM)
        {
            DatosCita c;
            if (i + 2 < tokens.size() && tokens[i + 2].getTipo() == TokenType::CADENA)
            {
                c.paciente = tokens[i + 2].getLexema();
                c.paciente = c.paciente.substr(1, c.paciente.length() - 2);
            }
            if (i + 4 < tokens.size() && tokens[i + 4].getTipo() == TokenType::CADENA)
            {
                c.medico = tokens[i + 4].getLexema();
                c.medico = c.medico.substr(1, c.medico.length() - 2);
            }

            size_t j = i + 4;
            while (j < tokens.size() && tokens[j].getTipo() != TokenType::CORCHETE_CIERRA)
            {
                if (tokens[j].getLexema() == "fecha" && j + 2 < tokens.size())
                {
                    c.fecha = tokens[j + 2].getLexema();
                }
                else if (tokens[j].getLexema() == "hora" && j + 2 < tokens.size())
                {
                    c.hora = tokens[j + 2].getLexema();
                }
                j++;
            }
            listaCitas.push_back(c);
        }
    }

    // 2. Detectar Conflictos (Mismo médico, misma fecha, misma hora)
    for (size_t i = 0; i < listaCitas.size(); i++)
    {
        for (size_t j = i + 1; j < listaCitas.size(); j++)
        {
            if (listaCitas[i].medico == listaCitas[j].medico &&
                listaCitas[i].fecha == listaCitas[j].fecha &&
                listaCitas[i].hora == listaCitas[j].hora)
            {
                listaCitas[i].tieneConflicto = true;
                listaCitas[j].tieneConflicto = true;
            }
        }
    }

    // 3. Obtener la fecha actual del sistema para validar "PENDIENTES"
    std::time_t t = std::time(nullptr);
    std::tm *now = std::localtime(&t);
    char buffer[15];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", now);
    std::string fechaActualHoy(buffer);

    // 4. Generar HTML (Usaremos Naranja Oscuro)
    std::ofstream archivo(rutaSalida);
    if (!archivo.is_open())
        return;

    archivo << "<!DOCTYPE html>\n<html lang=\"es\">\n<head>\n<meta charset=\"UTF-8\">\n"
            << "<title>Agenda de Citas</title>\n"
            << "<style>\n"
            << "body { font-family: 'Segoe UI', Arial, sans-serif; background-color: #f8f9fa; padding: 20px; }\n"
            << "h1 { color: #2c3e50; text-align: center; border-bottom: 2px solid #E67E22; padding-bottom: 10px; }\n"
            << "table { width: 90%; margin: 20px auto; border-collapse: collapse; background: white; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }\n"
            << "th, td { padding: 12px 15px; border: 1px solid #dee2e6; text-align: center; }\n"
            << "th { background-color: #E67E22; color: white; font-weight: bold; }\n"
            << "tr:nth-child(even) { background-color: #f2f2f2; }\n"
            << ".fila-conflicto { background-color: #f8d7da !important; color: #721c24; }\n" // El !important asegura que sobrescriba el color par/impar
            << ".estado-ok { background-color: #d4edda; color: #155724; font-weight: bold; padding: 5px; border-radius: 5px; }\n"
            << ".estado-error { background-color: #dc3545; color: white; font-weight: bold; padding: 5px; border-radius: 5px; }\n"
            << ".estado-pendiente { background-color: #ffe8cc; color: #d35400; font-weight: bold; padding: 5px; border-radius: 5px; }\n" // NUEVO ESTILO NARANJA
            << "</style>\n</head>\n<body>\n"
            << "<h1>Reporte 3: Agenda de Citas</h1>\n"
            << "<table>\n"
            << "<tr><th>Fecha</th><th>Hora</th><th>Paciente</th><th>Médico</th><th>Estado</th></tr>\n";

    for (const auto &c : listaCitas)
    {
        std::string claseFila = "";
        std::string claseEstado = "";
        std::string textoEstado = "";

        if (c.tieneConflicto)
        {
            claseFila = "fila-conflicto";
            claseEstado = "estado-error";
            textoEstado = "CONFLICTO";
        }
        else if (c.fecha > fechaActualHoy)
        {
            // NUEVO: Si la fecha es en el futuro, está PENDIENTE
            claseEstado = "estado-pendiente";
            textoEstado = "PENDIENTE";
        }
        else
        {
            claseEstado = "estado-ok";
            textoEstado = "CONFIRMADA";
        }

        archivo << "<tr class='" << claseFila << "'>"
                << "<td>" << c.fecha << "</td>"
                << "<td>" << c.hora << "</td>"
                << "<td>" << c.paciente << "</td>"
                << "<td>" << c.medico << "</td>"
                << "<td><span class='" << claseEstado << "'>" << textoEstado << "</span></td>"
                << "</tr>\n";
    }
    archivo << "</table>\n</body>\n</html>";
    archivo.close();
}
// ==========================================
// REPORTE 4: ESTADÍSTICO GENERAL
// ==========================================
struct InfoEspecialidad
{
    int medicos = 0;
    int citas = 0;
    std::set<std::string> pacientes;
};

void ReportGenerator::generarReporteEstadistico(const std::vector<Token> &tokens, const std::string &rutaSalida)
{
    // Variables para KPIs (Section A)
    int totalPacientes = 0, totalMedicos = 0, totalCitas = 0;
    int sumaEdades = 0;
    std::set<std::string> pacientesConDiagnostico;
    std::map<std::string, int> conteoMedicamentos;

    // Variables para cruzar datos
    std::map<std::string, std::string> medico_a_especialidad;
    std::map<std::string, int> citas_por_medico;

    // Variables para Section B
    std::map<std::string, InfoEspecialidad> statsEspecialidad;
    std::vector<DatosCita> listaCitasConflicto; // Reusamos la lógica básica del reporte 3

    // 1. PRIMER RECORRIDO: Extraer Pacientes y Médicos
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].getTipo() == TokenType::PR_PACIENTE_ELEM)
        {
            totalPacientes++;
            size_t j = i + 2;
            while (j < tokens.size() && tokens[j].getTipo() != TokenType::CORCHETE_CIERRA)
            {
                if (tokens[j].getLexema() == "edad" && j + 2 < tokens.size())
                {
                    sumaEdades += std::stoi(tokens[j + 2].getLexema());
                }
                j++;
            }
        }
        else if (tokens[i].getTipo() == TokenType::PR_MEDICO_ELEM)
        {
            totalMedicos++;
            std::string nombreMedico = "";
            if (i + 2 < tokens.size() && tokens[i + 2].getTipo() == TokenType::CADENA)
            {
                nombreMedico = tokens[i + 2].getLexema();
                nombreMedico = nombreMedico.substr(1, nombreMedico.length() - 2);
            }
            size_t j = i + 2;
            while (j < tokens.size() && tokens[j].getTipo() != TokenType::CORCHETE_CIERRA)
            {
                if (tokens[j].getLexema() == "especialidad" && j + 2 < tokens.size())
                {
                    std::string esp = tokens[j + 2].getLexema();
                    medico_a_especialidad[nombreMedico] = esp;
                    statsEspecialidad[esp].medicos++;
                }
                j++;
            }
        }
    }

    // 2. SEGUNDO RECORRIDO: Extraer Citas y Diagnósticos
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].getTipo() == TokenType::PR_CITA_ELEM)
        {
            totalCitas++;
            std::string paciente = "", medico = "", fecha = "", hora = "";
            if (i + 2 < tokens.size())
            {
                paciente = tokens[i + 2].getLexema();
                paciente = paciente.substr(1, paciente.length() - 2);
            }
            if (i + 4 < tokens.size())
            {
                medico = tokens[i + 4].getLexema();
                medico = medico.substr(1, medico.length() - 2);
            }

            size_t j = i + 4;
            while (j < tokens.size() && tokens[j].getTipo() != TokenType::CORCHETE_CIERRA)
            {
                if (tokens[j].getLexema() == "fecha" && j + 2 < tokens.size())
                    fecha = tokens[j + 2].getLexema();
                if (tokens[j].getLexema() == "hora" && j + 2 < tokens.size())
                    hora = tokens[j + 2].getLexema();
                j++;
            }

            // Guardar para conflictos
            DatosCita c;
            c.medico = medico;
            c.fecha = fecha;
            c.hora = hora;
            listaCitasConflicto.push_back(c);

            // Sumar a estadísticas
            citas_por_medico[medico]++;
            std::string esp = medico_a_especialidad[medico];
            statsEspecialidad[esp].citas++;
            statsEspecialidad[esp].pacientes.insert(paciente);
        }
        else if (tokens[i].getTipo() == TokenType::PR_DIAGNOSTICO_ELEM)
        {
            std::string paciente = "";
            if (i + 2 < tokens.size())
            {
                paciente = tokens[i + 2].getLexema();
                paciente = paciente.substr(1, paciente.length() - 2);
            }
            pacientesConDiagnostico.insert(paciente);

            size_t j = i + 2;
            while (j < tokens.size() && tokens[j].getTipo() != TokenType::CORCHETE_CIERRA)
            {
                if (tokens[j].getLexema() == "medicamento" && j + 2 < tokens.size())
                {
                    std::string med = tokens[j + 2].getLexema();
                    med = med.substr(1, med.length() - 2);
                    conteoMedicamentos[med]++;
                }
                j++;
            }
        }
    }

    // 3. CÁLCULOS MATEMÁTICOS FINALES
    // Promedio Edad
    double promedioEdad = (totalPacientes > 0) ? (double)sumaEdades / totalPacientes : 0.0;

    // Porcentaje diagnósticos
    double porcentajeDiag = (totalPacientes > 0) ? ((double)pacientesConDiagnostico.size() / totalPacientes) * 100.0 : 0.0;

    // Medicamento Top
    std::string medTop = "Ninguno";
    int maxMed = 0;
    for (const auto &par : conteoMedicamentos)
    {
        if (par.second > maxMed)
        {
            maxMed = par.second;
            medTop = par.first;
        }
    }

    // Médico y Especialidad Top
    std::string docTop = "Ninguno", espTop = "Ninguna";
    int maxCitasDoc = 0;
    for (const auto &par : citas_por_medico)
    {
        if (par.second > maxCitasDoc)
        {
            maxCitasDoc = par.second;
            docTop = par.first;
            espTop = medico_a_especialidad[par.first];
        }
    }

    // Conflictos
    int totalConflictos = 0;
    for (size_t i = 0; i < listaCitasConflicto.size(); i++)
    {
        for (size_t j = i + 1; j < listaCitasConflicto.size(); j++)
        {
            if (listaCitasConflicto[i].medico == listaCitasConflicto[j].medico &&
                listaCitasConflicto[i].fecha == listaCitasConflicto[j].fecha &&
                listaCitasConflicto[i].hora == listaCitasConflicto[j].hora)
            {
                totalConflictos++;
            }
        }
    }

    // 4. GENERAR HTML (Gris Oscuro Profesional)
    std::ofstream archivo(rutaSalida);
    if (!archivo.is_open())
        return;

    archivo << "<!DOCTYPE html>\n<html lang=\"es\">\n<head>\n<meta charset=\"UTF-8\">\n<title>Estadístico General</title>\n"
            << "<style>\n"
            << "body { font-family: 'Segoe UI', Arial, sans-serif; background-color: #f8f9fa; padding: 20px; }\n"
            << "h1, h2 { color: #343a40; border-bottom: 2px solid #6c757d; padding-bottom: 10px; }\n"
            << "table { width: 90%; margin: 20px auto; border-collapse: collapse; background: white; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }\n"
            << "th, td { padding: 12px 15px; border: 1px solid #dee2e6; text-align: left; }\n"
            << "th { background-color: #6c757d; color: white; font-weight: bold; }\n"
            << "tr:nth-child(even) { background-color: #f2f2f2; }\n"
            << ".prog-bg { width: 100%; background-color: #e9ecef; border-radius: 5px; overflow: hidden; }\n"
            << ".prog-bar { height: 20px; text-align: right; padding-right: 5px; color: white; font-weight: bold; font-size: 12px; line-height: 20px; }\n"
            << ".bar-normal { background-color: #28a745; }\n"
            << ".bar-peligro { background-color: #dc3545; }\n"
            << "</style>\n</head>\n<body>\n"
            << "<h1>Reporte 4: Estadístico General del Hospital</h1>\n"

            // SECCIÓN A: KPIs
            << "<h2>Sección A - Indicadores Clave (KPIs)</h2>\n<table>\n"
            << "<tr><th>Indicador</th><th>Valor</th></tr>\n"
            << "<tr><td>Nombre del hospital</td><td>Hospital MedLang</td></tr>\n"
            << "<tr><td>Total de pacientes registrados</td><td>" << totalPacientes << "</td></tr>\n"
            << "<tr><td>Total de médicos activos</td><td>" << totalMedicos << "</td></tr>\n"
            << "<tr><td>Total de citas programadas</td><td>" << totalCitas << "</td></tr>\n"
            << "<tr><td>Citas con conflicto de horario</td><td><b style='color:red;'>" << totalConflictos << "</b></td></tr>\n"
            << "<tr><td>Pacientes con diagnóstico activo</td><td>" << pacientesConDiagnostico.size() << " de " << totalPacientes << " (" << std::fixed << std::setprecision(1) << porcentajeDiag << "%)</td></tr>\n"
            << "<tr><td>Medicamento más prescrito</td><td>" << medTop << " (" << maxMed << " pacientes)</td></tr>\n"
            << "<tr><td>Especialidad con mayor carga de citas</td><td>" << espTop << " - " << docTop << " (" << maxCitasDoc << " citas)</td></tr>\n"
            << "<tr><td>Promedio de edad de los pacientes</td><td>" << std::fixed << std::setprecision(1) << promedioEdad << " años</td></tr>\n"
            << "</table>\n"

            // SECCIÓN B: Barras
            << "<h2>Sección B - Distribución de Carga por Especialidad</h2>\n<table>\n"
            << "<tr><th>Especialidad</th><th>Médicos</th><th>Citas</th><th>Pacientes Únicos</th><th>Barra de Ocupación</th></tr>\n";

    for (const auto &par : statsEspecialidad)
    {
        std::string esp = par.first;
        InfoEspecialidad info = par.second;
        double ocupacion = (totalCitas > 0) ? ((double)info.citas / totalCitas) * 100.0 : 0.0;

        std::string claseBarra = (ocupacion > 80.0) ? "bar-peligro" : "bar-normal";

        archivo << "<tr>"
                << "<td><b>" << esp << "</b></td>"
                << "<td>" << info.medicos << "</td>"
                << "<td>" << info.citas << "</td>"
                << "<td>" << info.pacientes.size() << "</td>"
                << "<td><div class='prog-bg'><div class='prog-bar " << claseBarra << "' style='width: " << ocupacion << "%;'>" << std::fixed << std::setprecision(1) << ocupacion << "%</div></div></td>"
                << "</tr>\n";
    }

    archivo << "</table>\n</body>\n</html>";
    archivo.close();
}

// ==========================================
// DIAGRAMA GRAPHVIZ (.dot) - A PRUEBA DE FALLOS
// ==========================================
void ReportGenerator::generarDiagramaGraphviz(const std::vector<Token>& tokens, const std::string& rutaSalida) {
    std::ofstream archivo(rutaSalida);
    if (!archivo.is_open()) return;

    // ESCUDO DE SEGURIDAD: Listas para evitar "Nodos Fantasma"
    std::set<std::string> pacientesRegistrados;
    std::set<std::string> medicosRegistrados;

    // Configuración inicial del grafo
    archivo << "digraph Hospital {\n"
            << "  rankdir=TB;\n"
            << "  node [shape=box, style=filled, fontname=\"Arial\"];\n\n"
            
            // Nodos principales del PDF
            << "  H [label=\"Hospital MedLang\", fillcolor=\"#1A4731\", fontcolor=white, shape=ellipse];\n"
            << "  P [label=\"PACIENTES\", fillcolor=\"#2E7D52\", fontcolor=white];\n"
            << "  M [label=\"MEDICOS\", fillcolor=\"#2E7D52\", fontcolor=white];\n"
            << "  C [label=\"CITAS\", fillcolor=\"#2E7D52\", fontcolor=white];\n"
            << "  D [label=\"DIAGNOSTICOS\", fillcolor=\"#2E7D52\", fontcolor=white];\n\n"
            
            << "  H -> P; H -> M; H -> C; H -> D;\n\n";

    int idDiagnostico = 1;

    for (size_t i = 0; i < tokens.size(); i++) {
        // --- 1. NODOS DE PACIENTES ---
        if (tokens[i].getTipo() == TokenType::PR_PACIENTE_ELEM && i + 2 < tokens.size()) {
            std::string nombre = tokens[i+2].getLexema();
            if (nombre.length() >= 2) nombre = nombre.substr(1, nombre.length() - 2);
            
            if (nombre.empty()) continue; // Evitar nodos vacíos o nulos
            pacientesRegistrados.insert(nombre); // Guardar como válido

            std::string sangre = "-", hab = "-";
            size_t j = i + 2;
            while (j < tokens.size() && tokens[j].getTipo() != TokenType::CORCHETE_CIERRA) {
                if (tokens[j].getLexema() == "tipo_sangre" && j + 2 < tokens.size()) {
                    sangre = tokens[j+2].getLexema();
                    if (sangre.length() >= 2) sangre = sangre.substr(1, sangre.length() - 2);
                } else if (tokens[j].getLexema() == "habitacion" && j + 2 < tokens.size()) {
                    hab = tokens[j+2].getLexema();
                }
                j++;
            }
            
            std::string labelP = nombre + "\\n" + sangre + " | Hab. " + hab;
            archivo << "  \"" << nombre << "\" [label=\"" << labelP << "\", fillcolor=\"#D4EDDA\"];\n";
            archivo << "  P -> \"" << nombre << "\";\n";
        }
        
        // --- 2. NODOS DE MÉDICOS ---
        else if (tokens[i].getTipo() == TokenType::PR_MEDICO_ELEM && i + 2 < tokens.size()) {
            std::string nombre = tokens[i+2].getLexema();
            if (nombre.length() >= 2) nombre = nombre.substr(1, nombre.length() - 2);
            
            if (nombre.empty()) continue; // Evitar nodos vacíos
            medicosRegistrados.insert(nombre); // Guardar como válido

            std::string esp = "-", cod = "-";
            size_t j = i + 2;
            while (j < tokens.size() && tokens[j].getTipo() != TokenType::CORCHETE_CIERRA) {
                if (tokens[j].getLexema() == "especialidad" && j + 2 < tokens.size()) esp = tokens[j+2].getLexema();
                else if (tokens[j].getLexema() == "codigo" && j + 2 < tokens.size()) {
                    cod = tokens[j+2].getLexema();
                    if (cod.length() >= 2) cod = cod.substr(1, cod.length() - 2);
                }
                j++;
            }
            
            std::string labelM = nombre + "\\n" + cod + " | " + esp;
            archivo << "  \"" << nombre << "\" [label=\"" << labelM << "\", fillcolor=\"#D6EAF8\"];\n";
            archivo << "  M -> \"" << nombre << "\";\n";
        }
        
        // --- 3. ARISTAS DE CITAS ---
        else if (tokens[i].getTipo() == TokenType::PR_CITA_ELEM && i + 4 < tokens.size()) {
            std::string paciente = tokens[i+2].getLexema();
            if (paciente.length() >= 2) paciente = paciente.substr(1, paciente.length() - 2);
            
            std::string medico = tokens[i+4].getLexema();
            if (medico.length() >= 2) medico = medico.substr(1, medico.length() - 2);
            
            // VALIDACIÓN: Si el paciente o médico no existen en nuestras listas, ignorar la cita
            if (pacientesRegistrados.find(paciente) == pacientesRegistrados.end() || 
                medicosRegistrados.find(medico) == medicosRegistrados.end()) {
                continue; 
            }

            std::string fecha = "-", hora = "-";
            size_t j = i + 4;
            while (j < tokens.size() && tokens[j].getTipo() != TokenType::CORCHETE_CIERRA) {
                if (tokens[j].getLexema() == "fecha" && j + 2 < tokens.size()) fecha = tokens[j+2].getLexema();
                if (tokens[j].getLexema() == "hora" && j + 2 < tokens.size()) hora = tokens[j+2].getLexema();
                j++;
            }
            
            std::string labelCita = fecha + "\\n" + hora;
            archivo << "  \"" << paciente << "\" -> \"" << medico << "\" [label=\"" << labelCita << "\", color=\"#E67E22\", style=dashed];\n";
        }
        
        // --- 4. NODOS DE DIAGNÓSTICOS ---
        else if (tokens[i].getTipo() == TokenType::PR_DIAGNOSTICO_ELEM && i + 2 < tokens.size()) {
            std::string paciente = tokens[i+2].getLexema();
            if (paciente.length() >= 2) paciente = paciente.substr(1, paciente.length() - 2);
            
            // VALIDACIÓN: Si el diagnóstico apunta a un paciente inválido o vacío, ignorarlo
            if (paciente.empty() || pacientesRegistrados.find(paciente) == pacientesRegistrados.end()) {
                continue;
            }

            std::string cond = "-", med = "-", dosis = "-";
            size_t j = i + 2;
            while (j < tokens.size() && tokens[j].getTipo() != TokenType::CORCHETE_CIERRA) {
                if (tokens[j].getLexema() == "condicion" && j + 2 < tokens.size()) {
                    cond = tokens[j+2].getLexema();
                    if (cond.length() >= 2) cond = cond.substr(1, cond.length() - 2);
                } else if (tokens[j].getLexema() == "medicamento" && j + 2 < tokens.size()) {
                    med = tokens[j+2].getLexema();
                    if (med.length() >= 2) med = med.substr(1, med.length() - 2);
                } else if (tokens[j].getLexema() == "dosis" && j + 2 < tokens.size()) {
                    dosis = tokens[j+2].getLexema();
                }
                j++;
            }
            
            std::string labelDiag = cond + "\\n" + med + " / " + dosis;
            std::string nombreNodoDiag = "Diag_" + std::to_string(idDiagnostico++);
            
            archivo << "  " << nombreNodoDiag << " [label=\"" << labelDiag << "\", fillcolor=\"#FDEBD0\"];\n";
            archivo << "  D -> " << nombreNodoDiag << ";\n";
            archivo << "  " << nombreNodoDiag << " -> \"" << paciente << "\" [label=\"diagnóstico activo\", color=\"#C0392B\"];\n";
        }
    }

    archivo << "}\n";
    archivo.close();
}