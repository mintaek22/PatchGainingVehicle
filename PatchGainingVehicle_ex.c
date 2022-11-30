#pragma config(Sensor, S1, cs_left, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4, cs_middle, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S3, cs_right, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Motor, motorB, lm, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor, motorC, rm, tmotorEV3_Large, PIDControl, encoder)

#define TICKRATE 20		//매 연산이 실행되는 틱레이트 - 단위는 ms
#define SPEED_MAX 8		//로봇의 최대 이동 속도

//컬러 인덱스
#define BLACK 1
#define BLUE 2
#define GREEN 3
#define YELLOW 4
#define RED 5
#define WHITE 6
#define BROWN 7




//과제에 따라 변경해야 할 변수들
#define TASK 1 				// 수행 과제 - 1: 1번 과제 | 2: 2번 과제
#define MAP_SIZE_COL 4		// 맵 격자 가로 크기
#define MAP_SIZE_ROW 4		// 맵 격자 세로 크기
#define LOC_START 0 		// 로봇의 시작 위치 정의 - (row,column)좌표값을 MAP_SIZE_COL * row  + column 로 나타낸다
#define DIR_START 2			// 로봇의 시작 방향 정의 - 1 : 위쪽 | 2: 오른쪽 | 3 : 아래쪽 | 4 : 왼쪽
#define LOC_DEST 15			// 도착점 좌표
#define LOC_MOV 15			// 이동점 좌표 - 도착점과 이동점 좌표가 같을 수도 있다
#define GRID_TICK 4 		// 가운데 컬러 센서가 좌우 컬러센서보다 뒤에 있을 경우 갱신을 위해 확인하는 틱

//상태 변수 관련 정보 - 상태 변수는 stat 배열에 저장되며 아래는 각 스텟의 인덱스이다 | get_stat(상태변수명)으로 접근 가능 | set_stat(상태변수명, 값)으로 셋 가능
#define STATUS_SIZE 4		// 상태 변수 배열의 크기
#define DETECT 0			// 현재 디텍팅 상태가 저장된 인덱스 - stat[DETECT] = 0 : 시작 전 | stat[DETECT] = 1 : 격자를 순서대로 탐색 중 | stat[DETECT] = 2 : 탐색 완료 후 도착점에서 이동점으로 이동 중  | stat[DETECT] = 3 : 패치 경로를 탐색 중 | stat[DETECT] = 4 : 패치 경로에 따라 되돌아오기 진행 중
#define MOVE 1 				// 현재 로봇의 이동 상태가 저장된 인덱스 stat[MOVE] = 0 : 현재 로봇은 정지 상태  | stat[MOVE] = 1 : 현재 로봇은 직진 상태  | stat[MOVE] = 2 : 현재 로봇은 dir_dest을 향해 회전 중 상태
#define ONGRID 2			// 현재 로봇이 격자를 발견하였는가 stat[ONGRID] = 0 : 로봇은 격자좌표 위에 있지 않음 | stat[ONGRID] = 1 : 왼쪽 혹은 오른쪽 컬러센서에 노란색이 인식됨 | stat[ONGRID] = 2 : 왼쪽 혹은 오른쪽 컬러센서가 막 노란색에서 흰색으로 변함 -> 격자 위에 진입함
#define OFFROAD 3			// 현재 로봇이 길을 벗어났는가 (가운데 컬러 센서 값이 흰색인가) - stat[OFFROAD] = 0 : 로봇은 길을 잘 따라가고 있음(가운데 컬러센서가 흰색이 아님) | stat[OFFROAD] = 1 : 로봇은 길을 벗어남(가운데 컬러센서가 흰색임)

#define DEST_QUEUE_SIZE 20 	// destination queue 의 크기를 정의 -  destination queue는 원형 큐로 dq_idx 변수가 현재 가리키고 있는 destination queue 의 인덱스이다 | 자세한 설명은 문서에

int stat[STATUS_SIZE];
int dq[DEST_QUEUE_SIZE]; 	// destination queue를 정의한다 -원형 큐이다 | 자세한 설명은 문서에
int map[MAP_SIZE_ROW][MAP_SIZE_COL]; // 0 : 빈 공간 | 1: 빨간 색 | 2 : 파란 색
int patch = 0; 				//패치를 저장하는 변수 - 가장 최근에 가운데 컬러 센서에 인식된 값이 [0 : 다른 색 | 1: 빨간 색 | 2 : 파란 색]의 형태로 저장되고, 빨간 색 혹은 파란색 값이 해당 위치의 맵 데이터에 복사 완료 후에는 다시 0로 변경된다
int grid_tick = GRID_TICK;

int score_q[DEST_QUEUE_SIZE];	// 점수 변화량을 저장하는 배열 -> 자세한 설명은 문서에

int dq_idx = 0;

int dir_cur = DIR_START;			// 로봇의 현재 방향을 나타내는 변수  - 1 : 위쪽 | 2: 오른쪽 | 3 : 아래쪽 | 4 : 왼쪽
int dir_dest = DIR_START;			// 로봇의 목표 방향을 나타내는 변수 - 회전 시 사용한다

int loc_cur = LOC_START;	// 로봇의 현재 위치를 나타내는 변수 - (row,column)좌표값을 MAP_SIZE_COL * row  + column 로 나타낸다  | ex - 5 x 4 맵에서 로봇이 1행 2열에 있다면 loc_cur = 7

int score = 0;				// 현재 점수


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
int get_loc(int x, int y);
char * dir_to_text(int dir);
void print_map(int startline);
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
void update_action(void);

task main()
{
	init_stat();
	init_map();
	init_dq();
	init_score_q();
	sleep(5000);  // 5초동안 대기 후 출발
	update_dq_detect(); //destination queue를 ㄹ 자 모양의 탐색으로 초기화
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

// 로봇의 STATUS를 초기화
void init_stat(void){
	for(int i = 0; i<STATUS_SIZE; i++)
		stat[i] = 0;
	dir_cur = DIR_START; //로봇의 시작 시 방향 정의
	loc_cur = LOC_START;
	score = 0;
	patch = 0; 
	grid_tick = GRID_TICK;
	return;
}

// input 인덱스에 해당하는 스텟 값을 가져오는 함수 - 사용 예시  : if(get_stat(DETECT) == 2) do something
int get_stat(int index){
	return stat[index];
}

// index에 해당하는 스텟 값을 value로 수정하는 함수 - 사용 예시 : set_stat(DETECT, 2)
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

// destination queue 를 -1으로 초기화한다
void init_dq(void){
	for(int i = 0; i< DEST_QUEUE_SIZE ; i++){
		dq[i] = -1;
	}
	dq_idx = 0;
}

// score queue 를 -1으로 초기화한다
void init_score_q(void){
	for(int i = 0; i< DEST_QUEUE_SIZE ; i++){
		score_q[i] = 0;
	}
}

int get_next_column(int col){
	if(col == 0) return MAP_SIZE_COL - 1;
	else return 0;
}

// destination queue를 ㄹ 자 모양의 탐색으로 초기화하는 함수
void update_dq_detect(void){
	int idx = 0;
	if(get_loc_row(LOC_START) == 0){
		int col = get_next_column(get_loc_col(LOC_START));
		dq[idx++] = get_loc(0,col);
		
		for(int i = 1; i < MAP_SIZE_ROW ; i++){
			dq[idx++] = get_loc(i,col);
			col = get_next_column(get_loc_col(LOC_START));
			dq[idx++] = get_loc(i,col);
		}
		return;
	}
	if(get_loc_row(LOC_START) == MAP_SIZE_ROW - 1){
		int col = get_next_column(get_loc_col(LOC_START));
		dq[idx++] = get_loc(MAP_SIZE_ROW - 1,col);
		
		for(int i = MAP_SIZE_ROW - 2; i > -1 ; i--){
			dq[idx++] = get_loc(i,col);
			col = get_next_column(get_loc_col(LOC_START));
			dq[idx++] = get_loc(i,col);
		}
		return;
	}
}

// loc의 row좌표를 얻어온다
int get_loc_row(int loc){
	return loc / MAP_SIZE_COL;
}
// loc의 column좌표를 얻어온다
int get_loc_col(int loc){
	return loc % MAP_SIZE_COL;
}

// (row, column) 좌표를 loc 값으로 반환한다
int get_loc(int row, int column){
	return MAP_SIZE_COL * row + column;
}


// direciton 숫자를 텍스트로 변환하는 함수
char * dir_to_text(int dir){
	if(dir == 1) return "up";
	if(dir == 2) return "right";
	if(dir == 3) return "down";
	if(dir == 4) return "left";
	return "err"
}

// 맵 정보를 출력하는 함수
void print_map(int startline){
	for(int i = 0; i< MAP_SIZE_ROW ; i++){
		for(int j = 0; j< MAP_SIZE_COL ; j++){
			displayBigStringAt(startline+i, 2 * j, map[i][j] == 0 ? "+" :(map[i][j] == 1 ? "O" : "X"));
		}
	}
}

// 현재 스텟을 출력하는 함수
void print_stat(){
	displayBigTextLine(1, "det %d mv %d off %d",get_stat(DETECT), get_stat(MOVE),get_stat(OFFROAD));
	displayBigTextLine(3, "cur [%d,%d] next [%d,%d]", get_loc_row(loc_cur), get_loc_col(loc_cur), get_loc_row(dq[dq_idx]), get_loc_col(dq[dq_idx]));
	displayBigTextLine(5, "cur %s dest %s score %d",dir_to_text(dir_cur),dir_to_text(dir_dest),score);
	print_map(7);
}



// left 와  right 모터의 스피드를 input값으로 변경한다
void set_motor(int left_speed, int right_speed){
	setMotorSpeed(lm, left_speed);
	setMotorSpeed(rm, right_speed);
}

// 현재 컬러에 따라 각 그리드 좌표 위를 지나는지 판단하는 함수
void update_stat_by_color(void){
	//컬러 센서 값들 받아오기
	int col_left = getColorName(cs_left);
	int col_middle = getColorName(cs_middle);
	int col_right = getColorName(cs_right);

	if(col_middle == WHITE){
		// 가운데 컬러 센서 값이 흰색인 경우 :  길을 벗어남 - CALIBRATION 필요
		// 단, 마지막 목적지에 도착 후 혹은 바깥 쪽 좌표에 도달 후 추가 전진되었을 경우 offroad처리가 되지 않도록 예외 처리가 필요!
		// GRID_TICK의 수치 조정과 각종 상태 변수를 확인하며 처리
		/* 
		code here!
		 */
		set_stat(OFFROAD,1);  // offroad 스텟을 on
	} else{
		if(!patch){
			// 가장 최근의 격자좌표 컬러를 저장하는 patch변수에 0이 저장되어 있고 현재 가운데 컬러센서에 빨간 색 혹은 파란색이 인식되었을 때 해당 값을 1 또는 2로 변경한다
			if(col_middle == RED) patch = 1;
			if(col_middle == BLUE) patch = 2;
		}
		// 왼쪽 오른쪽 센서가 둘다 흰색일 경우
		if(col_left == WHITE && col_right == WHITE){
			// 왼쪽 혹은 오른쪽 센서에 노란색 길이 인식되었다가 이제 막 흰색으로 변한 경우 -> 그리드 좌표 위에 올라옴 판단 
			if(get_stat(ONGRID) == 1){
				// 가운데 컬러 센서가 좌우 컬러센서보다 뒤에 있을 경우를 대비해 지정된 틱 횟수만큼 추가 확인 후 ongrid 스탯을 2로 변경
				if(!grid_tick--){
					set_stat(ONGRID,2);
					grid_tick = GRID_TICK;
					return;
				}
				
			}
		}else{
			// 왼쪽 오른쪽 센서 둘 중 하나 이상이 흰색이 아닐 경우

			// 흰색에서 이제 막 두 센서에 노란색이 인식되었을 경우 ongrid  스텟을 1로 변경
			if(get_stat(ONGRID) == 0)
				set_stat(ONGRID,1);
		}
		
	}
	
		 
}

// 컬러 센서로 받아온 정보에 따라서 부가적으로 stat을 업데이트 하는 함수
void update_status(void){
	//맵이 길을 벗어나지 않은 경우 - 나머지 기능 실험시에는 주석처리후 실험
	// if (!get_stat(OFFROAD)){
		// 격자 좌표위에 올라온 경우
		if (get_stat(ONGRID) == 2){
			int detect = get_stat(DETECT);

			// 로봇이 격자위에서 동작을 수행중인 경우
			if (detect){
				update_loc(); // 현재 좌표를 업데이트한다

				// 현재 격자를 탐색하는 중이고 최근 패치가 빨간 색 혹은 파란 색인 경우
				if (detect == 1 && !patch){
					map[get_loc_row(loc_cur)][get_loc_col(loc_cur)] = patch; // 맵의 현재 좌표의 컬러 값을 해당 색으로 변경한다
					patch = 0;												 // patch 변수를 초기화한다
				}

				// 만약 회귀 중이고 현재 과제가 제 2과제일 경우 이동시마다 스코어가 1씩 줄어야 한다
				if (detect == 4 && TASK == 2){
					score -= 1;
				}


				//현재 목표 위치에 도달하였을 경우 - detect 변수의 값이 1, 2, 4 가능
				if(loc_cur == dq[dq_idx]){
					// 만약 회귀 중이라면 -> 현재 스코어를 스코어 큐의 값만큼 업데이트 - 자세한 설명은 문서에
					if (detect == 4){
						score += score_q[dq_idx];
					}

					dq_idx = (dq_idx + 1) % DEST_QUEUE_SIZE; // destination queue 의 인덱스를 다음 인덱스 값으로 변경한다 -> 목표 위치 값을 갱신한다

					// 해당 상태(패치 탐색 or 도착점 -> 이동점 이동 or 이동점 -> 출발점 회귀)가 끝난 경우
					if(dq[dq_idx] == -1){
						// 패치 탐색 혹은 이동점으로 이동이 끝난 경우
						if(detect == 1 || detect == 2){
							update_dq(detect); 	// destination queue 를 갱신한다
							dq_idx = 0; 	// destination queue index 를 갱신한다
						}else if(detect == 4){
							// 회귀가 끝난 경우
							set_stat(MOVE,0); //정지한다
							return;
							// update_action에서 판단 시 : detect 가 4이고 move가 0이면 상황 종료
						}
					} 

					calculate_direction();  // 다음 목표를 위한 회전 방향을 설정한다
				}
			}
			else
			{
				set_stat(DETECT, 1); //로봇이 맵 바깥에서 막 맵에 진입하였을 때 - detect stat을 1로 변경
				return;
			}
		}
	// }
}

// 방향 값에 따라 MAP_SIZE_COL * row  + column 위치 좌표의 수정값을 리턴한다.
// 예를 들어 5 * 4 맵에서 현재 위치가 [row,col] : [1,2] = 7이고 아래쪽으로 한칸 가려면 얼마만큼 좌표값을 수정해야 하는 지 얻기 위해 get_loc_diff(3) 을 실행하면 (1 : 위쪽 | 2: 오른쪽 | 3 : 아래쪽 | 4 : 왼쪽)
// [2,2] 는 MAP_SIZE_COL * row  + column 위치 좌표가 12 이므로 7에서 +5 를 해야 한다. 따라서 get_loc_diff(7,3)의 return 값은 5이다. 추가로 예시를 들면 get_loc_diff(4) 의 return 값은 -1이다
int get_loc_diff(int dir){
	if(dir == 4) return -1;	
	if(dir == 2) return 1;
	if(dir == 1) return -1 * MAP_SIZE_COL;
	if(dir == 3) return MAP_SIZE_COL;
}

// 현재 로봇이 위치한 좌표를 업데이트 하는 함수
void update_loc(void){
	loc_cur += get_loc_diff(dir_cur);
}

// destination queue를 업데이트하는 함수
void update_dq(int ex_stat){
	// 패치 탐색이 끝난 경우
	if(ex_stat == 1){
		dq[0] = LOC_MOV;	// 다음 목표를 이동점으로 교체한다
		dq[1] = -1;			// 마지막 목표 이후의 값은 -1이어야 한다
		set_stat(DETECT,2); // 현재 detect 스텟을 도착점 -> 이동점 이동 중 상태로 교체한다 
	} 
	// 이동점으로 이동이 끝난 경우 -> 알고리즘에 따라 각 과제의 최대 점수 획득 경로를 계산한 후 dq를 해당 경로로 업데이트한다
	if(ex_stat == 2){
		set_stat(DETECT,3); // 현재 detect 스텟을 점수 획득 경로 연산 중 상태로 업데이트한다
		set_stat(MOVE,0);
		set_motor(0,0); 	//연산이 오래 걸릴 수도 있는 특수한 상황이므로 전체적인 자료 구조와는 맞지 않지만 정지 후 연산한다

		if(TASK == 1){
			update_dq_1();
			dq_idx = 0;
			set_stat(DETECT,4); // 현재 detect 스텟을 패치 경로에 따라 되돌아오기 진행 중 상태로 업데이트한다
		}
		if(TASK == 2){
			update_dq_2();
			dq_idx = 0;
			set_stat(DETECT,4); // 현재 detect 스텟을 패치 경로에 따라 되돌아오기 진행 중 상태로 업데이트한다
		}
	} 
}


// 1번 과제의 최다 획득 경로를 계산하고 해당 정보를 dq에 업데이트 하는 함수
void update_dq_1(void){
	/*
	code here!
	*/
	/*
	for ...
		dq[somthing] = something
	

	dq[end+1] = -1;   //-> 마지막 경로 다음 값을 -1로 수정하는 것 잊지 말기
	*/
}

// 2번 과제의 최다 획득 경로를 계산하고 해당 정보를 dq에 업데이트 하는 함수
void update_dq_2(void){
	/*
	code here!
	*/
	/*
	for ...
		dq[somthing] = something
	

	dq[end+1] = -1;   //-> 마지막 경로 다음 값을 -1로 수정하는 것 잊지 말기
	*/
}

// 다음 목표를 위한 회전 방향을 설정하는 함수 (1 : 위쪽 | 2: 오른쪽 | 3 : 아래쪽 | 4 : 왼쪽)
void calculate_direction(void){
	int dest = dq[dq_idx];

	// 현재 위치와 다음 목표의 행이 같은 경우
	if(get_loc_row(loc_cur) == get_loc_row(dest)){
		// 목표 위치가 현재 위치의 오른쪽에 있는 경우
		if(get_loc_col(dest) > get_loc_col(loc_cur)){
			dir_dest = 2;  		// 로봇의 목표 방향을 오른쪽으로 설정
			set_stat(MOVE,2); 	// 로봇의 움직임 상태를 회전 중으로 설정
			return;
		}
		// 목표 위치가 현재 위치의 왼쪽에 있는 경우
		if(get_loc_col(dest) < get_loc_col(loc_cur)){
			dir_dest = 4;  		// 로봇의 목표 방향을 오른쪽으로 설정
			set_stat(MOVE,2); 	// 로봇의 움직임 상태를 회전 중으로 설정
			return;
		}
		return;
	}

	// 현재 위치와 다음 목표의 열이 같은 경우
	if(get_loc_col(loc_cur) == get_loc_col(dest)){
		// 목표 위치가 현재 위치의 오른쪽에 있는 경우
		if(get_loc_row(dest) > get_loc_row(loc_cur)){
			dir_dest = 3;  		// 로봇의 목표 방향을 아래쪽으로 설정
			set_stat(MOVE,2); 	// 로봇의 움직임 상태를 회전 중으로 설정
			return;
		}
		// 목표 위치가 현재 위치의 왼쪽에 있는 경우
		if(get_loc_col(dest) < get_loc_col(loc_cur)){
			dir_dest = 1;  		// 로봇의 목표 방향을 위쪽으로 설정
			set_stat(MOVE,2); 	// 로봇의 움직임 상태를 회전 중으로 설정
			return;
		}
		return;
	}
}

// 현재 스텟에 따라 다음 행동을 지정하는 함수
void update_action(void){
	int detect = get_stat(DETECT);
	int move = get_stat(MOVE);

	if(get_stat(OFFROAD)){
		// 길을 미세하게 벗어났을 경우 calibration 하는 코드 - 단, 코드가 길어질 경우 함수로 따로 뺄 것
		/*
			code here!
		*/
		return;
	}

	if(move == 2){
		// 현재 회전 상태일 경우 현재 방향인 dir_cur 가 dir_dest와 같아지도록 회전 - 단, 코드가 길어질 경우 함수로 따로 뺄 것
		// 단, 현재 방향과 목표 회전 방향이 같을 경우 아무것도 하지 않고 리턴
		/*
			code here!
		*/
		set_stat(MOVE,1);		// 회전이 끝난 후 move를 직진 상태로 바꾸기
		move = get_stat(MOVE);
	}

	if(move == 1){
		set_motor(SPEED_MAX,SPEED_MAX);
		return;
	}

	

	if(!move){
		set_motor(0,0);

		// detect 가 4이고 move가 0이면 상황 종료
		if (detect == 4)
		{
			//상황 종료
		}
		return;
	}
}
