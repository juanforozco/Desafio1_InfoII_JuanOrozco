#include "masking_verification.h"
#include <iostream>

using namespace std;

/*
 * Función que verifica si el resultado de un enmascaramiento corresponde a la suma
 * entre una imagen transformada (imagen) y una máscara (mascara), desplazada con una semilla (seed).
 *
 * Parámetros:
 * - imagen: puntero a arreglo dinámico de la imagen transformada I_d
 * - mascara: puntero a arreglo dinámico de la máscara M
 * - resultado: puntero a arreglo dinámico con los valores del archivo Mx.txt
 * - seed: desplazamiento usado al aplicar la máscara
 * - m, n: dimensiones de la máscara (alto y ancho)
 * - width, height: dimensiones de la imagen (ancho y alto)
 *
 * Retorno:
 * - true si la verificación fue exitosa
 * - false si hubo algún error o inconsistencia
 */
bool verificarEnmascaramiento(unsigned char* imagen, unsigned char* mascara, unsigned int* resultado,
                              int seed, int m, int n, int width, int height, bool imprimirErrores) {
    if (!imagen || !mascara || !resultado) {
        if (imprimirErrores) {
            cout << "Error: puntero nulo en los datos recibidos." << endl;
        }
        return false;
    }

    int totalMascara = m * n * 3;
    int totalImagen = width * height * 3;
    //Verificar que el rango no sobre pase el tamaño de la imagen
    if (seed + totalMascara > totalImagen) {
        if (imprimirErrores) {
            cout << "Error: El rango excede el tamaño de la imagen." << endl;
        }
        return false;
    }

    //Verificacion de enmascarameinto con formula. Se debe castear a int por que los bytes vienen como tipo char
    //y podria generar inconvenientes de rangos
    for (int i = 0; i < totalMascara; i++) {
        int suma = static_cast<int>(imagen[seed + i]) + static_cast<int>(mascara[i]);
        if (suma != static_cast<int>(resultado[i])) {//Se compara la suma con el valor del Mx.txt
            if (imprimirErrores) {
                cout << "Error en pixel " << i/3 << ", canal " << i%3 //i/3 pixel, i%3 canal R, G o B.
                     << ". Esperado: " << resultado[i]
                     << ", Calculado: " << suma << endl;
            }
            return false;
        }
    }

    if (imprimirErrores) {
        cout << "Enmascaramiento verificado correctamente." << endl;
    }
    return true;
}


