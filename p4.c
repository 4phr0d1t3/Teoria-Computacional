#include <stdio.h>
#include <stdlib.h>

char str[10];

void solve(char state, char deltaPot) {
	if(str[deltaPot]) {
		printf(" -%d-> q%d \n δ(q%d, %c)", deltaPot, state, state, str[deltaPot]);

		switch (str[deltaPot]) {
			case 'a':
				if(state == 0) {
					solve(0, deltaPot+1); // δ(q0, a) -> q0
					printf("\n\tNuevo camino: ");
					solve(1, deltaPot+1); // δ(q0, a) -> q1
				}
				else if (state == 1)
					solve(2, deltaPot+1); // δ(q1, a) -> q2
			break;

			case 'b':
				if(state == 0)
					solve(0, deltaPot+1); // δ(q0, b) -> q0
			break;

			default:
				printf(" -> Ø\n----- No es valida -----\n"); // -> Ø
		}
	}
	else if(state == 0 || state == 2) // q0 || q2
		printf(" -%d-> q%d\n\nLa cadena es valida\n", deltaPot, state);
	else
		printf(" La cadena no es valida");
}

int main(int argc, char const *argv[]) {
	printf("Ingrese la cadena a verificar: ");
	fflush(stdin);
	scanf("%s", str);

	solve(0, 0);
	printf("\n");

	return 0;
}
