#include <stdio.h>
#include <stdlib.h>

char str[10], *path;
char size, num = 0;

char *states, *sigmaM, s, *fS, (*deltaTab)[3];

char* getFromFile() {
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

	// Just for printing
	// for (char i = 0; text[i]; ++i)
	// 	// if(text[i] != 10 && text[i] != 13)
	// 	if(text[i] != 10)
	// 		printf("%d = %c\n", i, text[i]);
	return text;
}

void printAFN(char x) {
	printf("\nEstados =");
	for(char i = 0; states[i]; ++i) printf(" %c", states[i]);
	
	printf("\nAlfabeto =");
	for(char i = 0;sigmaM[i]; ++i) printf(" %c",sigmaM[i]);

	printf("\nEstado inicial = %c", s);

	printf("\nEstado final =");
	for(char i = 0;fS[i]; ++i) printf(" %c",fS[i]);

	printf("\nDelta:\t");
	for(char n = 0; n<x; ++n){
		for(char m = 0; m<3; ++m)
			printf("%c", deltaTab[n][m]);
		printf("\n\t");
	}
}

char* getInfo(char *text, char *i) {
	char auxSize;
	for (auxSize = *i; text[auxSize] != 10; ++auxSize);

	auxSize = (auxSize - *i)/2;
	char *obj = (char*) malloc((auxSize+1) * sizeof(char));
	obj[auxSize] = '\0';

	for(char aux = 0; text[*i] != 10; *i+=2, ++aux)
		obj[aux] = text[*i];

	return obj;
}

void getAFN() {
	char *text = getFromFile();
	char auxSize, aux, i = 0;
	
	states = getInfo(text, &i); // states
	++i;
	sigmaM = getInfo(text, &i); // alphabet
	s = text[++i]; // initial state
	i+=3;
	fS = getInfo(text, &i); // final states

	// delta table
	char x;
	for (aux = i, x = 0; text[aux]; ++aux) if(text[aux] == 10) ++x;
	
	char auxDeltaTab[x][3];
	for(char n = 0, m = 0, aux = ++i; text[aux]; ++aux)
		if(text[aux] != 13 && text[aux] != ',') {
			auxDeltaTab[n][m] = text[aux];
			++m;
		}
		else if(text[aux] == 13) {
			aux++;
			++n;
			m = 0;
		}
	deltaTab = auxDeltaTab;
	printAFN(x);
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
	printf("Usando el AFN con:\n");
	getAFN();

	printf("\nIngrese la cadena a verificar: ");
	fflush(stdin);
	scanf("%s", str);

	for(size = 0; str[size]; ++size);
	path = (char *) malloc(size*(sizeof(char)));

	

	printf("\n");
	// solve(0, 0);

	printf("Caminos validos = %d\n", num);
	
	return 0;
}
