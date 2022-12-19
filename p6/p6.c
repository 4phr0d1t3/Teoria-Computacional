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

char isIn(char state, int *set) {
	for(char m = 0; set[m]; ++m) 
		if(set[m] == state)
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

void move(int *set, char num, int *whereTo, char numWhere, char delta) {
	for(char j = 0; j<rows; ++j)
		if(set[num*(states+1)+deltaTab[j*4]] && deltaTab[j*4+1] == delta)
			whereTo[numWhere*(states+1)+deltaTab[j*4+2]] = 1;
}

char numLetters = 0;

void newLetter(int *set, int *movement) {
	char newLetters[3] = {0, 0, 0};
	char repeated[3] = {0, 0, 0};
	char numRep = 0;
	char isZero = 0;
	// for(char k = 0; k<states; ++k)

	for(char i = 0; i<numSigmaM; ++i)
		for(char j = 0; set[j*(states+1)]; ++j)
			for(char k = 0; k<states+1; ++k)
				if(movement[i*(states+1)+k] != set[j*(states+1)+k]) {
					break;
				}
				else if(k = states)
					repeated[i] = j*(states+1);
	for(char i = 0; i<numSigmaM; ++i) {
		for(char k = 0; k<states; ++k) {
			if(movement[i*(states+1)+k]) {
				printf("\nno es 0 %d\n", i);
				break;
			}
			else if(k == states-1) {
				repeated[i] = -1;
				++numRep;
			}
			printf("%d", movement[i*(states+1)+k]);
		}
		printf("\n");
	}
	
	// printf("\nrepeated: %d", numRep);
	printf("\nrepeated:");
	for(char i = 0; i<numSigmaM; ++i)
		printf(" %d", repeated[i]);
	printf("\n");

	// printf("%d\n", numLetters);
	for(char i = 0; i<numSigmaM; ++i) {
		if(repeated[i] == 0) {
			set[(i+numLetters-numRep)*(states+1)] = 1;
			for(char j = 1; j<states+1; ++j)
				set[j+(i+numLetters-numRep)*(states+1)] = movement[j+i*(states+1)];
			// printf("\n");
		}
	}
	numLetters+= numSigmaM - numRep;
	// printf("\n numLett: %d", numLetters);

	for(int i = 0; i<(states+1)*states; ++i) {
		if(i%(states+1) == 0)
			printf("\n");
		printf("\t%d", set[i]);
	}
	printf("\n");
}

void go2(int *set, char num) {
	int whereTo[(states+1)*numSigmaM];

	for(int i = 0; i<(states+1)*numSigmaM; ++i)
		whereTo[i] = 0;
	
	// printf("\nWhere To");
	for(int i = 0; sigmaM[i]; ++i)
		move(set, num, whereTo, i, sigmaM[i]);

	// for(int i = 0; i<(states+1)*numSigmaM; ++i) {
	// 	if(i%(states+1) == 0)
	// 		printf("\n");
	// 	printf("\t%d", whereTo[i]);
	// }
	
	for(int i = 0; i<numSigmaM; ++i)
		for (int j = 0; j < states+1; ++j)
			if(whereTo[i*(states+1)+j])
				epsilonLock(j, whereTo, i);

	// printf("\nWhere To New");
	// for(int i = 0; i<(states+1)*numSigmaM; ++i) {
	// 	if(i%(states+1) == 0)
	// 		printf("\n");
	// 	printf("\t%d", whereTo[i]);
	// }
	// printf("\n");
	newLetter(set, whereTo);
}

void repeat(int * set) {
	// for(char i = 0; set[i*(states+1)] == 1; ++i)
	// 	go2(set, i);
	go2(set, 0);
	go2(set, 1);
	// go2(set, 2);
}

void toAFD() {
	int sets[(states+1)*states];
	for(int i = 0; i<(states+1)*states; ++i)
		sets[i] = 0;

	printf("\tSet");
	for(int i = 0; i<states; ++i)
		printf("\t%d", i+1);
	sets[0] = 1;
	sets[s] = 1;
	epsilonLock(s, sets, 0);
	++numLetters;
	for(int i = 0; i<(states+1)*states; ++i) {
		if(i%(states+1) == 0)
			printf("\n");
		printf("\t%d", sets[i]);
	}
	printf("\n");
	repeat(sets);

	// printAFD();
}

int main(int argc, char const *argv[]) {
	char err = getAInf(argv[argc-1]);
	if(err)
		printf("\t(Err) No se encontro el archivo\n");
	else {
		printf("\n\nPasando a AFD...\n\n");
		toAFD();
	}
	return 0;
}
