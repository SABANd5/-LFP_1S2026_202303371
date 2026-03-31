cmake_minimum_required(VERSION 3.16)

# Nombre de tu proyecto
project(MedLexer VERSION 1.0 LANGUAGES CXX)

# Requisito obligatorio: C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuraciones mágicas de Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# --- AQUÍ ESTÁ EL FIX CON TU RUTA EXACTA ---
set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/llvm-mingw_64" CACHE PATH "Ruta a Qt")
# Buscamos la librería Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Le decimos a CMake qué archivos compilar
add_executable(MedLexer
    src/main.cpp
    src/MainWindow.cpp
    src/Token.cpp
    src/LexicalAnalyzer.cpp
    src/ErrorManager.cpp
    src/ReportGenerator.cpp
)

# Enlazamos nuestro código con las ventanas y botones de Qt
target_link_libraries(MedLexer PRIVATE Qt6::Widgets)