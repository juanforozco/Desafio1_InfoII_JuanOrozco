#ifndef BIT_OPERATIONS_H
#define BIT_OPERATIONS_H

// XOR
void xorPixels(unsigned char* image, unsigned char* mask, int size);

//ROTACION: izquierda y derecha
void rotateRight(unsigned char* data, int size, int bits);
void rotateLeft(unsigned char* data, int size, int bits);
void rotarDerecha3(unsigned char* data, int size); //Adaptada para pruebas
void rotarIzquierda3(unsigned char* data, int size); //Adaptada para pruebas

//DESPLAZAMIENTO: izquierda y derecha
void shiftRight(unsigned char* data, int size, int bits);
void shiftLeft(unsigned char* data, int size, int bits);
void desplazarIzquierda3(unsigned char* data, int size); //Adaptada para pruebas
void desplazarDerecha3(unsigned char* data, int size); //Adapatada para pruebas

//Probar diferentes transformaciones y verificarlas con los arvichivos Mx.txt
bool probarTransformacion(unsigned char* original, int width, int height,
                          unsigned char* mascara, unsigned int* resultado,
                          int seed, int m, int n,
                          void (*operacion)(unsigned char*, int));

//Funcion para determinar la secuencia de transformaciones del caso 1.
bool encontrarTransformacionesCaso1(
    unsigned char* original, // I_O
    unsigned char* imagen_aleatoria, // I_M
    unsigned char* mascara, // M
    unsigned int* resultadoM1, int seed1, int m1, int n1,
    unsigned int* resultadoM2, int seed2, int m2, int n2,
    int width, int height
    );

/*
 * FUNCIÓN CORE DEL DESAFÍO:
 */

/*
 * PARTE 1: Funciones de transformaciones para cada cantidad de bits posibles.
 */

typedef void (*OperacionBit)(unsigned char*, int);

// ROTACIONES A LA DERECHA
void rotarDer0(unsigned char* d, int s);
void rotarDer1(unsigned char* d, int s);
void rotarDer2(unsigned char* d, int s);
void rotarDer3(unsigned char* d, int s);
void rotarDer4(unsigned char* d, int s);
void rotarDer5(unsigned char* d, int s);
void rotarDer6(unsigned char* d, int s);
void rotarDer7(unsigned char* d, int s);
void rotarDer8(unsigned char* d, int s);

// ROTACIONES A LA IZQUIERDA
void rotarIzq0(unsigned char* d, int s);
void rotarIzq1(unsigned char* d, int s);
void rotarIzq2(unsigned char* d, int s);
void rotarIzq3(unsigned char* d, int s);
void rotarIzq4(unsigned char* d, int s);
void rotarIzq5(unsigned char* d, int s);
void rotarIzq6(unsigned char* d, int s);
void rotarIzq7(unsigned char* d, int s);
void rotarIzq8(unsigned char* d, int s);

// DESPLAZAMIENTOS A LA DERECHA
void desplazarDer0(unsigned char* d, int s);
void desplazarDer1(unsigned char* d, int s);
void desplazarDer2(unsigned char* d, int s);
void desplazarDer3(unsigned char* d, int s);
void desplazarDer4(unsigned char* d, int s);
void desplazarDer5(unsigned char* d, int s);
void desplazarDer6(unsigned char* d, int s);
void desplazarDer7(unsigned char* d, int s);
void desplazarDer8(unsigned char* d, int s);

// DESPLAZAMIENTOS A LA IZQUIERDA
void desplazarIzq0(unsigned char* d, int s);
void desplazarIzq1(unsigned char* d, int s);
void desplazarIzq2(unsigned char* d, int s);
void desplazarIzq3(unsigned char* d, int s);
void desplazarIzq4(unsigned char* d, int s);
void desplazarIzq5(unsigned char* d, int s);
void desplazarIzq6(unsigned char* d, int s);
void desplazarIzq7(unsigned char* d, int s);
void desplazarIzq8(unsigned char* d, int s);

/*
 * PARTE 2: ALGORITMO DE INGENIERÍA INVERSA PARA DETECTAR QUE TRANSFORMACIONES FUERON REALIZADAS Y EN QUE ORDEN.
 */
int identificarTransformaciones(
    const char* basePath,
    int pasos,
    unsigned char* imagenAleatoria,
    unsigned char* mascara,
    unsigned int** archivosTxt,
    int* semillas,
    int* altos, int* anchos,
    char** registroTransformaciones
    );

bool reconstruirImagen(
    const char* rutaImagenFinal,
    const char* rutaImagenAleatoria,
    const char* rutaMascara,
    int seed,
    const char* rutaReconstruida,
    char** registroTransformaciones,
    int pasos
    );

bool compararImagenes(const char* ruta1, const char* ruta2);

#endif // BIT_OPERATIONS_H
