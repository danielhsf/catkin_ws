//Basic ROS package
#include <ros/ros.h>

// Point Cloud headers
#include <pcl/point_cloud.h>

// Header which contain PCL to ROS and ROS to pcl conversion functions
#include <pcl_conversions/pcl_conversions.h>

//sensor_msgs header for point cloud2
#include <sensor_msgs/PointCloud2.h>

// Format msg to receive IMU quaternions 
#include <std_msgs/Float32MultiArray.h>

// Header to transform view
#include <pcl/common/transforms.h>

//Header file for wriiting PCD file
#include <pcl/io/pcd_io.h>

Eigen::Quaternionf orientation;

void orientationcallback(std_msgs::Float32MultiArray sensorimu){
orientation.w() = sensorimu.data[0];
orientation.y() = sensorimu.data[1];
orientation.x() = sensorimu.data[2];
orientation.z() = sensorimu.data[3];
}

void callback(const sensor_msgs::PointCloud2 &input){
  // Converting received point cloud to pcl
  pcl::PointCloud<pcl::PointXYZRGBA> cloud_in;
  pcl::fromROSMsg(input, cloud_in);
  //defining the sensor orientation
  cloud_in.sensor_orientation_ = orientation;
  //defining the new set of orientation
  Eigen::Matrix4f origin = Eigen::Matrix4f::Identity();
  //Eigen::Vector4f translation = Eigen::Vector4f::Zero ();
  //Eigen::Quaternionf origin = Eigen::Quaternionf::Identity ();
  pcl::PointCloud<pcl::PointXYZRGBA> cloud_out;
  pcl::transformPointCloud(cloud_in, cloud_out,origin);
  //Saving the outputs
  //pcl::io::savePCDFileASCII("cloud_original.pcd", cloud_in);
  //pcl::io::savePCDFileASCII("cloud_transform.pcd", cloud_out);
  
  //Creating a sensor_msg of point Cloud
  sensor_msgs::PointCloud2 output_in;
  sensor_msgs::PointCloud2 output_transform;
  pcl::toROSMsg(cloud_in, output_in);
  output_in.header.frame_id = "point_cloud_original";
  pcl::toROSMsg(cloud_out, output_transform);
  cloud_out.header.frame_id = "point_cloud_transform";

  ros::NodeHandle nj;
  ros::Publisher pcl_pub_original = nj.advertise<sensor_msgs::PointCloud2>("pcl_original",1);
  ros::NodeHandle nk;
  ros::Publisher pcl_pub_transform = nk.advertise<sensor_msgs::PointCloud2>("pcl_transform",1);
  
  ros::Rate loop_rate(1);

  //Publishing point cloud data
  pcl_pub_original.publish(output_in);
  pcl_pub_transform.publish(output_transform);
  ros::spinOnce();
  loop_rate.sleep();


}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "sub_pcl");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("/camera/depth_registered/points",1,callback);
  ros::NodeHandle ni;
  ros::Subscriber sub2 = ni.subscribe("/sensorimu", 1, orientationcallback);
  
  ros::spin();
}