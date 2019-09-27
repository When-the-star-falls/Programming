#include <stdio.h>
#define scanf scanf_s//When using VS, this should be added
#define N 100//The max length of the number that can be added

int main()
{
	int m, n;
	int i, j;
	int a[N + 1] = {}, b[N + 1] = {};
	int flag = 0;

	scanf("%d\n", &m);
	for (i = m; i > 0; i--) {
		a[N + 1 - i] = (int)getchar()-48;
	}
	
	scanf("%d\n", &n);
	for (j = n; j > 0; j--) {
		b[N + 1 - j] = (int)getchar()-48;
	}

	i = N;
	j = ((m > n) ? m : n) + 1;
	while (i >= N + 1 - j) {
		a[i] = a[i] + b[i] + flag;
		if (a[i] > 9) {
			a[i] = a[i] - 10;
			flag = 1;
		}
		else {
			flag = 0;
		}
		i--;
	}
	if (a[N + 1 - j] != 0) {
		printf("%d", a[N + 1 - j]);
	}
	for (i = N + 2 - j; i < N + 1; i++) {
		printf("%d", a[i]);
	}
}
