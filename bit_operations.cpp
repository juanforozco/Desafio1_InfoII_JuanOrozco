#include "bit_operations.h"

// Esta función modifica directamente la imagen
void xorPixels(unsigned char* image, unsigned char* mask, int size) {
    for (int i = 0; i < size; i++) {
        image[i] = image[i] ^ mask[i]; // Operación XOR bit a bit
    }
}
