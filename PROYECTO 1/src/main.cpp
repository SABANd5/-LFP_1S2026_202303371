#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    MainWindow window;
    window.show(); // Mostramos tu nueva y hermosa interfaz
    
    return app.exec();
}