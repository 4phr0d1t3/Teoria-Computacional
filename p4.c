#include <stdio.h>
#include <stdlib.h>

char str[10];
char size;

void longCad() {
	for(size = 0; str[size]; ++size);
}

void solve(char state, char deltaPot) {
	
	if (str[deltaPot]) {
		printf(" -%d-> q%d \n δ(q%d, %c)", deltaPot, state, state, str[deltaPot]);

		switch (state) {
			case 0:
				if (str[deltaPot] == 'b')
					solve(0, deltaPot+1); // δ(q0, b) -> q0
				else if(str[deltaPot] == 'a') {
					solve(0, deltaPot+1); // δ(q0, a) -> q0
					solve(1, deltaPot+1); // δ(q0, a) -> q1
				}
			break;

			case 1:
				if (str[deltaPot] == 'b')
					solve(2, deltaPot+1); // δ(q1, b) -> q2
			break;

			default:
				printf(" -%d-> Ø" , deltaPot+1); // -> Ø
			break;
		}
	}
}

int main(int argc, char const *argv[]) {
	printf("Ingrese la cadena a verificar: ");
	fflush(stdin);
	scanf("%s", str);

	longCad();

	printf("\n");
	solve(0, 0);
	printf("\n");

	return 0;
}
