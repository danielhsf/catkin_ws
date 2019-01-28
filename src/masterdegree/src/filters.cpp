

#include <ros/ros.h>
#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <sensor_msgs/PointCloud2.h>

void callback(const sensor_msgs::PointCloud2 &input){
  printf("Received\n");

}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "sub_pcl");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("camera/depth_registered/points",1,callback);
  ros::spin();
}