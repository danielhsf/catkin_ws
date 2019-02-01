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

class cloudHandler{
public:
    cloudHandler(){
        ori = nh.subscribe("/sensorimu", 1, &cloudHandler::orientationcallback, this);
        transform_1 = Eigen::Matrix4f::Identity(); 
    }

    void orientationcallback(std_msgs::Float32MultiArray sensorimu){
        orientation.w() = sensorimu.data[0];
        orientation.x() = sensorimu.data[1];
        orientation.y() = sensorimu.data[2];
        orientation.z() = sensorimu.data[3];
        
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

        transform_1(0,0) = cos(pitch)*cos(yaw);
        transform_1(1,0) = cos(pitch)*sin(yaw);
        transform_1(2,0) = -sin(pitch);
        transform_1(0,1) = sin(roll)*sin(pitch)*cos(yaw) - cos(roll)*sin(yaw);
        transform_1(1,1) = sin(roll)*sin(pitch)*sin(yaw) + cos(roll)*cos(yaw);
        transform_1(2,1) = sin(roll)*cos(pitch);
        transform_1(0,2) = cos(roll)*sin(pitch)*cos(yaw) + sin(roll)*sin(yaw);
        transform_1(1,2) = cos(roll)*sin(pitch)*sin(yaw) - sin(roll)*cos(yaw);
        transform_1(2,2) = cos(roll)*cos(pitch);
        pcl::transformPointCloud(cloud,cloud,transform_1);
        printf("Nova Localização x = %f, y = %f, z = %f \n",cloud.points[0].x,cloud.points[0].y,cloud.points[0].z);

    }

protected:
    Eigen::Quaternionf orientation;
    Eigen::Matrix4f transform_1; 
    float roll;
    float pitch;
    float yaw;
    ros::NodeHandle nh;
    ros::Subscriber ori;
    
};

main(int argc, char** argv) {
    ros::init(argc, argv, "pcl_filter");
    ROS_INFO("Started Filter Node");
    cloudHandler handler;
    ros::spin();
    return 0;
}
