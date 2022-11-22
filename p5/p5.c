#include <stdio.h>
#include <stdlib.h>

char str[100], *path;
char size, x, num = 0;

char *states, *sigmaM, s, *fS, (*deltaTab)[4];

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

void printAFN() {
	printf("\nEstados =");
	for(char i = 0; states[i]; ++i) printf(" %c", states[i]);
	
	printf("\nAlfabeto =");
	for(char i = 0;sigmaM[i]; ++i) printf(" %c",sigmaM[i]);

	printf("\nEstado inicial = %c", s);

	printf("\nEstado final =");
	for(char i = 0;fS[i]; ++i) printf(" %c",fS[i]);

	printf("\nDelta:");
	for(char n = 0; n<x; ++n) printf("\t%s\n", deltaTab[n]);
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
	for (aux = ++i, x = 1; text[aux]; ++aux) if(text[aux] == 10) ++x;
	char auxDeltaTab[x][4];
	for(char n = 0, m = 0, aux = i; text[aux]; ++aux)
		if(text[aux] != 10 && text[aux] != 13 && text[aux] != ',') {
			auxDeltaTab[n][m] = text[aux];
			++m;
		}
		else if(text[aux] == 13) {
			auxDeltaTab[n][3] = '\0';
			++aux;
			++n;
			m = 0;
		}
	deltaTab = auxDeltaTab;
	printAFN();
}

char existsState(char state) {
	for(char i = 0; i < x; ++i)
		if(state == deltaTab[i][0])
			return i;
	return -1;
}

char isFinalState(char state) {
	for(char i = 0; fS[i]; ++i)
		if(state == fS[i])
			return 1;
	return 0;
}

void solve(char state, char deltaPot) {
	printf("\n----%c----\n", str[deltaPot]);
	for(char n = 0; n<x; ++n) printf("\t%s\n", deltaTab[n]);
	printf("Buscando: %c, %c\n", state, str[deltaPot]);

	if(str[deltaPot]) {
		for(char aux = 0; aux < x; ++aux) {
			printf("Probando: %c %c %c\n", deltaTab[aux][0], deltaTab[aux][1], deltaTab[aux][2]);
			// for(char n = 0; n<x; ++n) printf("\t%s\n", deltaTab[n]);

			if(state == deltaTab[aux][0] && str[deltaPot] == deltaTab[aux][1]) {
				printf("Encontrado: %c = %c, %c = %c -> %c\n", state, deltaTab[aux][0], str[deltaPot], deltaTab[aux][1], deltaTab[aux][2]);
				path[deltaPot] = deltaTab[aux][2];
				// for(char n = 0; n<x; ++n) printf("\t%s\n", deltaTab[n]);

				solve(deltaTab[aux][2], deltaPot + 1);
			}
		}
	}
	else if(deltaPot == size && isFinalState(state)) {
		printf("Camino Valido:\n\t--> q%c", s);
		for(char i = 0; i<size; ++i)
			printf(" -%c-> q%c", str[i], path[i]);
		printf("\n");
		++num;
	}
}

int main(int argc, char const *argv[]) {
	printf("Usando el AFN con:\n");
	getAFN();

	printf("Ingrese la cadena a verificar: ");
	fflush(stdin);
	scanf("%s", str);

	for(size = 0; str[size]; ++size);
	char auxPath[size];
	path = auxPath;

	solve('0', 0);

	printf("Caminos validos = %d\n", num);
	
	return 0;
}
