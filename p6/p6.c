#include <stdio.h>
#include <stdlib.h>

char states, *sigmaM, s, fS, *deltaTab;
char rows;

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

	printf("\nEstado inicial = %d", s);

	printf("\nEstado final = %d", fS);

	printf("\nDelta:\t");
	char flg = 1;
	for(char n = 0; deltaTab[n]; ++n)
		if(deltaTab[n] == ',')
			printf("\n\t");
		else if(deltaTab[n] == 32) {
			printf("-\t"); // ε
			flg = 2;
		}
		else if(flg != 0) {
			printf("%d\t", deltaTab[n]);
			--flg;
		}
		else {
			printf("%c\t", deltaTab[n]);
			flg = 2;
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

char getAInf(const char *nameOfFile) {
	char *text = getFromFile(nameOfFile);
	if(!text) return 1;

	char auxSize, i = 3;
	int aux;

	// states
	if(text[1]!= 13) {
		states = (text[0]-48)*10 + (text[1]-48);
		++i;
	}
	else
		states = text[0]-48;

	sigmaM = getInfo(text, &i); // alphabet
	s = text[++i]-48; // initial state
	i+=3;

	// final state
	if(text[i+1]!= 13) {
		fS = (text[i]-48)*10 + (text[i+1]-48);
		++i;
	}
	else
		fS = text[i]-48;

	// delta table
	for (aux = i+=3, rows = 1; text[aux]; ++aux) if(text[aux] == 13) ++rows;
	printf("\n");
	deltaTab = (char *) malloc((rows*3+1)*sizeof(char));

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

void printAFD() {
	printf("\nDelta del AFD:\nδ");
	for(char n = 0; sigmaM[n]; ++n)
		printf("\t%c", sigmaM[n]);
	printf("\n");
}

char n;

char isIn(char state, char *exp) {
	for(char m = 0; exp[m]; ++m) 
		if(exp[m] == state)
			return 1;
	return 0;
}

void epsilonLock(char actState, char *exp) {
	exp[n] = actState;
	exp[n+1] = '\0';
	++n;
	for(char n = 0; n<rows; ++n)
		if(deltaTab[n*4] == actState && deltaTab[n*4+1] == 32 && !isIn(deltaTab[n*4+2], exp))
			epsilonLock(deltaTab[n*4+2], exp);
}

char move(char *set, char delta) {
	for(char i = 0; set[i]; ++i)
		for(char j = 0; j<rows; ++j)
			if(deltaTab[j*4] == set[i] && deltaTab[j*4+1] == delta)
				return deltaTab[j*4+2];
	return 0;
}

void go2(char *eL) {
	for(char i = 0; eL[i]; ++i)
		printf(" %d", eL[i]);

	for(char j = 0; sigmaM[j]; ++j) {
		printf("\nSearching for %c", sigmaM[j]);
		if(move(eL, sigmaM[j]) != 0) {
			printf("\n%d", move(eL, sigmaM[j]));
		}
	}
}

void toAFD() {
	char exp[rows+1];
	exp[1] = '\0';
	n = 0;
	epsilonLock(s, exp);
	// for(char o = 0; exp[o];++o) printf(" %d", exp[o]);
	go2(exp);

	printAFD();
}

int main(int argc, char const *argv[]) {
	char err = getAInf(argv[argc-1]);
	if(err)
		printf("\t(Err) No se encontro el archivo\n");
	else {
		printf("\nPasando a AFD...\n\n");
		toAFD();
	}
	return 0;
}
