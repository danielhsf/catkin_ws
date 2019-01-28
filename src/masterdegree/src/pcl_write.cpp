#include <ros/ros.h>
#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <sensor_msgs/PointCloud2.h>
//Header file for wriiting PCD file
#include <pcl/io/pcd_io.h>

void cloudCB(const sensor_msgs::PointCloud2 &input){
    pcl::PointCloud<pcl::PointXYZRGBA> cloud;
    pcl::fromROSMsg(input, cloud);
    //Save data as test.pcd file
    pcl::io::savePCDFileASCII("test.pcd", cloud);
}

main(int argc, char **argv){
    ros::init(argc, argv, "pcl_write");
    ROS_INFO("Started PCL write node");

    ros::NodeHandle nh;
    ros::Subscriber bat_sub = nh.subscribe("camera/depth_registered/points",10,cloudCB);
    ros::spin();
}
