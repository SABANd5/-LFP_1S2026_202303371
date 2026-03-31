#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots: // Estas son las funciones que se activan al hacer clic en los botones
    void cargarArchivo();
    void analizarCodigo();
    void generarReportes();
    void limpiar();

private:
    // Elementos visuales de la ventana
    QTextEdit *txtCodigo;
    QTextEdit *txtConsola;
    QPushButton *btnCargar;
    QPushButton *btnAnalizar;
    QPushButton *btnReportes;
    QPushButton *btnLimpiar;
};

#endif // MAINWINDOW_H