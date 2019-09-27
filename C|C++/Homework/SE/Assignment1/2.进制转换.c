//The base that can be transferred to is among 2 to 20
#include <stdio.h>
#define M 10 //The base of input number

//#define scanf scanf_s//When using VS, this should be added

int result[200] = {};

int pow(int a, int b)
{
	if (b == 0) {
		return 1;
	} else {
		return a * pow(a, b - 1);
	}
}

void output(int a, int base)
{
	int i = 0;
	
	while (a >= base) {
		result[i] = a % base;
		a = (int)(a / base);
		i++;
	}
	result[i] = a;
	
	for (; i >= 0; i--) {
		if (result[i] < 10) {
			printf("%d", result[i]);
		} else {
			switch (result[i]) {
				case 10:printf("A"); break;
				case 11:printf("B"); break;
				case 12:printf("C"); break;
				case 13:printf("D"); break;
				case 14:printf("E"); break;
				case 15:printf("F"); break;
				case 16:printf("G"); break;
				case 17:printf("H"); break;
				case 18:printf("I"); break;
				case 19:printf("J"); break;
			}
		}
	}
}

int main()
{
	int m = 0;
	int k[200] = {};
	int q = 0;

	printf("Please input the base and the codes:\n");
	scanf("%d ", &m);
	if (m > 20 || m < 2) {
		printf("We are not request to take these situations into consideration!");
		return 0;
	}

	int i = 0;
	int j = 0;
	while (j != '\n' - '0') {
		j = (int)getchar() - '0';
		k[i] = j;
		i++;
	}
	i -= 1;
	
	for (j = 0; j < i; j++) {
		q += k[j] * pow(M, (i - j - 1));
	}
	
	output(q, m);
	return 0;
}
