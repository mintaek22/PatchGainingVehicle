
#define MAP_SIZE_COL 4
#define MAP_SIZE_ROW 4

int map[MAP_SIZE_ROW][MAP_SIZE_COL] = {
	{0,0,0,0},
	{0,1,0,2},
	{1,0,0,0},
	{2,1,0,1}
}


void print_map(){
	for(int i = 0; i<MAP_SIZE_ROW ; i++){
		for(int j = 0; j< MAP_SIZE_COL ; j++){

			//displayStringAt(startline+ i*2,"%s %s %s %s ", map[i][j] == 0 ? "+" :(map[i][j] == 1 ? "O" : "X"));
			displayStringAt(j*15,(MAP_SIZE_ROW-i)*15, map[i][j] == 0 ? "+" :(map[i][j] == 1 ? "O" : "X"));
		}
	}
}

void print_stat(){
	displayBigTextLine(1, "asdf");
	displayBigTextLine(3, "asdf]");
	displayBigTextLine(5, "asdf");
	print_map();
}
task main()
{
	print_stat();
	sleep(5000);


}
