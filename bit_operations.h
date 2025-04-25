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


bool encontrarTransformacionesCaso1(
    unsigned char* original, // I_O
    unsigned char* imagen_aleatoria, // I_M
    unsigned char* mascara, // M
    unsigned int* resultadoM1, int seed1, int m1, int n1,
    unsigned int* resultadoM2, int seed2, int m2, int n2,
    int width, int height
    );


#endif // BIT_OPERATIONS_H
