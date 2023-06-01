#include <stdio.h>
#include <stdlib.h>
#define NUM 20
typedef struct {
  int hour;
  int min;
} time;
typedef struct {
  int num;
  int position;
  time t;
  float money;
} Car;
typedef struct Node {
  Car data;
  struct Node *next;
} CQueueNode;
typedef struct {
  Car elem[NUM + 1];
  int top;
} Stack;
typedef struct {
  CQueueNode *front;
  CQueueNode *rear;
} LinkQueue;
void InitQueue(LinkQueue *Q);         // 初始化队列
int EnterQueue(LinkQueue *Q, Car *t); // 进队
void InitStack(Stack *S);             // 初始化栈
void Push(Stack *S, Car *r);          // 压栈
int IsEmpty(Stack *S);                // 判断栈空
int IsFull(Stack *S);                 // 判断栈满
int GetTop(Stack *S, Car *n);
int DeleteQueue(LinkQueue *Q, Car *x);
void CarIn(Stack *S, LinkQueue *Q, Car *r);
void CostCalculation(Car *r, int h, int m);
void CarOut(Stack *S, Stack *S0, Car *r, LinkQueue *Q);
int main(void) {
  int n, m, i = 1, j, flag = 0;
  Car c[10];
  Stack S, S0;
  LinkQueue Q;    // 便道
  InitStack(&S);  // 堆栈 S
  InitStack(&S0); // 临时堆栈 S0
  InitQueue(&Q);
  while (1) {
    printf("\t\t\t\t 欢迎停车");
    printf("\n\t\t 请选择:\n");
    printf("\n\t\t 1 :进入停车场");
    printf("\n\t\t 2 :离开停车场");
    printf("\n\t\t 3 :退出系统\n");
    printf("\n");
    scanf("%d", &m);
    switch (m) {
    case 1:
      printf("\n\t\t 请输入车牌号:");
      scanf("%d", &c[i].num);
      printf("\n\t\t 请输入到达/离开时间(形如2:00):");
      scanf("%d:%d", &c[i].t.hour, &c[i].t.min);
      CarIn(&S, &Q, &c[i]);
      i++; // 车辆的情况
      break;
    case 2:
      printf("\n\t\t 请输入车牌号:");
      scanf("%d", &n);
      for (j = 0; j < 10; j++)
        if (n == c[j].num)
          break;
      printf("\n\t\t 请输入到达/离开时间(形如2:00):");
      scanf("%d:%d", &c[j].t.hour, &c[j].t.min);
      CarOut(&S, &S0, &c[j], &Q); // 车辆的情况break;
    case 3:
      flag = 1;
      break;
    default:
      printf("\n\t\t 请输入 1 或 2 或 3\n");
    }
    if (flag)
      break; // 结束程序
  }
  return 0;
}
void InitQueue(LinkQueue *Q) {
  Q->front = (CQueueNode *)malloc(sizeof(CQueueNode));
  if (Q->front != NULL) {
    Q->rear = Q->front;
    Q->front->next = NULL;
  }
}
int EnterQueue(LinkQueue *Q, Car *t) {
  CQueueNode *NewNode;
  NewNode = (CQueueNode *)malloc(sizeof(CQueueNode));
  if (NewNode != NULL) {
    NewNode->data.num = t->num;
    NewNode->data.t.hour = t->t.hour;
    NewNode->data.t.min = t->t.min;
    NewNode->next = NULL;
    Q->rear->next = NewNode;
    Q->rear = NewNode;
    return 1;
  } else
    return 0;
}
void InitStack(Stack *S) { S->top = 0; }

void Push(Stack *S, Car *r) {
  S->top++;
  S->elem[S->top].num = r->num;
  r->position = S->elem[S->top].position = S->top;
  S->elem[S->top].t.hour = r->t.hour;
  S->elem[S->top].t.min = r->t.min;
}
int IsEmpty(Stack *S) // 判断车库是否为空
{
  return (S->top == 0 ? 1 : 0);
}
int IsFull(Stack *S) // 判断车库是否为满
{
  return (S->top == NUM ? 1 : 0);
}
int GetTop(Stack *S, Car *n) // 车离开车库
{
  n->num = S->elem[S->top].num;
  n->position = S->elem[S->top].position;
  n->t.hour = S->elem[S->top].t.hour;
  n->t.min = S->elem[S->top].t.min;
  return 1;
}
int DeleteQueue(LinkQueue *Q, Car *x) {
  CQueueNode *p;
  if (Q->front == Q->rear)
    return 0;         // 判断便道为空
  p = Q->front->next; // 将便道中的车放入车库Q->front->next = p->next;
  if (Q->rear == p)
    Q->rear = Q->front;
  x->num = p->data.num;
  x->t.hour = p->data.t.hour;
  x->t.min = p->data.t.min;
  free(p); // 释放临时指针
  return 1;
}
void CarIn(Stack *S, LinkQueue *Q, Car *r) {
  if (IsFull(S)) {
    printf("停车场已满，请在便道中等待");
    EnterQueue(Q, r); // 车进入便道
  } else {
    Push(S, r);
    printf("\n\t\t 所在位置 %d", r->position); // 打印车的位置}
  }
}
void CostCalculation(Car *r, int h, int m) {
  if (m > r->t.min) {
    r->t.min += 60;
    r->t.hour -= 1;
  }
  h = r->t.hour - h;
  m = r->t.min - m;
  printf("\n\t\t 停车 %d 小时 %d 分钟\n", h, m);
  printf("每小时收费 10 元\n");
  h = h * 20;
  m = h + m;
  r->money = 0.5 * m;
  printf("请支付金额%.2f 元\n", r->money); // 输出车主应付金额
}
void CarOut(Stack *S, Stack *S0, Car *r, LinkQueue *Q) {
  int tag = S->top;
  Car x;
  if (IsEmpty(S))
    printf("不存在该车辆");
  else {
    for (; r->num != S->elem[tag].num && tag > 0; tag--) {
      Push(S0, &S->elem[tag]);
      S->top--;
    }
    if (r->num == S->elem[tag].num) {
      CostCalculation(r, S->elem[tag].t.hour, S->elem[tag].t.min);
      S->top--;
      for (; S0->top > 0; S0->top--)
        Push(S, &S0->elem[S0->top]);
      if (S->top < NUM &&
          Q->front != Q->rear) // 判断车库是否有此车，有就找到此车，然后退出
      {
        DeleteQueue(Q, &x);
        Push(S, &x);
      }
    } else if (tag == 0) // 过道中的车无需收车费
    {
      printf("没有进入停车场支付金额 0 元");
      for (; S0->top > 0; S0->top--)
        Push(S, &S0->elem[S0->top]);
    }
  }
}