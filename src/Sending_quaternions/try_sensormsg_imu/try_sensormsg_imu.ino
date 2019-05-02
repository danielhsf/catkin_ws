#include <ros.h>
#include <sensor_msgs/Imu.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

ros::NodeHandle  nh;

sensor_msgs::Imu sensorimu;
ros::Publisher chatter("sensorimu", &sensorimu);

#define BNO055_SAMPLERATE_DELAY_MS (1000)

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup(void)
{
  nh.initNode(); 
  nh.advertise(chatter);
  bno.begin();
  delay(1000);
}

float values[] = {0, 0, 0, 0};
int x = 0;
void loop(void)
{ 
  sensorimu.header.seq = x;
  x = x+1;
  sensorimu.header.stamp = nh.now();
  sensorimu.header.frame_id = "imu";
  imu::Quaternion quat = bno.getQuat();
  sensorimu.orientation.w = quat.w();
  sensorimu.orientation.x = quat.x();
  sensorimu.orientation.y = quat.y();
  sensorimu.orientation.z = quat.z();
  chatter.publish(&sensorimu);
  nh.spinOnce();
  delay(10);
}
