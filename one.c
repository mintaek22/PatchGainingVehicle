#include<stdio.h>
#define DEST_QUEUE_SIZE 20
// changing vars
#define TASK 1
#define MAP_SIZE_COL 4
#define MAP_SIZE_ROW 4
#define LOC_START 0
#define DIR_START 1
#define LOC_DEST 15
#define LOC_MOV 15
#define GRID_TICK_IN 2
#define GRID_TICK_OUT 10
#define WARI_DIFF 15

//0,0 ??? 3,3 
int map[16]={0,1,0,0,
            0,-1,-1,0,
            1,1,0,0,
            1,1,0,0};
int dp[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int dq[DEST_QUEUE_SIZE]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

int front=-1;
int rear=-1;
int queue[DEST_QUEUE_SIZE]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

int IsEmpty(void){
    if(front==rear)//front?? rear?? ?????? ??? 
        return 1;
    else return 0;
}

void addq(int value){
	rear = (rear+1)%DEST_QUEUE_SIZE;
	 queue[rear]=value;
}
int deleteq(){
        front = (front+1)%DEST_QUEUE_SIZE;
        return queue[front];
}


// bfs search - dp
void dyn() {
    int count=0;
    addq(0);

    while (count < MAP_SIZE_COL + MAP_SIZE_ROW - 2)    // ??? ?? 
    {
       int arr[DEST_QUEUE_SIZE];
       for(int i =0 ;i < DEST_QUEUE_SIZE ; i++){
        arr[i] = -1;
       }
       // printf("%d",arr[0]);
       int index = 0;
       while (!IsEmpty())
       {
         int num =deleteq();
         arr[index] = num;
         index++;
       //  printf("%d",num);
       }

        for(int i=0;i<DEST_QUEUE_SIZE;i++){
             if(count ==1){
                   // printf("%d",arr[i]);
                }
            if(arr[i]==-1){
                break;
            }

            int number = arr[i];
           //  printf("%d",number);
            //?????? ???
            if((number+1) % MAP_SIZE_COL !=0){
                int now = number+1;
              //  printf("%d",now);
                addq(now);
                if(dp[now]<dp[number]+map[now]){
                    dp[now] = dp[number]+map[now];
                }
            }

            //??? ?????
            if(number+MAP_SIZE_COL < MAP_SIZE_COL*MAP_SIZE_ROW){
                int now = number+MAP_SIZE_COL;
               //  printf("%d",now);
                addq(now);
                 if(dp[now]<dp[number]+map[now]){
                    dp[now] = dp[number]+map[now];
                }
            }
        }

       count++;
    }
}
void print_map(){
    for (int i = 0; i < MAP_SIZE_ROW; i++)
    {
        for (int j = 0; j < MAP_SIZE_COL; j++)
        {
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
    // sleep(10000);
}

void make_dq(){
	int i = 0;
	int number = 0;
	int index = LOC_MOV;
	while(index>LOC_START){
		int l=-1,u=-1;
		if((index-1)%MAP_SIZE_ROW != 0){
			l = index-1;
		}
		if(index-MAP_SIZE_ROW>0){
			u = index -MAP_SIZE_ROW;
		}	
		if(l>0 && u>0){
			if(dp[l]>dp[u]){
				number = l;
			}
			else{
				number = u;
			}
		}
		else if(l<0){
			number = u;
		}
		else{
			number = l;
		}
		dq[i] = number;
		index = number;
		i++;
	}

}

int main(){
    dyn();
    make_dq();
    print_map();
}