#include <Arduino.h>
#include "channels.h"
#include <AutoPID.h>
#include <Wire.h>
//////////////////////////////////
#define OUTPUT_PIN1 3   // 3 as 2 
#define OUTPUT_PIN2 4   // 4 as 3
#define OUTPUT_PIN3 6   // 6 as 5
#define OUTPUT_PIN4 7   // 7 as 6
#define OUTPUT_PIN5 12  // 12 as 7
#define OUTPUT_PIN6 13  // 13 as 8 
#define OUTPUT_PIN7 14  // 14 as 9
#define OUTPUT_PIN8 15  // 15 as 10
#define TEMP_READ_DELAY 800 //can only read digital temp sensor every ~750ms //800//1000
#define OUTPUT_MIN 0 //pid settings and gains
#define OUTPUT_MAX 255
#define KP 1.05  //.12   // 5.5  // 1.05 FOR30-80  // 0.85 for 90 to 100  // 2 as 1.05 //1.60 as 1.40//1.60 
#define KI 0.05  //.0003 // 0.5  // 0.5            // 0.5
#define KD 0.05  //0     // 0.5  // 0.5            // 0.5   // Kp-1.05 Ki-0.05 Kd-0.05 => for 30 to 100 for eight channel
/////////////////////////////////// Pins & Initialisation temperature sensor ntc & Variables decalaration
String de;
int V1, V2, V3, V4, V5, V6, V7, V8, T1 = A0, T2 = A1, T3 = A2, T4 = A3, T5 = A4, T6 = A5, T7 = A6, T8 = A7, OUTPUT_PIN;
float f, R1 = 10000, logR21, logR22, logR23, logR24, logR25, logR26 , logR27, logR28, R21, R22, R23, R24, R25, R26, R27, R28, IT1, IT2, IT3, IT4, IT5, IT6, IT7, IT8, I_Tc1, I_Tc2, I_Tc3, I_Tc4, I_Tc5, I_Tc6 , I_Tc7, I_Tc8, c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
double temperature_read = 0.0,temperature_read1 = 0.0, temperature_read2 = 0.0, temperature_read3 = 0.0, temperature_read4 = 0.0, temperature_read5 = 0.0, temperature_read6 = 0.0, temperature_read7 = 0.0, temperature_read8 = 0.0, outputVal, outputVal1, outputVal2, outputVal3, outputVal4, outputVal5, outputVal6, outputVal7, outputVal8;
/////// SETPOINT VALUE
double setPoint1 = 0, setPoint2 = 0, setPoint3 = 0, setPoint4 = 0, setPoint5 = 0, setPoint6 = 0, setPoint7 = 0, setPoint8 = 0;
/////////////////////////////////// Auto-PID-function ///////////////////////////////////////////////////////
AutoPID myPID1(&temperature_read1, &setPoint1, &outputVal1, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD); // Channel 1
AutoPID myPID2(&temperature_read2, &setPoint2, &outputVal2, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD); // Channel 2
AutoPID myPID3(&temperature_read3, &setPoint3, &outputVal3, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD); // Channel 3
AutoPID myPID4(&temperature_read4, &setPoint4, &outputVal4, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD); // Channel 4
AutoPID myPID5(&temperature_read5, &setPoint5, &outputVal5, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD); // Channel 5
AutoPID myPID6(&temperature_read6, &setPoint6, &outputVal6, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD); // Channel 6
AutoPID myPID7(&temperature_read7, &setPoint7, &outputVal7, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD); // Channel 7
AutoPID myPID8(&temperature_read8, &setPoint8, &outputVal8, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD); // Channel 8
/////////////////////////////////// Functions /////////////////////////////////////////////////////
void AutoPID_init() {
  pinMode(OUTPUT_PIN1, OUTPUT);
  pinMode(OUTPUT_PIN2, OUTPUT);
  pinMode(OUTPUT_PIN3, OUTPUT);
  pinMode(OUTPUT_PIN4, OUTPUT);
  pinMode(OUTPUT_PIN5, OUTPUT);
  pinMode(OUTPUT_PIN6, OUTPUT);
  pinMode(OUTPUT_PIN7, OUTPUT);
  pinMode(OUTPUT_PIN8, OUTPUT);
  Serial.begin(9600);
  Wire.begin(0x08);
  Wire.onReceive(receiveEvent); /* register receive event */
  Wire.onRequest(requestEvent); /* register reqsuest event */
  myPID1.setBangBang(0.5, -0.5); //if temperature is more than 4 degrees below or above setpoint, OUTPUT will be set to min or max respectively
  myPID1.setTimeStep(2000);      //set PID update interval to 4000ms //2000 2000/8=250
  myPID2.setBangBang(0.5, -0.5);
  myPID2.setTimeStep(2000);      // 3000 //3000/8=375
  myPID3.setBangBang(0.5, -0.5);
  myPID3.setTimeStep(2000);
  myPID4.setBangBang(0.5, -0.5);
  myPID4.setTimeStep(2000);
  myPID5.setBangBang(0.5, -0.5);
  myPID5.setTimeStep(2000);
  myPID6.setBangBang(0.5, -0.5);
  myPID6.setTimeStep(2000);
  myPID7.setBangBang(0.5, -0.5);
  myPID7.setTimeStep(2000);
  myPID8.setBangBang(0.5, -0.5);
  myPID8.setTimeStep(2000);
}
void receiveEvent(int numBytes)
{
 setPoint1=0;
  setPoint2=0;
  setPoint3=0;
  setPoint4=0;
  setPoint5=0;
  setPoint6=0;
  setPoint7=0;
  setPoint8=0;
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
        setPoint1 = atoi(str);
      else if (i == 2)
        setPoint2 = atoi(str);
      else if (i == 3)
        setPoint3 = atoi(str);
      else if (i == 4)
        setPoint4 = atoi(str);
      else if (i == 5)
        setPoint5 = atoi(str);
      else if (i == 6)
        setPoint6 = atoi(str);
      else if (i == 7)
        setPoint7 = atoi(str);
      else if (i == 8)
        setPoint8 = atoi(str);      
      i++;
    }
  }
  Serial.print(setPoint1);
  Serial.print(" ");
  Serial.print(setPoint2);
  Serial.print(" ");
  Serial.print(setPoint3);
  Serial.print(" ");
  Serial.print(setPoint4);
  Serial.print(" ");
  Serial.print(setPoint5);
  Serial.print(" ");
  Serial.print(setPoint6);
  Serial.print(" ");
  Serial.print(setPoint7);
  Serial.print(" ");
  Serial.println(setPoint8);
 
}
void channels(int k) {
  for (int i = 1; i <= k ; i++) {
    if (i == 1) {
      temperature_read1 = readT1();
      myPID1.run(); //call every loop, updates automatically at certain time interval
      OUTPUT_PIN = OUTPUT_PIN1;
      outputVal = outputVal1;
    }
    else if (i == 2) {
      temperature_read2 = readT2();
      myPID2.run(); //call every loop, updates automatically at certain time interval
      OUTPUT_PIN = OUTPUT_PIN2;
      outputVal = outputVal2;
    }
    else if (i == 3) {
      temperature_read3 = readT3();
      myPID3.run(); //call every loop, updates automatically at certain time interval
      OUTPUT_PIN = OUTPUT_PIN3;
      outputVal = outputVal3;
    }
    else if (i == 4) {
      temperature_read4 = readT4();
      myPID4.run(); //call every loop, updates automatically at certain time interval
      OUTPUT_PIN = OUTPUT_PIN4;
      outputVal = outputVal4;
    }
    else if (i == 5) {
      temperature_read5 = readT5();
      myPID5.run(); //call every loop, updates automatically at certain time interval
      OUTPUT_PIN = OUTPUT_PIN5;
      outputVal = outputVal5;
    }
    else if (i == 6) {
      temperature_read6 = readT6();
      myPID6.run(); //call every loop, updates automatically at certain time interval
      OUTPUT_PIN = OUTPUT_PIN6;
      outputVal = outputVal6;
    }
    else if (i == 7) {
      temperature_read7 = readT7();
      myPID7.run(); //call every loop, updates automatically at certain time interval
      OUTPUT_PIN = OUTPUT_PIN7;
      outputVal = outputVal7;
    }
    else if (i == 8) {
      temperature_read8 = readT8();
      myPID8.run(); //call every loop, updates automatically at certain time interval
      OUTPUT_PIN = OUTPUT_PIN8;
      outputVal = outputVal8;
    }
//    Serial.print(temperature_read);   
//    Serial.print(" ");
    analogWrite(OUTPUT_PIN, outputVal);
    delay(100);  //100 as 12.5
  }
  //  Serial.println();
  //  Serial.println("Endloop for every channels 1 to 8");
}
double readT1() {
  V1 = analogRead(T1);
  R21 = R1 * (1023.0 / (float)V1 - 1.0);
  logR21 = log(R21);
  IT1 = (1.0 / (c1 + c2 * logR21 + c3 * logR21 * logR21 * logR21));
  I_Tc1 = IT1 - 273.15;
  return I_Tc1;
}
double readT2() {
  V2 = analogRead(T2);
  R22 = R1 * (1023.0 / (float)V2 - 1.0);
  logR22 = log(R22);
  IT2 = (1.0 / (c1 + c2 * logR22 + c3 * logR22 * logR22 * logR22));
  I_Tc2 = IT2 - 273.15;
  return I_Tc2;
}
double readT3() {
  V3 = analogRead(T3);
  R23 = R1 * (1023.0 / (float)V3 - 1.0);
  logR23 = log(R23);
  IT3 = (1.0 / (c1 + c2 * logR23 + c3 * logR23 * logR23 * logR23));
  I_Tc3 = IT3 - 273.15;
  return I_Tc3;
}
double readT4() {
  V4 = analogRead(T4);
  R24 = R1 * (1023.0 / (float)V4 - 1.0);
  logR24 = log(R24);
  IT4 = (1.0 / (c1 + c2 * logR24 + c3 * logR24 * logR24 * logR24));
  I_Tc4 = IT4 - 273.15;
  return I_Tc4;
}
double readT5() {
  V5 = analogRead(T5);
  R25 = R1 * (1023.0 / (float)V5 - 1.0);
  logR25 = log(R25);
  IT5 = (1.0 / (c1 + c2 * logR25 + c3 * logR25 * logR25 * logR25));
  I_Tc5 = IT5 - 273.15;
  return I_Tc5;
}
double readT6() {
  V6 = analogRead(T6);
  R26 = R1 * (1023.0 / (float)V6 - 1.0);
  logR26 = log(R26);
  IT6 = (1.0 / (c1 + c2 * logR26 + c3 * logR26 * logR26 * logR26));
  I_Tc6 = IT6 - 273.15;
  return I_Tc6;
}
double readT7() {
  V7 = analogRead(T7);
  R27 = R1 * (1023.0 / (float)V7 - 1.0);
  logR27 = log(R27);
  IT7 = (1.0 / (c1 + c2 * logR27 + c3 * logR27 * logR27 * logR27));
  I_Tc7 = IT7 - 273.15;
  return I_Tc7;
}
double readT8() {
  V8 = analogRead(T8);
  R28 = R1 * (1023.0 / (float)V8 - 1.0);
  logR28 = log(R28);
  IT8 = (1.0 / (c1 + c2 * logR28 + c3 * logR28 * logR28 * logR28));
  I_Tc8 = IT8 - 273.15;
  return I_Tc8;
}
void requestEvent() {
  String s1 = String(temperature_read1, 0);
  String s2 = String(temperature_read2, 0);
  String s3 = String(temperature_read3, 0);
  String s4 = String(temperature_read4, 0);
  String s5 = String(temperature_read5, 0);
  String s6 = String(temperature_read6, 0);
  String s7 = String(temperature_read7, 0);
  String s8 = String(temperature_read8, 0);
  String all = s1 + " " + s2 + " " + s3 + " " + s4 + " " + s5 + " " + s6 + " " + s7 + " " + s8;
  char buf[30];
  all.toCharArray(buf, 30);
  Wire.write(buf, 30);
}
//
