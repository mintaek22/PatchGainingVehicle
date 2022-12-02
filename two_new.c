#include <stdio.h>
#define DEST_QUEUE_SIZE 20
// changing vars
#define TASK 1
#define MAP_SIZE_COL 5
#define MAP_SIZE_ROW 4
#define LOC_START 0
#define DIR_START 1
#define LOC_DEST 15
#define LOC_MOV 19
#define GRID_TICK_IN 2
#define GRID_TICK_OUT 10
#define WARI_DIFF 15
#define SCORE_RED 5
#define SCORE_BLU -5




int get_loc_row(int loc);
int get_loc_col(int loc);
int get_loc(int row, int col);
void print_dq(void);


void find_red(void);
void insert_array_dq(int index, int data);
int get_map_near(int cur, int row, int col);
int get_weight_2(int index, int from);
int remove_red_by_value(int value);
void add_branch(void);
int connect_branch(int loc_red);
int get_dq_index_by_value(int value);


// 0,0 ??? 3,3
// int map[MAP_SIZE_ROW][MAP_SIZE_COL] =
//     {{0, 1, 0, 0, 0},
//      {0, 2, 1, 0, 2},
//      {0, 1, 0, 0, 1},
//      {0, 0, 0, 1, 0}};
int map[MAP_SIZE_ROW][MAP_SIZE_COL] =
    {{0, 1, 0, 0, 0},
     {0, 2, 1, 0, 2},
     {0, 0, 0, 0, 1},
     {0, 1, 0, 1, 0}};
int dp[MAP_SIZE_ROW * MAP_SIZE_COL];

int dq[DEST_QUEUE_SIZE] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

int front = -1;
int rear = -1;
int queue[DEST_QUEUE_SIZE] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};


int red_list[DEST_QUEUE_SIZE] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
// int red_list[DEST_QUEUE_SIZE];
int red_index = 0;



void find_red(void){
    for(int i = 0 ; i < MAP_SIZE_ROW ; i++){
        for(int j = 0 ; j < MAP_SIZE_COL ; j++){
            if(map[i][j] == 1){
                red_list[red_index++] = get_loc(i,j);
            }
        }
    }
}

int remove_red_by_value(int value){
    int result = -1;
    for(int i = 0; i < red_index; i++){
        if(red_list[i] == value){
            result = i;
            red_list[i] = -1;
            for(int j = i+1;  j < red_index;j++){
                red_list[j-1] = red_list[j];
            }
            red_index--;
            break;
        }
    }
    return result;
}



void insert_array_dq(int index, int data){
    for(int i = DEST_QUEUE_SIZE - 1 ; i > index ; i--){
        dq[i] = dq[i-1];
    }
    dq[index] = data;
    print_dq();
}

int get_map_near(int cur, int row, int col){
    int row_new = get_loc_row(cur) + row;
    int col_new = get_loc_col(cur) + col;

    if(row_new > 0 && row_new < MAP_SIZE_ROW && col_new > 0 && col_new < MAP_SIZE_COL)
        return map[row_new][col_new];
    return -1000;
}

// from 0 : from down | from 1 : from right
int get_weight_2(int index, int from){
    int check_list[12][2] = {{1,0},{0,1},{-1,0},{0,-1},{2,0},{0,2},{-2,0},{0,-2},{1,1},{1,-1},{-1,-1},{-1,1}};
    int weight_of_weight_1[12] = {10,9,9,10,3,0,1,3,2,0,2,4};
    int weight_of_weight_0[12] = {10,9,9,10,3,1,0,3,2,0,2,4};
    int max = 0;
    for (int i = 0; i < 12; i++){
        int via = 0;
        int dest_weight = get_map_near(index, check_list[i][0], check_list[i][1]) -2;
        int temp_weight = -1000;
        if(i > 3 && i < 8) via = get_map_near(index, check_list[i - 4][0], check_list[i - 4][1]) -2;
        if(i > 8){
            via = get_map_near(index, check_list[i][0], 0);
            if (get_map_near(index, 0, check_list[i][1]) > via)
                via = get_map_near(index, 0, check_list[i][1]);
            via -= 2;
        }
        if(via == 3) via = 1;

        if(via <-999 || dest_weight < -999) temp_weight = -1000;
        else temp_weight = dest_weight + via;
        if (temp_weight > -1000){
            // if(from) printf("i %2d from (%2d,%2d) to (%2d,%2d) temp_weight %3d   weight %4d\n",i,get_loc_row(index),get_loc_col(index),get_loc_row(index) +check_list[i][0],get_loc_col(index) +check_list[i][1],temp_weight,temp_weight * weight_of_weight_1[i]);
            // else printf("i %2d from (%2d,%2d) to (%2d,%2d) temp_weight %3d   weight %4d\n",i,get_loc_row(index),get_loc_col(index),get_loc_row(index) +check_list[i][0],get_loc_col(index) +check_list[i][1],temp_weight,temp_weight * weight_of_weight_0[i]);
            
            if(from) max += temp_weight * weight_of_weight_1[i];
            else max += temp_weight * weight_of_weight_0[i];
        }

    }
    return max;
}

int get_dq_index_by_value(int value){
    int result = -1;
    for(int i  =0; i< DEST_QUEUE_SIZE ; i++){
        if(dq[i] == -1) break;
        if(dq[i] == value){
            result = i;
            break;
        }
    }
    return result;
}
    



//
int get_loc_row(int loc)
{
    return loc / MAP_SIZE_COL;
}

int get_loc_col(int loc)
{
    return loc % MAP_SIZE_COL;
}

int get_loc(int row, int col)
{
    return MAP_SIZE_COL * row + col;
}
//////

int IsEmpty(void)
{
    if (front == rear) // front?? rear?? ?????? ???
        return 1;
    else
        return 0;
}

void addq(int value)
{
    rear = (rear + 1) % DEST_QUEUE_SIZE;
    queue[rear] = value;
}
int deleteq()
{
    front = (front + 1) % DEST_QUEUE_SIZE;
    return queue[front];
}

// bfs search - dp
void dyn()
{
    int count = 0;
    addq(0);

    while (count < MAP_SIZE_COL + MAP_SIZE_ROW - 2) // ??? ??
    {
        int arr[DEST_QUEUE_SIZE];
        int enqueue_arr[DEST_QUEUE_SIZE];
        for (int i = 0; i < DEST_QUEUE_SIZE; i++)
        {
            arr[i] = -1;
            enqueue_arr[i] = -1;
        }
        int enqueue_idx = 0;
        // printf("%d",arr[0]);
        int index = 0;
        while (!IsEmpty())
        {
            int num = deleteq();
            arr[index] = num;
            index++;
            // printf("num ");
            // printf("%d ",num);
        }
        // printf("\n");

        for (int i = 0; i < DEST_QUEUE_SIZE; i++)
        {
            //    if (count == 1)
            //    {
            //        // printf("%d",arr[i]);
            //    }
            if (arr[i] == -1)
            {
                break;
            }

            int number = arr[i];
            
            //?????? ???
            if ((number + 1) % MAP_SIZE_COL != 0)
            {
                int now = number + 1;
                // printf("number %d now %d\n",number,now);

                int is_add = 1;
                for(int i = 0; i < enqueue_idx ; i++){
                    if(enqueue_arr[i] == now){
                        is_add = 0;
                        break;
                    }
                }
                if(is_add){
                    enqueue_arr[enqueue_idx++] = now;
                }
                

                if (dp[now] < dp[number] + map[get_loc_row(now)][get_loc_col(now)])
                {
                    dp[now] = dp[number] + map[get_loc_row(now)][get_loc_col(now)];
                }
            }

            //??? ?????
            if (number + MAP_SIZE_COL < MAP_SIZE_COL * MAP_SIZE_ROW)
            {
                int now = number + MAP_SIZE_COL;
                // printf("number %d now %d\n",number,now);

                int is_add = 1;
                for(int i = 0; i < enqueue_idx ; i++){
                    if(enqueue_arr[i] == now){
                        is_add = 0;
                        break;
                    }
                }
                if(is_add){
                    enqueue_arr[enqueue_idx++] = now;
                }
                
                if (dp[now] < dp[number] + map[get_loc_row(now)][get_loc_col(now)])
                {
                    dp[now] = dp[number] + map[get_loc_row(now)][get_loc_col(now)];
                }
            }
            // addq(now);
        }

        for(int i = 0; i< enqueue_idx ; i++){
            // printf("%d ",enqueue_arr[i]);
            addq(enqueue_arr[i]);
        }
        // printf("\n");


        // printf("dp\n");
        // for (int i = 0; i < MAP_SIZE_ROW; i++)
        // {
        //     for (int j = 0; j < MAP_SIZE_COL; j++)
        //     {
        //         printf("%d ", dp[MAP_SIZE_COL * i + j]);
        //     }
        //     printf("\n");
        // }


        count++;
    }
}
void print_map()
{
    for (int i = 0; i < MAP_SIZE_ROW; i++)
    {
        for (int j = 0; j < MAP_SIZE_COL; j++)
        {
            printf("%3d ", map[i][j]);
        }
        printf("\n");
    }
    // sleep(10000);
}

void make_dq()
{
    int i = 0;
    int number = 0;
    int index = LOC_MOV;
    while (index > LOC_START)
    {
        int l = -1, u = -1;
        if (index % MAP_SIZE_COL != 0)
        {
            l = index - 1;
        }
        if (index - MAP_SIZE_COL > 0)
        {
            u = index - MAP_SIZE_COL;
        }
        if (l > 0 && u > 0)
        {
            if (dp[l] > dp[u])
            {
                number = l;
            }
            else if (dp[l] < dp[u])
            {
                number = u;
            }
            else{
                int weight_up = get_weight_2(u,0);
                int weight_left = get_weight_2(l,1);
                printf("weight %d = %d | %d = %d\n",u,weight_up,l,weight_left);
                if(weight_up > weight_left)
                    number = u;
                else
                    number = l;
            }
        }
        else if (l < 0)
        {
            number = u;
        }
        else
        {
            number = l;
        }
        dq[i] = number;
        index = number;
        i++;
    }
    dq[--i] = LOC_START;
    dq[++i] = -1;
}

void add_branch(void){
    for(int i  =0; i< DEST_QUEUE_SIZE ; i++){
        if(dq[i] == -1) break;
        
        if(remove_red_by_value(dq[i]) != -1){
            map[get_loc_row(dq[i])][get_loc_col(dq[i])] = -2;
        }
    }
    print_map();
    // printf("red left\n");
    // for (int i = 0; i < red_index; i++)
    // {
    //     printf("%d ", red_list[i]);
    // }
    // printf("\n");

    for(int i = 0; i < red_index ; i++){
        printf("red %d\n",red_list[i]);
        if(connect_branch(red_list[i]) != -1){
            printf("red deleted %d\n",red_list[i]);
            map[get_loc_row(red_list[i])][get_loc_col(red_list[i])] = -2;
            remove_red_by_value(red_list[i]);
            i--;
        }
    }
    print_map();
}

int connect_branch(int loc_red){
    int check_list[12][2] = {{1,0},{0,1},{-1,0},{0,-1},{2,0},{0,2},{-2,0},{0,-2},{1,1},{1,-1},{-1,-1},{-1,1}};
    int result  = -1;
    int result_dq_idx = -1;
    // int result_via_idx = -1;
    // int result_via = -1;

    int result_list[DEST_QUEUE_SIZE];
    for(int i = 0; i< DEST_QUEUE_SIZE ; i++){
        result_list[i] = -1;
    }
    int result_list_idx = 0;
    int result_map_value = -5;


    for (int i = 0; i < 4; i++){
        int dest_dq_idx = get_dq_index_by_value(get_loc(get_loc_row(loc_red) + check_list[i][0], get_loc_col(loc_red) + check_list[i][1]));
        int dest_dq = dq[dest_dq_idx];
        
        printf("to %d idx %d : %d\n",get_loc(get_loc_row(loc_red) + check_list[i][0], get_loc_col(loc_red) + check_list[i][1]),dest_dq_idx,dest_dq);
        if(dest_dq_idx != -1 && map[get_loc_row(loc_red)][get_loc_col(loc_red)] > result_map_value){
            result_list_idx = 0;
            result_list[result_list_idx++] = loc_red;
            result_list[result_list_idx++] = dest_dq;
            result_dq_idx = dest_dq_idx;
            result = dest_dq;
        }
    }
    if(result != -1){
        for(int i = 0; i<result_list_idx ; i++){
            insert_array_dq(result_dq_idx,result_list[i]);
        }
    }
    else{
        for (int i = 4; i < 8; i++){
            int via_idx = get_loc(get_loc_row(loc_red) +check_list[i - 4][0],get_loc_col(loc_red) + check_list[i - 4][1]);
            int via_value = map[get_loc_row(via_idx)][get_loc_col(via_idx)];
            int dest_dq_idx = get_dq_index_by_value(get_loc(get_loc_row(loc_red) + check_list[i][0], get_loc_col(loc_red) + check_list[i][1]));
            int dest_dq = dq[dest_dq_idx];
            
            printf("to %d idx %d : %d\n",get_loc(get_loc_row(loc_red) + check_list[i][0], get_loc_col(loc_red) + check_list[i][1]),dest_dq_idx,dest_dq);
            if(dest_dq_idx != -1 && map[get_loc_row(loc_red)][get_loc_col(loc_red)] > result_map_value && via_value == 0){
                result_list_idx = 0;
                result_list[result_list_idx++] = via_idx;
                result_list[result_list_idx++] = loc_red;
                result_list[result_list_idx++] = via_idx;
                result_list[result_list_idx++] = dest_dq;
                result_dq_idx = dest_dq_idx;
                result = dest_dq;
                // result_via_idx = via;
                // result_via_value
                break;
            }
        }
        if(result != -1){
            for(int i = 0; i<result_list_idx ; i++){
                insert_array_dq(result_dq_idx,result_list[i]);
            }
        }else{
            for (int i = 8; i < 12; i++){
                int via_idx = get_loc(get_loc_row(loc_red) +check_list[i][0],get_loc_col(loc_red) + 0);
                int via_value = map[get_loc_row(via_idx)][get_loc_col(via_idx)];
                if (get_map_near(loc_red, 0, check_list[i][1]) > via_value){
                    via_idx = get_loc(get_loc_row(loc_red)+ 0,get_loc_col(loc_red) + check_list[i][1]);
                    via_value = map[get_loc_row(via_idx)][get_loc_col(via_idx)];
                }
                int dest_dq_idx = get_dq_index_by_value(get_loc(get_loc_row(loc_red) + check_list[i][0], get_loc_col(loc_red) + check_list[i][1]));
                int dest_dq = dq[dest_dq_idx];
                
                printf("to %d idx %d : %d\n",get_loc(get_loc_row(loc_red) + check_list[i][0], get_loc_col(loc_red) + check_list[i][1]),dest_dq_idx,dest_dq);
                if(dest_dq_idx != -1 && map[get_loc_row(loc_red)][get_loc_col(loc_red)] > result_map_value && via_value == 0){
                    result_list_idx = 0;
                    result_list[result_list_idx++] = via_idx;
                    result_list[result_list_idx++] = loc_red;
                    result_list[result_list_idx++] = via_idx;
                    result_list[result_list_idx++] = dest_dq;
                    result_dq_idx = dest_dq_idx;
                    result = dest_dq;
                    // result_via_idx = via;
                    // result_via_value
                    break;
                }
            }
            if(result != -1){
                for(int i = 0; i<result_list_idx ; i++){
                    insert_array_dq(result_dq_idx,result_list[i]);
                }
            }
        }
    }


    // print_dq();
    return result;
}

void caculate_map_score(void){
    for(int i  =0; i< MAP_SIZE_ROW ; i++){
        for(int j = 0; j < MAP_SIZE_COL ; j++){
            if(map[i][j] == 1){
                map[i][j] = SCORE_RED;
            }else if (map[i][j] == 2){
                map[i][j] = SCORE_BLU;
            }
        }
    }
}

void print_dq(void){
    printf("dq\n");
    for (int i = 0; i < DEST_QUEUE_SIZE; i++)
    {
        
        printf("%d ", dq[i]);
    }
    printf("\n");
}

int main()
{
    // for (int i = 0; i < MAP_SIZE_ROW * MAP_SIZE_COL; i++)
    // {
    //     dp[i] = 0;
    // }
    find_red();
    caculate_map_score();
    print_map();
    dyn();

    printf("dp\n");
    for(int i  =0; i< MAP_SIZE_ROW ; i++){
        for(int j = 0; j < MAP_SIZE_COL ; j++){
            printf("%3d ",dp[MAP_SIZE_COL * i + j]);
        }
        printf("\n");
    }

    make_dq();
    // printf("\n");
    
    // print_map();
    print_dq();
    
    printf("red\n");
    for (int i = 0; i < red_index; i++)
    {
        printf("%d ", red_list[i]);
    }
    printf("\n");

    add_branch();

    

    printf("\n");
}