// 센서 - 아마 순서 바뀌어야 함
#pragma config(Sensor, S1, cs_left, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S2, cs_middle, sensorEV3_Color, modeEV3Color_Color)
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
#define LOC_START 0 		// 로봇의 시작 위치 정의 - (row,column)좌표값을 MAP_SIZE_ROW * row  + column 로 나타낸다
#define DIR_START 2			// 로봇의 시작 방향 정의 - 1 : 위쪽 | 2: 오른쪽 | 3 : 아래쪽 | 4 : 왼쪽
#define LOC_DEST 15			// 도착점 좌표
#define LOC_MOV 15			// 이동점 좌표

//상태 변수 관련 정보 - 상태 변수는 stat 배열에 저장되며 아래는 각 스텟의 인덱스이다 | get_stat(상태변수명)으로 접근 가능 | set_stat(상태변수명, 값)으로 셋 가능
#define STATUS_SIZE 3		// 상태 변수 배열의 크기
#define DETECT 0			// 현재 디텍팅 상태가 저장된 인덱스 - stat[DETECT] = 0 : 시작 전 | stat[DETECT] = 1 : 격자를 순서대로 탐색 중 | stat[DETECT] = 2 : 탐색 완료 후 패치 경로를 탐색 중 | stat[DETECT] = 3 : 도착점에서 이동점으로 이동 중 | stat[DETECT] = 4 : 패치 경로에 따라 되돌아오기 진행 중
#define MOVE 1 				// 현재 로봇의 이동 상태가 저장된 인덱스 stat[MOVE] = 0 : 현재 로봇은 정지 상태  | stat[MOVE] = 1 : 현재 로봇은 직진 상태  | stat[MOVE] = 2 : 현재 로봇은 dir_dest을 향해 회전 중 상태
#define ONGRID 2			// 현재 로봇이 격자를 발견하였는가 stat[ONGRID] = 0 : 로봇은 선 위 | stat[ONGRID] = 1 : 로봇은 격자 위에

#define DEST_QUEUE_SIZE 20 	// destination queue 의 크기를 정의 -  destination queue는 원형 큐로 dq_idx 변수가 현재 가리키고 있는 destination queue 의 인덱스이다 | 자세한 설명은 문서에

int stat[STATUS_SIZE];
int dq[DEST_QUEUE_SIZE]; 	// destination queue를 정의한다 -원형 큐이다 | 자세한 설명은 문서에
int map[MAP_SIZE_ROW][MAP_SIZE_COL]; // 0 : 빈 공간 | 1: 빨간 색 | 2 : 파란 색

int dq_idx = 0;

int dir_cur = 2;			// 로봇의 현재 방향을 나타내는 변수  - 1 : 위쪽 | 2: 오른쪽 | 3 : 아래쪽 | 4 : 왼쪽
int dir_dest = 2;			// 로봇의 목표 방향을 나타내는 변수 - 회전 시 사용한다

int loc_cur = 0;			// 로봇의 현재 위치를 나타내는 변수 - (row,column)좌표값을 MAP_SIZE_COL * row  + column 로 나타낸다  | ex - 5 x 4 맵에서 로봇이 1행 2열에 있다면 loc_cur = 7

int score = 0;				// 현재 점수


void init_stat(void);
int get_stat(int index);
void set_stat(int index, int value);
void init_map(void);
void init_dq(void);
int get_loc_row(int loc);
int get_loc_col(int loc);
int get_loc(int x, int y);
char * dir_to_text(int dir);
void print_map(int startline);
void print_stat(void);

task main()
{
	init_stat();
	init_map();
	init_dq();
	sleep(5000);  // 5초동안 대기 후 출발

	while(1){

	}
}

// 로봇의 STATUS를 초기화
void init_stat(void){
	for(int i = 0; i<STATUS_SIZE; i++)
		stat[i] = 0;
	dir_cur = DIR_START; //로봇의 시작 시 방향 정의
	loc_cur = LOC_START;
	score = 0;
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

//destination queue 를 0으로 초기화한다
void init_dq(void){
	for(int i = 0; i< DEST_QUEUE_SIZE ; i++){
		dq[i] = 0;
	}
	dq_idx = 0;
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
	displayBigTextLine(1, "det %d mv %d score %d",get_stat(DETECT), get_stat(MOVE),score);
	displayBigTextLine(3, "cur [%d,%d] next [%d,%d]", get_loc_row(loc_cur), get_loc_col(loc_cur), get_loc_row(dq[dq_idx]), get_loc_col(dq[dq_idx]));
	displayBigTextLine(5, "cur %s dest %s",dir_to_text(dir_cur),dir_to_text(dir_dest));
	print_map(7);
}

