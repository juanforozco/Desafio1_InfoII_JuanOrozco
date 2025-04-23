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

#endif // BIT_OPERATIONS_H
