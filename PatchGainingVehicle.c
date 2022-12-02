#pragma config(Sensor, S1, cs_left, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4, cs_middle, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S3, cs_right, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Motor, motorB, lm, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor, motorC, rm, tmotorEV3_Large, PIDControl, encoder)

#define TICKRATE 20
#define SPEED_MAX 20

//color
#define BLACK 1
#define BLUE 2
#define GREEN 3
#define YELLOW 4
#define RED 5
#define WHITE 6
#define BROWN 7




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
#define SCORE_RED 1
#define SCORE_BLU -1


//status
#define STATUS_SIZE 4
#define DETECT 0
#define MOVE 1		// 0 : stop | 1: straint | 2: rotate | 3: ongrid warigari
#define ONGRID 2	 // 0 : not on grid | 1 : on grid | 2 : on grid warigari | 3 :warigari done | 4 : grid out wait
#define WARIGARI 3   // 0: straignt | 1: left | 2: right

#define DEST_QUEUE_SIZE 20

int stat[STATUS_SIZE];
int dq[DEST_QUEUE_SIZE];
int queue[DEST_QUEUE_SIZE];
int dp[MAP_SIZE_ROW*MAP_SIZE_COL];
int map[MAP_SIZE_ROW][MAP_SIZE_COL];
int patch = 0;
int grid_tick_in = GRID_TICK_IN;
int grid_tick_out = GRID_TICK_IN;
int ongrid_warigari = 32;
int wari_diff = WARI_DIFF;

int score_q[DEST_QUEUE_SIZE];

int dq_idx = 0;

int dir_cur = DIR_START;
int dir_dest = DIR_START;

int loc_cur = LOC_START;

int score = 0;

int front=-1;
int rear=-1;
int red_list[DEST_QUEUE_SIZE];
int red_index = 0;


int check_list[12][2] = {{1,0},{0,1},{-1,0},{0,-1},{2,0},{0,2},{-2,0},{0,-2},{1,1},{1,-1},{-1,-1},{-1,1}};
int weight_of_weight_1[12] = {10,9,9,10,3,0,1,3,2,0,2,4};
int weight_of_weight_0[12] = {10,9,9,10,3,1,0,3,2,0,2,4};

void init_stat(void);
int get_stat(int index);
void set_stat(int index, int value);
void init_map(void);
void init_dq(void);
void init_score_q(void);
int get_next_column(int col);
void update_dq_detect(void);
int get_loc_row(int loc);
int get_loc_col(int loc);
int get_loc(int row, int col);
char * dir_to_text(int dir);
void print_map(void);
void print_dq(void);
void print_stat(void);


void set_motor(int left_speed, int right_speed);
void update_stat_by_color(void);
void update_status(void);
int get_loc_diff(int dir);
void update_loc(void);
void update_dq(int ex_stat);
void update_dq_1(void);
void update_dq_2(void);
void calculate_direction(void);
void turn_left(void);
void turn_right(void);
void update_action(void);


//alg 1
int IsEmpty(void);
void addq(int value);
int deleteq(void);
void dyn(void);
void make_dq();
void caculate_map_score(void);
void find_red(void);
void insert_array_dq(int index, int data);
int get_map_near(int cur, int row, int col);
int get_weight_2(int index, int from);
int remove_red_by_value(int value);
void add_branch(void);
int connect_branch(int loc_red);
int get_dq_index_by_value(int value);


task main()
{
	init_stat();
	init_map();
	init_dq();
	init_score_q();

	update_dq_detect();
	print_dq();

	sleep(3000);
	set_stat(MOVE,1);

	while(1){
		print_stat();
		update_stat_by_color();
		print_stat();
		update_status();
		print_stat();
		update_action();
		print_stat();
		sleep(TICKRATE);
	}
}


void init_stat(void){
	for(int i = 0; i<STATUS_SIZE; i++)
		stat[i] = 0;
	dir_cur = DIR_START;
	dir_dest = DIR_START;
	loc_cur = LOC_START;
	score = 0;
	patch = 0;
	grid_tick_in = GRID_TICK_IN;
	return;
}

int get_stat(int index){
	return stat[index];
}

void set_stat(int index, int value){
	if(index < STATUS_SIZE)
		stat[index] = value;
	return;
}

void init_map(void){
	for(int i = 0; i< MAP_SIZE_ROW ; i++){
		for(int j = 0; j< MAP_SIZE_COL ; j++){
			map[i][j] = 0;
		}
	}
}

void init_dq(void){
	for(int i = 0; i< DEST_QUEUE_SIZE ; i++){
		dq[i] = -1;
		queue[i] = -1;
		red_list = -1;
	}
	for(int i = 0; i< MAP_SIZE_ROW*MAP_SIZE_COL ; i++){
		dp[i] = 0;
	}
	dq_idx = 0;
	front = -1;
    rear = -1;
	red_index = 0;
	for (int i = 0; i < 300; i++)
	{
		displayStringAt(i, 75, " ");
	}
}

void init_score_q(void){
	for(int i = 0; i< DEST_QUEUE_SIZE ; i++){
		score_q[i] = 0;
	}
}

int get_next_column(int col){
	if(col == 0) return MAP_SIZE_COL - 1;
	else return 0;
}

void update_dq_detect(void){
	int idx = 0;
	if(get_loc_row(LOC_START) == 0){
		int col = get_next_column(get_loc_col(LOC_START));
		dq[idx++] = get_loc(0,col);

		for(int i = 1; i < MAP_SIZE_ROW ; i++){
			dq[idx++] = get_loc(i,col);
			col = get_next_column(get_loc_col(dq[idx-1]));
			dq[idx++] = get_loc(i,col);
		}
		return;
	}
	if(get_loc_row(LOC_START) == MAP_SIZE_ROW - 1){
		int col = get_next_column(get_loc_col(LOC_START));
		dq[idx++] = get_loc(MAP_SIZE_ROW - 1,col);

		for(int i = MAP_SIZE_ROW - 2; i > -1 ; i--){
			dq[idx++] = get_loc(i,col);
			col = get_next_column(get_loc_col(dq[idx-1]));
			dq[idx++] = get_loc(i,col);
		}
		return;
	}
}

int get_loc_row(int loc){
	return loc / MAP_SIZE_COL;
}

int get_loc_col(int loc){
	return loc % MAP_SIZE_COL;
}

int get_loc(int row, int col){
	return MAP_SIZE_COL * row + col;
}

char * dir_to_text(int dir){
	if(dir == 0) return "up";
	if(dir == 1) return "right";
	if(dir == 2) return "down";
	if(dir == 3) return "left";
	return "err";
}

void print_map(){
	for(int i = 0; i<MAP_SIZE_ROW ; i++){
		for(int j = 0; j< MAP_SIZE_COL ; j++){
			displayStringAt(j*15,(MAP_SIZE_ROW-i)*15, map[i][j] == 0 ? "+" :(map[i][j] == 1 ? "O" : "X"));
		}
	}
}
void print_dq(){
	for (int i = 0; i < DEST_QUEUE_SIZE; i++)
	{
		displayStringAt(i * 15, 75, "%d", dq[i]);
	}
}

void print_stat(){
	displayTextLine(1, "det %d mv %d ongrid %d wari %d %d",get_stat(DETECT), get_stat(MOVE),get_stat(ONGRID),get_stat(WARIGARI),wari_diff);
	displayTextLine(3, "cur[%d,%d]dir[%d,%d] pat%d dq %d", get_loc_row(loc_cur), get_loc_col(loc_cur), get_loc_row(dq[dq_idx]), get_loc_col(dq[dq_idx]),patch,dq_idx);
	displayTextLine(5, "cu %s de %s sc %d",dir_to_text(dir_cur),dir_to_text(dir_dest),score);
	print_map();
	print_dq();
}


void set_motor(int left_speed, int right_speed){
	setMotorSpeed(lm, left_speed);
	setMotorSpeed(rm, right_speed);
}


void update_stat_by_color(void){

	int col_left = getColorName(cs_left);
	int col_middle = getColorName(cs_middle);
	int col_right = getColorName(cs_right);
	if(get_stat(MOVE) != 2){
		if (get_stat(DETECT) && col_middle == WHITE){
			int warigari = get_stat(WARIGARI);
			if (warigari == 2){
				// if(!wari_diff--){
					set_stat(WARIGARI, 1);
					// wari_diff = WARI_DIFF;
				// }
			}
			else{
				// if(!wari_diff--){
					set_stat(WARIGARI, 2);
					// wari_diff = WARI_DIFF;
				// }
			}
		}
		else{
			set_stat(WARIGARI, 0);
			wari_diff = WARI_DIFF;
			int ongrid = get_stat(ONGRID);

			if (!patch && ongrid != 4){
				if (col_middle == RED)
					patch = 1;
				if (col_middle == GREEN)
					patch = 2;
			}

			if(col_left == WHITE && col_right == WHITE){
				if(ongrid == 4){
					if(!grid_tick_out--){
						set_stat(ONGRID,0);
						grid_tick_out = GRID_TICK_OUT;
						return;
					}
				}
			}else{
				if (ongrid == 0){
					if (!grid_tick_in--){
						set_stat(ONGRID, 1);
						grid_tick_in = GRID_TICK_IN;
					}
				}
			}
		}
	}



}


void update_status(void){
		// if (!get_stat(WARIGARI)){
		if (get_stat(ONGRID) == 1){
			if(get_stat(DETECT)) update_loc();
			set_stat(ONGRID,2);
			set_stat(MOVE,3);
			return;
		}
		if (get_stat(ONGRID) == 3){

			int detect = get_stat(DETECT);
			if(detect == 0){
				set_stat(DETECT, 1);
			}
			if (detect == 1){
				// if (!patch){
				// 	map[get_loc_row(loc_cur)][get_loc_col(loc_cur)] = patch;
				// 	patch = 0;
				// }
				map[get_loc_row(loc_cur)][get_loc_col(loc_cur)] = patch;
				patch = 0;
			}
			if (detect == 4 && TASK == 2)
			{
				score -= 1;
			}

			if (loc_cur == dq[dq_idx])
			{

				if (detect == 4)
				{
					score += score_q[dq_idx];
				}

				dq_idx = (dq_idx + 1) % DEST_QUEUE_SIZE;
				if (dq[dq_idx] == -1)
				{
					if (detect == 1 || detect == 2)
					{
						update_dq(detect);
						dq_idx = 0;

					}
					else if (detect == 4)
					{

						set_stat(MOVE, 0);
						return;
					}
				}
				calculate_direction();
			}

			set_stat(ONGRID,4);
		}
	// }
}

int get_loc_diff(int dir){
	if(dir == 3) return -1;
	if(dir == 1) return 1;
	if(dir == 0) return -1 * MAP_SIZE_COL;
	if(dir == 2) return MAP_SIZE_COL;
	return 0;
}

void update_loc(void){
	loc_cur += get_loc_diff(dir_cur);
}

void update_dq(int ex_stat){

	if(ex_stat == 1){
		dq[0] = LOC_MOV;
		dq[1] = -1;
		set_stat(DETECT,2);
	}
	if(ex_stat == 2){
		set_stat(DETECT,3);
		// set_motor(SPEED_MAX,SPEED_MAX);
		// sleep(10000/SPEED_MAX);
		set_motor(0,0);
		if(TASK == 1){
			update_dq_1();
			dq_idx = 0;
			set_stat(DETECT,4);
			set_stat(MOVE,2);
		}
		if(TASK == 2){
			update_dq_2();
			dq_idx = 0;
			set_stat(DETECT,4);
			set_stat(MOVE,2);
		}
	}
}


void update_dq_1(void){
	init_dq();
	caculate_map_score();
    // print_map();
    dyn();
    make_dq();


}


void update_dq_2(void){
	init_dq();
	find_red();
	caculate_map_score();
    print_map();
    dyn();
	make_dq();
	add_branch();
	print_dq();
}

void calculate_direction(void){
	int dest = dq[dq_idx];


	if(get_loc_row(loc_cur) == get_loc_row(dest)){
		if(get_loc_col(dest) > get_loc_col(loc_cur)){
			dir_dest = 1;
			set_stat(MOVE,2);
			return;
		}
		if(get_loc_col(dest) < get_loc_col(loc_cur)){
			dir_dest = 3;
			set_stat(MOVE,2);
			return;
		}
		return;
	}

	if(get_loc_col(loc_cur) == get_loc_col(dest)){

		if(get_loc_row(dest) > get_loc_row(loc_cur)){
			dir_dest = 2;
			set_stat(MOVE,2);
			return;
		}

		if(get_loc_col(dest) < get_loc_col(loc_cur)){
			dir_dest = 0;
			set_stat(MOVE,2);
			return;
		}
		return;
	}
}
void turn_left(void)
{
	set_motor(-SPEED_MAX, SPEED_MAX);
	// while (getColorName(cs_left) != YELLOW){sleep(TICKRATE);}
	sleep(12000/SPEED_MAX);
	while (getColorName(cs_middle) != YELLOW){sleep(TICKRATE);}
	sleep(1200/SPEED_MAX);
	// set_stat(WARIGARI,2);
	set_motor(0, 0);
}
void turn_right(void)
{
	set_motor(SPEED_MAX, -SPEED_MAX);
	// while (getColorName(cs_right) != YELLOW){sleep(TICKRATE);}
	sleep(12000/SPEED_MAX);
	while (getColorName(cs_middle) != YELLOW){sleep(TICKRATE);}
	sleep(1200/SPEED_MAX);
	// set_stat(WARIGARI,1);
	set_motor(0, 0);
}

void update_action(void){
	int detect = get_stat(DETECT);
	int move = get_stat(MOVE);

	if(move == 3){
		// if(ongrid_warigari-- > 28){
		// 	// set_motor(0.2*SPEED_MAX,-0.2*SPEED_MAX);
		// 	set_motor(0.5*SPEED_MAX,-0.5*SPEED_MAX);
		// 	return;
		// }else if(ongrid_warigari-- > 8){
		// 	set_motor(-0.5*SPEED_MAX,0.5*SPEED_MAX);
		// 	return;
		// }else if(ongrid_warigari-- > -1){
		// 	set_motor(0.5*SPEED_MAX,-0.5*SPEED_MAX);
		// 	return;
		// }
			set_stat(MOVE,1);
			set_stat(ONGRID,3);
			ongrid_warigari = 32;
		return;
	}

	if (move == 2)
	{
		set_motor(SPEED_MAX,SPEED_MAX);
		sleep(11000/SPEED_MAX);
		if (dir_cur == dir_dest)
		{ // 0 rotation
			return;
		}
		if ((dir_cur - dir_dest + 4) % 4 == 3)
		{ // 90 rotation
			turn_right();
		}
		else if ((dir_cur - dir_dest + 4) % 4 == 1)
		{ // -90 rotation
			turn_left();
		}
		else if ((dir_cur - dir_dest + 4) % 4 == 2)
		{ // 180 rotation
			turn_right();
			// dir_cur = (dir_cur + 1)%4;
			turn_right();
		}
		dir_cur = dir_dest;
		set_stat(MOVE, 1);
		set_stat(ONGRID,0);
		move = get_stat(MOVE);

	}

	if(move == 1){
		int warigari = get_stat(WARIGARI);

		if(warigari == 2){
			//right
			int isreverse = 0;
			set_motor(-0.5*SPEED_MAX,-0.5*SPEED_MAX);
			sleep(400/SPEED_MAX);
			set_motor(0.8*SPEED_MAX,-0.8*SPEED_MAX);
			wari_diff = WARI_DIFF;
			while(getColorName(cs_middle)!=YELLOW){
				sleep(TICKRATE);
				if(!wari_diff--){
					wari_diff = WARI_DIFF;
					isreverse = 1;
					break;
				}
			}
			if (isreverse){
				set_motor(-0.8 * SPEED_MAX, 0.8 * SPEED_MAX);
				while(getColorName(cs_middle)!=YELLOW){sleep(TICKRATE);}
			}
			sleep(600/SPEED_MAX);
		}else if (warigari == 1){
			int isreverse = 0;
			set_motor(-0.5*SPEED_MAX,-0.5*SPEED_MAX);
			sleep(400/SPEED_MAX);
			set_motor(-0.8*SPEED_MAX, 0.8*SPEED_MAX);
			wari_diff = WARI_DIFF;
			while(getColorName(cs_middle)!=YELLOW){
				sleep(TICKRATE);
				if(!wari_diff--){
					wari_diff = WARI_DIFF;
					isreverse = 1;
					break;
				}
			}
			if (isreverse){
				set_motor(0.8 * SPEED_MAX, -0.8 * SPEED_MAX);
				while(getColorName(cs_middle)!=YELLOW){sleep(TICKRATE);}
			}
			sleep(600/SPEED_MAX);
		}
		set_motor(SPEED_MAX,SPEED_MAX);

		return;
	}



	if(!move){
		set_motor(0,0);
		if (detect == 4)
		{

		}
		return;
	}
}


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
	score = dp[MAP_SIZE_COL*MAP_SIZE_ROW-1];
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
                // printf("weight %d = %d | %d = %d\n",u,weight_up,l,weight_left);
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
        // printf("red %d\n",red_list[i]);
        if(connect_branch(red_list[i]) != -1){
            // printf("red deleted %d\n",red_list[i]);
            map[get_loc_row(red_list[i])][get_loc_col(red_list[i])] = -2;
            remove_red_by_value(red_list[i]);
            i--;
        }
    }
    print_map();
}

int connect_branch(int loc_red){
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

        // printf("to %d idx %d : %d\n",get_loc(get_loc_row(loc_red) + check_list[i][0], get_loc_col(loc_red) + check_list[i][1]),dest_dq_idx,dest_dq);
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

            // printf("to %d idx %d : %d\n",get_loc(get_loc_row(loc_red) + check_list[i][0], get_loc_col(loc_red) + check_list[i][1]),dest_dq_idx,dest_dq);
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

                // printf("to %d idx %d : %d\n",get_loc(get_loc_row(loc_red) + check_list[i][0], get_loc_col(loc_red) + check_list[i][1]),dest_dq_idx,dest_dq);
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
