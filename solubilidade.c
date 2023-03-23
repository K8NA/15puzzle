#include <stdio.h>

int inversoes(int matriz[4][4]) {
	int num_inversoes = 0;
	int array[16];
	int a=0;

	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++) {
			array[a] = matriz[i][j];
			a++;
		}

	for (int i=0; i<15; i++)
		for (int j=i+1; j<16; j++)
			if (array[j] < array[i] && array[j] != 0)
				num_inversoes++;

	return num_inversoes;
}

int paridade_zero(int matriz[4][4]) {
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			if (matriz[i][j] == 0)
				return (i+1)%2+1;
}

int paridade_matriz(int matriz[4][4]) {
	if (paridade_zero(matriz) == 0) {
		if (inversoes(matriz)%2 == 0)
			return 0;    // paridade par e #inversoes par
		else return 1; // ao contrario
	}	else {
		if (inversoes(matriz)%2 == 0)
			return 1;
		else return 0;
	}
}

int resoluvel(int inicial[4][4], int final [4][4]) {
	return paridade_matriz(inicial) == paridade_matriz(final);
}


int main() {
	int inicial[4][4] = {{1, 2, 3, 4}, {5, 6, 8, 12}, {13, 9, 0, 7}, {14, 11, 10, 15}};
	int inicial2[4][4] = {{1, 2, 3, 4}, {13, 6, 8, 12}, {5, 9, 0, 7}, {14, 11, 10, 15}};
	int final[4][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 0}};

	if (resoluvel(inicial, final))
		printf("True\n");
	else printf("False\n");

	return 0;
}
