#ifndef BIT_OPERATIONS_H
#define BIT_OPERATIONS_H

// XOR
void xorPixels(unsigned char* image, unsigned char* mask, int size);

//Rotaciones a izquierda y derecha
void rotateRight(unsigned char* data, int size, int bits);
void rotateLeft(unsigned char* data, int size, int bits);

//Desplazmientos a izquierda y derecha
void shiftRight(unsigned char* data, int size, int bits);
void shiftLeft(unsigned char* data, int size, int bits);

//Probar diferentes transformaciones y verificarlas con los arvichivos Mx.txt
bool probarTransformacion(unsigned char* original, int width, int height,
                          unsigned char* mascara, unsigned int* resultado,
                          int seed, int m, int n,
                          void (*operacion)(unsigned char*, int));


#endif // BIT_OPERATIONS_H
