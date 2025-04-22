#include <QCoreApplication>
#include <iostream>
#include "procesamiento.h"
#include "bit_operations.h"

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

    return 0;
}

//Funcion para imprimir los bits luego de la rotacion en las pruebas
void printBits(unsigned char byte) {
    for (int i = 7; i >= 0; i--) {
        cout << ((byte >> i) & 1);
    }
}
