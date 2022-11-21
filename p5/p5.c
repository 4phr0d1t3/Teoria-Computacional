#include <stdio.h>
#include <stdlib.h>

char str[10], *path;
char size, num = 0;

char *states, *sigmaM, s, *fS;

void getFromFile() {
	FILE *textFile;
	char *text;
	long numbytes;

	textFile = fopen("AFND.txt", "r");

	fseek(textFile, 0L, SEEK_END);
	numbytes = ftell(textFile);
	fseek(textFile, 0L, SEEK_SET);

	text = (char*) calloc(numbytes, sizeof(char));

	fread(text, sizeof(char), numbytes, textFile);
	fclose(textFile);

	// just for printing
	for (char i = 0; text[i]; ++i)
		// if(text[i] != 10 && text[i] != 13)
		if(text[i] != 10)
			printf("%d = %c\n", i, text[i]);

// obtencion del agente
	char auxSize, aux, i = 0;
// states
	for (auxSize = 0; text[auxSize] != 10; ++auxSize);
	auxSize /= 2;
	states = (char*) malloc((auxSize+1) * sizeof(char));
	
	for(aux = 0; text[i] != 10; i+=2, ++aux) states[aux] = text[i];
	states[auxSize] = '\0';
	printf("states = %s\n", states);
// alphabet
	for (auxSize = ++i; text[auxSize] != 10; ++auxSize);
	auxSize = (auxSize-i)/2;

	sigmaM = (char*) malloc((auxSize+1) * sizeof(char));
	for(aux = 0; text[i] != 10; i+=2, ++aux) sigmaM[aux] = text[i];
	sigmaM[auxSize] = '\0';

	printf("alphabet = %s\n", sigmaM);
// initial state
	s = text[++i];

	printf("initial state = %c\n", s);
// final states
	for(auxSize = (i+=3); text[auxSize] != 10; ++auxSize);
	auxSize = (auxSize-i)/2;

	fS = (char*) malloc((auxSize+1) * sizeof(char));
	for(aux = 0; text[i] != 10; i+=2, ++aux) fS[aux] = text[i];
	fS[auxSize] = '\0';

	printf("final states = %s\n", fS);
// delta table
	printf("i = %d\n", i);
}

// to be changed
// void solve(char state, char deltaPot) {
// 	if(str[deltaPot]) {
// 		if(state == 0) {
// 			if(str[deltaPot] == 'b') {
// 				path[deltaPot] = 0;
// 				solve(0, deltaPot+1); // δ(q0, b) -> q0
// 			}
// 			else if(str[deltaPot] == 'a') {
// 				path[deltaPot] = 0;
// 				solve(0, deltaPot+1); // δ(q0, a) -> q0
				
// 				path[deltaPot] = 1;
// 				solve(1, deltaPot+1); // δ(q0, a) -> q1
// 			}
// 		}
// 		else if(state == 1)
// 			if(str[deltaPot] == 'b') {
// 				path[deltaPot] = 2;
// 				solve(2, deltaPot+1); // δ(q1, b) -> q2
// 			}
// 	}
// 	else if(deltaPot == size && (state == 0 || state == 2)){
// 		printf("Camino Valido:\n\t--> q0");
// 		for(char i = 0; i<size; ++i)
// 			printf(" -%c-> q%d", str[i], path[i]);
// 		printf("\n");
// 		++num;
// 	}
// }

int main(int argc, char const *argv[]) {
	printf("Ingrese la cadena a verificar: ");
	fflush(stdin);
	scanf("%s", str);

	for(size = 0; str[size]; ++size);
	path = (char *) malloc(size*(sizeof(char)));

	getFromFile();

	printf("\n");
	// solve(0, 0);

	printf("Caminos validos = %d\n", num);
	
	return 0;
}
