#pragma config(Sensor, S1, c1, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4, c2, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S3, c3, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Motor, motorB, lm, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor, motorC, rm, tmotorEV3_Large, PIDControl, encoder)
#define DIFF 10

int nMotorSpeedSetting =30,vertex=0,count=0,row=0,val;
int li[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};

void print_map(){
   for(int i = 0; i< 4 ; i++){
     displayBigTextLine(2*i+1,"%d %d %d %d",li[i][0],li[i][1],li[i][2],li[i][3]);
   }
   sleep(10000);
}

void stopMotor()
{
   setMotorSpeed(lm,0);
   setMotorSpeed(rm,0);
}

void go()
{
   val=5;
   if(getColorName(c2)==4)
   {
      setMotorSpeed(lm,nMotorSpeedSetting - val);
      setMotorSpeed(rm,nMotorSpeedSetting + val);
   }
   else{
      setMotorSpeed(lm,nMotorSpeedSetting + val);
      setMotorSpeed(rm,nMotorSpeedSetting - val);
   }
   if(row%2==1 || row==4){
      if(getColorName(c1)==4)vertex++;
      else vertex = 0;
   }
   else{
      if(getColorName(c3)==4) vertex++;
      else vertex=0;
   }
   if(vertex == 1 && count!=4) {
     setMotorSpeed(lm,nMotorSpeedSetting);
     setMotorSpeed(rm,0);
     sleep(100);
     stopMotor();
     sleep(1000);
     int color =getColorName(c2);
     if(color == 5)
     {
        li[row][count]=1;
    }
      else if(color == 2)
      {
         li[row][count] = -1;
      }
     displayBigTextLine(2,"%d",li[row][count]);
     count++;
   }
}

void turnLeft()
{
   setMotorSpeed(lm,30);
   setMotorSpeed(rm,30);
   sleep(100);
   while(getColorName(c1)>4){
      setMotorSpeed(lm,-nMotorSpeedSetting*6/10);
      setMotorSpeed(rm,nMotorSpeedSetting*6/10);
      sleep(10);
   }
   while(getColorName(c2)>4){
      setMotorSpeed(lm,-nMotorSpeedSetting*6/10);
      setMotorSpeed(rm,nMotorSpeedSetting*6/10);
      sleep(10);
   }
   sleep(50);
   setMotorSpeed(lm,0);
   setMotorSpeed(rm,0);
   sleep(100);
}

void turnRight()
{
   setMotorSpeed(lm,30);
   setMotorSpeed(rm,30);
   sleep(100);
   while(getColorName(c3)>4){
      setMotorSpeed(lm,nMotorSpeedSetting*6/10);
      setMotorSpeed(rm,-nMotorSpeedSetting*6/10);
      sleep(10);
   }
   while(getColorName(c2)>4){
      setMotorSpeed(lm,nMotorSpeedSetting*6/10);
      setMotorSpeed(rm,-nMotorSpeedSetting*6/10);
      sleep(10);
   }
   sleep(50);
   setMotorSpeed(lm,0);
   setMotorSpeed(rm,0);
   sleep(100);
}


void completeSearch()
{
   while(true)
   {
      go();
      if (count == 4)
      {
         if(row==3) break;
         if(row%2 == 0)
         {
            setMotorSpeed(lm,35);
            setMotorSpeed(rm,30);
            sleep(400);
            turnRight();
         }
         else
         {
              setMotorSpeed(lm,30);
            setMotorSpeed(rm,35);
            sleep(400);
            turnLeft();
         }

         if(row%2==0)
         {
            while(getColorName(c3) == 6) go();
            setMotorSpeed(lm,35);
            setMotorSpeed(rm,30);
            sleep(400);
            turnRight();
         }
         else
         {
            while(getColorName(c1)==6)go();
            setMotorSpeed(lm,30);
            setMotorSpeed(rm,35);
            sleep(400);
            turnLeft();
         }
         setMotorSpeed(lm,-30);
         setMotorSpeed(rm,-30);
         sleep(900);
         setMotorSpeed(lm,30);
         setMotorSpeed(rm,30);
         sleep(300);
         row ++;
         count = 0;
      }
    }
}

task main(){
   completeSearch();
    stopMotor();
    print_map();
}
