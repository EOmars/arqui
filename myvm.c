/*
 * Autores:
 * 
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myvm.h"

/**
 * Invierte el orden de los bytes de una palabra
 * @param b Apuntador al primer byte de la palabra en su orden original,
 * @return Devuelve un apuntador al primer byte de la palabra ordenada.
 */
char* toEndian(char *b){
	w[0] = b[3], w[1] = b[2], w[2] = b[1], w[3] = b[0];
	return w;
}

/**
 * Metodo que unifica los bytes de una palabra para determinar su valor entero.
 * @param b Apuntador al primer byte de la palabra.
 * @return Devuelve el valor entero de la palabra.
 */
int getIntValue(char *b){
	/* Recorriendo los bytes segun su posicion en el entero. */
	return (b[0]<<24) + (b[1]<<16) + (b[2]<<8) + (b[3]);
}

void volcado() {
	FILE* archivo = fopen("volcado.bin", "wb");
	fwrite(memory, 1, sp, archivo);
	fflush(archivo);
	fclose(archivo);
}

/**
* Funcion que detiene el programa, mostrando la causa en consola.
* @param e Codigo de salida.
* @return Devuelve un codigo de salida.
*/
int exitVM(int error){
	switch(error) {
	case 0: printf("Ejecucion finalizada con exito.");
		break;
	case 1: fprintf(stderr, "Division entre cero.");
		break;
	case 2: fprintf(stderr, "Direccion de memory invalida.");
		break;
	case 3: fprintf(stderr, "Memoria agotada.");
		break;
	case 4: fprintf(stderr, "Numero de registro invalido.");
		break;
	case 5: fprintf(stderr, "Codigo de operacion invalido.");
		break;
	case 6: fprintf(stderr, "Codigo de llamada al sistema invalido.");
		break;
	case 7: fprintf(stderr, "Error al cargar el archivo.");
		break;
	case 8: fprintf(stderr, "Argumentos invalidos.");
		break;
	default: break;
	} 
	printf("\n");
	exit(error);
}

/**
 * Metodo encargado de realizar las llamadas al sistema.
 */
void syscall(){
	int a = 0, b = 0;
	/* Se analiza el caso del codigo de llamada. */
	switch(r[8].i){
	case 0: /* Leer entero */
		fflush(stdin);
		scanf("%d",&(r[10].i));
		break;
	case 1: /* Leer caracter */
		r[10].i = getchar();
		break;
	case 2: /* Leer flotante. */
		fflush(stdin);
		scanf("%f",&(r[10].f));
		break;
	case 3: /* Leer cadena */
		s = calloc(sp,1);
		/* Se carga la direccion de almacenamiento de la cadena. */
		a = bytes + r[9].i;
		if(a < bytes) exitVM(2);
		/* Se toma lectura de la cadena. */
		gets(s);
		/* Se almacena la cadena en la memoria. */
		do{	if ((a + b) > sp) exitVM(2);
			else memory[a + b] = s[b];
		}while(s[b++] != 0);
		r[10].i = b;
		break;
	case 4: /* Escribir entero */
		printf("%d\n",r[9].i);
		break;
	case 5: /* Escribir caracter */
		printf("%c\n",(char)r[9].i);
		break;	
	case 6: /* Escribir flotante */
		printf("%.3f\n",r[9].f);
		break;
	case 7: /* Escribir cadena */
		/* Se carga la direccion de lectura de la cadena. */
		a = bytes + r[9].i;
		if(a < bytes || a > sp) exitVM(2);
		/* Se carga la longitud de la cadena. */
		b = a + r[10].i;
		if(b > sp) exitVM(2);
		/* Se imprime la cadena. */
		while(a < b) printf("%c",memory[a++]);
		printf("\n");
		break;
	case 8: /* Salir del programa */
		exitVM(0);
		break;
	default: exitVM(6);
	}
}

/**
 * Metodo que modifica 
void setR(int i, int t){
	
}

/**
 * Metodo que realiza recursivamente el Fecth, Decode, Excecute, Mem y WriteBack
 * de la instruccion apuntada por el program counter.
 */
void cpu(){
	int op = memory[pc], dr = 0, a = 0, b = 0, i;
	printf("PC: %d\tCiclos: %d\n", pc, ciclos);	
	r[12].i = pc; /* Se actualiza el registro contador de programa.*/
	for(i = 0; i < 14; i++) printf("Registro %d: %d\t%.3f\n", i, r[i].i, r[i].f);
	/* Se verifica que se haya terminado la ejecucion.*/
	if(pc == bytes) exitVM(0);
	/* Se verifica que la instruccion actual tenga en memory sus parametros.*/
	if((pc + 3) >= bytes) exitVM(8);
	/* Se carga el byte del registro destino. */
	dr = memory[pc + 1];
	if(dr < 0 || dr > 13) exitVM(4);
	/* Se cargan los parametros para operaciones binarias. */
	if((0 <= op) && (op < 11)){
		/* Se carga el byte del primer parametro.*/
		a = memory[pc + 2];
		if(a < 0 || a > 13) exitVM(4);
		/* Se carga el byte del segundo parametro.*/
		b = memory[pc + 3];
		if(b < 0 || b > 13) exitVM(4);
		printf("OpCode: %d\tDestino: $r%d\tOpA: $r%d\tOpB: $r%d\n", op, dr, a, b);
	}else{/* Se verifican los parametros para la instruccion especial li.*/
	if(op == 16){ if((pc + 5) >= bytes) exitVM(5);}
	else printf("OpCode: %d\trd: $r%d\topA: $r%d\topB: $r%d\n", op, memory[pc + 1], memory[pc + 2], memory[pc + 3]);}
	/* Se analiza el caso del opCode. */
	switch(op) {
	case 0: /* add: Suma entera (con signo) */
		printf("Suma: $r%d = %d + %d\n", dr, r[a].i, r[b].i);
		r[dr].i = r[a].i + r[b].i;
		ciclos += 3;
		break;
	case 1: /* sub: Resta entera (con signo) */
		printf("Resta: $r%d = %d - %d\n", dr, r[a].i, r[b].i);
		r[dr].i = r[a].i - r[b].i;
		ciclos += 4;
		break;
	case 2: /* mul: Producto entero (con signo) */
		printf("Multiplicacion: $r%d = %d * %d\n", dr, r[a].i, r[b].i);
		r[dr].i = r[a].i * r[b].i;		
		ciclos += 10;		
		break;		
	case 3: /* div: Cociente entero (con signo) */		
		if (r[b].i==0) exitVM(1);					
		printf("Division: $r%d = %d / %d\n", dr, r[a].i, r[b].i);
		r[dr].i = r[a].i / r[b].i;		
		ciclos += 11;		
		break;		
	case 4: /* fadd: Suma flotante */
		printf("Suma: $r%d = %.5f + %.5f\n", dr, r[a].f, r[b].f);
		r[dr].f = r[a].f + r[b].f;		
		ciclos += 4;
		break;
	case 5: /* fsub: Resta flotante */
		printf("Resta: $r%d = %.5f - %.5f\n", dr, r[a].f, r[b].f);
		r[dr].f = r[a].f - r[b].f;		
		ciclos += 5;		
		break;		
	case 6: /* fmul: Producto flotante */		
		printf("Multiplicacion: $r%d = %.5f * %.5f\n", dr, r[a].f, r[b].f);		
		r[dr].f = r[a].f * r[b].f;		
		ciclos += 9;		
		break;		
	case 7: /* fdiv: Cociente flotante */
		if(r[b].f==0.0f) exitVM(1);
		printf("Division: $r%d = %.5f / %.5f\n", dr, r[a].f, r[b].f);		
		r[dr].f = r[a].f / r[b].f;		
		ciclos += 10;
		break;		
	case 8: /* and: Operador de bits AND */
		printf("AND: $r%d = %d & %d\n", dr, r[a].i, r[b].i);		
		r[dr].i = r[a].i & r[b].i;		
		ciclos += 1;		
		break;		
	case 9: /* or: Operador de bits OR */
		printf("OR: $r%d = %d | %d\n", dr, r[a].i, r[b].i);
		r[dr].i = r[a].i | r[b].i;
		ciclos += 1;		
		break;		
	case 10: /* xor: Operador de bits XOR */
		printf("XOR: $r%d = %d xor %d\n", dr, r[a].i, r[b].i);		
		r[dr].i = r[a].i ^ r[b].i;		
		ciclos += 1;		
		break;
	case 11: /* not: Operador de bits NOT */	
		/* Parametro.*/
		a = memory[pc + 3];		
		if(a < 0 || a > 13) exitVM(4);
		printf("NOT : $r%d = not %d\n", dr, r[a].i);
		r[dr].i = ~r[a].i;
		ciclos += 1;
		break;
	case 12: /* lb: Cargar Byte */		
		/* Registro a leer.*/
		a = memory[pc + 3];
		if(a < 0 || a > 13) exitVM(4);
		/* Direccion de memory en el registro.*/
		b = bytes + r[a].i;
		if(b < bytes || b > sp) exitVM(2);
		/* Carga del byte en el registro.*/
		r[dr].i = memory[b];
		printf("Cargando %d en $r%d\n", memory[b], dr);
		ciclos += 500;
		break;
	case 13: /* lw: Cargar palabra (4 bytes) */
		/* Registro a leer.*/
		a = memory[pc + 3];
		if(i < 0 || i > 13) exitVM(4);
		/* Direccion de memory en el registro.*/
		b = bytes + r[a].i;
		if(b < bytes || (b + 3) > sp) exitVM(2);
		/* Carga de la palabra en el registro.*/
		r[dr].i = getIntValue(toEndian(&memory[b]));
		printf("Cargando %d en $r%d\n", r[dr].i, dr);
		ciclos += 1500;
		break;
	case 14: /* sb: Guardar Byte */
		/* Registro del byte a guardar.*/
		a = memory[pc + 3];
		if(a < 0 || a > 13) exitVM(4);
		/* Direccion de memory para guardar el byte.*/
		b = bytes + r[dr].i;
		if(b < bytes || b > sp) exitVM(2);
		/* Almacenamiento del byte en la memory.*/
		memory[b] = (char)r[a].i;
		printf("Guardando %d en ($r%d)\n", memory[b], dr);
		ciclos += 700;
		break;
	case 15: /* sw: Guardar palabra (4 bytes) */
		/* Registro de la palabra a guardar.*/
		a = memory[pc + 3];
		if(a < 0 || a > 13) exitVM(4);
		/* Direccion de memory para guardar la palabra.*/
		b = bytes + r[dr].i;
		if(b < bytes || (b + 3) > sp) exitVM(2);
		/* Almacenamiento de la palabra en la memory.*/
		toEndian((char*)&r[a].i);
		memory[b] = w[0], memory[b + 1] = w[1], memory[b + 2] = w[2], memory[b + 3] = w[3];
		printf("Guardando %d en ($r%d)\n",r[a].i, dr);
		ciclos += 2100;
		break;
	case 16: /* li: Cargar valor constante */
		/* Carga del valor constante en el registro*/
		r[dr].i = getIntValue(&memory[pc + 2]);
		printf("OpCode: 16\trD: $r%d\tCte: %d\n", dr, r[dr].i);
		printf("Cargando %d en $r%d\n", r[dr].i, dr);
		pc += 2;
		ciclos += 1500;
		break;
	case 17: /* b: Salto incondicional */
		/* Se carga el indice del registro destino. */
		dr = memory[pc + 3];
		if(dr < 0 || dr > 13) exitVM(4);
		/* Se carga la direccion del salto. */
		b = r[dr].i;
		if(b < 0 || b >= bytes) exitVM(2);
		pc = b - 4;
		printf("Saltando a %d\n", b);
		/* Se guarda la direccion de retorno en el registro. */
		r[11].i = pc + 4;
		ciclos += 1;
		break;
	case 18: /* beqz: Salto si es igual a cero */
		/* Se carga la direccion del salto. */
		b = r[dr].i;
		if(b < 0 || b >= bytes) exitVM(2);
		/* Registro a evaluar. */
		a = memory[pc + 3];
		if(a < 0 || a > 13) exitVM(4);
		if(r[a].i == 0){
			pc = b - 4;
			printf("Saltando a %d\n", b);
			/* Se guarda la direccion de retorno en el registro. */
			r[11].i = pc + 4;
		}ciclos += 4;
		break;
	case 19: /* bltz: Salto si es menor que cero */
		/* Se carga la direccion del salto. */
		b = r[dr].i;
		if(b < 0 || b >= bytes) exitVM(2);
		/* Registro a evaluar. */
		a = memory[pc + 3];
		if(a < 0 || a > 13) exitVM(4);
		if(r[a].i < 0){
			pc = b - 4;
			printf("Saltando a %d\n", b);
			/* Se guarda la direccion de retorno en el registro. */
			r[11].i = pc + 4;			
		}ciclos += 5;
		break;
	case 20: /* syscall: Llamada al sistema */
		printf("Syscall:\tCodigo: %d\tArgumento: %d\tDato: %d\n",r[8].i, r[9].i, r[10].i);
		syscall();
		ciclos += 50;
		break;
	default: exitVM(5);
	} pc += 4;
	//getchar();
	cpu();
}

/**
* Metodo que carga la memory desde un archivo binario.
* @param n Cantidad de memory asignada.
* @param filename Nombre del archivo.
*/
void inicializar(int n, char filename[]){
	FILE *fileptr;
	char *buffer;
	long filelen;
	memory = (char*)calloc(n, sizeof(char));

	fileptr = fopen(filename, "rb"); 
	fseek(fileptr, 0, SEEK_END);     
	filelen = ftell(fileptr);        
	if(filelen>n) exitVM(3);
	rewind(fileptr);                 

	buffer = (char *)malloc((filelen+1)*sizeof(char)); // file + \0
	fread(buffer, filelen, 1, fileptr); 
	fclose(fileptr); 

	int i =0;
	for(i=0; i<filelen; i++){

		if(buffer[i] != EOF){
			memory[i] = buffer[i];
			printf("%d\n", memory[i]);
		}
	}
	bytes = filelen;

	sp = n - 1; //apunta a la ultima celda de la memory
	r[13].i = sp-bytes;
}

/**
 * Funcion principal de la ejecucion del programa.
 * @param n Cantidad de parametros.
 * @param args Argumentos dados en la linea de comandos.
 * @return Devuelve un codigo de salida.
 */
int main(int n, char *args[]) {	
	if(n < 2) exitVM(8);
	if(n < 3) inicializar(1000, args[2]);
	else inicializar(atoi(args[2]), args[3]);
	cpu();
	return 0;
}
