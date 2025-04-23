#include <QCoreApplication>
#include <iostream>
#include "procesamiento.h"
#include "bit_operations.h"
#include "masking_verification.h"


using namespace std;

void printBits(unsigned char byte);

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // ================= PRUEBA DE ROTACION =================
    cout << "\n--- PRUEBA DE ROTACIONES ---" << endl;

    // Dato base
    unsigned char original = 0b01101100; // 108
    int bits = 3;

    cout << "Original:\n";
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
    QString rutaMascara = "C:/Caso 1/M.bmp";
    unsigned char* mascara = loadPixels(rutaMascara, width_mask, height_mask);
    if (mascara == nullptr) {
        cout << "Error: no se pudo cargar la máscara." << endl;
        return -1;
    }


    // Cargar archivo de resultado (M1.txt)
    int seed = 0, n_pixels = 0;
    unsigned int* resultado = loadSeedMasking("C:/Caso 1/M1.txt", seed, n_pixels);
    if (!resultado) {
        cout << "Error: no se pudo cargar el archivo de resultados." << endl;
        return -1;
    }

    int width_image = 0, height_image = 0;
    unsigned char* pixelDataVer = loadPixels("C:/Caso 1/I_D.bmp", width_image, height_image);
    if (pixelDataVer == nullptr) {
        cout << "Error: no se pudo cargar la imagen I_D.bmp" << endl;
        return -1;
    }

    // Imprimir dimensiones para verificar
    cout << "Dimensiones imagen: " << width_image << "x" << height_image << endl;
    cout << "Dimensiones máscara: " << width_mask << "x" << height_mask << endl;

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



    return 0;
}

//Funcion para imprimir los bits luego de la rotacion en las pruebas
void printBits(unsigned char byte) {
    for (int i = 7; i >= 0; i--) {
        cout << ((byte >> i) & 1);
    }
}
