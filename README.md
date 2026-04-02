- **Interfaz Gráfica:** Qt 6 (compatible con MinGW, ej. `llvm-mingw_64`)
- **Editor:** Visual Studio Code con la extensión de CMake Tools
---
##  Instrucciones de Compilación (Paso a Paso)

El proyecto utiliza `CMakeLists.txt` para enlazar el código fuente con las librerías de Qt.

### 1. Clonar el repositorio y abrir la carpeta raíz

Abra la carpeta `PROYECTO 1` en Visual Studio Code.

### 2. Configurar el entorno de Qt en el sistema

Asegúrese de que la carpeta `bin` de su instalación de Qt esté agregada a la variable `Path` de Windows.

**Ejemplo:**
C:\Qt\6.x.x\llvm-mingw_64\bin

### 3. Configurar CMake

En la terminal integrada de VS Code o con CMake Tools:

cmake -S . -B build -G "MinGW Makefiles"
### 4. Compilar el proyecto (Build)
cmake --build build --config Debug --target all
### 5. Ejecutar la aplicación
.\build\MedLexer.exe
Instrucciones de Uso
Cargar archivo:
En la interfaz de MedLexer, haga clic en "Abrir Archivo" y seleccione un archivo .med (ejemplos en /examples).
Ejecutar análisis:
Haga clic en "Ejecutar Análisis". La consola mostrará el recuento de tokens y errores léxicos.
Generar reportes:
Haga clic en "Generar Reportes HTML".
Visualizar resultados:
Diríjase a /reportes y abra:
reporte_pacientes.html → Historial y triage
reporte_medicos.html → Carga laboral por especialidad
reporte_citas.html → Agenda y detección de conflictos
reporte_estadistico.html → KPIs y gráficas
Diagrama Graphviz:
El archivo hospital.dot puede visualizarse en herramientas como Graphviz Online.
Estructura del Repositorio
/src → Código fuente en C++ (.cpp, .h) y CMakeLists.txt
/docs → Manual Técnico, Manual de Usuario y Casos de Prueba
/tests → Archivos .med con errores para pruebas del modo pánico
/examples → Archivos .med válidos con múltiples validaciones
