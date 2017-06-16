#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *memoria, c, word[4], *s;
// gp = apuntador a la pila de memoria
int bytes = 0, gp, pc=0, ciclos=0, dr=0;

float r[14];

void endianConv(char *b, char *w){
	word[0] = b[3], word[1] = b[2], word[2] = b[1], word[3] = b[0];
}


float btor(char *b){
	return b[3]; 
			//| ((b[1]) << 8 )
			//| ((b[2]) << 16)
			//| ((b[3]) << 24);
}

void guardaVolcado() {
	FILE* archivo = fopen("volMem.bin", "wb");
	fwrite(memoria, 1, gp, archivo);
	fflush(archivo);
	fclose(archivo);
}

int exitVM(int e){
	switch(e) {
	case 0: printf("Ejecucion finalizada con exito.");
		break;
	case 1: fprintf(stderr, "Division entre cero.");
		break;
	case 2: fprintf(stderr, "Direccion de memoria invalida.");
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
	guardaVolcado();
	exit(e);
}

void mysyscall(){
	int a = 0, b = 0;
	/* Se analiza el caso del codigo de llamada. */
	switch((int)r[8]){
	case 0: /* Leer entero */
		scanf("%d",&a);
		r[10] = a;
		break;
	case 1: /* Leer caracter */
		scanf("%c",&c);
		r[10] = c;
		break;
	case 2: /* Leer flotante. */
		scanf("%f",&(r[10]));
		break;
	case 3: /* Leer cadena */
		s = calloc(gp,1);
		/* Se carga la direccion de almacenamiento de la cadena. */
		a = r[9];
		if(a < 0) exitVM(2);
		/* Se toma lectura de la cadena. */
		gets(s);
		/* Se almacena la cadena en la memoria. */
		do{	if ((a + b) > gp) exitVM(2);
			else memoria[a + b] = s[b];
		}while(s[b++] != 0);
		r[10] = b;
		break;
	case 4: /* Escribir entero */
		printf("%d\n",(int)r[9]);
		break;
	case 5: /* Escribir caracter */
		printf("%c\n",(char)r[9]);
		break;	
	case 6: /* Escribir flotante */
		printf("%.3f\n",r[9]);
		break;
	case 7: /* Escribir cadena */
		/* Se carga la direccion de lectura de la cadena. */
		a = r[9];
		if(a < 0 || a > gp) exitVM(2);
		/* Se imprime la cadena. */
		while(memoria[a] != 0) printf("%c",memoria[a++]);
		printf("\n");
		break;
	case 8: /* Salir del programa */

		exitVM(0);
		break;
	default: exitVM(6);
	}
}

void fdemwb(){
	int op = memoria[pc], dr = 0, a = 0, b = 0, i;
	fflush(stdin);
	printf("PC: %d\tCiclos: %d\n", pc, ciclos);	
	r[12] = pc; /* Se actualiza el registro contador de programa.*/
	//for(i = 0; i < 14; i++) printf("Registro %d: %.0f\t%.3f\n", i, r[i], r[i]);
	/* Se verifica que se haya terminado la ejecucion.*/
	if(pc == bytes) {
		printf("pc == bytes\n");
	 	exitVM(0);
	 }
	/* Se verifica que la instruccion actual tenga en memoria sus parametros.*/
	if((pc + 3) >= bytes) exitVM(8);
	/* Se carga el byte del registro destino. */
	dr = memoria[pc + 1];
	if(dr < 0 || dr > 13) exitVM(4);
	/* Se cargan los parametros para operaciones binarias. */
	if((0 <= op) && (op < 11)){
		/* Se carga el byte del primer parametro.*/
		a = memoria[pc + 2];
		if(a < 0 || a > 13) exitVM(4);
		/* Se carga el byte del segundo parametro.*/
		b = memoria[pc + 3];
		if(b < 0 || b > 13) exitVM(4);
		printf("OpCode: %d\tDestino: $r%d\tOpA: $r%d\tOpB: $r%d\n", op, dr, a, b);
	}else{/* Se verifican los parametros para la instruccion especial li.*/
		if(op == 16){
	 		if((pc + 5) >= bytes) exitVM(5);
	 	}else 
	 		printf("OpCode: %d\trd: $r%d\topA: $r%d\topB: $r%d\n", op, memoria[pc + 1], memoria[pc + 2], memoria[pc + 3]);
	 }
	/* Se analiza el caso del opCode. */
	switch(op) {
	case 0: /* add: Suma entera (con signo) */
		printf("Suma: $r%d = %.0f + %.0f\n", dr, r[a], r[b]);
		r[dr] = r[a] + r[b];
		ciclos += 3;
		break;
	case 1: /* sub: Resta entera (con signo) */
		printf("Resta: $r%d = %.0f - %.0f\n", dr, r[a], r[b]);
		r[dr] = r[a] - r[b];
		ciclos += 4;
		break;
	case 2: /* mul: Producto entero (con signo) */
		printf("Multiplicacion: $r%d = %.0f * %.0f\n", dr, r[a], r[b]);
		r[dr] = r[a] * r[b];		
		ciclos += 10;		
		break;		
	case 3: /* div: Cociente entero (con signo) */		
		if (r[b]==0) exitVM(1);					
		printf("Division: $r%d = %.0f / %.0f\n", dr, r[a], r[b]);
		r[dr] = r[a] / r[b];		
		ciclos += 11;		
		break;		
	case 4: /* fadd: Suma flotante */
		printf("Suma: $r%d = %.5f + %.5f\n", dr, r[a], r[b]);
		r[dr] = r[a] + r[b];		
		ciclos += 4;
		break;
	case 5: /* fsub: Resta flotante */
		printf("Resta: $r%d = %.5f - %.5f\n", dr, r[a], r[b]);
		r[dr] = r[a] - r[b];		
		ciclos += 5;		
		break;		
	case 6: /* fmul: Producto flotante */		
		printf("Multiplicacion: $r%d = %.5f * %.5f\n", dr, r[a], r[b]);		
		r[dr] = r[a] * r[b];		
		ciclos += 9;		
		break;		
	case 7: /* fdiv: Cociente flotante */
		if(r[b]==0.0f) exitVM(1);
		printf("Division: $r%d = %.5f / %.5f\n", dr, r[a], r[b]);		
		r[dr] = r[a] / r[b];		
		ciclos += 10;
		break;		
	case 8: /* and: Operador de bits AND */
		printf("AND: $r%d = %.0f & %.0f\n", dr, r[a], r[b]);
		r[dr] = (int)r[a] & (int)r[b];		
		ciclos += 1;		
		break;		
	case 9: /* or: Operador de bits OR */
		printf("OR: $r%d = %.0f | %.0f\n", dr, r[a], r[b]);
		r[dr] = (a == b)? r[a] : (int)r[a] | (int)r[b];
		ciclos += 1;		
		break;		
	case 10: /* xor: Operador de bits XOR */
		printf("XOR: $r%d = %.0f xor %.0f\n", dr, r[a], r[b]);		
		r[dr] = (int)r[a] ^ (int)r[b];		
		ciclos += 1;		
		break;
	case 11: /* not: Operador de bits NOT */	
		/* Parametro.*/
		a = memoria[pc + 3];		
		if(a < 0 || a > 13) exitVM(4);
		printf("NOT : $r%d = not %.0f\n", dr, r[a]);
		r[dr] = ~(int)r[a];
		ciclos += 1;
		break;
	case 12: /* lb: Cargar Byte */		
		dr = memoria[pc + 3];
		/* Registro a leer.*/
		a = memoria[pc + 1];
		if(a < 0 || a > 13) exitVM(4);
		/* Direccion de memoria en el registro.*/
		b = r[a];
		if(b < 0 || b > gp) exitVM(2);
		/* Carga del byte en el registro.*/
		r[dr] = memoria[b];
		printf("Cargando %d en $r%d\n", memoria[b], dr);
		ciclos += 500;
		break;
	case 13: /* lw: Cargar palabra (4 bytes) */
		dr = memoria[pc + 3];
		/* Registro a leer.*/
		a = memoria[pc + 1];
		if(a < 0 || a > 13) exitVM(4);
		/* Direccion de memoria en el registro.*/
		b = r[a];
		if(b < 0 || (b + 3) > gp) exitVM(2);
		/* Carga de la palabra en el registro.*/
		endianConv(&memoria[b], word);
		r[dr] = btor(word);
		printf("Cargando %.0f en $r%d\n", r[dr], dr);
		ciclos += 1500;
		break;
	case 14: /* sb: Guardar Byte */
		/* Registro del byte a guardar.*/
		a = memoria[pc + 3];
		if(a < 0 || a > 13) exitVM(4);
		/* Direccion de memoria para guardar el byte.*/
		b = r[dr];
		if(b < 0 || b > gp) exitVM(2);
		/* Almacenamiento del byte en la memoria.*/
		memoria[b] = (char)r[a];
		printf("Guardando %d en ($r%d)\n", memoria[b], dr);
		ciclos += 700;
		break;
	case 15: /* sw: Guardar palabra (4 bytes) */
		/* Registro de la palabra a guardar.*/
		a = memoria[pc + 3];
		if(a < 0 || a > 13) exitVM(4);
		/* Direccion de memoria para guardar la palabra.*/
		b = r[dr];
		if(b < 0 || (b + 3) > gp) exitVM(2);
		/* Almacenamiento de la palabra en la memoria.*/
		endianConv((char*)&r[a], &memoria[b]);
		printf("Guardando %0.f en ($r%d)\n",r[a], dr);
		ciclos += 2100;
		break;
	case 16: /* li: Cargar valor constante */
		/* Carga del valor constante en el registro*/
		r[dr] = btor(&memoria[pc + 2]);
		printf("OpCode: 16\trD: $r%d\tCte: %.0f\n", dr, r[dr]);
		printf("Cargando %.0f en $r%d\n", r[dr], dr);
		pc += 7; //son 6 bytes
		ciclos += 1500;
		if (ciclos >= 4500) exitVM(0);
		break;
	case 17: 
		break;
	case 18: /* beqz: Salto si es igual a cero */
		/* Se carga la direccion del salto. */
		b = r[dr];
		if(b < 0 || b >= bytes) exitVM(2);
		/* Registro a evaluar. */
		a = memoria[pc + 3];
		if(a < 0 || a > 13) exitVM(4);
		if(r[a] == 0){
			pc = b - 4;
			printf("Saltando a %d\n", b);
			/* Se guarda la direccion de retorno en el registro. */
			r[11] = pc + 4;
		}ciclos += 4;
		break;
	case 19: /* bltz: Salto si es menor que cero */
		/* Se carga la direccion del salto. */
		b = r[dr];
		if(b < 0 || b >= bytes) exitVM(2);
		/* Registro a evaluar. */
		a = memoria[pc + 3];
		if(a < 0 || a > 13) exitVM(4);
		if(r[a] < 0){
			pc = b - 4;
			printf("Saltando a %d\n", b);
			/* Se guarda la direccion de retorno en el registro. */
			r[11] = pc + 4;			
		}ciclos += 5;
		break;
	case 20: /* syscall: Llamada al sistema */
		printf("Syscall:\tCodigo: %.0f\tArgumento: %.0f\tDato: %.0f\n",r[8], r[9], r[10]);
		mysyscall();
		ciclos += 50;
		break;
	default: exitVM(5);
	} 
	pc += 8;
	printf("PC: %d\n", pc);
	fdemwb();
}

void inicializar(int n, char nombre[]){
	FILE *fileptr;
	char *buffer;
	long filelen;
	memoria = (char*)calloc(n, sizeof(char));

	fileptr = fopen("err1.bin", "rb");  // Open the file in binary mode
	fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(fileptr);             // Get the current byte offset in the file
	if(filelen>n) exitVM(3);
	rewind(fileptr);                      // Jump back to the beginning of the file

	buffer = (char *)malloc((filelen+1)*sizeof(char)); // Enough memory for file + \0
	fread(buffer, filelen, 1, fileptr); // Read in the entire file
	fclose(fileptr); // Close the file

	int i =0;
	for(i=0; i<filelen; i++){

		if(buffer[i] != EOF){
			memoria[i] = buffer[i];
			printf("%d\n", memoria[i]);
		}
	}
	bytes = filelen;

	gp = n - 1; //apunta a la ultima celda de la memoria
	r[13] = gp;


}

int main(int n, char *args[]) {	
	if(n < 2) exitVM(8);
	if(n < 3) inicializar(1000, args[1]);
	else inicializar(atoi(args[2]), args[3]);
	fdemwb();
	return 0;
}
