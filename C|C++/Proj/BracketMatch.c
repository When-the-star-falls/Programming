#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char stack[51];
unsigned int sp = 0;


bool match(char m, char n) {
	if ((m == '(' && n == ')') ||
		(m == '[' && n == ']') ||
		(m == '{' && n == '}') ||
		(m == '<' && n == '>') ||
		(m == '\'' && n == '\'') ||
		(m == '\"' && n == '\"'))
		return true;
	else return false;
}

bool ignore(char x) {
	//Check whether we should ignore this character
	switch (x) {
	case'(':
	case')':
	case'[':
	case']':
	case'{':
	case'}':
	case'<':
	case'>':
	case'\'':
	case'\"':
	case'\n':return false;
	default: return true;
	}
}

void push(char c) {
	++sp;
	stack[sp] = c;
}

void pop() {
	sp--;
}

char top() {
	return stack[sp];
}

#if 1
bool is_matched(const char* str)
{
	int i;

	sp = 0;
	push('\0');
	for (i = 0; str[i] != '\0'; i++) {
		if (match(top(), str[i])) {
			pop();
		}
		else {
			push(str[i]);
		}
	}

	return top() == '\0';
}

void check(const char* str, bool expected)
{
	const char* result[] = { "UNMATCH", "MATCHED" };
	bool actual = is_matched(str);
	if (actual == expected) {
		printf("[ OK ] %s %s\n", result[actual], str);
	}
	else {
		printf("[FAIL] %s %s\n", result[actual], str);
	}
}

int main()
{
	check("", true);
	check("(", false);
	check("()", true);
	check(")(", false);

	return 0;
}

#else

int main() {
	//Declaration
	char str[100];
	int  i, j, k;

	//Initialization
	for (i = 0; i < 51; i++) {
		str[i] = '\0';
		stack[i] = '\0';
	}
	for (i = 51; i < 100; i++) {
		str[i] = '\0';
	}

	//Input
	i = 0;
	printf("Please input,notice that after you pressed \'Enter\',you could not input any more.\n");
	while (i <= 99) {
		//scanf("%c",&str[i]);
		str[i] = getchar();
		if (ignore(str[i])) {
			;
		}
		else if (str[i] == '\n') {
			str[i] = '\0';
			break;
		}
		else {
			i++;
		}
	}
	k = i;
#if 0
	//Match
	stack[0] = '\0';
	for (i = 0, j = 1; i < k; i++) {
		if (match(stack[j - 1], str[i])) {
			j--;
			//stack[j] = '\0';
		}
		else {
			stack[j] = str[i];
			j++;
		}
	}

	//Output
	puts(str);
	if (stack[j - 1] == '\0')
		printf("Right");
	else
		printf("Wrong");
#else

	push('\0');
	for (i = 0;i < k;i++){
		if (match(top(),str[i])){
			pop();
		}
		else{
			push(str[i]);
		}
	}

	//Output
	puts(str);
	if (top() == '\0')
		printf("Right");
	else
		printf("Wrong");
#endif
	printf("\n");
	system("pause");
	return 0;
}
#endif
