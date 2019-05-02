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

// Header for Pass Through filter
#include <pcl/filters/passthrough.h>

//Header for Voxel Grid Filter
#include <pcl/filters/voxel_grid.h>

//Header file for wriiting PCD file
#include <pcl/io/pcd_io.h>

//Header file for Float32MultiArray
#include <std_msgs/Float32MultiArray.h>

//Header to make the Transform View
#include <pcl/common/transforms.h>

// Transform view sin,cos...
#include <cmath>

//Header for RANSAC
#include <pcl/filters/extract_indices.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>

class cloudHandler{
public:
    cloudHandler(){
        pcl_sub = nh.subscribe("/camera/depth_registered/points", 1, &cloudHandler::callback, this);
        pcl_pub = nh.advertise<sensor_msgs::PointCloud2>("pcl_filtered", 1);
    }


    void callback(const sensor_msgs::PointCloud2& input){
        pcl::PointCloud<pcl::PointXYZRGBA> cloud;
        pcl::PointCloud<pcl::PointXYZRGBA> cloud_filtered;
        sensor_msgs::PointCloud2 output;
        pcl::fromROSMsg(input, cloud);
        //Removing NaNs
        std::vector<int> indices;
        pcl::removeNaNFromPointCloud(cloud,cloud_filtered,indices);
        printf("Original Point Cloud Size = %d\n",cloud.width * cloud.height); 
        printf("After filtering nans = %d\n",cloud_filtered.width * cloud_filtered.height);
        pcl::toROSMsg(cloud_filtered, output);
    	output.header.frame_id = "point_cloud";
        pcl::io::savePCDFileASCII("filtrado.pcd", cloud_filtered);
        pcl_pub.publish(output);
        
    }

protected:
    ros::NodeHandle nh;
    ros::Subscriber pcl_sub;
    ros::Publisher pcl_pub;
};

main(int argc, char** argv)
{
    ros::init(argc, argv, "pcl_receiver");
    ROS_INFO("Started Filter Node");
    cloudHandler handler;
    ros::spin();
    return 0;
}
