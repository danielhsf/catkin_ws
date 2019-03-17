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
        ori = nh.subscribe("/sensorimu", 1, &cloudHandler::orientationcallback, this);
        pcl_sub = nh.subscribe("/camera/depth_registered/points", 1, &cloudHandler::callback, this);
        pcl_pub = nh.advertise<sensor_msgs::PointCloud2>("pcl_filtered", 1);
        
        transform_0 = Eigen::Matrix4f::Zero();
        transform_0(0,0) = -1; 
        transform_0(1,2) = -1; 
        transform_0(2,1) = -1;
        transform_0(3,3) = 1; 
        transform_1 = Eigen::Matrix4f::Identity();
        transform_1(2,3) = 0.8;
        transform_2 = Eigen::Matrix4f::Identity();
        //transform_2(1,1) = -1;
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
        //pitch = 0;
        // yaw (z-axis rotation)
        //float siny_cosp = +2.0 * (orientation.w() * orientation.z() + orientation.x() * orientation.y());
        //float cosy_cosp = +1.0 - 2.0 * (orientation.y() * orientation.y() + orientation.z() * orientation.z());  
        //yaw = atan2f(siny_cosp, cosy_cosp);
        //roll = roll;
        //pitch = pitch;
        //yaw = - yaw;
        yaw = 0;
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
        //PassThrough Filter
        
        //printf("After Pass Through Filter = %d\n",cloud_filtered.width * cloud_filtered.height);
        //x
        pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloudPTR(new pcl::PointCloud<pcl::PointXYZRGBA>);
        pcl::PassThrough<pcl::PointXYZRGBA> pass;
        cloudPTR = cloud_filtered.makeShared();
        pass.setInputCloud(cloudPTR);
        pass.setFilterFieldName("x");
        pass.setFilterLimits(-0.3,0.3);
        pass.filter(cloud_filtered);
        printf("After Pass Through Filter = %d\n",cloud_filtered.width * cloud_filtered.height);
        
        //Voxel Grid
        pcl::VoxelGrid<pcl::PointXYZRGBA> sor;
        cloudPTR = cloud_filtered.makeShared();
        sor.setInputCloud(cloudPTR);
        sor.setLeafSize(0.01f, 0.01f, 0.01f);
        sor.filter(cloud_filtered);
        printf("After Voxel Grid Filter = %d\n",cloud_filtered.width * cloud_filtered.height);
        //Transform View
        transform_1(0,0) = cos(pitch)*cos(yaw);
        transform_1(1,0) = cos(pitch)*sin(yaw);
        transform_1(2,0) = -sin(pitch);
        transform_1(0,1) = sin(roll)*sin(pitch)*cos(yaw) - cos(roll)*sin(yaw);
        transform_1(1,1) = sin(roll)*sin(pitch)*sin(yaw) + cos(roll)*cos(yaw);
        transform_1(2,1) = sin(roll)*cos(pitch);
        transform_1(0,2) = cos(roll)*sin(pitch)*cos(yaw) + sin(roll)*sin(yaw);
        transform_1(1,2) = cos(roll)*sin(pitch)*sin(yaw) - sin(roll)*cos(yaw);
        transform_1(2,2) = cos(roll)*cos(pitch);
        printf("Roll = %f, Pitch = %f , yaw = %f \n", roll*180/M_PI, pitch*180/M_PI, yaw*180/M_PI);
        //cloud_filtered.sensor_orientation_ = orientation;
        //pcl::PointCloud<pcl::PointXYZRGBA> cloud_transformed;
        //pcl::io::savePCDFileASCII("Original.pcd", cloud_filtered);
        pcl::transformPointCloud(cloud_filtered,cloud_filtered,transform_0);
        //pcl::io::savePCDFileASCII("Transformado.pcd", cloud_filtered);
        pcl::transformPointCloud(cloud_filtered,cloud_filtered,transform_1);
        pcl::transformPointCloud(cloud_filtered,cloud_filtered,transform_2);
        //y
        cloudPTR = cloud_filtered.makeShared();
        pass.setInputCloud(cloudPTR);
        pass.setFilterFieldName("y");
        pass.setFilterLimits(-1.5,0);
        pass.filter(cloud_filtered);
        // RANSAC
        // create the Segmentation object
        pcl::SACSegmentation<pcl::PointXYZRGBA> seg;
        // Mandatory
        seg.setModelType (pcl::SACMODEL_PLANE);
        seg.setMethodType (pcl::SAC_RANSAC);
        seg.setMaxIterations (1000);
        seg.setDistanceThreshold (0.01);
        // Create the filtering object
        pcl::ExtractIndices<pcl::PointXYZRGBA> extract;
        // create the inliers and coefficients parameters
        pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients ());
        pcl::PointIndices::Ptr inliers (new pcl::PointIndices ());
        pcl::PointCloud<pcl::PointXYZRGBA> cloud_p;
        //pcl::PointCloud<pcl::PointXYZRGBA> cloud_p (new pcl::PointCloud<pcl::PointXYZRGBA>);
        pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_f (new pcl::PointCloud<pcl::PointXYZRGBA>);
        // tranfor the cloud filtered in boost pcl
        cloudPTR = cloud_filtered.makeShared();
        int i = 0, nr_points = (int) cloudPTR->points.size();
        //printf("Point numbers = %d\n ", nr_points);
        while (cloudPTR->points.size() > 0.05 * nr_points){
        //while (true){
        // Segment the largest planar component from the remaining cloud
        seg.setInputCloud (cloudPTR);
        seg.segment (*inliers, *coefficients);
        if (inliers->indices.size () == 0)
        {
            std::cerr << "Could not estimate a planar model for the given dataset." << std::endl;
            break;
        }
	    std::cerr << "Coeficientes do plano (A,B,C,D): " << coefficients->values[0] << " " 
                                      << coefficients->values[1] << " "
                                      << coefficients->values[2] << " " 
                                      << coefficients->values[3] << std::endl;
    
        // Extract the inliers
        extract.setInputCloud (cloudPTR);
        extract.setIndices (inliers);
        extract.setNegative (false);
        extract.filter (cloud_p);
        //std::cerr << "Pontos que pertencem ao plano " << cloud_p.width * cloud_p.height << " data points." << std::endl;
	    // Create the filtering object
        extract.setNegative (true);
        extract.filter (*cloud_f);
        cloudPTR.swap (cloud_f);
        i++;
        }
        
        printf("Detected Planes = %d\n ", i );
        //output
        pcl::toROSMsg(cloud_filtered, output);
    	output.header.frame_id = "point_cloud";
        //pcl::io::savePCDFileASCII("filtrado.pcd", cloud_filtered);
        
        pcl_pub.publish(output);
    }

protected:
    Eigen::Quaternionf orientation;
    Eigen::Matrix4f transform_0; 
    Eigen::Matrix4f transform_1; 
    Eigen::Matrix4f transform_2;
    float roll;
    float pitch;
    float yaw;
    ros::NodeHandle nh;
    ros::Subscriber pcl_sub;
    ros::Subscriber ori;
    ros::Publisher pcl_pub;
};

main(int argc, char** argv)
{
    ros::init(argc, argv, "pcl_filter");
    ROS_INFO("Started Filter Node");
    cloudHandler handler;
    ros::spin();
    return 0;
}
