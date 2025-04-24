#include "bit_operations.h"
#include "masking_verification.h" // Necesario para llamar a verificarEnmascaramiento

/*
 * Funcion XOR entre dos arreglos de bytes.
 * void: No tiene retorno, ya que modifica directamente en memoria.
 * unsigned char*: puntero a los arreglos dinamicos, de la imagen y la mascara - Valores entre 0 y 255.
 * int size: numero de bytes.
 * Funcionamiento: Por cada byte se aplcia xor con la mascara. Esto retorna 1 solo cuando son diferentes.
 */
void xorPixels(unsigned char* image, unsigned char* mask, int size) {
    for (int i = 0; i < size; i++) {
        image[i] = image[i] ^ mask[i]; // Operación XOR bit a bit
    }
}

/*
 * Funciones de rotacion a la derecha y a la izquierda:
 * Void: No tiene retorno, ya que modifica directamente en memoria.
 * unsigned char*: Valores entre 0-255(8 bits) - Se pasa un arreglo dinamico.
 * size: numero de bytes
 * bits: cuantos bits se va rotar por byte
 * Funcionamiento: Desplaza los bits a la derecha o izquierda y luego desplaza a la izquierda oderecha
 *                 lo que debe aparecer al otro lado, en la rotación no se pierden los bits.
 *                 Luego se unen con | (or) y se asegura que al final siga siendo 8 bits con &0xFF (Los 8 menos significativos).
 */
void rotateRight(unsigned char* data, int size, int bits) {
    for (int i = 0; i < size; ++i) {
        data[i] = ((data[i] >> bits) | (data[i] << (8 - bits))) & 0xFF;
    }
}

void rotateLeft(unsigned char* data, int size, int bits) {
    for (int i = 0; i < size; ++i) {
        data[i] = ((data[i] << bits) | (data[i] >> (8 - bits))) & 0xFF;
    }
}

/*
 * Funciones de desplazamiento a la derecha y a la izquierda:
 * Void: No tiene retorno, ya que modifica directamente en memoria.
 * unsigned char*: Valores entre 0-255(8 bits) - Se pasa un arreglo dinamico.
 * size: numero de bytes de data
 * bits: cuantos bits se va a desplazar
 * Funcionamiento: Desplaza los bits a la derecha o izquierda. Para el caso de la izquierda se pierden los bits mas significativos
 *                 y se agregan ceros y en este caso se asegura el resultado a 8 bits con & 0xFF, por prevención de desbordamiento.
 *                 En el caso de la derecha se pierden los menos significativos y tambien se agregan ceros. No hay necesidad de asegurar.
 *                 Es decir, aquí es donde puede existir mas distorsión, debido a la perdida de datos.
 */

void shiftLeft(unsigned char* data, int size, int bits) {
    for (int i = 0; i < size; ++i) {
        data[i] = (data[i] << bits) & 0xFF;
    }
}

void shiftRight(unsigned char* data, int size, int bits) {
    for (int i = 0; i < size; ++i) {
        data[i] = data[i] >> bits;
    }
}


/*Funcion que permite probar una transformacion sobre una imágen (Usando una copia para no modificarla) y verificar si su
 * enmascaramiento coincide con el archivo Mx.txt:
 * unsigned char*: Arreglo dinámico con la imágen "original".
 * int width, height: Dimensiones de la imagen "original".
 * unsigned char* mascara: Arreglo dinámico con la mascara.
 * unsigned int* resultado: Arreglo con los valores del archivo Mx.txt (Puede ser mayor a 255 entonces debe ser int).
 * int seed: Semilla, desplazamiento usado para aplicar la mascara.
 * int m, n: Dimensiones de la mascara.
 *
 * void (*operation)(unsigned char*, int): Puntero a funcion: Recibe una funcion como parámetro (XOR, rotación, desplazamiento).
 */

bool probarTransformacion(unsigned char* original, int width, int height,
                          unsigned char* mascara, unsigned int* resultado,
                          int seed, int m, int n,
                          void (*operacion)(unsigned char*, int)) {

    int total = width * height * 3;

    // Crear una copia dinámica de la imagen original
    unsigned char* copia = new unsigned char[total];

    //Copia byte a byte
    for (int i = 0; i < total; ++i) {
        copia[i] = original[i];
    }

    // Aplicar la operación bit a bit
    operacion(copia, total);

    // Verificar el resultado del enmascaramiento con la copia modificada
    bool exito = verificarEnmascaramiento(copia, mascara, resultado, seed, m, n, width, height);

    // Liberar memoria usada por la copia
    delete[] copia;

    return exito;
}




