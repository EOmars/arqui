
#include <stdio.h>
#include <stdlib.h>

typedef union Registro {
	int* entero;
	float* flotante;
};

void decode(char opcode) {
	switch(opcode) {
		case 0x0:
			break;		
		case 0x1:
			break;		
		case 0x2:
			break;		
		case 0x3:
			break;		
		case 0x4:
			break;		
		case 0x5:
			break;		
		case 0x6:
			break;		
		case 0x7:
			break;		
		case 0x8:
			break;		
		case 0x9:
			break;		
		case 0x10:
			break;		
		case 0x11:
			break;		
		case 0x12:
			break;		
		case 0x13:
			break;		
		case 0x14:
			break;		
		case 0x15:
			break;		
		case 0x16:
			break;		
		case 0x17:
			break;		
		case 0x18:
			break;		
		case 0x19:
			break;		
		default:
			break;
	}
}

/**
* Funcion que carga un archivo binario.
* @param nombre Es el nombre del archivo que se carga.
*/
int cargaArchivo(char buffer[], char nombre[]) {
	FILE* archivo = fopen(strcat(nombre, ".bin"), "rb");
	int nBytes, r;
	fseek(archivo, 0, SEEK_END);
	nBytes = ftell(archivo);
	r = fread(buffer, 1, nBytes, archivo);
	fflush(archivo);
	close(archivo);
	return r;
}

int main(int n, char *args[]) {
	char *memoria, nombre[] = "mem.bin";
	FILE* f;
	int m, nBytes;	
	/* Lectura de archivo. */
	/* Declaracion de memoria */
	n = atoi(args[1]);
	memoria = (char*)calloc(n, 1);
	f = fopen(nombre, "rb");
	fseek(f, 0, SEEK_END);
	nBytes = ftell(f);
	fread(memoria, 1, n, f);
	for(m = 0; m < nBytes; m++) printf("\nChar %d: %x", m, memoria[m]);
	close(f);
	return 0;
}
/**
* Funcion que guarda datos en un archivo binario.
* @param nombre Es el nombre del archivo con que se guardan los datos.
*/
void guardaEnArchivo(int n, char* args[], void* w, char nombre[]) {
	/* Escritura directa de linea de comandos.
	char *w = (char*)calloc(--n, 1);
	f = fopen(nombre, "wb");
	printf("Argumentos leidos: %d", n);
	while(args[++m] != NULL){
	printf("\nCadena leida: %x", atoi(args[m--]));
	printf("\nAntes de la asignacion: %x", w[m]);
	w[m++] = (char)atoi(args[m--]);
	printf("\nCadena asignada: %x", w[m++]);
	}
	fwrite(w, 1, n, f);
	fflush(f);
	close(f); */
}

