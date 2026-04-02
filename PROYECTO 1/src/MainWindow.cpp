#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

// Incluimos tu motor léxico
#include "LexicalAnalyzer.h"
#include "ReportGenerator.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("MedLexer - Analizador Lexico");
    resize(900, 600);

    // 1. Crear los widgets (elementos visuales)
    txtCodigo = new QTextEdit(this);
    txtCodigo->setFontFamily("Courier"); // Letra de programador
    txtCodigo->setPlaceholderText("Escribe o carga tu código MedLang aquí...");

    txtConsola = new QTextEdit(this);
    txtConsola->setReadOnly(true); // La consola no se edita, solo muestra texto
    txtConsola->setStyleSheet("background-color: #1e1e1e; color: #00ff00; font-family: Courier;");
    txtConsola->setText(">> Consola MedLexer iniciada...\n");

    btnCargar = new QPushButton("Abrir Archivo", this);
    btnAnalizar = new QPushButton("Ejecutar Análisis", this);
    btnReportes = new QPushButton("Generar Reportes HTML", this);
    btnLimpiar = new QPushButton("Limpiar Todo", this);

    // 2. Organizar todo en la pantalla (Layouts)
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget(btnCargar);
    buttonLayout->addWidget(btnAnalizar);
    buttonLayout->addWidget(btnReportes);
    buttonLayout->addWidget(btnLimpiar);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(txtCodigo, 2);  // Ocupa 2/3 de la pantalla
    mainLayout->addWidget(txtConsola, 1); // Ocupa 1/3 de la pantalla

    setCentralWidget(centralWidget);

    // 3. Conectar los botones con sus funciones
    connect(btnCargar, &QPushButton::clicked, this, &MainWindow::cargarArchivo);
    connect(btnAnalizar, &QPushButton::clicked, this, &MainWindow::analizarCodigo);
    connect(btnReportes, &QPushButton::clicked, this, &MainWindow::generarReportes);
    connect(btnLimpiar, &QPushButton::clicked, this, &MainWindow::limpiar);
}

MainWindow::~MainWindow() {}

// --- FUNCIONES DE LOS BOTONES ---

void MainWindow::cargarArchivo()
{
    // Abre la ventana de Windows para buscar archivos
    QString fileName = QFileDialog::getOpenFileName(this, "Abrir Archivo MedLang", "", "Archivos Med (*.med);;Todos los archivos (*.*)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo");
        return;
    }

    QTextStream in(&file);
    txtCodigo->setPlainText(in.readAll());
    txtConsola->append(">> Archivo cargado: " + fileName);
    file.close();
}

void MainWindow::analizarCodigo()
{
    QString codigo = txtCodigo->toPlainText();
    if (codigo.isEmpty())
    {
        QMessageBox::information(this, "Aviso", "No hay código para analizar.");
        return;
    }

    // ¡Aquí llamamos a tu motor léxico!
    LexicalAnalyzer lexer(codigo.toUtf8().constData());
    std::vector<Token> tokens = lexer.analyzeAll();
    ErrorManager errores = lexer.getErrorManager();

    txtConsola->append("\n>> --- ANÁLISIS COMPLETADO ---");
    txtConsola->append(">> Tokens encontrados: " + QString::number(tokens.size()));
    txtConsola->append(">> Errores léxicos encontrados: " + QString::number(errores.getErrores().size()));

    if (errores.hayErrores())
    {
        txtConsola->append(">> [!] ADVERTENCIA: Se detectaron errores. Genera el reporte para ver los detalles.");
    }
    else
    {
        txtConsola->append(">> [OK] Análisis limpio. Estructura correcta.");
    }
}

void MainWindow::generarReportes()
{
    QString codigo = txtCodigo->toPlainText();
    if (codigo.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Analiza código primero antes de generar reportes.");
        return;
    }

    LexicalAnalyzer lexer(codigo.toUtf8().constData());
    std::vector<Token> tokens = lexer.analyzeAll();

    // --- NUEVA RUTA BASE ---
    // Usamos barras normales (/) porque C++ en Windows las prefiere para evitar errores de escape
    std::string rutaBase = "C:/Users/SabanD/Desktop/USAC/SEMESTRE 4/LAB LENGUAJES FORMALES/PROYECTOS/PROYECTO 1/reportes/";

    ReportGenerator::generarReporteTokens(tokens, rutaBase + "reporte_tokens.html");
    ReportGenerator::generarReporteErrores(lexer.getErrorManager(), rutaBase + "reporte_errores.html");
    ReportGenerator::generarReportePacientes(tokens, rutaBase + "reporte_pacientes.html");
    ReportGenerator::generarReporteMedicos(tokens, rutaBase + "reporte_medicos.html");
    ReportGenerator::generarReporteCitas(tokens, rutaBase + "reporte_citas.html");
    ReportGenerator::generarReporteEstadistico(tokens, rutaBase + "reporte_estadistico.html");
    ReportGenerator::generarDiagramaGraphviz(tokens, rutaBase + "hospital.dot");

    txtConsola->append(">> Reportes HTML y Diagrama exportados exitosamente.");
    QMessageBox::information(this, "Éxito", "Reportes generados en la carpeta 'reportes'.");
}

void MainWindow::limpiar()
{
    txtCodigo->clear();
    txtConsola->setText(">> Consola MedLexer iniciada...\n");
}