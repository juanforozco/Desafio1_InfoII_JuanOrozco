#include "bit_operations.h"
#include "masking_verification.h" // Necesario para llamar a verificarEnmascaramiento
#include "procesamiento.h"
#include <iostream>

using namespace std;

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

//Funciones adaptadoras para corregir pruebas de transformaciones:
void rotarDerecha3(unsigned char* data, int size) {
    rotateRight(data, size, 3);
}

void rotarIzquierda3(unsigned char* data, int size) {
    rotateLeft(data, size, 3);
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

//Funciones adaptadoras para corregir pruebas de transformaciones:
void desplazarIzquierda3(unsigned char* data, int size) {
    shiftLeft(data, size, 3);
}

void desplazarDerecha3(unsigned char* data, int size) {
    shiftRight(data, size, 3);
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

/*
 * Funcion de ingenieria inversa especifica para caso 1 proporcionado como material de desafío.
 *      A partir de acá se podría generalizar.
 * unsigned char* IO: iagen original (I_O.bmp), ocupa 8 bits (0-255)
 * unsigned char* IM: iagen de enmascaramiento (I_O.bmp), ocupa 8 bits (0-255)
 * int width, height: Dimensiones de IO.
 * unsigned char* mascara: imagen de la máscara (M.bmp), para aplicar el enmascaramiento.
 * int m, n: dimensiones de la máscara (filas y columnas).
 * unsigned int* M1: datos del archivo M1.txt (resultado del enmascaramiento). Tipo unsigned int* porque pueden exceder 255 (8 bits) ya que son sumas.
 * int seed1: desplazamiento usado en M1.txt.
 * unsigned int* M2: datos del archivo M2.txt.
 * int seed2: desplazamiento usado en M2.txt.
 * Funcionamiento: Recibe una imagen, una imagen aleatoria de enmascaramiento y una mascara.
 *                 Crea una copia de IO para no modificarla de acuerdo al tamaño de la misma.
 *                 Aplica las transformaciones del caso 1:
 *                 1. XOR entre IO e IM. Aqui verifica si el resultado corresponde con M1.txt
 *                 2. Rotacion de 3 bits a la derecha y verifica con M2.txt.
 *                 Confirma que secuencia encontro o si obtuvo algún error.
 */


bool encontrarTransformacionesCaso1(
    unsigned char* original, // I_O
    unsigned char* imagen_aleatoria, // I_M
    unsigned char* mascara, // M
    unsigned int* resultadoM1, int seed1, int m1, int n1,
    unsigned int* resultadoM2, int seed2, int m2, int n2,
    int width, int height
    ) {
    int total = width * height * 3;

    // Crear copia dinámica de I_O
    unsigned char* copia = new unsigned char[total];
    for (int i = 0; i < total; ++i) copia[i] = original[i];

    // Paso 1: XOR con I_M
    xorPixels(copia, imagen_aleatoria, total);

    // Verificar con M1.txt
    bool ok1 = verificarEnmascaramiento(copia, mascara, resultadoM1,
                                        seed1, m1, n1, width, height);
    if (!ok1) {
        cout << "Fallo al verificar con M1.txt tras XOR.\n";
        delete[] copia;
        return false;
    }

    // Paso 2: rotación a la derecha 3 bits
    rotateRight(copia, total, 3);

    // Verificar con M2.txt
    bool ok2 = verificarEnmascaramiento(copia, mascara, resultadoM2,
                                        seed2, m2, n2, width, height);
    if (!ok2) {
        cout << "Fallo al verificar con M2.txt tras ROT_RIGHT_3.\n";
        delete[] copia;
        return false;
    }

    // Si pasó ambas verificaciones, está correcta la cadena
    cout << "Secuencia encontrada: XOR + ROT_RIGHT_3\n";
    delete[] copia;
    return true;
}


/*
 * FUNCIÓN CORE DEL DESAFÍO:
 */

/*
 * PARTE 1: Funciones de transformaciones para cada cantidad de bits posibles.
 */
// Declaración de alias para punteros a funciones de transformación bit a bit: Se usa para no declararla una por una
        //sino, para guardarlas en un arreglo y recorrerlo.

typedef void (*OperacionBit)(unsigned char*, int);

// ROTACIONES A LA DERECHA
void rotarDer0(unsigned char* d, int s) {
    rotateRight(d, s, 0); }
void rotarDer1(unsigned char* d, int s) {
    rotateRight(d, s, 1); }
void rotarDer2(unsigned char* d, int s) {
    rotateRight(d, s, 2); }
void rotarDer3(unsigned char* d, int s) {
    rotateRight(d, s, 3); }
void rotarDer4(unsigned char* d, int s) {
    rotateRight(d, s, 4); }
void rotarDer5(unsigned char* d, int s) {
    rotateRight(d, s, 5); }
void rotarDer6(unsigned char* d, int s) {
    rotateRight(d, s, 6); }
void rotarDer7(unsigned char* d, int s) {
    rotateRight(d, s, 7); }
void rotarDer8(unsigned char* d, int s) {
    rotateRight(d, s, 8); }

// ROTACIONES A LA IZQUIERDA
void rotarIzq0(unsigned char* d, int s) {
    rotateLeft(d, s, 0); }
void rotarIzq1(unsigned char* d, int s) {
    rotateLeft(d, s, 1); }
void rotarIzq2(unsigned char* d, int s) {
    rotateLeft(d, s, 2); }
void rotarIzq3(unsigned char* d, int s) {
    rotateLeft(d, s, 3); }
void rotarIzq4(unsigned char* d, int s) {
    rotateLeft(d, s, 4); }
void rotarIzq5(unsigned char* d, int s) {
    rotateLeft(d, s, 5); }
void rotarIzq6(unsigned char* d, int s) {
    rotateLeft(d, s, 6); }
void rotarIzq7(unsigned char* d, int s) {
    rotateLeft(d, s, 7); }
void rotarIzq8(unsigned char* d, int s) {
    rotateLeft(d, s, 8); }

// DESPLAZAMIENTOS A LA DERECHA
void desplazarDer0(unsigned char* d, int s) {
    shiftRight(d, s, 0); }
void desplazarDer1(unsigned char* d, int s) {
    shiftRight(d, s, 1); }
void desplazarDer2(unsigned char* d, int s) {
    shiftRight(d, s, 2); }
void desplazarDer3(unsigned char* d, int s) {
    shiftRight(d, s, 3); }
void desplazarDer4(unsigned char* d, int s) {
    shiftRight(d, s, 4); }
void desplazarDer5(unsigned char* d, int s) {
    shiftRight(d, s, 5); }
void desplazarDer6(unsigned char* d, int s) {
    shiftRight(d, s, 6); }
void desplazarDer7(unsigned char* d, int s) {
    shiftRight(d, s, 7); }
void desplazarDer8(unsigned char* d, int s) {
    shiftRight(d, s, 8); }

// DESPLAZAMIENTOS A LA IZQUIERDA
void desplazarIzq0(unsigned char* d, int s) {
    shiftLeft(d, s, 0); }
void desplazarIzq1(unsigned char* d, int s) {
    shiftLeft(d, s, 1); }
void desplazarIzq2(unsigned char* d, int s) {
    shiftLeft(d, s, 2); }
void desplazarIzq3(unsigned char* d, int s) {
    shiftLeft(d, s, 3); }
void desplazarIzq4(unsigned char* d, int s) {
    shiftLeft(d, s, 4); }
void desplazarIzq5(unsigned char* d, int s) {
    shiftLeft(d, s, 5); }
void desplazarIzq6(unsigned char* d, int s) {
    shiftLeft(d, s, 6); }
void desplazarIzq7(unsigned char* d, int s) {
    shiftLeft(d, s, 7); }
void desplazarIzq8(unsigned char* d, int s) {
    shiftLeft(d, s, 8); }

//ARREGLO CON TODAS LAS TRANSFORMACIONES PARA PODER RECORRERLAS TODAS:

// Rotaciones
OperacionBit rotacionesDerecha[9] = {
    rotarDer0, rotarDer1, rotarDer2, rotarDer3, rotarDer4,
    rotarDer5, rotarDer6, rotarDer7, rotarDer8
};

OperacionBit rotacionesIzquierda[9] = {
    rotarIzq0, rotarIzq1, rotarIzq2, rotarIzq3, rotarIzq4,
    rotarIzq5, rotarIzq6, rotarIzq7, rotarIzq8
};

// Desplazamientos
OperacionBit desplazamientosDerecha[9] = {
    desplazarDer0, desplazarDer1, desplazarDer2, desplazarDer3, desplazarDer4,
    desplazarDer5, desplazarDer6, desplazarDer7, desplazarDer8
};

OperacionBit desplazamientosIzquierda[9] = {
    desplazarIzq0, desplazarIzq1, desplazarIzq2, desplazarIzq3, desplazarIzq4,
    desplazarIzq5, desplazarIzq6, desplazarIzq7, desplazarIzq8
};

/*
 * PARTE 2: ALGORITMO DE INGENIERÍA INVERSA PARA DETECTAR QUE TRANSFORMACIONES FUERON REALIZADAS Y EN QUE ORDEN.
 */


/*
 * NUEVA LÓGICA: Utilizar las imágenes de cada paso para identificar la transformacion, verificarla y encontrar la secuencia
*/

bool identificarTransformaciones(
    const char* basePath,            // Ruta base donde están las imágenes y archivos
    int pasos,                       // Número de pasos (cantidad de transformaciones a identificar)
    unsigned char* imagenAleatoria,  // Imagen IM para posibles operaciones XOR (no usada aún)
    unsigned char* mascara,          // Imagen M.bmp (máscara de enmascaramiento)
    unsigned int** archivosTxt,      // Arreglo de punteros a los archivos M0.txt, M1.txt, ...
    int* semillas,                   // Arreglo de seeds de cada archivo Mx.txt
    int* altos, int* anchos,          // Altura y ancho de la máscara por paso
    char** registroTransformaciones  // Arreglo donde se registran las transformaciones detectadas
    ) {
    // Variables auxiliares
    int width = 0, height = 0;
    unsigned char* imagenActual = nullptr;

    // Bucle para recorrer cada paso de transformación
    for (int paso = 0; paso < pasos; ++paso) {
        cout << "\n==========================================\n";
        cout << "Procesando paso " << paso << "...\n";

        // Construir la ruta del archivo Px.bmp
        char rutaImagen[256];
        sprintf(rutaImagen, "%sP%d.bmp", basePath, paso+1);

        // Cargar la imagen actual
        imagenActual = loadPixels(rutaImagen, width, height);
        if (!imagenActual) {
            cout << "Error al cargar " << rutaImagen << endl;
            return false;
        }

        int totalBytes = width * height * 3;

        // Variables para registrar si encontramos la transformación correcta
        bool encontrada = false;

        // ===================== NUEVO: Probar primero XOR con IM =====================
        if (!encontrada && imagenAleatoria != nullptr) {
            unsigned char* copiaXOR = new unsigned char[totalBytes];
            for (int i = 0; i < totalBytes; ++i) {
                copiaXOR[i] = imagenActual[i];
            }

            xorPixels(copiaXOR, imagenAleatoria, totalBytes);

            if (verificarEnmascaramiento(copiaXOR, mascara, archivosTxt[paso],
                                         semillas[paso], altos[paso], anchos[paso], width, height)) {
                cout << "Paso " << paso << ": XOR con IM\n";
                registroTransformaciones[paso] = new char[20];
                sprintf(registroTransformaciones[paso], "XOR con IM");
                encontrada = true;
            }

            delete[] copiaXOR;
        }

        // ===================== Luego probar rotaciones y desplazamientos =====================

        // Probar rotaciones a la izquierda (0 a 8 bits)
        for (int b = 0; b <= 8 && !encontrada; ++b) {
            if (probarTransformacion(imagenActual, width, height, mascara,
                                     archivosTxt[paso], semillas[paso],
                                     altos[paso], anchos[paso],
                                     rotacionesIzquierda[b])) {
                cout << "Paso " << paso << ": Rotacion a la izquierda " << b << " bits\n";
                registroTransformaciones[paso] = new char[30];
                sprintf(registroTransformaciones[paso], "ROT_LEFT %d bits", b);
                encontrada = true;
            }
        }

        // Probar rotaciones a la derecha (0 a 8 bits)
        for (int b = 0; b <= 8 && !encontrada; ++b) {
            if (probarTransformacion(imagenActual, width, height, mascara,
                                     archivosTxt[paso], semillas[paso],
                                     altos[paso], anchos[paso],
                                     rotacionesDerecha[b])) {
                cout << "Paso " << paso << ": Rotacion a la derecha " << b << " bits\n";
                registroTransformaciones[paso] = new char[30];
                sprintf(registroTransformaciones[paso], "ROT_RIGHT %d bits", b);
                encontrada = true;
            }
        }

        // Probar desplazamientos a la izquierda (0 a 8 bits)
        for (int b = 0; b <= 8 && !encontrada; ++b) {
            if (probarTransformacion(imagenActual, width, height, mascara,
                                     archivosTxt[paso], semillas[paso],
                                     altos[paso], anchos[paso],
                                     desplazamientosIzquierda[b])) {
                cout << "Paso " << paso << ": Desplazamiento a la izquierda " << b << " bits\n";
                registroTransformaciones[paso] = new char[30];
                sprintf(registroTransformaciones[paso], "SHIFT_LEFT %d bits", b);
                encontrada = true;
            }
        }

        // Probar desplazamientos a la derecha (0 a 8 bits)
        for (int b = 0; b <= 8 && !encontrada; ++b) {
            if (probarTransformacion(imagenActual, width, height, mascara,
                                     archivosTxt[paso], semillas[paso],
                                     altos[paso], anchos[paso],
                                     desplazamientosDerecha[b])) {
                cout << "Paso " << paso << ": Desplazamiento a la derecha " << b << " bits\n";
                registroTransformaciones[paso] = new char[30];
                sprintf(registroTransformaciones[paso], "SHIFT_RIGHT %d bits", b);
                encontrada = true;
            }
        }

        // Si no encontramos ninguna transformación válida, reportar error
        if (!encontrada) {
            cout << "Error: No se encontró transformación valida en el paso " << paso << endl;
            delete[] imagenActual;
            return false;
        }

        // Liberar la imagen cargada de este paso antes de cargar la siguiente
        delete[] imagenActual;
        imagenActual = nullptr;
    }

    // Si completamos correctamente todos los pasos
    cout << "\n==========================================\n";
    cout << "*** Todas las transformaciones fueron identificadas exitosamente. ***\n";
    cout << "==========================================\n";

    return true;
}



/*
 * LA FUNCION encontrarTransformacionesGenerico no fue lo suficientemente útil, debido a errores de lógica
 * ya que se trataba de retroceder desde la imagen final y comenzar a reconstruirla, pero no se tenia en cuenta
 * las imagenes intermedias y no esta dando buenos resultados.
 */

/*
bool encontrarTransformacionesGenerico(
    unsigned char* imagenInicial,  // I_D
    unsigned char* imagenAleatoria, // I_M
    unsigned char* mascara,         // M
    unsigned int** resultados,      // Arreglo de punteros a archivos Mx.txt
    int* seeds,                     // Arreglo de semillas de cada archivo Mx.txt
    int* alturas, int* anchuras,    // Altura y ancho de la máscara en cada paso
    int pasos,                      // Número de transformaciones (cantidad de archivos Mx.txt)
    int width, int height           // Dimensiones de la imagen
    ) {
    int total = width * height * 3;

    // Paso 1: XOR inverso para obtener Pn.bmp (antes del último XOR)
    unsigned char* actual = new unsigned char[total];
    for (int i = 0; i < total; ++i) {
        actual[i] = imagenInicial[i] ^ imagenAleatoria[i];
    }

    // Arreglo para registrar las transformaciones aplicadas
    const char* nombres[4] = {"ROT_RIGHT", "ROT_LEFT", "SHIFT_RIGHT", "SHIFT_LEFT"};
    OperacionBit* transformaciones[4] = {
        rotacionesDerecha,
        rotacionesIzquierda,
        desplazamientosDerecha,
        desplazamientosIzquierda
    };

    // Vamos hacia atrás, desde Mx.txt hasta M1.txt
    for (int paso = pasos - 1; paso >= 0; --paso) {
        bool encontrado = false;
        //cout << "\nPaso " << paso + 1 << ": intentando transformaciones sobre base: "
          //   << (int)actual[0] << endl;
        // Recorrer todas las operaciones posibles
        for (int tipo = 0; tipo < 4 && !encontrado; ++tipo) {

            for (int b = 0; b <= 8 && !encontrado; ++b) {

                // Hacer copia del arreglo actual
                unsigned char* copia = new unsigned char[total];
                for (int i = 0; i < total; ++i) copia[i] = actual[i];

                // Aplicar la transformación inversa
                transformaciones[tipo][b](copia, total);


                // Verificar y corregir el crasheo
                string nombre1 = "ROT_RIGHT_" + to_string(paso);
                string nombre2 = "SHIFT_LEFT_" + to_string(tipo);
                string nombre3 = "ROT_LEFT_" + to_string(b);

                cout << "Intentando: [" << nombre1 << " -> " << nombre2 << " -> " << nombre3 << "]" << endl;


                // Verificar si coincide con el archivo Mx.txt
                if (verificarEnmascaramiento(copia, mascara, resultados[paso],
                                             seeds[paso], alturas[paso], anchuras[paso], width, height)) {
                    cout << "Paso " << paso + 1 << ": " << nombres[tipo] << " " << b << " bits\n";
                    //cout << "Primer byte de imagenActual luego del paso " << paso + 1 << ": "
                      //   << (int)actual[0] << endl;
                    encontrado = true;
                    delete[] actual;
                    actual = copia;
                } else {
                    delete[] copia;
                }
            }
        }

        if (!encontrado) {
            cout << "No se encontro transformacion para el paso " << paso + 1 << endl;
            delete[] actual;
            return false;
        }
    }

    // Verificar si lo que queda es I_O haciendo XOR con I_M
    for (int i = 0; i < total; ++i) actual[i] = actual[i] ^ imagenAleatoria[i];

    cout << "\nTransformaciones encontradas exitosamente.\n";
    delete[] actual;
    return true;
}
*/
