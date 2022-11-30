#include <stdio.h>
#include <stdlib.h>

//0,0 출발 3,3 도착
int map[16]={0,1,0,0,
            0,-1,1,0,
            0,0,0,-1,
            0,1,0,0};
int dp[16]={0,};
int dq[5]={0,};
 
typedef struct Node 
{
    int data;
    struct Node *next;
}Node;
 
 
typedef struct Queue 
{
    Node *front;
    Node *rear; 
    int count; // 큐 안의 노드 개수  
}Queue;
 
void initQueue(Queue *queue)
{
    queue->front = queue->rear = NULL; 
    queue->count = 0;    // 큐 안의 노드 개수를 0으로 설정
}
 
int isEmpty(Queue *queue)
{
    return queue->count == 0;    // 큐안의 노드 개수가 0이면 빈 상태
}
 
void enqueue(Queue *queue, int data)
{
    Node *newNode = (Node *)malloc(sizeof(Node)); // newNode 생성
    newNode->data = data;
    newNode->next = NULL;
 
    if (isEmpty(queue))    // 큐가 비어있을 때
    {
        queue->front = newNode;       
    }
    else    // 비어있지 않을 때
    {
        queue->rear->next = newNode;    //맨 뒤의 다음을 newNode로 설정
    }
    queue->rear = newNode;    //맨 뒤를 newNode로 설정   
    queue->count++;    //큐안의 노드 개수를 1 증가
}
 
int dequeue(Queue *queue)
{
    int data;
    Node *ptr;
    if (isEmpty(queue))    //큐가 비었을 때
    {
        printf("Error : Queue is empty!\n");
        return 0;
    }
    ptr = queue->front;    //맨 앞의 노드 ptr 설정 
    data = ptr->data;    // return 할 데이터  
    queue->front = ptr->next;    //맨 앞은 ptr의 다음 노드로 설정
    free(ptr);    // ptr 해제 
    queue->count--;    //큐의 노드 개수를 1 감소
    
    return data;
}

void BFS() {
    int count=0;
    Queue queue;
     
    initQueue(&queue);//큐 초기화
    enqueue(&queue,0);
    
    while (count<6)    // 큐가 빌 때까지 
    {   
        int arr[20]={-1,};
       // printf("%d",arr[0]);
       int index = 0;
       while (!isEmpty(&queue))
       {
        
         int num =dequeue(&queue);
         arr[index] = num;
         index++;
       //  printf("%d",num);
       }

        for(int i=0;i<10;i++){
             if(count ==1){
                   // printf("%d",arr[i]);
                }
            if(arr[i]==-1){
                break;
            }
        
            int number = arr[i];
           //  printf("%d",number);
            //오른쪽 이동 가능할때
            if((number+1)%4 !=0){
                int now = number+1;
              //  printf("%d",now);
                enqueue(&queue,now);
                if(dp[now]<dp[number]+map[now]){
                    dp[now] = dp[number]+map[now];
                }
            }

            //아래 이동이 가능할때
            if(number+4<16){
                int now = number+4;
               //  printf("%d",now);
                enqueue(&queue,now);
                 if(dp[now]<dp[number]+map[now]){
                    dp[now] = dp[number]+map[now];
                }
            }   
        }
        
       count++;
    }
    
}

int main(){
    BFS();
    for(int i=0;i<16;i++){
         printf("%d",dp[i]);
    }
    return 0;
}
