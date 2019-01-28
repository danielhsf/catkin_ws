// This will be the final code to be implemented

//Basic ROS package
#include <ros/ros.h>

// Point Cloud headers
#include <pcl/point_cloud.h>

// Header which contain PCL to ROS and ROS to pcl conversion functions
#include <pcl_conversions/pcl_conversions.h>

//sensor_msgs header for point cloud2
#include <sensor_msgs/PointCloud2.h>

// Header for removing NaNs
#include <pcl/filters/filter.h>

//Header file for Float32MultiArray
#include <std_msgs/Float32MultiArray.h>

//Header to make the Transform View
#include <pcl/common/transforms.h>

//Header file for wriiting PCD file
#include <pcl/io/pcd_io.h>
 
#include <cmath>

main(int argc, char** argv) {
    ros::init(argc, argv, "pcl_filter");
    ROS_INFO("Started transform one Point Node");
    ros::NodeHandle nh;
    ros::Publisher pcl_pub = nh.advertise<sensor_msgs::PointCloud2>("pcl_filtered", 1);
    ros::Rate loop_rate(0.5);
    int count = 0;

    Eigen::Quaternionf orientation;
    orientation.w() = 0.965925826289;
    orientation.x() = 0.258819045103;
    orientation.y() = 0.0;
    orientation.z() = 0.0;

    Eigen::Matrix4f transform_1 = Eigen::Matrix4f::Identity();

    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::PointCloud<pcl::PointXYZ> cloud_out;
    sensor_msgs::PointCloud2 output;

    cloud.width = 1;
    cloud.height = 1;
    cloud.is_dense = false;
    cloud.points.resize(1);
    cloud.points[0].x = 0;
    cloud.points[0].y = 0;
    cloud.points[0].z = 1.1313708498984762;

    cloud.sensor_orientation_ = orientation;
    float roll;
    float pitch;
    float yaw;
    
    // roll (x-axis rotation)
    float sinr_cosp = +2.0 * (orientation.w() * orientation.x() + orientation.y() * orientation.z());
    float cosr_cosp = +1.0 - 2.0 * (orientation.x() * orientation.x() + orientation.y() * orientation.y());
    roll = atan2f(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = +2.0 * (orientation.w() * orientation.y() - orientation.z() * orientation.x());
    if (fabs(sinp) >= 1)
	    pitch = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
	    pitch = asin(sinp);
    // yaw (z-axis rotation)
    float siny_cosp = +2.0 * (orientation.w() * orientation.z() + orientation.x() * orientation.y());
    float cosy_cosp = +1.0 - 2.0 * (orientation.y() * orientation.y() + orientation.z() * orientation.z());  
    yaw = atan2f(siny_cosp, cosy_cosp);

    transform_1(0,0) = cos(pitch)*cos(yaw);
    transform_1(1,0) = cos(pitch)*sin(yaw);
    transform_1(2,0) = -sin(pitch);
    transform_1(0,1) = sin(roll)*sin(pitch)*cos(yaw) - cos(roll)*sin(yaw);
    transform_1(1,1) = sin(roll)*sin(pitch)*sin(yaw) + cos(roll)*cos(yaw);
    transform_1(2,1) = sin(roll)*cos(pitch);
    transform_1(0,2) = cos(roll)*sin(pitch)*cos(yaw) + sin(roll)*sin(yaw);
    transform_1(1,2) = cos(roll)*sin(pitch)*sin(yaw) - sin(roll)*cos(yaw);
    transform_1(2,2) = cos(roll)*cos(pitch);
    //transform_1(1,3) = 0.8;

    //roll = atan2f(2*(orientation.w()*orientation.x() + orientation.y()*orientation.z()),(1-2*(orientation.x()*orientation.x() + orientation.y()*orientation.y())));
    //pitch = asinf(2*(orientation.w()*orientation.y() - orientation.x()*orientation.z()));
    //yaw = atan2f(2*(orientation.w()*orientation.z() + orientation.x() + orientation.y()),(1-2*(orientation.y()*orientation.y() + orientation.z()*orientation.z())));

    pcl::transformPointCloud(cloud, cloud_out, transform_1);
    pcl::toROSMsg(cloud_out, output);
    pcl::io::savePCDFileASCII("transformado.pcd", cloud_out);

    pcl_pub.publish(output);
    ros::spinOnce(); 
    loop_rate.sleep(); 
    count++;
   return 0;
}
