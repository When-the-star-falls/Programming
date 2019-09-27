#include <stdio.h>

//#define scanf scanf_s//When using VS, this should be added.

int main()
{
	int a, b, c;
	scanf("%d %d %d", &a, &b, &c);
	
	if (a + b > c && b + c > a && c + a > b) {
		printf("封印解除");
	} else {
		printf("解除失败");
	}

	return 0;
}
