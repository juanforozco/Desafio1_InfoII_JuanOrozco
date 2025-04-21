#include <QCoreApplication>
#include <iostream>
#include "procesamiento.h"
#include "bit_operations.h" // Agregamos nuestra nueva función

using namespace std;

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // ================= PRUEBA DE XOR =================
    cout << "--- PRUEBA DE XOR CON ARREGLOS ---" << endl;

    unsigned char image[] = {0x10, 0x20, 0x30, 0x40};
    unsigned char mask[]  = {0x01, 0x02, 0x03, 0x04};
    int size = 4;

    xorPixels(image, mask, size);

    for (int i = 0; i < size; i++) {
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

