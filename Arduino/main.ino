/*
���� ������ �ˣ�8��A0
������ ������ ������4��7
����������2
�����1��5
�����2��6
�����3��3

*/

const int person1 = 8; //trig��
const int person2 = A0; //echo��
const int trash1 = 4; //trig��
const int trash2 = 7; //echo��
float cm_p;//�˵ľ���
float cm_t;//�����ľ���
int x1;
int x2;
int x;//�Ƿ�����
int y;//�Ƿ�������
int minute_left;//��������������
int minute_right;//�ص����˵Ĳ��
int m; //��������ѭ��
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
int buzzer=2;//������-2
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
///////////////////��ʼ״̬ ������ر�//////////////////////////////////////
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
  if(cm_p<50 and cm_p>3){x1=1;}//x=1������״̬��x=0������״̬
  delay(1000);
  digitalWrite(person1,LOW);
  delayMicroseconds(2);
  digitalWrite(person1,HIGH);  
  delayMicroseconds(10);  
  digitalWrite(person1,LOW);    
  cm_p = pulseIn(person2,HIGH) / 58.0;
  if(cm_p<50 and cm_p>3){x2=1;}//x=1������״̬��x=0������״̬
  if(x1==1 and x2==1){x=1;}
  else{x=0;}
  //Serial.println(x);
  delay(300);

//////////////////////////////////////////////////////////////////////////////////////////////////////
  if(x==1){
    Serial.write(1);//�����ˣ��������ź�1;������ʶ��;�򿪶�ά��ʶ��
///////////////////////////////�����������////////////////////////////////////////////////////////////////////////
    digitalWrite(trash1,LOW);
    delayMicroseconds(2);
    digitalWrite(trash1,HIGH);  
    delayMicroseconds(10);  
    digitalWrite(trash1,LOW);    
    cm_t = pulseIn(trash2,HIGH) / 58.0;
    //Serial.print(cm_t);
    if(cm_t<23 and cm_t>0){y=1;}//y=1��������״̬��y=0��������״̬
//////////////////////////////������////////////////////////////////
    if(y==1){
 
     
////////////////////////////������ȷ������//////////////////////////
       if(Serial.available()>0){
           while(Serial.available()>0){
           Serial.read();
           }
          do{
             myservo3.write(0);//0��ȷ 90����
             pos1=pos1+1;
             pos2=pos2-1;
             myservo1.write(pos1);
             myservo2.write(pos2);
             delay(60); 
             }while(pos1 <= 180 and a==0);
          delay(1000);
          myservo1.write(180);
          }
////////////////////////////��������������//////////////////////////
       else{minute_right = millis();
                    do{//do�ڵĲ����Ƿ�������������
                          digitalWrite(buzzer,HIGH);//������
                          delay(4000);
                          do{myservo3.write(90);//0��ȷ 90����
                             pos1=pos1+1;
                             pos2=pos2-1;
                             myservo1.write(pos1);
                             myservo2.write(pos2);
                             delay(60); 
                             }while(pos1 <= 180 and a==0);
                          delay(1000);
                          myservo1.write(180); 
                      }while ((millis() - minute_right) > 60000 );    //�ж�������״̬�Ƿ�ά��һ������
                    
            }
            }
///////////////////////////////////������x=1&y=0�����///////////////////////////////////////////
   /*  else{
        minute_right = millis();
            for(int m = 0;m < 1000;m=m+0){
              if((millis()-minute_right)<30000) break;
              }
            }
   */
  }
////////////////////////////////////������x=0�����////////////////////////////////
  else{
       myservo1.write(156);
       myservo2.write(24);
       myservo3.write(0);
       }

}
