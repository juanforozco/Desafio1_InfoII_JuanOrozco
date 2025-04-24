#include <QCoreApplication>
#include <iostream>
#include "procesamiento.h"
#include "bit_operations.h"
#include "masking_verification.h"

/*
 * Ruta para commits:
 * cd "C:\Users\FELIPE\OneDrive - Universidad de Antioquia\PERSONAL\UDEA\2025-1\Cursos\Informática II\Desafío 1\Solucion\Desafio1_InfoII_JuanOrozco_Davielys"
*/


using namespace std;

void printBits(unsigned char byte);
void imprimirRGB(const unsigned char* data, const string& mensaje);

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    /*
    // ================= PRUEBA DE ROTACION =================
    cout << "\n--- PRUEBA DE ROTACIONES ---" << endl;

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

    // ================= PRUEBA DE XOR =================
    cout << "\n--- PRUEBA DE XOR CON ARREGLOS ---" << endl;

    unsigned char image[] = {0x10, 0x20, 0x30, 0x40};
    unsigned char mask[]  = {0x01, 0x02, 0x03, 0x04};
    int size_xor = 4;

    xorPixels(image, mask, size_xor);

    for (int i = 0; i < size_xor; i++) {
        cout << "Resultado[" << i << "] = " << (int)image[i] << endl;
    }

    // ============ PRUEBA DE CARGA Y EXPORTACIÓN ============
    cout << "\n--- PRUEBA DE IMAGEN BMP ---" << endl;

    int width = 0, height = 0;
    QString rutaImagen = "C:/Users/FELIPE/OneDrive - Universidad de Antioquia/PERSONAL/UDEA/2025-1/Cursos/Informática II/Desafío 1/DesafíoI/Caso 1/I_O.bmp";

    unsigned char* pixelData = loadPixels(rutaImagen, width, height);

    if (pixelData != nullptr) {
        exportImage(pixelData, width, height, "I_D.bmp");
        delete[] pixelData;
        pixelData = nullptr;
    }

    // ============ PRUEBA DE ENMASCARAMIENTO ============
    cout << "\n--- VERIFICACION DE ENMASCARAMIENTO ---" << endl;

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

    // ================= PRUEBA DE DESPLAZAMIENTOS =================
    cout << "\n--- PRUEBA DE DESPLAZAMIENTOS ---" << endl;

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

    // ============ PRUEBA DE DESPLAZAMIENTO EN PIXEL RGB ============

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

    /*cout << "\n=== PRUEBA MANUAL DE TRANSFORMACIONES SOBRE I_O.bmp ===" << endl;

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
*/

    // === PRUEBA DE SECUENCIA: XOR + ROTACIÓN DERECHA ===
    cout << "\n=== PRUEBA SECUENCIA XOR + ROTACION DERECHA ===" << endl;

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

    // Cargar archivo M2.txt
    int seed2 = 0, n_pixels2 = 0;
    unsigned int* resultado2 = loadSeedMasking("C:/Caso1/M2.txt", seed2, n_pixels2);
    if (!resultado2) {
        cout << "Error al cargar M2.txt\n";
        return -1;
    }

    // Paso 1: aplicar XOR
    int totalBytes = width_o * height_o * 3;
    unsigned char* copiaSecuencia = new unsigned char[totalBytes];
    for (int i = 0; i < totalBytes; i++) copiaSecuencia[i] = I_O[i];
    xorPixels(copiaSecuencia, I_M, totalBytes);

    // Paso 2: aplicar rotación a la derecha
    rotateRight(copiaSecuencia, totalBytes, 3);

    // Paso 3: verificar enmascaramiento con M2.txt
    bool okSecuencia = verificarEnmascaramiento(copiaSecuencia, mascara, resultado2,
                                                seed2, height_m, width_m, width_o, height_o);

    if (okSecuencia) {
        cout << "La secuencia XOR + ROT_RIGHT_3 genera correctamente M2.txt\n";
    } else {
        cout << "La secuencia no coincide con M2.txt\n";
    }

    delete[] resultado2;
    delete[] copiaSecuencia;
    delete[] I_O;
    delete[] I_M;
    delete[] mascara;



    return 0;
}

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
