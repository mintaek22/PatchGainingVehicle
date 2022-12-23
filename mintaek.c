#pragma config(Sensor, S1, ts, sensorEV3_Touch)
#pragma config(Sensor, S2, gs, sensorEV3_Gyro)
#pragma config(Sensor, S3, cs, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4, ss, sensorEV3_Ultrasonic)
#pragma config(Motor, motorB, lm, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor, motorC, rm, tmotorEV3_Large, PIDControl, encoder)


#define Black 1
#define Blue 2
#define Green 3
#define Yellow 4
#define Red 5
#define White 6
#define Brown 7


int color, before_color, dist, finish_line, speed = 10, start_val = 0, cnt = 0;

float t;

//거리를 변수로 받아 회전해야할 엔코더 값 리터
int convert(float dist)
{
   return (int)(360.0 * dist / 17.58);
}

//s의 속도로 전진
void go(int s)
{
   setMotorSpeed(lm, s);
   setMotorSpeed(rm, s);
}

//자이로 센서를 이용한 90도 포인트턴(True:시계방향,False는 반시계)
void turn(bool dir){
	int initval = getGyroDegrees(gs);
	int degree = 0;
	//right
	if(dir){
		while(degree<90){
			setMotorSpeed(lm,speed/2);
			setMotorSpeed(rm,-speed/2);
			degree = getGyroDegrees(gs)-initval;
		}
	}
	//left
	else{
		while(degree>-90){
			setMotorSpeed(lm,-speed/2);
			setMotorSpeed(rm,speed/2);
			degree = getGyroDegrees(gs)-initval;
		}
	}
}

//회전해야할 엔코더값만큼 전진 or 후진
void go_encoder(bool dir,int enc_degree){

	resetMotorEncoder(lm);
	resetMotorEncoder(rm);

	//straight
	if(dir){
		setMotorTarget(lm,enc_degree,speed/2);
		setMotorTarget(rm,enc_degree,speed/2);
	}
	else{
		setMotorTarget(lm,enc_degree,-speed/2);
		setMotorTarget(rm,enc_degree,-speed/2);
	}
	waitUntilMotorStop(lm);
}

task main()
{

  //터치센서를 누르면서 시작
   while(getTouchValue(ts) == 0){}
   while(getTouchValue(ts) == 1){}


   color = getColorName(cs);

   while(1)
   {
      dist = getUSDistance(ss);
      go(speed);
      before_color = color;


    //장애물과의 거리가 20cm 미만일때
     if(dist<20){
        playSound(soundBeepBeep);
        while(1){
        	dist = getUSDistance(ss);
        	go(speed/2);
			//장애물과의 거리가 4cm 미만일때
       		if(dist<4){
       			go(0);
				go_encoder(false,convert(20));
				turn(true);
				go_encoder(true,convert(10*cnt));
				turn(true);
				break;
					}
				}
				sleep(500);
				color=White;
      }



      //전에 인식한 칼라 값이랑 현재 칼라값이 다르면
      if(getColorName(cs) != before_color){
		 
		 //최대한 라인 중앙에 가서 확실하게 칼라 인식을 할수있게 0.175초만큼 이동
      	 sleep(175);
      	 go(0);
         sleep(500);
		 
		 //멈추고 칼라 인식을 받는다(움직이면 컬러인식이 정확하지 않을수도 있기 때문에)
         color = getColorName(cs);

		//처음으로 흰색이 아닌 다른 칼라를 만났을때(시작지점)
         if(!start_val && (color != White))
         {
			//타이머를 시작한다
            clearTimer(T1);

			//시작지점과 끝지점의 색깔은 같으므로 변수로 지정
            finish_line = color;

			//led초록색불빛
            setLEDColor(ledGreen);
            go(speed);
            
			//라인을 빠져나오게 하기위해 0.5초 sleep
			sleep(500);
            start_val++;
            color = getColorName(cs);
         }

		//Black
         if(color == Black){
			
			//검은색 라인을 밟은 수
            cnt++;
            sleep(500);
         }

		//Blue
         else if(color == Blue){
           	if(cnt == 0){
           		 playSound(soundBeepBeep);
           	}
			
			//검은색 라인 밟은 수 만큼 소리
           	else{
           		for(int i=0;i<cnt;i++){
           			playSound(soundBeepBeep);
           		}
           	}
            go(speed);
            sleep(500);
         }

         //Red
         else if(color == Red){

			//반시계 포인트턴
           	turn(false);
			//검은색 라인 밝은수 *10만큼 전진
			go_encoder(true,convert(10*cnt));
			turn(false);
            sleep(500);
       }
   		//finish
       if((color == finish_line) || (cnt == 10)){
		  go(0);
          t = time1[T1];

		  //끝난지점 까지 걸린 시간 출력
          displayBigTextLine(1, "record = %.2f sec", t/1000);
          sleep(5000);
          break;
       }
     }
   }
}
