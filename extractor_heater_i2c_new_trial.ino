#include <Wire.h>

int T = 0;
int TR = 0;
int pwm = 0;
int settemp = 0;
int t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0;
///////////////////////////->PWM PINS
int pwmt1 = 3; //3 as 4
int pwmt2 = 4;
int pwmt3 = 6;
int pwmt4 = 7;
int pwmt5 = 12;
int pwmt6 = 13;
int pwmt7 = 14;
int pwmt8 = 15;
//////////////////////////->NTC VARIABLES INITIALIZING
int V0;
float R1 = 10000;
float logR2, R2, IT, RT, I_Tc, R_Tc, R_Tf, T1, T2, T3, T4, T5, T6, T7, T8;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
int pwm_val = 0;

String de;

void setup()
{
  pinMode(pwmt1, OUTPUT);
  pinMode(pwmt2, OUTPUT);
  pinMode(pwmt3, OUTPUT);
  pinMode(pwmt4, OUTPUT);
  pinMode(pwmt5, OUTPUT);
  pinMode(pwmt6, OUTPUT);
  pinMode(pwmt7, OUTPUT);
  pinMode(pwmt8, OUTPUT);
  Serial.begin(9600);
  Wire.begin(0x08);
  Wire.onReceive(receiveEvent); /* register receive event */
  Wire.onRequest(requestEvent); /* register reqsuest event */
}

void loop()
{
  delay(30);
  tempset();
  //requestEvent();
}

void receiveEvent(int numBytes)
{
  t1=0;
  t2=0;
  t3=0;
  t4=0;
  t5=0;
  t6=0;
  t7=0;
  t8=0;
  unsigned char rc[30] = "";
  int count = 0;
  while (Wire.available())
  {
    char c = Wire.read();
    rc[count] = c;
    count++;
  }
  de = rc;
  Serial.println(de);
  delay(100);
  if (de.substring(0, 1) == "W")
  {
    char buf[30];
    de.toCharArray(buf, 30);
    char *p = buf;
    char *str;
    int i = 0;
    while ((str = strtok_r(p, " ", &p)) != NULL) // delimiter is the space
    {
      if (i == 1)
        t1 = atoi(str);
      else if (i == 2)
        t2 = atoi(str);
      else if (i == 3)
        t3 = atoi(str);
      else if (i == 4)
        t4 = atoi(str);
      else if (i == 5)
        t5 = atoi(str);
      else if (i == 6)
        t6 = atoi(str);
      else if (i == 7)
        t7 = atoi(str);
      else if (i == 8)
        t8 = atoi(str);      
      i++;
    }
  }
  Serial.print(t1);
  Serial.print(" ");
  Serial.print(t2);
  Serial.print(" ");
  Serial.print(t3);
  Serial.print(" ");
  Serial.print(t4);
  Serial.print(" ");
  Serial.print(t5);
  Serial.print(" ");
  Serial.print(t6);
  Serial.print(" ");
  Serial.print(t7);
  Serial.print(" ");
  Serial.println(t8);



}

void tempset()
{
  for (int t = 0; t < 8; t++)
  {
    if (t == 0)
    {
      T = A0;
      pwm = pwmt1;
      settemp = t1;
    }
    else if (t == 1)
    {
      T = A1;
      pwm = pwmt2;
      settemp = t2;
    }
    else if (t == 2)
    {
      T = A2;
      pwm = pwmt3;
      settemp = t3;
    }
    else if (t == 3)
    {
      T = A3;
      pwm = pwmt4;
      settemp = t4;
    }
    else if (t == 4)
    {
      T = A4;
      pwm = pwmt5;
      settemp = t5;
    }
    else if (t == 5)
    {
      T = A5;
      pwm = pwmt6;
      settemp = t6;
    }
    else if (t == 6)
    {
      T = A6;
      pwm = pwmt7;
      settemp = t7;
    }
    else if (t == 7)
    {
      T = A7;
      pwm = pwmt8;
      settemp = t8;
    }
    //Serial.println(T);
    //Serial.println(pwm);
    //Serial.println(settemp);
    V0 = analogRead(T);
    R2 = R1 * (1023.0 / (float)V0 - 1.0);
    logR2 = log(R2);
    IT = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
    I_Tc = IT - 273.15;
    if (t == 0)
      T1 = I_Tc;
    else if (t == 1)
      T2 = I_Tc;
    else if (t == 2)
      T3 = I_Tc;
    else if (t == 3)
      T4 = I_Tc;
    else if (t == 4)
      T5 = I_Tc;
    else if (t == 5)
      T6 = I_Tc;
    else if (t == 6)
      T7 = I_Tc;
    else if (t == 7)
      T8 = I_Tc;
    //Serial.print(settemp);
    //Serial.print(" ");
    //if(t==7)
    //Serial.println(settemp);
    if (settemp >= 80 && settemp < 100)
    {
      if (I_Tc < (settemp - (settemp / 100) * 20))
      {
        pwm_val = 110; //250 as 125 // 120 // 110
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 10))
      {
        pwm_val = 60; // 150 as 75 // 70 // 60
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 5))
      {
        pwm_val = 52.5; // 135 as 67.5 // 62.5 //52.5
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 4))
      {
        pwm_val = 45; // 120 as 60 // 55 //45
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 3))
      {
        pwm_val = 40; // 110 as 55 //50 //40
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 2))
      {
        pwm_val = 35; // 100 as 50 // 45 //35
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 1))
      {
        pwm_val = 30;  // 90 as 45 // 40 //30
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 0.5))
      {
        pwm_val = 27.5; // 85 as 42.5 // 37.5// 27.5
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 0.2))
      {
        pwm_val = 20; // 70 as 35 //30//22.5//20
        analogWrite(pwm, pwm_val);
      }
      else
      {
        pwm_val = 0; //50 as 25
        analogWrite(pwm, pwm_val);
      }
    }
    else if (settemp >= 60 && settemp < 80)
    {
      if (I_Tc < (settemp - (settemp / 100) * 20))
      {
        pwm_val = 67.5; //135 as 67.5
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 10))
      {
        pwm_val = 40; //80 as 40
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 5))
      {
        pwm_val = 32.5; //65 as 32.5
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 4))
      {
        pwm_val = 30; // 60 as 30
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 3))
      {
        pwm_val = 27.5; //55 as 27.5
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 2))
      {
        pwm_val = 25; //50 as 25
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 1))
      {
        pwm_val = 22.5; //45 as 22.5
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 0.5))
      {
        pwm_val = 20; //40 as 20
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 0.2))
      {
        pwm_val = 17.5; //35 as 17.5
        analogWrite(pwm, pwm_val);
      }
      else
      {
        pwm_val = 0;
        analogWrite(pwm, pwm_val);
      }
    }
    else if (settemp >= 40 && settemp < 60)
    {
      if (I_Tc < (settemp - (settemp / 100) * 20))
      {
        pwm_val = 60; //120 as 100 // 80 // 70 //60
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 10))
      {
        pwm_val = 30; //70 as 60 // 50 // 40 //30
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 5))
      {
        pwm_val = 20; //60 as 50 // 40 // 30 //20
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 4))
      {
        pwm_val = 15; //55 as 45 // 35 // 25 //15
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 3))
      {
        pwm_val = 10; //50 as 40 // 30 //20 //10
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 2))
      {
        pwm_val = 7.5; // 45 as 35 // 25 // 15 //7.5
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 1))
      {
        pwm_val = 5; //40 as 30 // 20 // 10 //5
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 0.5))
      {
        pwm_val = 3.75; //35 as 25 // 15 // 7.5 //3.75
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 0.2))
      {
        pwm_val = 2.5;//30 as 20 // 10 // 5 //2.5
        analogWrite(pwm, pwm_val);
      }
      else
      {
        pwm_val = 0;
        analogWrite(pwm, pwm_val);
      }
    }
    else if (settemp >= 20 && settemp < 40)
    {
      if (I_Tc < (settemp - (settemp / 100) * 20))
      {
        pwm_val = 49; //50//80//90//100//110
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 10))
      {
        pwm_val = 29; //29//30//40//50//60
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 5))
      {
        pwm_val = 24; //24//25//35//40//45
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 4))
      {
        pwm_val = 19; //19//20//30//35//40
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 3))
      {
        pwm_val = 14; //14//15//25//30//35
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 2))
      {
        pwm_val = 9; //9//10//20//25//30
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 1))
      {
        pwm_val = 4; //4//5//15//20//25
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 0.5))
      {
        pwm_val = 2.5; //2.5//3.5//10//15//20
        analogWrite(pwm, pwm_val);
      }
      else if (I_Tc < (settemp - (settemp / 100) * 0.2))
      {
        pwm_val = 1; //1//2//5//10//15
        analogWrite(pwm, pwm_val);
      }
      else
      {
        pwm_val = 0;
        analogWrite(pwm, pwm_val);
      }
    }
    else
    {
      pwm_val = 0;
      analogWrite(pwm, pwm_val);
    }
  }


}

void requestEvent()
{
  /*for (int k = 0; k < 8; k++)
    { if (k == 0)
      TR = A0;
    else if (k == 1)
      TR = A1;
    else if (k == 2)
      TR = A2;
    else if (k == 3)
      TR = A3;
    else if (k == 4)
      TR = A4;
    else if (k == 5)
      TR = A5;
    else if (k == 6)
      TR = A6;
    else if (k == 7)
      TR = A7;

    //    V0 = 0;
    //    R2 = 0;
    //    logR2 = 0;
    //    IT = 0;
    //    T1=T2=T3=T4=T5=T6=T7=T8=0;
    V0 = analogRead(TR);
    R2 = R1 * (1023.0 / (float)V0 - 1.0);
    logR2 = log(R2);
    IT = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
    if (k == 0)
      T1 = IT - 273.15;
    else if (k == 1)
      T2 = IT - 273.15;
    else if (k == 2)
      T3 = IT - 273.15;
    else if (k == 3)
      T4 = IT - 273.15;
    else if (k == 4)
      T5 = IT - 273.15;
    else if (k == 5)
      T6 = IT - 273.15;
    else if (k == 6)
      T7 = IT - 273.15;
    else if (k == 7)
      T8 = IT - 273.15;
    }*/
  String s1 = String(T1, 0);
  String s2 = String(T2, 0);
  String s3 = String(T3, 0);
  String s4 = String(T4, 0);
  String s5 = String(T5, 0);
  String s6 = String(T6, 0);
  String s7 = String(T7, 0);
  String s8 = String(T8, 0);
  String all = s1 + " " + s2 + " " + s3 + " " + s4 + " " + s5 + " " + s6 + " " + s7 + " " + s8;
  char buf[30];
  all.toCharArray(buf, 30);
  Wire.write(buf, 30);
  //Serial.println(all);
  //Serial.println(T1);
}
