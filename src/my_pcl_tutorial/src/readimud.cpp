#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>

void orientationcallback(std_msgs::Float32MultiArray sensorimu){

printf ("Quaternions: qw = %f, qy = %f, qx = %f, qz = %f\n", sensorimu.data[0],sensorimu.data[1],sensorimu.data[2],sensorimu.data[3]);
}



int main(int argc, char** argv)
{
  ros::init(argc, argv, "read_imu");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("/sensorimu", 1, orientationcallback);
  ros::spin();
}
