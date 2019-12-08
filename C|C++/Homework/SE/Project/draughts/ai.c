#define TEST
#define VS

#include <stdio.h>
#include <stdbool.h>//bool
#include <string.h> //memcpy(),memset()
#include <stdlib.h> //
#include <time.h>	//


#ifdef VS 
#include "D:/Study/C Programming/VSCcodes/Lib/include/ShutupVS.h"
#endif


/****************************************
*****************************************
*										*
*				 宏常量					*
*										*
*****************************************
*****************************************/

#if 1

#define CNULL 0
#define SPACE 0
#define BLACK 1
#define WHITE 2

#define PIECES 12
#define BR		8
#define BC		8

#define MIN_STEP 2
#define MAX_STEP PIECES + 1

#define MAX_SEARCHING_LEVELS 2
#define MAX_SHIFTING_ROUNDS  2 * MAX_SEARCHING_LEVELS

#define MAX_PATH_NUM 10
#define ALL_PATH_NUM 1000

#endif


/****************************************
*****************************************
*										*
*				类型声明					*
*										*
*****************************************
*****************************************/

#if 1

typedef short ID;
typedef short Side;
typedef short XY;
typedef short num;
typedef int Score;
typedef struct Coordinate {
	XY r;
	XY c;
}Pos;
typedef struct Direction {
	XY dr;
	XY dc;
}Dir;
typedef struct Path {
	Pos  pos[MAX_STEP];
	num numStep;
} Path;
typedef Path Command;
typedef struct Chess {
	ID prev;
	ID next;
	Pos  pos;
	Side colour;
	bool king;
} Chess;
typedef struct Tree {
	num level;
	Score score;
	num sonNum;
	int pathID;
	struct Tree* father;
	struct Tree* son[MAX_PATH_NUM];
} Tree;

#endif

/****************************************
*****************************************
*										*
*				函数声明					*
*										*
*****************************************
*****************************************/

#if 1

void initBoard();
void initChess();
void initPath();
void initTree();


ID board(const Pos pos);
Pos alt(const Pos pos, const Dir dir);
bool isBlocked(const Pos pos);
bool isRival(const Pos my, const Pos aim);
bool canEat(const Pos pos, const num dir);
bool canMove(const Pos pos, const num dir);


void drop(const ID id, const Pos pos);
bool coronate(const ID id);
void kill(const num id);
void revive(num id);
void move(const ID id, const Pos aim);
void place(Path path);


void extendPath(const Pos pos);
void withdrawPath();
void updateLongest();


void addSon(Tree* const ptrTree, Path* road);
void addBrother(Tree* const ptrTree);
void deleteBrunch(Tree* const ptrTree, Tree* const ptrExcpt);
Tree* whichToInherit(const Command* order);
void inheritTree(Tree* const ptrTree);


void findEat(ID id);
void findMove(Side x);
void alphaBeta(Side x);
Score evaluate(Side x);


void io();

void Turn();
void End(num status);

#endif


/****************************************
*****************************************
*										*
*				全局常量					*
*										*
*****************************************
*****************************************/

#if 1

const Dir moveDir[4] = {
	{-1,-1}	,	{-1,+1}
			,
	{+1,-1}	,	{+1,+1}
};
const Dir eatDir[4] = {
	{-2,-2}	,	{-2,+2}
			,
	{+2,-2}	,	{+2,+2}
};
/* 2: White, 1: Black */
Side SIDE = 0;

#endif


/****************************************
*****************************************
*										*
*				全局变量					*
*										*
*****************************************
*****************************************/

#if 1

/* head[SIDE]即为我方棋子的开头 */
Chess allChess[1 + PIECES * 2];
ID head[3] = { 0, 1, PIECES + 1 };

Path current = { {0,0},0 };
Path longest[MAX_PATH_NUM] = { {{0,0},0} };
int	pathID = 0;

Score alpha = -10000, beta = 10000;
int order = 0;

Path allPath[ALL_PATH_NUM] = { { {0,0},0 } };
Tree* maxMin;
Tree* present;

Command command = { {0,0},0 };

/* Original State */
ID Board[8][8] = { 0 };
/*
	{0,2,0,2,0,2,0,2},
	{2,0,2,0,2,0,2,0},
	{0,2,0,2,0,2,0,2},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{1,0,1,0,1,0,1,0},
	{0,1,0,1,0,1,0,1},
	{1,0,1,0,1,0,1,0}
*/

#endif


/****************************************
*****************************************
*										*
*				 宏函数					*
*										*
*****************************************
*****************************************/

#if 1

#define print(Format,...) do{			\
	printf(Format,__VA_ARGS__);			\
	fflush(stdout);						\
}while(0)

#define safe(NUM) ((NUM) % ALL_PATH_NUM)

#ifdef TEST

#define assert(EXPR) do{							\
	if (!EXPR){										\
		printf("Assert Error: " #EXPR "\n");		\
		exit(233);									\
	}												\
}while(0)

#define debug(isStart, isOver, Format, ...) do{		\
	if(isStart) printf("DEBUG ");					\
	printf(Format,__VA_ARGS__);						\
	if(isOver) printf("\n");						\
	fflush(stdout);									\
}while(0)

void PRINTBOARD(const ID board[8][8]) {
	int i, j;
	char ch;
	printf("DEBUG ");
	printf("$$\\left[\\begin{array}{cccccccc} ");
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (board[i][j] == 0) {
				ch = ' ';
			}
			else if (board[i][j] > 12) {
				ch = 'O';
			}
			else {
				ch = '@';
			}
			printf("{%c} ", ch);
			if (j != 7) {
				printf("& ");
			}
		}
		if (i != 7) {
			printf("\\\\ ");
		}
		else {
			printf("\\end{array}\\right]$$\n");
		}
	}
	fflush(stdout);
}
#else
#define assert(EXPR)
#define debug(isStart, isOver, Format, ...) {}
void PRINTBOARD(ID board[8][8]) {}
#endif

#endif


/****************************************
*****************************************
*										*
*				棋盘函数					*
*										*
*****************************************
*****************************************/

#if 1

void initBoard() {
	Pos pos;
	Pos _pos;
	int id = 1;

	/* Black: ID > 13, White: ID < 13, 关于棋盘中心对称 */
	for (pos.r = BR / 2 + 1; pos.r < BR; pos.r++) {
		for (pos.c = (pos.r & 1) ^ 1; pos.c < BC; pos.c += 2) {
			_pos.r = 7 - pos.r; _pos.c = 7 - pos.c;
			drop(id, pos);
			drop(id + PIECES, _pos);
			id++;
		}
	}
}

ID board(const Pos pos) {
	return Board[pos.r][pos.c];
}

Pos alt(const Pos pos, const Dir dir) {
	Pos aim;
	aim.r = pos.r + dir.dr;
	aim.c = pos.c + dir.dc;
	return aim;
}

bool isBlocked(const Pos pos) {
	if (pos.r >= BR || pos.c >= BC) return true;	/* Boundary */
	if (pos.r < 0 || pos.c < 0)return true;			/* Boundary */
	if (board(pos) != 0)return true;		/* NotSpace */
	return false;
}

bool isRival(const Pos my, const Pos aim) {
	return allChess[board(my)].colour != allChess[board(aim)].colour;
}

bool canEat(const Pos pos, const num dir) {
	Pos aim = alt(pos, eatDir[dir]);
	return isRival(pos, aim) && !isBlocked(aim);
}

bool canMove(const Pos pos, const num dir) {
	return !isBlocked(alt(pos, moveDir[dir]));
}

#endif

/****************************************
*****************************************
*										*
*				棋子函数					*
*										*
*****************************************
*****************************************/

#if 1

void initChess() {
	num i;
	for (i = 1; i < 1 + PIECES * 2; i++) {
		allChess[i].prev = i - 1;
		allChess[i].next = i + 1;
		allChess[i].king = false;
		allChess[i].colour = (i <= PIECES) + 1;
	}
	allChess[PIECES].next = CNULL;
	allChess[PIECES + 1].prev = CNULL;
	allChess[PIECES + PIECES].next = CNULL;
}

void drop(const ID id, const Pos pos) {
	Board[pos.r][pos.c] = id;
	allChess[id].pos.r = pos.r;
	allChess[id].pos.c = pos.c;
}

void move(const ID id, const Pos aim) {
	drop(SPACE, allChess[id].pos);
	drop(id, aim);

	if ((allChess[id].pos.r + aim.r) & 1) {
		Pos mid;
		mid.r = (allChess[id].pos.r + aim.r) / 2;
		mid.c = (allChess[id].pos.r + aim.c) / 2;
		kill(board(mid));
	}
}

bool coronate(const ID id) {
	bool temp = allChess[id].king || ((id / 13 && allChess[id].pos.r) ? true : false);
	if (!allChess[id].king && (id / 13 && allChess[id].pos.r)) {
		allChess[id].king = temp;
		return true;
	}
	else {
		allChess[id].king = temp;
		return false;
	}
}

void kill(const num id) {
	if (CNULL == allChess[id].prev) {
		head[id / 13 + 1] = allChess[id].next;
	}
	allChess[allChess[id].prev].next = allChess[id].next;
	allChess[allChess[id].next].prev = allChess[id].prev;
	drop(SPACE, allChess[id].pos);
}

void revive(num id) {
	allChess[allChess[id].prev].next = id;
	allChess[allChess[id].next].prev = id;
	drop(id, allChess[id].pos);
}

void place(Path path) {
	int i;
	for (i = 0; i < path.numStep - 1; i++) {
		move(board(path.pos[i]), path.pos[i + 1]);
	}
}

#endif


/****************************************
*****************************************
*										*
*				路径函数					*
*										*
*****************************************
*****************************************/

#if 1

void initPath() {
	pathID = 0;
	current.numStep = 0;
	longest[pathID].numStep = MIN_STEP;
}

void extendPath(const Pos pos) {
	current.numStep++;
	current.pos[current.numStep] = pos;
}

void withdrawPath() {
	current.numStep--;
}

void updateLongest() {
	if (current.numStep < longest[0].numStep) {
		return;
	}

	if (current.numStep > longest[0].numStep) {
		pathID = 0;
	}
	else if (pathID >= MAX_PATH_NUM) {
		return;
	}

	memcpy(&longest[pathID], &current, sizeof(current));
	pathID++;
}

bool pathcmp(Path pathL, Path pathR) {
	if (pathL.numStep != pathR.numStep) {
		return false;
	}
	else {
		num i;
		for (i = 0; i < pathL.numStep; i++) {
			if (pathL.pos[i].r != pathR.pos[i].r) return false;
			if (pathL.pos[i].c != pathR.pos[i].c) return false;
		}
	}
	return true;
}

int next(int id) {
	int i;
	for (i = 1; i < ALL_PATH_NUM; i++) {
		if (!allPath[safe(id + i)].numStep) {
			return safe(id + 1);
		}
	}
	return - 1;
}

#endif


/****************************************
*****************************************
*										*
*				 树函数					*
*										*
*****************************************
*****************************************/

#if 1

void initTree() {
	maxMin = NULL;
	while (!maxMin) {
		maxMin = (Tree*)calloc(1, sizeof(Tree));
	}
	maxMin->father = NULL;
	maxMin->level = MAX_SHIFTING_ROUNDS;
	maxMin->pathID = 0;
	maxMin->sonNum = 0;
	present = maxMin;
}

void addSon(Tree* const ptrTree, Path* road) {
	if (!ptrTree) return;
	if (!ptrTree->level) return;
	int temp = next(ptrTree->pathID);
	if (-1 == temp) return;

	ptrTree->son[ptrTree->sonNum] = NULL;
	while (!ptrTree->son[ptrTree->sonNum]) {
		ptrTree->son[ptrTree->sonNum] = (Tree*)calloc(1, sizeof(Tree));
	}
	
	present = ptrTree->son[ptrTree->sonNum];
	ptrTree->sonNum++;
	
	present->level = ptrTree->level - 1;
	present->father = ptrTree;
	if (!road) {
		present->pathID = ptrTree->pathID;
	}
	else {
		present->pathID = temp;
		memcpy(allPath + temp, road, sizeof(Path));
		road->numStep = 0;
	}
}

void addBrother(Tree* const ptrTree) {
	if (!ptrTree) return;
	if (!ptrTree->father) return;

	present = ptrTree->father;
	addSon(present, NULL);
}

void deleteBrunch(Tree* const ptrTree, Tree* const ptrExcpt) {
	num i;
	bool noExcpt = !ptrExcpt;
	if (!ptrTree) return;
	
	for (i = ptrTree->sonNum - 1; i >= 0; i--) {
		if (noExcpt) {
			deleteBrunch(ptrTree->son[i], NULL);
		}
		else if (ptrExcpt == ptrTree->son[i]) {
			noExcpt = true;
			continue;
		}
		else {
			deleteBrunch(ptrTree->son[i], ptrExcpt);
		}
	}


	if (ptrTree->father) {
		present = ptrTree->father;
		allPath[ptrTree->pathID].numStep = 0;
		free(ptrTree);
	}
}

Tree* whichToInherit(const Command* order){
	num i;
	Tree* ptrTemp = maxMin;
	if (!order->numStep) return maxMin;
	for (i = 0; i < maxMin->sonNum; i++) {
		if (pathcmp(allPath[maxMin->son[i]->pathID], *order)) {
			ptrTemp = maxMin->son[i];
		}
		else {
			deleteBrunch(maxMin->son[i], NULL);
			if(!ptrTemp) ptrTemp = maxMin;
		}
	}
	return ptrTemp;
}

void updateLevel(const num delta, Tree * const ptrTree) {
	if (!delta) return;
	if (!ptrTree->son[0]) {
		ptrTree->level += delta;
		present = ptrTree->father->son[0];
		return;
	}
	
	num i;
	for (i = ptrTree->sonNum - 1; i >= 0; i--) {
		updateLevel(delta, ptrTree->son[i]);
	}
}

void inheritTree(Tree* const ptrTree) {
	deleteBrunch(maxMin, ptrTree);
	if(ptrTree) maxMin = ptrTree;
	updateLevel(maxMin->level - MAX_SHIFTING_ROUNDS, maxMin);
}

#endif


/****************************************
*****************************************
*										*
*				核心函数					*
*										*
*****************************************
*****************************************/

#if 1

void findEat(ID id) {
	num dir;
	bool end = true;
	Pos pos, posMid, posAim;
	ID midID;

	pos = allChess[id].pos;
	extendPath(pos);

	for (dir = 0; dir < 4; dir++) {
		if (!canEat(allChess[id].pos, dir)) {
			continue;
		}

		end = false;

		posMid = alt(pos, moveDir[dir]);
		posAim = alt(pos, eatDir[dir]);
		midID = board(posMid);

		drop(SPACE, pos);
		kill(midID);
		drop(id, posAim);

		findEat(id);

		drop(id, pos);
		revive(midID);
		drop(SPACE, posAim);
	}

	if (end) {
		updateLongest();
	}

	withdrawPath();
}

void findMove(Side x) {
	ID	n;
	num	dir, dirmax;
	Pos	pos0, pos1;

	pathID = 0;
	for (n = head[x]; n != CNULL; n = allChess[n].next) {

		/* 确定运动方向 */
		if (allChess[n].king) {
			dir = 0;
			dirmax = 4;
		}
		else if (BLACK == x) {
			dir = 0;
			dirmax = 2;
		}
		else {
			dir = 2;
			dirmax = 4;
		}

		pos0 = allChess[n].pos;

		for (dir ; dir < dirmax; dir++) {
			pos1 = alt(pos0, moveDir[dir]);

			if (isBlocked(pos1)) continue;

			current.numStep = 2;
			current.pos[0] = pos0;
			current.pos[1] = pos1;

			if (++pathID >= MAX_PATH_NUM)
				return;
		}
	}
}

void alphaBeta(Side x) {
	if (!present->level) {
		present->score = evaluate(x);
	}
	else {
		ID n;
		initPath();

		for (n = head[x]; n != CNULL; n = allChess[n].next) {
			findEat(n);
		}
		if (!pathID) {
			findMove(x);
		}

		while (pathID > 0) {
			addSon(present, longest + pathID);
			alphaBeta(3 - x);
			present = present->father;
			if (x == SIDE) {
				int i;
				for (i = 0; i < present->sonNum; i++) {
					if (present->score < present->son[i]->score) {
						present->score = present->son[i]->score;
						order = present->pathID;
					}
				}
			}
			else {
				int i;
				for (i = 0; i < present->sonNum; i++) {
					if (present->score > present->son[i]->score) {
						present->score = present->son[i]->score;
						order = present->pathID;
					}
				}
			}
		}
	}
}

Score evaluate(Side x) {
	Score score = 0;
	ID n;
	
	if (CNULL == head[SIDE]) {
		return -10000;
	}

	if (x == SIDE) {
		return -10000;
	}

	if (CNULL == head[3 - SIDE]) {
		return 10000;
	}
	
	if (x == 3 - SIDE) {
		return 10000;
	}

	for (n = head[SIDE]; n != CNULL; n = allChess[n].next) {
		score += 1 + 2 * allChess[n].king;
	}

	for (n = head[3 - SIDE]; n != CNULL; n = allChess[n].next) {
		score -= (1 + 2 * allChess[n].king);
	}

	return score;
}

#endif


/****************************************
*****************************************
*										*
*				IO函数					*
*										*
*****************************************
*****************************************/

void Start() {
	srand((unsigned int)(time(NULL)));
	int i = rand() % 3;
	switch (i) {
	case 0:
		allPath[0].numStep = 2;
		allPath[0].pos[0].r = 5;
		allPath[0].pos[0].c = 6;
		allPath[0].pos[1].r = 4;
		allPath[0].pos[1].c = 5;
		break;
	case 1:
		allPath[0].numStep = 2;
		allPath[0].pos[0].r = 5;
		allPath[0].pos[0].c = 0;
		allPath[0].pos[1].r = 4;
		allPath[0].pos[1].c = 1;
		break;
	case 2:
		allPath[0].numStep = 2;
		allPath[0].pos[0].r = 5;
		allPath[0].pos[0].c = 2;
		allPath[0].pos[1].r = 4;
		allPath[0].pos[1].c = 3;
		break;
	}
}

void Place() {
	place(command);
	if (whichToInherit(&command) == maxMin) {
		deleteBrunch(maxMin, NULL);
	}
	else {
		inheritTree(whichToInherit(&command));
	}

	present = maxMin;
	while (present->sonNum) {
		present = present->son[0];
	}
}

void Turn() {
	Side x;
	x = allChess[board(allPath[present->pathID].pos[allPath[present->pathID].numStep - 1])].colour;
	alphaBeta(3 - x);
	place(allPath[order]);
	print("PLACE");
	int i;
	for (i = 0; i < allPath[order].numStep; i++) {
		print(" %hd,%hd", allPath[order].pos[i].r, allPath[order].pos[i].c);
	}
	print("\n");
}

void End(num status) {
	if (SIDE == status) {
		debug(1, 1, "WIN\n");
	}
	else {
		debug(1, 1, "LOSE\n");
	}
	exit(0);
}

void io() {
	char tag[6] = { 0 };
	num status = -1;
	num count = 1;

	while (true)
	{
		memset(tag, 0, sizeof(tag));
		scanf("%s", tag);

		if (strcmp(tag, "START") == 0) {
			scanf(" %hd", &SIDE);
			print("OK\n");
			Start();
			debug(1, 1, "My : %hd", SIDE);
			PRINTBOARD(Board);
		}
		else if (strcmp(tag, "PLACE") == 0) {
			scanf(" %hd", &command.numStep);
			debug(1, 0, "RIVAL PLACE: %hd", command.numStep);
			for (int i = 0; i < command.numStep; i++) {
				scanf(" %hd,%hd", &command.pos[i].r, &command.pos[i].c);
				debug(0, i == command.numStep - 1, " %hd,%hd", command.pos[i].r, command.pos[i].c);
			}
			Place();
			PRINTBOARD(Board);
		}
		else if (strcmp(tag, "TURN") == 0) {
			if (1 == count) {
				place(allPath[order]);
				print("PLACE %hd", allPath[order].numStep);
				int i;
				for (i = 0; i < allPath[order].numStep; i++) {
					print(" %hd,%hd", allPath[order].pos[i].r, allPath[order].pos[i].c);
				}
				print("\n");
			}
			else {
				Turn();
			}
			PRINTBOARD(Board);
			count++;
		}
		else if (strcmp(tag, "END") == 0) {
			scanf(" %hd", &status);
			PRINTBOARD(Board);
			End(status);
		}
	}
}


/****************************************
*****************************************
*										*
*				 主函数					*
*										*
*****************************************
*****************************************/

int main() {

	initChess();
	initBoard();
	initTree();
	io();
	assert(1 != 1);

	return 0;
}
