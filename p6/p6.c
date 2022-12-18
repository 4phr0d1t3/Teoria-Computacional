#include <stdio.h>
#include <stdlib.h>

char x;
char states, *sigmaM, s, *fS, *deltaTab;
char *deltaTabAux;

char* getFromFile(const char *nameOfFile) {
	FILE *textFile;
	char *text;
	long numbytes;

	textFile = fopen(nameOfFile, "r");
	if(textFile == NULL)
		return 0;

	fseek(textFile, 0L, SEEK_END);
	numbytes = ftell(textFile);
	fseek(textFile, 0L, SEEK_SET);

	text = (char*) calloc(numbytes, sizeof(char));

	fread(text, sizeof(char), numbytes, textFile);
	fclose(textFile);

	return text;
}

void printAInf() {
	printf("\nEstados = 1 - %d", states);
	
	printf("\nAlfabeto =");
	for(char i = 0; sigmaM[i]; ++i) printf(" %c",sigmaM[i]);

	printf("\nEstado inicial = %c", s);

	printf("\nEstado final =");
	for(char i = 0; fS[i]; ++i) printf(" %c",fS[i]);

	printf("\nDelta:\t");
	for(char n = 0; deltaTab[n]!='\0'; ++n)
		if(deltaTab[n] == ',')
			printf("\n\t");
		else if(deltaTab[n] == 32)
			printf("ε\t");
		else if(deltaTab[n] < 32)
			printf("%d\t", deltaTab[n]);
		else
			printf("%c\t", deltaTab[n]);
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

char getAInf(const char *nameOfFile) {
	char *text = getFromFile(nameOfFile);
	if(!text) return 1;

	char auxSize, aux, i = 0;

	// states
	if(text[1]!= 13) {
		states = (text[0]-48)*10 + (text[1]-48);
		i = 4;
	}
	else {
		states = text[0]-48;
		i = 3;
	}
	sigmaM = getInfo(text, &i); // alphabet
	s = text[++i]; // initial state
	i+=3;
	fS = getInfo(text, &i); // final states

	// delta table
	for (aux = ++i, x = 1; text[aux]; ++aux) if(text[aux] == 10) ++x;
	deltaTab = (char *) malloc((x*3+1)*sizeof(char));
	deltaTabAux = (char *) malloc((x*3+1)*sizeof(char));

	char j;
	for(j = 0, aux = i; text[aux]; ++aux)
		if(text[aux] != 13 && text[aux] != ',') {
			if(text[aux] == 10)
				deltaTab[j] = ',';
			else if(text[aux] > 47 && text[aux] < 58) {
				if(text[aux+1] > 47 && text[aux+1] < 58) {
					deltaTab[j] = (text[aux] - 48)*10 + text[aux+1] - 48;
					++aux;
				}
				else
					deltaTab[j] = text[aux] - 48;
			}
			else
				deltaTab[j] = text[aux];
			++j;
		}
	deltaTab[j] = '\0';

	printAInf();

	return 0;
}

int main(int argc, char const *argv[]) {
	char err = getAInf(argv[argc-1]);
	if(err)
		printf("\t(Err) No se encontro el archivo\n");
	else {
		printf("\nPasando a AFD...\n\n");
	}
	return 0;
}
