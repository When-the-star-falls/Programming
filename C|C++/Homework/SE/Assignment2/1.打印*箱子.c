#include <stdio.h>
#define scanf scanf_s//When using VS, this should be added

int main() {
	int a, b;
	int i,j;

	scanf("%d %d", &a, &b);
	for (i = 0; i < b; i++) {
		printf("*");
	}
	printf("\n");

	for (i = 0; i < a - 2; i++) {
		printf("*");
		for (j = 0; j < b - 2; j++) {
			printf(" ");
		}
		printf("*\n");
	}

	for (i = 0; i < b; i++) {
		printf("*");
	}
	printf("\n");

	return 0;
}
