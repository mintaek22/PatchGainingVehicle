#include<stdio.h>
#define DEST_QUEUE_SIZE 20
//0,0 ??? 3,3 
int map[20]={0,5,0,0,-5,
            0,-5,5,0,-5,
            0,5,0,0,5,
            0,0,0,5,0};
int dp[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int dq[DEST_QUEUE_SIZE]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

int front=-1;
int rear=-1;
int queue[DEST_QUEUE_SIZE]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

int IsEmpty(void){
    if(front==rear)//front?? rear?? gcc ?????? ??? 
        return 1;
    else return 0;
}

void addq(int value){
	rear = (rear+1)%10;
	 queue[rear]=value;
}
int deleteq(){
        front = (front+1)%10;
        return queue[front];
}

void dyn() {
    int count=0;
    addq(0);

    while (count<7)    // ??? ?? 
    {
       int arr[10]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
       // printf("%d",arr[0]);
       int index = 0;
       while (!IsEmpty())
       {
         int num =deleteq();
         arr[index] = num;
         index++;
      //   printf("%d,",num);
       }
     //    printf("\n");

        for(int i=0;i<10;i++){
            if(arr[i]==-1){
                break;
            }

            int number = arr[i];
           //  printf("%d",number);
            //?????? ???
            if((number+1)%5 !=0){
                int now = number+1;
              //  printf("%d",now);
                addq(now);
                if(dp[now]<dp[number]+map[now]){
                    dp[now] = dp[number]+map[now];
                }
            }

            //??? ?????
            if(number+5<20){
                int now = number+5;
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
   for(int i = 0; i< 4 ; i++){
       printf("%d %d %d %d %d",dq[5*i],dq[5*i+1],dq[5*i+2],dq[5*i+3],dq[5*i+4]);
       printf("\n");
   }
 //sleep(10000);
}

void make_dq(){
	int i = 0;
	int number = 0;
	int index = 19;
	while(index>0){
		int l=-1,u=-1;
		if((index-1)%5 != 0){
			l = index-1;
		}
		if(index-5>0){
			u = index -5;
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