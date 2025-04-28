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
    bool exito = verificarEnmascaramiento(copia, mascara, resultado, seed, m, n, width, height, false);

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

    //Crear copia dinámica de I_O
    unsigned char* copia = new unsigned char[total];
    for (int i = 0; i < total; ++i) copia[i] = original[i];

    //1. XOR con I_M
    xorPixels(copia, imagen_aleatoria, total);

    // Verificar con M1.txt
    bool ok1 = verificarEnmascaramiento(copia, mascara, resultadoM1,
                                        seed1, m1, n1, width, height);
    if (!ok1) {
        cout << "Fallo al verificar con M1.txt tras XOR.\n";
        delete[] copia;
        return false;
    }

    //2. Rotación a la derecha 3 bits
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
 *
 *Declaración de alias para punteros a funciones de transformación bit a bit: Se usa para no declararla una por una
 *sino, para guardarlas en un arreglo y recorrerlo.
 * (*OperacionBit) es un puntero a una funcion que recibe un char y un int.
 * Se llamaria de la siguiente forma: "rotacionesDerecha[b](data, size)" donde b es la posicion en el arreglo de las
 * rotaciones a la derecha y se ejecuta con data y size, sus parametros."
 */

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
 * Funcionamiento: Se aplican todas las operaciones posibles a la imagen Px.bmp, por medio de la funcion
 * probarTransformacion, que internamente llama a verificarEnmascaramiento. Al llamar la funcion se cargan los archivos
 * Mx.txt y el resultado de aplicara cada una de las transformaciones se compara con estos y si coincide, detecta la
 * transformacion realizada, activa la bandera encontrada y almacena en el arreglo a punteros con el nombre para poder
 * reconstruirla.
 */


int identificarTransformaciones( //Retorna el numero de transformaciones detectadas
    const char* basePath, //const por que no se modifica dentro de la funcion, char* para no usar Qstring
                            //Ruta base para caragar todo.
    int pasos, //Numero de transformaciones para analizar
    unsigned char* imagenAleatoria, //Arreglos dinamico de char (1 byte por canal) y unsigned por ser (0-255)
    unsigned char* mascara,
    unsigned int** archivosTxt, //Arreglo de punteros a los datos del archivo. Int porque puede tener valores mas grandes
                                    //de 255
    int* semillas, //Valores de la semilla de cada archivo
    int* altos, int* anchos, //Valores de las dimensiones de la mascara
    char** registroTransformaciones //Arreglo de punteros a cadenas donde se guardan los nombres de las operaciones
                                        //encontradas
    ) {
    int width = 0, height = 0; //Dimensiones de la imagen intermedia
    unsigned char* imagenActual = nullptr; //Imagen de cada paso
    int transformacionesDetectadas = 0; //Contador que aumenta cada vez que se encuentra una transformacion

    //Cargar imagen temporal
    for (int paso = 0; paso < pasos; ++paso) {
        cout << "\n==========================================\n";
        cout << "Procesando paso " << paso << "...\n";

        char rutaImagen[256];
        sprintf(rutaImagen, "%sP%d.bmp", basePath, paso+1); //Construye la ruta P{paso+1}.bmp asi:
        //rutaImagen es donde va escribir, %s es lo que hay en basePath, %d numero del paso+1

        //Carga la imagen intermedia a partir de la ruta creada
        imagenActual = loadPixels(rutaImagen, width, height);
        if (!imagenActual) {
            cout << "Error al cargar " << rutaImagen << endl;
            return -1; // Devuelve -1 si hubo error
        }

        //A partir de las dimensiones que resultan de la carga de la imagen, se calcula el total de bytes
        int totalBytes = width * height * 3;
        bool encontrada = false;

        //Primero se verifica la operacion XOR con I_M
        if (!encontrada && imagenAleatoria != nullptr) {
            unsigned char* copiaXOR = new unsigned char[totalBytes];
            for (int i = 0; i < totalBytes; ++i) copiaXOR[i] = imagenActual[i];

            xorPixels(copiaXOR, imagenAleatoria, totalBytes);

            //Se verifica con el archivo .txt
            if (verificarEnmascaramiento(copiaXOR, mascara, archivosTxt[paso],
                                         semillas[paso], altos[paso], anchos[paso], width, height, false)) {
                cout << "Paso " << paso << ": XOR con IM\n";
                registroTransformaciones[transformacionesDetectadas] = new char[20]; //Se usa memoria dinamica
                //para poder usarlo nuevamente y guardar adecuadamente las transformaciones detectadas y que
                //sea independiente de cuantas se encuentran.
                sprintf(registroTransformaciones[transformacionesDetectadas], "XOR con IM"); //Crea el nombre de la
                                                                                            //operacion detectada
                encontrada = true;
            }

            delete[] copiaXOR;
        }

        //Misma logica que para el XOR, con cada una de las operaciones, haciendo uso de los arreglos con todas las
        //operaciones
        for (int b = 0; b <= 8 && !encontrada; ++b) { //Hasta 8 (Cantidad max de operacion) o hasta encontrar una
            if (probarTransformacion(imagenActual, width, height, mascara,
                                     archivosTxt[paso], semillas[paso],
                                     altos[paso], anchos[paso],
                                     rotacionesIzquierda[b])) {
                cout << "Paso " << paso << ": Rotacion a la izquierda " << b << " bits\n";
                registroTransformaciones[transformacionesDetectadas] = new char[30];
                sprintf(registroTransformaciones[transformacionesDetectadas], "ROT_LEFT %d bits", b);
                encontrada = true;
            }
        }

        for (int b = 0; b <= 8 && !encontrada; ++b) {
            if (probarTransformacion(imagenActual, width, height, mascara,
                                     archivosTxt[paso], semillas[paso],
                                     altos[paso], anchos[paso],
                                     rotacionesDerecha[b])) {
                cout << "Paso " << paso << ": Rotacion a la derecha " << b << " bits\n";
                registroTransformaciones[transformacionesDetectadas] = new char[30];
                sprintf(registroTransformaciones[transformacionesDetectadas], "ROT_RIGHT %d bits", b);
                encontrada = true;
            }
        }

        for (int b = 0; b <= 8 && !encontrada; ++b) {
            if (probarTransformacion(imagenActual, width, height, mascara,
                                     archivosTxt[paso], semillas[paso],
                                     altos[paso], anchos[paso],
                                     desplazamientosIzquierda[b])) {
                cout << "Paso " << paso << ": Desplazamiento a la izquierda " << b << " bits\n";
                registroTransformaciones[transformacionesDetectadas] = new char[30];
                sprintf(registroTransformaciones[transformacionesDetectadas], "SHIFT_LEFT %d bits", b);
                encontrada = true;
            }
        }

        for (int b = 0; b <= 8 && !encontrada; ++b) {
            if (probarTransformacion(imagenActual, width, height, mascara,
                                     archivosTxt[paso], semillas[paso],
                                     altos[paso], anchos[paso],
                                     desplazamientosDerecha[b])) {
                cout << "Paso " << paso << ": Desplazamiento a la derecha " << b << " bits\n";
                registroTransformaciones[transformacionesDetectadas] = new char[30];
                sprintf(registroTransformaciones[transformacionesDetectadas], "SHIFT_RIGHT %d bits", b);
                encontrada = true;
            }
        }

        if (!encontrada) {
            cout << "Error: No se encontro transformacion valida en el paso " << paso << endl;
            delete[] imagenActual;
            return -1;
        }

        //Liberar memoria
        delete[] imagenActual;
        imagenActual = nullptr;

        transformacionesDetectadas++;
    }

    cout << "\n==========================================\n";
    cout << "*** Todas las transformaciones fueron identificadas exitosamente. ***\n";
    cout << "==========================================\n";

    return transformacionesDetectadas;
}


/*
 * LA FUNCION encontrarTransformacionesGenerico no fue lo suficientemente útil, debido a errores de lógica
 * ya que se trataba de retroceder desde la imagen final y comenzar a reconstruirla, pero no se tenia en cuenta
 * las imagenes intermedias y no esta dando buenos resultados.
 */

/*
 * FUNCION PARA RECONSTRUIR IMAGEN ORIGINAL:
*/

bool reconstruirImagen( //Si se pudo reconstruir la imagen retorna true, si hubo un error retorna false
    const char* rutaImagenFinal, //Ruta de I_D
    const char* rutaImagenAleatoria, //Ruta de I_M
    const char* rutaMascara, //Ruta de M
    int seed, //Valor semilla
    const char* rutaReconstruida, //Ruta para guardar la iamgen reconstruida
    char** registroTransformaciones, //Arreglo de nombres de transformaciones detectadas en el algoritmo
    int pasos //Cantidad de transformaciones
    ) {

    //Cargar imagen final
    int width = 0, height = 0;
    unsigned char* imagen = loadPixels(rutaImagenFinal, width, height);
    if (!imagen) {
        cout << "Error al cargar imagen final (I_D.bmp)." << endl;//Si imagen == nullptr error en la carga
        return false;
    }

    //Cargar imagen aleatoria
    int widthIM = 0, heightIM = 0;
    unsigned char* imagenAleatoria = loadPixels(rutaImagenAleatoria, widthIM, heightIM);
    if (!imagenAleatoria) {
        cout << "Error al cargar imagen aleatoria (I_M.bmp)." << endl; //Si imagenAl == nullptr error en la carga
        delete[] imagen;
        return false;
    }

    //Cargar mascara
    int widthM = 0, heightM = 0;
    unsigned char* mascara = loadPixels(rutaMascara, widthM, heightM);
    if (!mascara) {
        cout << "Error al cargar mascara (M.bmp)." << endl;//Si mascara == nullptr error en la carga
        delete[] imagen;
        delete[] imagenAleatoria;
        return false;
    }

    int totalBytesImagen = width * height * 3;
    int totalBytesMascara = widthM * heightM * 3;

    //Verificar que al enmascarar no se salga de la imagen, no haya desbordamiento
    if (seed + totalBytesMascara > totalBytesImagen) {
        cout << "Error: La mascara y seed exceden el tamaño de la imagen." << endl;
        delete[] imagen;
        delete[] imagenAleatoria;
        delete[] mascara;
        return false;
    }

    //Aplicar transformaciones inversas, de la ultima a la primera
    for (int i = pasos - 1; i >= 0; --i) {

        if (registroTransformaciones[i] == nullptr) continue; //Si no hay transformacion, continua

        cout << "\nAplicando inversa de: " << registroTransformaciones[i] << endl;

        if (strcmp(registroTransformaciones[i], "XOR con IM") == 0) { //Se verifica si es una XOR
            //strcmp compara cadenas de caracteres
            xorPixels(imagen, imagenAleatoria, totalBytesImagen); //XOR se devuelve con XOR
        } else {
            char tipo[20];
            int bits = 0;
            sscanf(registroTransformaciones[i], "%s %*s %d", tipo, &bits); //Extraer la transformacion y la cantidad de bits

            //Segun el tipo se aplica la inversa
            if (strcmp(tipo, "ROT_LEFT") == 0) {
                rotateRight(imagen, totalBytesImagen, bits);
            }
            else if (strcmp(tipo, "ROT_RIGHT") == 0) {
                rotateLeft(imagen, totalBytesImagen, bits);
            }
            else if (strcmp(tipo, "SHIFT_LEFT") == 0) {
                cout << "Advertencia: desplazamiento izquierda no es perfectamente reversible.\n";
                shiftRight(imagen, totalBytesImagen, bits);
            }
            else if (strcmp(tipo, "SHIFT_RIGHT") == 0) {
                cout << "Advertencia: desplazamiento derecha no es perfectamente reversible.\n";
                shiftLeft(imagen, totalBytesImagen, bits);
            }
            else {
                cout << "Error: tipo de transformacion desconocido: " << tipo << endl;
                delete[] imagen;
                delete[] imagenAleatoria;
                delete[] mascara;
                return false;
            }
        }
    }

    //Finalmente exportar imagen reconstruida:
    if (!exportImage(imagen, width, height, rutaReconstruida)) {
        cout << "Error al exportar la imagen reconstruida." << endl;
        //Liberar la memoria
        delete[] imagen;
        delete[] imagenAleatoria;
        delete[] mascara;
        return false;
    }

    cout << "\nImagen reconstruida exitosamente como " << rutaReconstruida << endl;
    //Liberar la memoria
    delete[] imagen;
    delete[] imagenAleatoria;
    delete[] mascara;
    return true;
}


/*
 * FUNCION AUXILIAR: Esta funcion fue creada para revisar la diferencia entre la iamgen reconstruida y la original
 */

bool compararImagenes(const char* ruta1, const char* ruta2) {
    int width1 = 0, height1 = 0;
    int width2 = 0, height2 = 0;

    unsigned char* img1 = loadPixels(ruta1, width1, height1);
    unsigned char* img2 = loadPixels(ruta2, width2, height2);

    if (!img1 || !img2) {
        cout << "Error al cargar las imagenes para comparar." << endl;
        if (img1) delete[] img1;
        if (img2) delete[] img2;
        return false;
    }

    if (width1 != width2 || height1 != height2) {
        cout << "Las imagenes tienen dimensiones diferentes." << endl;
        delete[] img1;
        delete[] img2;
        return false;
    }

    int totalBytes = width1 * height1 * 3;

    for (int i = 0; i < totalBytes; ++i) {
        if (img1[i] != img2[i]) {
            cout << "Diferencia encontrada en byte " << i
                 << ". Imagen 1: " << (int)img1[i]
                 << ", Imagen 2: " << (int)img2[i] << endl;
            delete[] img1;
            delete[] img2;
            return false;
        }
    }

    cout << "\n Las imagenes son iguales." << endl;
    delete[] img1;
    delete[] img2;
    return true;
}

