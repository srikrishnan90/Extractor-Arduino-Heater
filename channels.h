#ifndef CHANNELS_H
#define CHANNELS_H
#include <Arduino.h>

void AutoPID_init();

void channels(int k);

double readT1();
double readT2();
double readT3();
double readT4();
double readT5();
double readT6();
double readT7();
double readT8();  

void receiveEvent(int numBytes);

void requestEvent();

#endif
