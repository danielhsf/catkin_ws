// This will be the final code to be implemented

//Basic ROS package
#include <ros/ros.h>

// Point Cloud headers
#include <pcl/point_cloud.h>

// Header which contain PCL to ROS and ROS to pcl conversion functions
#include <pcl_conversions/pcl_conversions.h>

//sensor_msgs header for point cloud2
#include <sensor_msgs/PointCloud2.h>

//Header for RANSAC
#include <pcl/filters/extract_indices.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>

//Header file for Float32MultiArray
#include <std_msgs/Float32MultiArray.h>


class cloudHandler{
public:
    cloudHandler(){
        pcl_sub = nh.subscribe("/pcl_filtros", 1, &cloudHandler::callback, this);
        pcl_pub = nh.advertise<std_msgs::Float32MultiArray>("Plane_Equations", 1);
    }

    void callback(const sensor_msgs::PointCloud2& input){
        pcl::PointCloud<pcl::PointXYZRGBA> cloud_filtered;
        sensor_msgs::PointCloud2 output;
        pcl::fromROSMsg(input, cloud_filtered);
        pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloudPTR(new pcl::PointCloud<pcl::PointXYZRGBA>);
        std::vector< float > arr;
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
        arr.push_back(coefficients->values[0]);
        arr.push_back(coefficients->values[1]);
        arr.push_back(coefficients->values[2]);
        arr.push_back(coefficients->values[3]);
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
        printf("Planos detectados = %d \n",i);
        for(int aux = 0;i<arr.size();i+=4){
        //std::cerr << "Coeficientes do plano (A,B,C,D) 2: " << arr[aux] << " " << arr[aux+1] << " " << arr[aux+2] << " "  << arr[aux+3] << std::endl;
        }

        // fill out message:
        dat.layout.dim.push_back(std_msgs::MultiArrayDimension());
        dat.layout.dim.push_back(std_msgs::MultiArrayDimension());
        dat.layout.dim[0].label = "height";
        dat.layout.dim[1].label = "width";
        dat.layout.dim[0].size = int(arr.size()/4);
        dat.layout.dim[1].size = 4;
        dat.layout.dim[0].stride = arr.size();
        dat.layout.dim[1].stride = 4;
        dat.layout.data_offset = 0;
        dat.data = arr;
        pcl_pub.publish(dat);
        //output
        //pcl::toROSMsg(cloud_filtered, output);
    	//output.header.frame_id = "point_cloud";
        //pcl::io::savePCDFileASCII("filtrado.pcd", cloud_filtered);
        
        //pcl_pub.publish(output);
    }

protected:
    ros::NodeHandle nh;
    ros::Subscriber pcl_sub;
    ros::Publisher pcl_pub;
    std_msgs::Float32MultiArray dat;
};

main(int argc, char** argv)
{
    ros::init(argc, argv, "pcl_planes_detection");
    ROS_INFO("Iniciando detecção de planos");
    cloudHandler handler;
    ros::spin();
    return 0;
}
