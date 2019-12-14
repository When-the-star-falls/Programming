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

#define MAX_SEARCHING_LEVELS 4
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
void decoronate(const ID id);
void kill(const num id);
void revive(num id);
void move(const ID id, const Pos aim);
void demove(const ID id, const Pos aim);
bool place(const Path path);
void deplace(Path path, bool flag);


void extendPath(const Pos pos);
void withdrawPath();
void updateLongest();
bool pathcmp(Path pathL, Path pathR);
int next(int pathID);


void addSon(Path* const road);
void addBrother(Path* const road);
void deleteBranch(Tree* const ptrTree, Tree* const ptrExcpt);
Tree* whichToInherit(const Command* order);
void inheritTree(Tree* const ptrTree);


void findEat(ID id);
void findMove(Side x);
void alphaBeta(Side x);
Score evaluate();


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

int order = 0;

Path allPath[ALL_PATH_NUM] = { { {0,0},0 } };
Tree root = { 0 };
Tree* maxMin = &root;
Tree* present = &root;

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
	printf(Format,##__VA_ARGS__);		\
	fflush(stdout);						\
}while(0)

#define safe(NUM) ((NUM) % ALL_PATH_NUM)

#ifdef TEST

#define assert(EXPR) do{							\
	if (!EXPR){										\
		print("Assert Error: " #EXPR "\n");			\
		exit(233);									\
	}												\
	else{											\
		print("DEBUG Assert: " #EXPR "\n");			\
	}												\
}while(0)

#define debug(isStart, isOver, Format, ...) do{		\
	if(isStart) printf("DEBUG ");					\
	printf(Format,##__VA_ARGS__);					\
	if(isOver) printf("\n");						\
	fflush(stdout);									\
}while(0)

void PRINTBOARD(const ID board[8][8]) {
	int i, j;
	char ch;
	printf("DEBUG ");
	printf("$$\\left[\\begin{array}{ccccccccc} { } ");
	for (i = 0; i < 8; i++) {
		printf("& {%d}", i);
	}
	printf(" \\\\");
	for (i = 0; i < 8; i++) {
		printf("{%d} ", i);
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
			printf("& {%c} ", ch);
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

inline ID board(const Pos pos) {
	return Board[pos.r][pos.c];
}

inline Pos alt(const Pos pos, const Dir dir) {
	Pos aim;
	aim.r = pos.r + dir.dr;
	aim.c = pos.c + dir.dc;
	return aim;
}

inline bool isBlocked(const Pos pos) {
	if (pos.r >= BR || pos.c >= BC) return true;	/* Boundary */
	if (pos.r < 0 || pos.c < 0)return true;			/* Boundary */
	if (board(pos) != 0)return true;				/* NotSpace */
	return false;
}

inline bool isRival(const Pos my, const Pos aim) {
	return board(aim) && allChess[board(my)].colour != allChess[board(aim)].colour;
}

inline bool canEat(const Pos pos, const num dir) {
	return !isBlocked(alt(pos, eatDir[dir])) && isRival(pos, alt(pos, moveDir[dir]));
}

inline bool canMove(const Pos pos, const num dir) {
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

inline void drop(const ID id, const Pos pos) {
	Board[pos.r][pos.c] = id;
	allChess[id].pos.r = pos.r;
	allChess[id].pos.c = pos.c;
}

void move(const ID id, const Pos aim) {
	drop(SPACE, allChess[id].pos);
	
	if ((allChess[id].pos.r + aim.r) % 2 == 0) {
		Pos mid;
		mid.r = (allChess[id].pos.r + aim.r) / 2;
		mid.c = (allChess[id].pos.c + aim.c) / 2;
		kill(board(mid));
	}

	drop(id, aim);
}

void demove(const ID id, const Pos aim) {
	drop(SPACE, allChess[id].pos);

	if ((allChess[id].pos.r + aim.r) % 2 == 0) {
		Pos mid;
		mid.r = (allChess[id].pos.r + aim.r) / 2;
		mid.c = (allChess[id].pos.c + aim.c) / 2;
		num i, temp = (1 - id / 13) * 12;
		for (i = 1; i <= PIECES; i++) {
			if (allChess[i + temp].pos.r == mid.r) {
				if (allChess[i + temp].pos.c == mid.c) {
					revive(i + temp);
					break;
				}
			}
		}
	}

	drop(id, aim);
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

void decoronate(const ID id) {
	allChess[id].king = false;
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

bool place(const Path path) {
	int i;
	for (i = 0; i < path.numStep - 1; i++) {
		move(board(path.pos[i]), path.pos[i + 1]);
	}
	bool flag = coronate(board(path.pos[path.numStep - 1]));
	return flag;
}

void deplace(Path path, bool flag) {
	int i;
	if (flag) {
		decoronate(board(path.pos[path.numStep - 1]));
	}
	for (i = path.numStep - 1; i > 0; i--) {
		demove(board(path.pos[i]), path.pos[i - 1]);
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
	current.pos[current.numStep] = pos;
	current.numStep++;
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
			return safe(id + i);
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
	maxMin = &root;
	maxMin->father = NULL;
	maxMin->level = MAX_SHIFTING_ROUNDS;
	maxMin->pathID = 0;
	maxMin->sonNum = 0;
	present = maxMin;
}

void addSon(Path* const road) {
	if (!present) return;
	if (!present->level) return;
	int temp = next(present->pathID);
	if (-1 == temp) return;

	present->son[present->sonNum] = NULL;
	while (!present->son[present->sonNum]) {
		present->son[present->sonNum] = (Tree*)calloc(1, sizeof(Tree));
	}

	present->son[present->sonNum]->father = present;
	present->son[present->sonNum]->level = present->level - 1;
	if (!road) {
		present->son[present->sonNum]->pathID = present->pathID;
	}
	else {
		present->son[present->sonNum]->pathID = temp;
		memcpy(allPath + temp, road, sizeof(Path));
	}
	present->sonNum++;
	present = present->son[present->sonNum - 1];
}

void addBrother(Path* const road) {
	if (!present) return;
	if (!present->father) return;

	present = present->father;
	addSon(NULL);
}

void deleteBranch(Tree* const ptrTree, Tree* const ptrExcpt) {
	num i;
	bool noExcpt = !ptrExcpt;
	if (!ptrTree) return;
	
	for (i = ptrTree->sonNum - 1; i >= 0; i--) {
		if (noExcpt) {
			deleteBranch(ptrTree->son[i], NULL);
		}
		else if (ptrExcpt == ptrTree->son[i]) {
			noExcpt = true;
			continue;
		}
		else {
			deleteBranch(ptrTree->son[i], ptrExcpt);
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
			deleteBranch(maxMin->son[i], NULL);
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
	if (ptrTree) {
		maxMin = ptrTree;
		updateLevel(MAX_SHIFTING_ROUNDS - maxMin->level, maxMin);
	}
}

#endif

void inherit(Path* road) {
	Tree* temp = whichToInherit(road);
	if (temp != maxMin) {
		inheritTree(temp);
	}
	else {
		maxMin->sonNum = 0;
	}

	present = maxMin;
	while (present->sonNum) {
		present = present->son[0];
	}
}


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
	bool flag = true;
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
			
			updateLongest();
		}
	}
}

void alphaBeta(Side x) {
	if (!present->level) {
		present->father->score = evaluate();
	}
	else {
		ID n;
		initPath();
		Score alpha = -10000, beta = 10000;
		for (n = head[x]; n != CNULL; n = allChess[n].next) {
			findEat(n);
		}
		if (!pathID) {
			findMove(x);
		}

		while (pathID > 0) {
			pathID--;
			addSon(longest + pathID);
			(longest + pathID)->numStep = 0;
			present = present->father;
		}
		int i = 0;
		while (i < present->sonNum) {
			present = present->son[i];
			bool flag = place(allPath[present->pathID]);
			alphaBeta(3 - x);
			deplace(allPath[present->pathID], flag);
			present = present->father;
			if (1 == present->level) return;
			
			present->score = present->son[0]->score;
			order = present->son[0]->pathID;

			if (x == SIDE) {
				int i;
				for (i = 1; i < present->sonNum; i++) {
					if (present->score < present->son[i]->score) {
						present->score = present->son[i]->score;
						order = present->son[i]->pathID;
					}
				}
			}
			else {
				int i;
				for (i = 0; i < present->sonNum; i++) {
					if (present->score > present->son[i]->score) {
						present->score = present->son[i]->score;
						order = present->son[i]->pathID;
					}
				}
			}
			i++;
		}
	}
}

Score evaluate() {
	Score score = 0;
	ID n;
	
	if (CNULL == head[SIDE]) {
		return -10000;
	}

	if (CNULL == head[3 - SIDE]) {
		return 10000;
	}
	
	for (n = head[SIDE]; n != CNULL; n = allChess[n].next) {
		score += 100 + 200 * allChess[n].king;
	}

	for (n = head[3 - SIDE]; n != CNULL; n = allChess[n].next) {
		score -= (100 + 200 * allChess[n].king);
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
	inherit(&command);
}

void Turn() {
	alphaBeta(SIDE);
	place(allPath[order]);
	print("PLACE");
	int i;
	print(" %hd", allPath[order].numStep);
	for (i = 0; i < allPath[order].numStep; i++) {
		print(" %hd,%hd", allPath[order].pos[i].r, allPath[order].pos[i].c);
	}
	print("\n");
	inherit(allPath + order);
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
			count++;
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
