// This will be the final code to be implemented

//Basic ROS package
#include <ros/ros.h>

// Point Cloud headers
#include <pcl/point_cloud.h>

// Header which contain PCL to ROS and ROS to pcl conversion functions
#include <pcl_conversions/pcl_conversions.h>

//sensor_msgs header for point cloud2
#include <sensor_msgs/PointCloud2.h>



class cloudHandler{
public:
    cloudHandler(){
        pcl_sub = nh.subscribe("/camera/depth_registered/points", 1, &cloudHandler::callback, this);
    }

    void callback(const sensor_msgs::PointCloud2& input){
        pcl::PointCloud<pcl::PointXYZRGBA> cloud;
        sensor_msgs::PointCloud2 output;
        pcl::fromROSMsg(input, cloud);
        pcl::io::savePCDFileASCII("point_cloud.pcd", cloud);
    }

protected:
    ros::NodeHandle nh;
    ros::Subscriber pcl_sub;
};

main(int argc, char** argv)
{
    ros::init(argc, argv, "pcl_filter");
    ROS_INFO("Started Filter Node");
    cloudHandler handler;
    ros::spin();
    return 0;
}
