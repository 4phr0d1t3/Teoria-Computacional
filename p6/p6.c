#include <stdio.h>
#include <stdlib.h>

char states, *sigmaM, s, fS, *deltaTab;
char rows, numSigmaM;

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

	numSigmaM = i;
	sigmaM = getInfo(text, &i); // alphabet
	numSigmaM = ((numSigmaM*-1)+i)/2;

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

// void printAFD() {
// 	printf("\nDelta del AFD:\nδ");
// 	for(char n = 0; sigmaM[n]; ++n)
// 		printf("\t%c", sigmaM[n]);
// 	printf("\n");
// }

char n;

char isIn(char state, int *exp) {
	for(char m = 0; exp[m]; ++m) 
		if(exp[m] == state)
			return 1;
	return 0;
}

void epsilonLock(char actState, int *set, char num) {
	for(char i = 0; i<rows; ++i)
		if(deltaTab[i*4] == actState && deltaTab[i*4+1] == 32) {
			set[(num*(states+1))+deltaTab[i*4+2]] = 1;
			epsilonLock(deltaTab[i*4+2], set, num);
		}
}

char move(int *set, char delta, char num) {
	for(char j = 0; j<rows; ++j)
		if(set[num*states+deltaTab[j*4]] && deltaTab[j*4+1] == delta)
			return deltaTab[j*4+2];
	return 0;
}

void go2(int *set, char num) {
	char whereTo[states];
	printf("\n\tNow:");
	for(char i = 1; i<=states; ++i) {
		printf("\t%d", set[num*states+i]);
		// whereTo[i-1] = 0;
	}
	printf("\n");

	for(char j = 0; sigmaM[j]; ++j) {
		printf("Searching for %c\n", sigmaM[j]);
		if(move(set, sigmaM[j], num) != 0) {
			// whereTo[move(set, sigmaM[j], num)] = 1;
			printf("%d\n", move(set, sigmaM[j], num));
		}
	}
	// printf("\n\t");
	// for(char i = 0; i<states; ++i)
		// printf("\t%d", whereTo[i]);
	printf("\n");
}

void toAFD() {
	int sets[(states+1)*states];
	for(int i = 0; i<(states+1)*states; ++i)
		sets[i] = 0;

	// for(char i = 0; sets[i]; ++i) {
	// 	printf("%d ", sets[i]);
	// 	if(i%4 == 0)
	// 		printf("\n");
	// }

	char exp[rows+1];
	exp[1] = '\0';
	sets[0] = 1;
	n = 1;

	printf("\tSet");
	for(int i = 0; i<states; ++i)
		printf("\t%d", i+1);
	sets[s] = 1;
	epsilonLock(s, sets, 0);

	for(int i = 0; i<(states+1)*states; ++i) {
		if(i%(states+1) == 0)
			printf("\n");
		printf("\t%d", sets[i]);
	}
	char whereTo[states];
	go2(sets, 0);

	// printAFD();
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
