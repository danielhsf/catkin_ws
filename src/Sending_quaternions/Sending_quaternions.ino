#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

ros::NodeHandle  nh;

std_msgs::Float32MultiArray sensorimu;
ros::Publisher chatter("sensorimu", &sensorimu);

#define BNO055_SAMPLERATE_DELAY_MS (1000)

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup(void)
{
  nh.initNode();
  sensorimu.data_length = 4;  
  nh.advertise(chatter);
  bno.begin();
  delay(1000);
}

float values[] = {0, 0, 0, 0};

void loop(void)
{ 
  imu::Quaternion quat = bno.getQuat();
  values[0] = quat.w();
  values[1] = quat.x();
  values[2] = quat.y();
  values[3] = quat.z();
  sensorimu.data = values;
  chatter.publish(&sensorimu);
  nh.spinOnce();
  delay(1000);
}
