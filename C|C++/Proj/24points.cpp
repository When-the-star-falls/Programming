#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "D:\学习\高级语言程序设计\VSCcodes\Lib\include\fraction.h"//In Github:
#include <iostream>//It's included in the fraction.h
using namespace std;

//#define scanf scanf_s//When using VS,this should be added.

//随机生成一张牌
int deal() {
	return (rand() % 13 + 1);
}

enum {
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE
};

union op {
	unsigned val;
	struct {
		unsigned op1 : 2;
		unsigned op2 : 2;
		unsigned op3 : 2;
	};
};

char OP[4] = { '+','-','*','/' };

int temp[24][4] = {};
bool vis[4] = {};
int num;

void DFS(int x = 0)
{
  //全排列输出
  
	if (x >= 4) {
		num++;
		int i = 0;
		while (vis[i] && i < 4) {
			temp[num][i] = temp[num - 1][i];
			i++;
		}
		return ;
	}
  
	for (int i = 0; i < 4; i++) {
		if (!vis[i]) {
			temp[num][x] = i; 
			vis[i] = true;
			DFS(x + 1);
			vis[i] = false;
		}
	}
	return ;
}

//计算
Fraction cal(Fraction a,Fraction b,int op)
{
	switch (op) {
	case PLUS:return a + b;
	case MINUS:return a - b;
	case MULTIPLY:return a * b;
	case DIVIDE:return a / b;
	default: return 0;
	}
	return 0;
}

//((a + b) + c) + d
int tree1(int a, int b, int c, int d, int op1, int op2, int op3)
{
	if (cal(cal(cal(a, b, op1), c, op2), d, op3) == 24) {
		printf("((%d %c %d) %c %d) %c %d = 24\n", a, OP[op1], b, OP[op2], c, OP[op3], d);
		return 1;
	}
	return 0;
}

//(a + (b + c)) + d
int tree2(int a, int b, int c, int d, int op1, int op2, int op3)
{
	if (cal(cal(a, cal(b, c, op1), op2), d, op3) == 24) {
		printf("(%d %c (%d %c %d)) %c %d = 24\n", a, OP[op2], b, OP[op1], c, OP[op3], d);
		return 1;
	}
	return 0;
}

//(a + b) + (c + d)
int tree3(int a, int b, int c, int d, int op1, int op2, int op3)
{
	if (cal(cal(a, b, op1), cal(c, d, op2), op3) == 24) {
		printf("(%d %c %d) %c (%d %c %d) = 24\n", a, OP[op1], b, OP[op3], c, OP[op2], d);
		return 1;
	}
	return 0;
}

//a + ((b + c) + d)
int tree4(int a, int b, int c, int d, int op1, int op2, int op3)
{
	if (cal(a, cal(cal(b, c, op1), d, op2), op3) == 24) {
		printf("%d %c ((%d %c %d) %c %d) = 24\n", a, OP[op3], b, OP[op1], c, OP[op2], d);
		return 1;
	}
	return 0;
}

//a + (b + (c + d))
int tree5(int a, int b, int c, int d, int op1, int op2, int op3)
{
	if (cal(a, cal(b, cal(c, d, op1), op2), op3) == 24) {
		printf("%d %c (%d %c (%d %c %d)) = 24\n", a, OP[op3], b, OP[op2], c, OP[op1], d);
		return 1;
	}
	return 0;
}

int main()
{
	srand(time(NULL));

	//发牌
	int i, j, m;
	int count = 0;
	unsigned int cards[4];//存放4张牌面
	printf("Welcome to the cards game 24 points!\n");
	printf("Enter 0 for playing, or you can enter 1 for solving:");
	scanf_s("%d", &m);
	if (m == 0) {
		for (i = 0; i < 4; i++) {
			cards[i] = deal();
			printf("%d ", cards[i]);
		}
		printf("\n按任意键获取答案：\n");
		system("pause");
	} else {
		printf("Please enter four numbers between 1 to 13, seperated by space:\n");
		scanf("%d %d %d %d", cards, cards + 1, cards + 2, cards + 3);
	}
	
	union op op;

	DFS();
  
	for (i = 0; i < 5; i++) {//5种树形
		for (op.val = 0; op.val < 64; ++op.val) {//64种符号全排列
			for (j = 0; j < 24; j++) {
				switch (i) {
				case 0:count += tree1(cards[temp[j][0]], cards[temp[j][1]], cards[temp[j][2]], cards[temp[j][3]], op.op1, op.op2, op.op3); break;
				case 1:count += tree2(cards[temp[j][0]], cards[temp[j][1]], cards[temp[j][2]], cards[temp[j][3]], op.op1, op.op2, op.op3); break;
				case 2:count += tree3(cards[temp[j][0]], cards[temp[j][1]], cards[temp[j][2]], cards[temp[j][3]], op.op1, op.op2, op.op3); break;
				case 3:count += tree4(cards[temp[j][0]], cards[temp[j][1]], cards[temp[j][2]], cards[temp[j][3]], op.op1, op.op2, op.op3); break;
				case 4:count += tree5(cards[temp[j][0]], cards[temp[j][1]], cards[temp[j][2]], cards[temp[j][3]], op.op1, op.op2, op.op3); break;
				}
			}
		}
	}
	
  if (count == 0) {
		printf("NO ANSWER!");
	}
  
  return 0;
}
