#include <stdio.h>
#include <stdlib.h>

typedef struct _state {
	struct _state *a[2];
	struct _state *b[2];
	char q;
} state;

typedef state* sND;

sND q0, q1, q2;
sND qA;
char str[10];

void createAFN() {
	q0 = (sND) malloc(sizeof(state));
	q1 = (sND) malloc(sizeof(state));
	q2 = (sND) malloc(sizeof(state));

	q0->q = 0;
	q0->a[0] = q0;
	q0->a[1] = q1;
	q0->b[0] = q0;
	q0->b[1] = NULL;

	q1->q = 1;
	q1->a[0] = q2;
	q1->a[1] = NULL;
	q1->b[0] = NULL;
	q1->b[1] = NULL;

	q2->q = 2;
	q1->a[0] = NULL;
	q1->a[1] = NULL;
	q1->b[0] = NULL;
	q1->b[1] = NULL;
}

void solve(char state, char deltaPot) {
	if(str[deltaPot]) {
		printf(" -%d-> q%d \n δ(q%d, %c)", deltaPot, state, state, str[deltaPot]);
		printf("%d", state);

		switch (str[deltaPot]) {
		case 'a':
			if(state == 0) {
				solve(0, deltaPot+1);
				printf("\n\tNuevo camino: ");
				solve(1, deltaPot+1);
			}
			else if (state == 1)
				solve(2, deltaPot+1);
		break;

		case 'b': 
			if(state == 0)
				solve(0, deltaPot+1);
		break;

		default:
			printf(" -> Ø\n----- No es valida -----\n");
		}
	}
	else if(state == 0 || state == 2)
		printf(" -%d-> q%d\n\nLa cadena es valida\n", deltaPot, state);
	else
		printf(" La cadena no es valida");
}

int main(int argc, char const *argv[]) {
	printf("Ingrese la cadena a verificar: ");
	fflush(stdin);
	scanf("%s", str);

	createAFN();
	qA = q0;

	solve(qA->q, 0);
	printf("\n");

	return 0;
}
