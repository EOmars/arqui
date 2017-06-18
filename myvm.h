/*
 * Proyecto final
 * Julio David Sanchez Chavez
 * Jose Alejandro Perez Marquez
 * Elías Omar Pérez Martínez
 * 
 *
 */
/* Arreglo que representa la memory y variables auxiliares.*/

char	*memory, 
		c, 
		w[4], 
		*s;

/* Variables auxiliares.*/
// sp = apuntador a la pila de memoria
int 	bytes, 
		sp, 
		pc, 
		ciclos, 
		dr;

/* Representacion de los registros.*/
union registro{
	int i;
	float f;
}r[14];


void volcado();

/**
 * Invierte el orden de los bytes de una palabra
 * @param b Apuntador al primer byte de la palabra en su orden original,
 * @return Devuelve un apuntador al primer byte de la palabra ordenada.
 */
char* toEndian(char *);

/**
 * Obtiene el valor entero de 4 bytes
 * @param b Apuntador al primer byte de la palabra.
 * @return Devuelve el valor entero de la palabra.
 */
int getIntValue(char *b);

/**
* Funcion que detiene el programa, mostrando la causa en consola.
* @param e Codigo de salida.
* @return Devuelve un codigo de salida.
*/
int exitVM(int error);

/**
 * Metodo encargado de realizar las llamadas al sistema.
 */
void syscall();

void cpu();

/**
* Metodo que carga la memory desde un archivo binario.
* @param n Cantidad de memory asignada.
* @param filename Nombre del archivo.
*/
void start(int n, char filename[]);