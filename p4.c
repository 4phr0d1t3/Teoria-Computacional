#include <stdio.h>
#include <stdlib.h>

char str[10];
char size;

void solve(char state, char deltaPot) {
	if(str[deltaPot]) {
		printf("\tδ(q%d, %c) -%d-> ", state, str[deltaPot], deltaPot);
		if(state == 0)
			if(str[deltaPot] == 'b') {
				printf("q0\n");
				solve(0, deltaPot+1); // δ(q0, b) -> q0
			}
			else if(str[deltaPot] == 'a') {
				printf("q0\n");
				solve(0, deltaPot+1); // δ(q0, a) -> q0
				
				printf(" Nuevo camino en: -%d-> q1\n", deltaPot);
				solve(1, deltaPot+1); // δ(q0, a) -> q1
			}
			else
				printf("Ø\n       --- No valido ---\n"); // -> Ø
		else if(state == 1)
			if(str[deltaPot] == 'b') {
				printf("q2\n");
				solve(2, deltaPot+1); // δ(q1, b) -> q2
			}
			else
				printf("Ø\n       --- No valido ---\n"); // -> Ø
		else if(state == 2)
			printf("Ø\n       --- No valido ---\n"); // -> Ø
	}
	else if(deltaPot != size)
		printf(" -%d-> q%d\n", deltaPot, state);
	else if(state != 0 && state !=2)
		printf("       --- No valido ---\n");
	else
		printf("\t--- Valido ---\n");
}

int main(int argc, char const *argv[]) {
	printf("Ingrese la cadena a verificar: ");
	fflush(stdin);
	scanf("%s", str);

	for(size = 0; str[size]; ++size);

	printf("\n");
	solve(0, 0);

	return 0;
}
