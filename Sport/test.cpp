#include <stdio.h>
#include <stdlib.h>
#define OVERFLOW -2
#define SIZE_H 6 // 迷宫矩阵的行数
#define SIZE_L 5 // 迷宫矩阵的列数
int curstep = 1;
int map_flag[6][5] = {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}}; 
int maze[6][5] = {{0, 1, 0, 0, 0}, {1, 0, 1, 1, 0}, {1, 0, 1, 1, 0},
                  {1, 1, 0, 1, 1}, {0, 0, 1, 0, 1}, {1, 1, 1, 0, 0}};
//定义迷宫
typedef int Status;
typedef struct {
  int ord;
  // 路径中走过的序号
  int seat[2];
  // 坐标
  int di;
  // 从这一块走向下一块的方向
} SElemType;
#define STACK_INIT_SIZE 10 /* 存储空间初始分配量 */
#define STACKINCREMENT 2
/* 存储空间分配增量 */
typedef struct SqStack {
  SElemType *base; /* 在栈构造之前和销毁之后， base 的值为 NULL */
  SElemType *top;  /* 栈顶指针 */
  int stacksize;
  /* 当前已分配的存储空间，以元素为单位 */
} SqStack;
/* 顺序栈 */
SqStack S;
Status InitStack(SqStack &S) { /* 构造一个空栈 S */
  S.base = (SElemType *)malloc(STACK_INIT_SIZE * sizeof(SElemType));
  if (!S.base)
    exit(OVERFLOW); /* 存储分配失败 */
  S.top = S.base;
  S.stacksize = STACK_INIT_SIZE;
  return 1;
}
Status Pop(SqStack &S, SElemType &e) { /* 若栈不空，则删除 S 的栈顶元素，用 e
                                     返回其值，并返回 OK; 否则返回 ERROR */
  if (S.top == S.base)
    return 0;
  e = *--S.top;
  return 1;
}
Status Push(SqStack &S, SElemType e) {
  /* 插入元素 e 为新的栈顶元
  素 */
  if (S.top - S.base >= S.stacksize) /* 栈满，追加存储空间 */
  {
    S.base = (SElemType *)realloc(S.base, (S.stacksize + STACKINCREMENT) *
                                              sizeof(SElemType));
    if (!S.base)
      exit(OVERFLOW); /* 存储分配失败 */
    S.top = S.base + S.stacksize;
    S.stacksize += STACKINCREMENT;
  }
  *(S.top)++ = e;
  return 1;
}
Status StackEmpty(SqStack S) { /* 若栈 S 为空栈，则返回 TRUE ，否则返回 FALSE */
  if (S.top == S.base)
    return 1;
  else
    return 0;
}
void FootPrint(int *curpos) {
  map_flag[curpos[0]][curpos[1]] = 1;
  printf("经过(%d,%d),", curpos[0], curpos[1]);
}
void MarkPrint(int *seat) {
  if (map_flag[seat[0]][seat[1]] != 1) {
    map_flag[seat[0]][seat[1]] = -1;
    printf("到(%d,%d),此路不通，掉头!\n", seat[0], seat[1]);
  } else
    map_flag[seat[0]][seat[1]] = -1;
}
Status Pass(int x, int y) {
  if (maze[x][y] != 1 && map_flag[x][y] == 0)
    return 1;
  else
    return 0;
}
void NextPos(int curpos[2], int x, int y, int di) {
  int m = curpos[0];
  int n = curpos[1];
  if (di == 1) {
    curpos[0] = x;
    curpos[1] = y + 1;
  } else if (di == 2) {
    curpos[0] = x + 1;
    curpos[1] = y + 1;
  } else if (di == 3) {
    curpos[0] = x + 1;
    curpos[1] = y;
  } else if (di == 4) {
    curpos[0] = x + 1;
    curpos[1] = y - 1;
  } else if (di == 5) {
    curpos[0] = x;
    curpos[1] = y - 1;
  } else if (di == 6) {
    curpos[0] = x - 1;
    curpos[1] = y - 1;
  } else if (di == 7) {
    curpos[0] = x - 1;
    curpos[1] = y;
  } else if (di == 8) {
    curpos[0] = x - 1;
    curpos[1] = y + 1;
  }
  if (curpos[0] < 0 || curpos[0] > 5)
    curpos[0] = m;
  if (curpos[1] < 0 || curpos[1] > 4)
    curpos[1] = n;
}
Status Mazepath(int *start, int *end) {
  int curpos[2];
  SElemType e;
  InitStack(S);
  curpos[0] = start[0];
  curpos[1] = start[1];
  curstep = 1;
  do {
    if (Pass(curpos[0], curpos[1])) {
      FootPrint(curpos);
      e.ord = curstep;
      e.seat[0] = curpos[0];
      e.seat[1] = curpos[1];
      e.di = 1;
      Push(S, e);
      if ((curpos[0] == end[0]) && (curpos[1] == end[1]))
        return 1;
      NextPos(curpos, curpos[0], curpos[1], 1);
      curstep++;
    } else {
      if (!StackEmpty(S)) {
        Pop(S, e);
        while (e.di == 8 && !StackEmpty(S)) {
          MarkPrint(e.seat);
          Pop(S, e);
        }
      }
      if (e.di < 8) {
        e.di++;
        Push(S, e);
        NextPos(curpos, e.seat[0], e.seat[1], e.di);
      }
    }
  } while (!StackEmpty(S));
  return 0;
}
int main(void) {
  int i = 0, j = 0;
  int start[2] = {0, 0};
  int end[2] = {5, 4};
  if (Mazepath(start, end) != 1)
    printf("\n 迷宫无解");
  else {
    printf("抵达出口\n");
    printf("路径如图，“1”标识迷宫的解，“-1”标识试错的路径\n");
  }
  for (i = 0; i < SIZE_H; i++) {
    for (j = 0; j < SIZE_L; j++) {
      printf("%d ", map_flag[i][j]);
    }
    printf("\n");
  }
  return 0;
}