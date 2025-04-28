#include <QCoreApplication>
#include <iostream>
#include "procesamiento.h"
#include "bit_operations.h"

/*
 * Ruta para commits:
 * cd "C:\Users\FELIPE\OneDrive - Universidad de Antioquia\PERSONAL\UDEA\2025-1\Cursos\Informática II\Desafío 1\Solucion\Desafio1_InfoII_JuanOrozco_Davielys"
*/

using namespace std;
/*
//Funciones auxiliares para pruebas especificas.
void printBits(unsigned char byte); //Imprime todos los bits luego de probar la rotacion
void imprimirRGB(const unsigned char* data, const string& mensaje); //Imprimir los bits en una prueba de desplazamiento
            //especifica para un pixel.
*/

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv); //Se usa debido a la creacion del proyecto que al revisar las funciones
                                    //proporcionadas en la guía se encuentra utilizada Qimage, Qstring...
                                    //Por lo tanto se crea el proyecto como una aplicacion de QT.
                                    //Sin embargo no se usa ninguna de estas, solo para las pruebas individuales
                                    //Para la carga y exporte de iamgenes y la carga de lso archivos .txt

    cout << "\n==========================================\n";
    cout << "*** FUNCIONAMIENTO PRINCIPAL DEL DESAFIO: ***\n";
    cout << "==========================================\n";

    #define MAX_PASOS 10 //Se define un numero maximo de transformaciones como global para poder modificarlo

    // Arreglo de punteros a char para registrar las transformaciones detectadas.
    char* registroTransformaciones[MAX_PASOS] = {nullptr};

    // Path base para importar
    const char* basePath = "C:/Caso2/"; // O "C:/Caso2/"

    unsigned int* archivosTxt[MAX_PASOS];//Punteros a arrays con datos de Mx.txt
    int semillas[MAX_PASOS]; //Semillas de .txt
    int altos[MAX_PASOS]; //Dimensiones de la mascara
    int anchos[MAX_PASOS];
    int pixelesLeidos[MAX_PASOS]; //Pixeles leidos en cada archivo

    // Variables auxiliares
    unsigned char* mascara = nullptr;
    unsigned char* imagenAleatoria = nullptr;
    int widthM = 0, heightM = 0;
    int widthIM = 0, heightIM = 0;

    // Paso 1: Cargar la máscara
    char rutaMascara[256];
    sprintf(rutaMascara, "%sM.bmp", basePath); //Armar ruta completa a partir de la base
    mascara = loadPixels(rutaMascara, widthM, heightM);
    if (!mascara) {
        cout << "Error al cargar M.bmp" << endl;
        return -1;
    }

    // Paso 2: Cargar imagen aleatoria IM
    char rutaIM[256];
    sprintf(rutaIM, "%sI_M.bmp", basePath);
    imagenAleatoria = loadPixels(rutaIM, widthIM, heightIM);
    if (!imagenAleatoria) {
        cout << "Error al cargar I_M.bmp" << endl;
        delete[] mascara;
        return -1;
    }

    // Paso 3: Cargar archivos Mx.txt
    int pasos = 6; // 2 para Caso1, 6 para Caso2

    for (int i = 0; i < pasos; ++i) {
        char nombreArchivo[256];
        sprintf(nombreArchivo, "%sM%d.txt", basePath, i);

        semillas[i] = 0;
        archivosTxt[i] = loadSeedMasking(nombreArchivo, semillas[i], pixelesLeidos[i]);

        if (!archivosTxt[i]) {
            cout << "Error al cargar " << nombreArchivo << endl;
            for (int j = 0; j < i; ++j) delete[] archivosTxt[j];
            delete[] mascara;
            delete[] imagenAleatoria;
            return -1;
        }

        altos[i] = heightM;
        anchos[i] = widthM;
    }

    // Paso 4: Llamar a identificarTransformaciones: Funcion que realiza todo el procedimiento
    int transformacionesDetectadas = identificarTransformaciones(
        basePath,
        pasos,
        imagenAleatoria,
        mascara,
        archivosTxt,
        semillas,
        altos,
        anchos,
        registroTransformaciones
        );

    // Paso 5: Mostrar resultados de transformaciones
    if (transformacionesDetectadas <= 0) {
        cout << "No se pudieron identificar correctamente las transformaciones." << endl;
    } else {
        cout << "\nTransformaciones detectadas en orden:\n";
        for (int i = 0; i < transformacionesDetectadas; ++i) {
            cout << "Paso " << i << ": " << registroTransformaciones[i] << endl;
        }
    }

// ========================== RECONSTRUIR LA IMAGEN ==========================
    cout << "\n==========================================\n";
    cout << "*** RECONSTRUCCION DE LA IMAGEN ORIGINAL. ***\n";
    cout << "==========================================\n";

    if (transformacionesDetectadas > 0) {
        const char* rutaID = "C:/Caso2/I_D.bmp";           // Imagen final
        const char* rutaIM = "C:/Caso2/I_M.bmp";            // Imagen aleatoria
        const char* rutaMascara = "C:/Caso2/M.bmp";         // Máscara
        const char* rutaReconstruida = "C:/Caso2/I_O_reconstruida.bmp"; // Imagen reconstruida

        bool reconstruccionExitosa = reconstruirImagen(
            rutaID,
            rutaIM,
            rutaMascara,
            semillas[0],
            rutaReconstruida,
            registroTransformaciones,
            transformacionesDetectadas
            );

        if (reconstruccionExitosa) {
            cout << "\n Imagen original reconstruida exitosamente.\n";

            // ========================== COMPARAR IMÁGENES ==========================
            cout << "\n=== COMPARACION FINAL ===" << endl;
            bool iguales = compararImagenes(
                "C:/Caso2/I_O.bmp",
                "C:/Caso2/I_O_reconstruida.bmp"
                );

            if (iguales) {
                cout << "\n Las imagenes coinciden perfectamente.\n";
            } else {
                cout << "\n Las imagenes NO son exactamente iguales.\n";
            }

        } else {
            cout << "\n Error al reconstruir la imagen.\n";
        }
    }

    // ========================== LIBERACIÓN DE MEMORIA ==========================

    for (int i = 0; i < transformacionesDetectadas; ++i) {
        delete[] registroTransformaciones[i];
    }

    // Liberar memoria de los Mx.txt, mascara e imagen aleatoria
    for (int i = 0; i < pasos; ++i) {
        delete[] archivosTxt[i];
    }
    delete[] mascara;
    delete[] imagenAleatoria;


    /*

    cout << "\n==========================================\n";
    cout << "*** PRUEBAS COMPLEMENTARIAS Y ESPECIFICAS: ***\n";
    cout << "==========================================\n";

    cout << "\n==========================================\n";
    cout << "*** ROTACIONES: ***\n";
    cout << "==========================================\n";

    // Dato base
    unsigned char original = 0b01101100; // 108
    int bits = 3;

    cout << "Original: \n";
    cout << "Binario: ";
    printBits(original);
    cout << " | Decimal: " << (int)original << "\n\n";

    // Rotación a la izquierda
    unsigned char datoIzq = original;
    rotateLeft(&datoIzq, 1, bits);

    cout << "Rotacion a la izquierda " << bits << " bits:\n";
    cout << "Binario: ";
    printBits(datoIzq);
    cout << " | Decimal: " << (int)datoIzq << "\n\n";

    // Rotación a la derecha
    unsigned char datoDer = original;
    rotateRight(&datoDer, 1, bits);

    cout << "Rotacion a la derecha " << bits << " bits:\n";
    cout << "Binario: ";
    printBits(datoDer);
    cout << " | Decimal: " << (int)datoDer << "\n";

    cout << "\n==========================================\n";
    cout << "*** XOR: ***\n";
    cout << "==========================================\n";

    unsigned char image[] = {0x10, 0x20, 0x30, 0x40};
    unsigned char mask[]  = {0x01, 0x02, 0x03, 0x04};
    int size_xor = 4;

    xorPixels(image, mask, size_xor);

    for (int i = 0; i < size_xor; i++) {
        cout << "Resultado[" << i << "] = " << (int)image[i] << endl;
    }

    cout << "\n==========================================\n";
    cout << "*** CARGA DE IAMGEN BMP: ***\n";
    cout << "==========================================\n";

    int width = 0, height = 0;
    QString rutaImagen = "C:/Users/FELIPE/OneDrive - Universidad de Antioquia/PERSONAL/UDEA/2025-1/Cursos/Informática II/Desafío 1/DesafíoI/Caso 1/I_O.bmp";

    unsigned char* pixelData = loadPixels(rutaImagen, width, height);

    if (pixelData != nullptr) {
        exportImage(pixelData, width, height, "I_D.bmp");
        delete[] pixelData;
        pixelData = nullptr;
    }

    cout << "\n==========================================\n";
    cout << "*** ENMASCARAMEINTO: ***\n";
    cout << "==========================================\n";

    // Cargar imagen (I_D.bmp ya cargada como pixelData)
    // Cargar máscara (M.bmp)
    int width_mask = 0, height_mask = 0;
    QString rutaMascara = "C:/Caso1/M.bmp";
    unsigned char* mascara = loadPixels(rutaMascara, width_mask, height_mask);
    if (mascara == nullptr) {
        cout << "Error: no se pudo cargar la máscara." << endl;
        return -1;
    }


    // Cargar archivo de resultado (M1.txt)
    int seed = 0, n_pixels = 0;
    unsigned int* resultado = loadSeedMasking("C:/Caso1/M1.txt", seed, n_pixels);
    if (!resultado) {
        cout << "Error: no se pudo cargar el archivo de resultados." << endl;
        return -1;
    }

    int width_image = 0, height_image = 0;
    unsigned char* pixelDataVer = loadPixels("C:/Caso1/I_D.bmp", width_image, height_image);
    if (pixelDataVer == nullptr) {
        cout << "Error: no se pudo cargar la imagen I_D.bmp" << endl;
        return -1;
    }

    // Imprimir dimensiones para verificar
    cout << "Dimensiones imagen: " << width_image << "x" << height_image << endl;
    cout << "Dimensiones mascara: " << width_mask << "x" << height_mask << endl;

    // Verificar enmascaramiento
    bool ok = verificarEnmascaramiento(pixelDataVer, mascara, resultado, seed,
                                       height_mask, width_mask, width_image, height_image);


    if (!ok) {
        cout << "La verificacion del enmascaramiento fallo." << endl;
    } else {
        cout << "La verificacion del enmascaramiento fue EXITOSA." << endl;
    }

    // Liberar memoria
    delete[] mascara;
    delete[] resultado;
    delete[] pixelDataVer;

    cout << "\n==========================================\n";
    cout << "*** DESPLAZAMIENTOS: ***\n";
    cout << "==========================================\n";

    //0b11010010 = 210
    //0b11100011 = 227
    //0b01010101 = 85

    unsigned char datosShift[] = {0b01010101}; // Valor base: 227 en decimal
    int sizeShift = sizeof(datosShift) / sizeof(datosShift[0]);
    int bitsShift = 3;

    cout << "Valor original: ";
    for (int i = 0; i < sizeShift; i++) {
        cout << "Binario: ";
        for (int b = 7; b >= 0; b--) {
            cout << ((datosShift[i] >> b) & 1);
        }
        cout << " | Decimal: " << (int)datosShift[i] << endl;
    }

    // Desplazamiento a la izquierda
    shiftLeft(datosShift, sizeShift, bitsShift);
    cout << "Despues de desplazamiento a la izquierda " << bitsShift << " bits:\n";
    for (int i = 0; i < sizeShift; i++) {
        cout << "Binario: ";
        for (int b = 7; b >= 0; b--) {
            cout << ((datosShift[i] >> b) & 1);
        }
        cout << " | Decimal: " << (int)datosShift[i] << endl;
    }

    // Restaurar valor original para la siguiente prueba
    datosShift[0] = 0b01010101;

    // Desplazamiento a la derecha
    shiftRight(datosShift, sizeShift, bitsShift);
    cout << "Despues de desplazamiento a la derecha " << bitsShift << " bits:\n";
    for (int i = 0; i < sizeShift; i++) {
        cout << "Binario: ";
        for (int b = 7; b >= 0; b--) {
            cout << ((datosShift[i] >> b) & 1);
        }
        cout << " | Decimal: " << (int)datosShift[i] << endl;
    }

    cout << "\n==========================================\n";
    cout << "*** DESPLAZAMIENTO EN PIXEL ALEATORIO: ***\n";
    cout << "==========================================\n";

    unsigned char pixel[3] = {120, 45, 200}; // R, G, B

    imprimirRGB(pixel, "\nValor original del pixel (RGB):");

    // Desplazamiento a la izquierda
    shiftLeft(pixel, 3, 3);  // 3 canales, 3 bits
    imprimirRGB(pixel, "Despues de desplazamiento a la izquierda 3 bits:");

    // Restaurar valores originales
    pixel[0] = 120;
    pixel[1] = 45;
    pixel[2] = 200;

    // Desplazamiento a la derecha
    shiftRight(pixel, 3, 3);
    imprimirRGB(pixel, "Despues de desplazamiento a la derecha 3 bits:");

    */

    /*

    cout << "\n==========================================\n";
    cout << "*** TRANSFORMACIONES MANUALES: ***\n";
    cout << "==========================================\n";

    // Cargar imagen original I_O
    int width_o = 0, height_o = 0;
    unsigned char* I_O = loadPixels("C:/Caso1/I_O.bmp", width_o, height_o);
    if (!I_O) {
        cout << "Error al cargar I_O.bmp\n";
        return -1;
    }

    // Cargar imagen IM
    int width_im = 0, height_im = 0;
    unsigned char* I_M = loadPixels("C:/Caso1/I_M.bmp", width_im, height_im);
    if (!I_M) {
        cout << "Error al cargar I_M.bmp\n";
        return -1;
    }

    // Cargar máscara
    int width_m = 0, height_m = 0;
    unsigned char* mascara = loadPixels("C:/Caso1/M.bmp", width_m, height_m);
    if (!mascara) {
        cout << "Error al cargar la máscara\n";
        return -1;
    }

    // Cargar archivo M1.txt
    int seed = 0, n_pixels = 0;
    unsigned int* resultado = loadSeedMasking("C:/Caso1/M2.txt", seed, n_pixels);
    if (!resultado) {
        cout << "Error al cargar M2.txt\n";
        return -1;
    }

    // Prueba 1: XOR entre I_O y I_M
    cout << "\nProbar XOR entre I_O y I_M..." << endl;
    unsigned char* copiaXOR = new unsigned char[width_o * height_o * 3];
    for (int i = 0; i < width_o * height_o * 3; i++) copiaXOR[i] = I_O[i];
    xorPixels(copiaXOR, I_M, width_o * height_o * 3);
    bool ok1 = verificarEnmascaramiento(copiaXOR, mascara, resultado, seed, height_m, width_m, width_o, height_o);
    delete[] copiaXOR;

    // Prueba 2: rotación a la derecha
    cout << "\nProbar rotacion a la derecha..." << endl;
    bool ok2 = probarTransformacion(I_O, width_o, height_o, mascara, resultado, seed, height_m, width_m, rotarDerecha3);

    // Prueba 3: rotación a la izquierda
    cout << "\nProbar rotacion a la izquierda..." << endl;
    bool ok3 = probarTransformacion(I_O, width_o, height_o, mascara, resultado, seed, height_m, width_m, rotarIzquierda3);

    // Prueba 4: desplazamiento a la izquierda
    cout << "\nProbar desplazamiento a la izquierda..." << endl;
    bool ok4 = probarTransformacion(I_O, width_o, height_o, mascara, resultado, seed, height_m, width_m, desplazarIzquierda3);

    // Prueba 5: desplazamiento a la derecha
    cout << "\nProbar desplazamiento a la derecha..." << endl;
    bool ok5 = probarTransformacion(I_O, width_o, height_o, mascara, resultado, seed, height_m, width_m, desplazarDerecha3);

    // Mostrar resumen
    cout << "\n=== RESULTADOS DE LAS TRANSFORMACIONES ===" << endl;
    if (ok1) cout << "Correcto: XOR con I_M coincide con Mx.txt\n";
    if (ok2) cout << "Correcto: ROTACION DERECHA coincide con Mx.txt\n";
    if (ok3) cout << "Correcto: ROTACION IZQUIERDA coincide con Mx.txt\n";
    if (ok4) cout << "Correcto: DESPLAZAMIENTO IZQUIERDA coincide con Mx.txt\n";
    if (ok5) cout << "Correcto: DESPLAZAMIENTO DERECHA coincide con Mx.txt\n";

    if (!ok1 && !ok2 && !ok3 && !ok4 && !ok5) {
        cout << "Incorrecto: Ninguna transformacion coincidio con Mx.txt\n";
    }

    delete[] I_O;
    delete[] I_M;
    delete[] mascara;
    delete[] resultado;


    cout << "\n==========================================\n";
    cout << "*** IDENTIFICAR TRANSFORMACIONES PARA CASO 1: 'encontrarTransformacionesCaos1' ***\n";
    cout << "==========================================\n";

    // Cargar I_O.bmp
    int width_o = 0, height_o = 0;
    unsigned char* I_O = loadPixels("C:/Caso1/I_O.bmp", width_o, height_o);
    if (!I_O) {
        cout << "Error al cargar I_O.bmp\n";
        return -1;
    }

    // Cargar I_M.bmp
    int width_im = 0, height_im = 0;
    unsigned char* I_M = loadPixels("C:/Caso1/I_M.bmp", width_im, height_im);
    if (!I_M) {
        cout << "Error al cargar I_M.bmp\n";
        return -1;
    }

    // Cargar M.bmp
    int width_m = 0, height_m = 0;
    unsigned char* M = loadPixels("C:/Caso1/M.bmp", width_m, height_m);
    if (!M) {
        cout << "Error al cargar M.bmp\n";
        return -1;
    }

    // Cargar M1.txt
    int seed1 = 0, n_pixels1 = 0;
    unsigned int* resultadoM1 = loadSeedMasking("C:/Caso1/M1.txt", seed1, n_pixels1);
    if (!resultadoM1) {
        cout << "Error al cargar M1.txt\n";
        return -1;
    }

    // Cargar M2.txt
    int seed2 = 0, n_pixels2 = 0;
    unsigned int* resultadoM2 = loadSeedMasking("C:/Caso1/M2.txt", seed2, n_pixels2);
    if (!resultadoM2) {
        cout << "Error al cargar M2.txt\n";
        return -1;
    }

    // Ejecutar la función
    bool encontrado = encontrarTransformacionesCaso1(
        I_O, I_M, M,
        resultadoM1, seed1, height_m, width_m,
        resultadoM2, seed2, height_m, width_m,
        width_o, height_o
        );

    if (encontrado) {
        cout << "Secuencia correcta detectada exitosamente.\n";
    } else {
        cout << "No se detecto la secuencia esperada.\n";
    }

    // Liberar memoria
    delete[] I_O;
    delete[] I_M;
    delete[] M;
    delete[] resultadoM1;
    delete[] resultadoM2;

*/
    return 0;
}

/*
//Funcion para imprimir los bits luego de la rotacion en las pruebas
void printBits(unsigned char byte) {
    for (int i = 7; i >= 0; i--) {
        cout << ((byte >> i) & 1);
    }
}

//Funcion para imprimir los bits de un pixel RGB
void imprimirRGB(const unsigned char* data, const string& mensaje) {
    cout << mensaje << endl;
    for (int i = 0; i < 3; ++i) {
        cout << "Canal [" << i << "] = " << (int)data[i] << " | Binario: ";
        for (int j = 7; j >= 0; --j) {
            cout << ((data[i] >> j) & 1);
        }
        cout << endl;
    }
    cout << "-------------------------------------" << endl;
}
*/
