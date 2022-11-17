#include <stdio.h>
#include <stdlib.h>

char str[10], *path;
char size, num = 0;

void solve(char state, char deltaPot) {
	if(str[deltaPot]) {
		//printf("\tδ(q%d, %c) -%d-> ", state, str[deltaPot], deltaPot);
		if(state == 0)
			if(str[deltaPot] == 'b') {
				//printf("q0\n");
				path[deltaPot] = 0;
				solve(0, deltaPot+1); // δ(q0, b) -> q0
			}
			else if(str[deltaPot] == 'a') {
				//printf("q0\n");
				path[deltaPot] = 0;
				solve(0, deltaPot+1); // δ(q0, a) -> q0
				
				//printf(" Nuevo camino en: -%d-> q1\n", deltaPot);
				path[deltaPot] = 1;
				solve(1, deltaPot+1); // δ(q0, a) -> q1
			}
			else{}
				//printf("Ø\n       --- No valido ---\n"); // -> Ø
		else if(state == 1)
			if(str[deltaPot] == 'b') {
				//printf("q2\n");
				path[deltaPot] = 2;
				solve(2, deltaPot+1); // δ(q1, b) -> q2
			}
			else{}
				//printf("Ø\n       --- No valido ---\n"); // -> Ø
		else if(state == 2){}
			//printf("Ø\n       --- No valido ---\n"); // -> Ø
	}
	//else if(deltaPot != size)
	//	printf(" -%d-> q%d\n", deltaPot, state);
	//else if(state != 0 && state !=2)
	//	printf("       --- No valido ---\n");
	else if(deltaPot == size && (state == 0 || state == 2)){
		printf("Camino Valido:\t-> q0");
		for(char i = 0; i<size; ++i)
			printf(" -> q%d", path[i]);
		printf("\n");
		++num;
	}
}

int main(int argc, char const *argv[]) {
	printf("Ingrese la cadena a verificar: ");
	fflush(stdin);
	scanf("%s", str);

	for(size = 0; str[size]; ++size);
	path = (char *) malloc(size*(sizeof(char)));

	printf("\n");
	solve(0, 0);

    printf("Caminos validos = %d", num);
	return 0;
}
