#include <ros.h>
#include <ros/time.h>
#include <tf/transform_broadcaster.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

ros::NodeHandle  nh;

geometry_msgs::TransformStamped t;
tf::TransformBroadcaster broadcaster;

char base_link[] = "/camera_link";
char odom[] = "/tf";

#define BNO055_SAMPLERATE_DELAY_MS (1000)

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup()
{
  nh.initNode();
  broadcaster.init(nh);
  bno.begin();
  delay(1000);
}

void loop()
{  
  t.header.frame_id = odom;
  t.child_frame_id = base_link;
  t.transform.translation.x = 1.0; 
  imu::Quaternion quat = bno.getQuat();
  t.transform.rotation.x = quat.x();
  t.transform.rotation.y = quat.y();
  t.transform.rotation.z = quat.z(); 
  t.transform.rotation.w = quat.w(); 
  t.header.stamp = nh.now();
  broadcaster.sendTransform(t);
  nh.spinOnce();
  delay(10);
}
