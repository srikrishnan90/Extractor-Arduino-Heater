#include "channels.h"

void setup() 
{
  AutoPID_init();
}

void loop() 
{
  delay(30);
  channels(8);

}
