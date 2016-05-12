/*
   FLC05 water auto-sampler
   Clearpath Robotics - 2016
   Takes in int, (-8) -> +8.
   0 means do nothing
   -ve value means empty that pump (-2 for pump 2, etc
   +ve value means fill that pump (+4 for pump 4, etc)
   Returns system status:
   0 - Nominal
   1 - Flush
 * *** NOTE: System can only cycle one pump at a time
*/

#include <ros.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Byte.h>

int CW = 15; //Clockwise (Fill) rotation
int CCW = 14; //Counter-Clockwise (Empty) rotation
int Button = 8; //flush button
int LED = 10;
//int pumpPins[] = {0,2,7,4,16,17,5,3,6};
int pumpPins[] = {0, 16, 17, 2, 7, 6, 4, 5, 3};

ros::NodeHandle  nh;

std_msgs::Byte sample;
ros::Publisher sample_pub("samples", &sample);

void pumpMessage(const std_msgs::Int8& val) {
  for (int i = 1; i < 9; i++) {
    digitalWrite(pumpPins[i], LOW);
  }
  if (val.data != 0) {
    if (abs(val.data) > 8) {
      for (int i = 1; i < 9; i++) {
        digitalWrite(pumpPins[i], HIGH);
      }
    }
    else {
      digitalWrite(pumpPins[abs(val.data)], HIGH);
    }
    if (val.data > 0) {
      digitalWrite(CW, HIGH);
      digitalWrite(CCW, LOW);
    }
    else {
      digitalWrite(CCW, HIGH);
      digitalWrite(CW, LOW);
    }
  }
}

ros::Subscriber<std_msgs::Int8> pump_ctl("pump_ctl", pumpMessage );

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(CW, OUTPUT);
  digitalWrite(CW, LOW);
  pinMode(CCW, OUTPUT);
  digitalWrite(CCW, LOW);
  // ready pump pins and disable
  for (int i = 0; i < 8; i++) {
    pinMode(pumpPins[i], OUTPUT);
    digitalWrite(pumpPins[i], LOW);
  }
  nh.initNode();

  nh.advertise(sample_pub);
  nh.subscribe(pump_ctl);
}

void loop()
{
  // Flush Button
  sample.data = 0;
  if (digitalRead(Button) != HIGH) {
    sample.data = 1;
  }
  // heartbeat
  digitalWrite(LED, HIGH - digitalRead(LED));
  sample_pub.publish( &sample );
  nh.spinOnce();
  delay(25);
}
