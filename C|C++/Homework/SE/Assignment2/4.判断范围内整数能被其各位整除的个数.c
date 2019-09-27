#include <stdio.h>
#include <stdlib.h>
#define scanf scanf_s

int main()
{
	int l, r;
	int i, j, k;
	int temp;
	int str[6] = {};
	int flag = 1;
	int count = 0;

	scanf("%d %d", &l, &r);

	for (i = l; i <= r; i++) {
		temp = i;
		k = 0;

		while (temp != 0) {
			str[k] = temp - 10 * (int)(temp / 10);
			temp = (int)(temp / 10);
			k++;
		}

		for (j = 0; j < k; j++) {
			if (str[j] == 0) {
				flag = 0;
			}
			else if (i % str[j] != 0) {
				flag = 0;
			}
		}

		if (flag == 1) {
			count++;
		}
		else {
			flag = 1;
		}
	}

	printf("%d", count);

	return 0;
}
