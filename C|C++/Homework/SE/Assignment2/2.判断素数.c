#include <stdio.h>
#include <math.h>
#define scanf scanf_s//When using VS, this should be added

int main() {
	unsigned int n;
	unsigned int i;
	bool flag = false;

	scanf("%d", &n);

	if (n > 2 && n % 2 == 0) {
		flag = true;
	} else {
		for (i = 3; !flag && i <= (unsigned int)(sqrt(n)); i += 2) {
			if (n % i == 0) {
				flag = true;
			}
		}
	}

	if (flag) {
		printf("no");
	} else {
		printf("yes");
	}

	return 0;
}
