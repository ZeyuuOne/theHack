/*
【人 超声】 人：8；A0
【垃圾 超声】 垃圾：4；7
【蜂鸣器】2
【舵机1】5
【舵机2】6
【舵机3】3

*/

const int person1 = 8; //trig人
const int person2 = A0; //echo人
const int trash1 = 4; //trig人
const int trash2 = 7; //echo人
float cm_p;//人的距离
float cm_t;//垃圾的距离
int x1;
int x2;
int x;//是否有人
int y;//是否有垃圾
int minute_left;//引发错误垃圾箱
int minute_right;//回到对人的测距
int m; //用于无限循环
//////////////////////////////////////////////
#include <Servo.h>
Servo myservo1; 
int pos1;   
Servo myservo2;  
int pos2;   
Servo myservo3;  
int pos3;    
int a=0;
/////////////////////
int buzzer=2;//蜂鸣器-2
/////////////////////
int renlian;



void setup() {
  pinMode(person1, OUTPUT);   
  pinMode(person2, INPUT);
  pinMode(trash1, OUTPUT);   
  pinMode(trash2, INPUT);   
  x=0;
  x1=0;
  x2=0;
  y=0;

  //myservo1.attach(5); 
  myservo2.attach(6); 
  myservo3.attach(3);
  pos1=156;
  pos2=24;
  pos3=0;

  pinMode(buzzer,OUTPUT);

  Serial.begin(9600);
} 




void loop() {
///////////////////初始状态 垃圾箱关闭//////////////////////////////////////
  myservo1.write(156);
  //myservo2.write(24);
  myservo3.write(0);
  x=0;
  x1=0;
  x2=0;
  y=0;
///////////////////////////////////////////////////////////////////////////
  digitalWrite(person1,LOW);
  delayMicroseconds(2);
  digitalWrite(person1,HIGH);  
  delayMicroseconds(10);  
  digitalWrite(person1,LOW);    
  cm_p = pulseIn(person2,HIGH) / 58.0;
  if(cm_p<50 and cm_p>3){x1=1;}//x=1是有人状态；x=0是无人状态
  delay(1000);
  digitalWrite(person1,LOW);
  delayMicroseconds(2);
  digitalWrite(person1,HIGH);  
  delayMicroseconds(10);  
  digitalWrite(person1,LOW);    
  cm_p = pulseIn(person2,HIGH) / 58.0;
  if(cm_p<50 and cm_p>3){x2=1;}//x=1是有人状态；x=0是无人状态
  if(x1==1 and x2==1){x=1;}
  else{x=0;}
  //Serial.println(x);
  delay(300);

//////////////////////////////////////////////////////////////////////////////////////////////////////
  if(x==1){
    Serial.write(1);//人来了，给电脑信号1;打开人脸识别;打开二维码识别；
///////////////////////////////超声垃圾检测////////////////////////////////////////////////////////////////////////
    digitalWrite(trash1,LOW);
    delayMicroseconds(2);
    digitalWrite(trash1,HIGH);  
    delayMicroseconds(10);  
    digitalWrite(trash1,LOW);    
    cm_t = pulseIn(trash2,HIGH) / 58.0;
    //Serial.print(cm_t);
    if(cm_t<23 and cm_t>0){y=1;}//y=1是有垃圾状态；y=0是无垃圾状态
//////////////////////////////有垃圾////////////////////////////////
    if(y==1){
 
     
////////////////////////////触发正确垃圾箱//////////////////////////
       if(Serial.available()>0){
           while(Serial.available()>0){
           Serial.read();
           }
          do{
             myservo3.write(0);//0正确 90错误
             pos1=pos1+1;
             pos2=pos2-1;
             myservo1.write(pos1);
             myservo2.write(pos2);
             delay(60); 
             }while(pos1 <= 180 and a==0);
          delay(1000);
          myservo1.write(180);
          }
////////////////////////////触发错误垃圾箱//////////////////////////
       else{minute_right = millis();
                    do{//do内的部分是蜂鸣器发出声响
                          digitalWrite(buzzer,HIGH);//发声音
                          delay(4000);
                          do{myservo3.write(90);//0正确 90错误
                             pos1=pos1+1;
                             pos2=pos2-1;
                             myservo1.write(pos1);
                             myservo2.write(pos2);
                             delay(60); 
                             }while(pos1 <= 180 and a==0);
                          delay(1000);
                          myservo1.write(180); 
                      }while ((millis() - minute_right) > 60000 );    //判断有垃圾状态是否维持一分钟了
                    
            }
            }
///////////////////////////////////以下是x=1&y=0的情况///////////////////////////////////////////
   /*  else{
        minute_right = millis();
            for(int m = 0;m < 1000;m=m+0){
              if((millis()-minute_right)<30000) break;
              }
            }
   */
  }
////////////////////////////////////以下是x=0的情况////////////////////////////////
  else{
       myservo1.write(156);
       myservo2.write(24);
       myservo3.write(0);
       }

}
