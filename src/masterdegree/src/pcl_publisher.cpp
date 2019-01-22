#include <ros/ros.h>

// Point Cloud headers
#include <pcl/point_cloud.h>

// Header which contain PCL to ROS and ROS to pcl conversion functions
#include <pcl_conversions/pcl_conversions.h>

//sensor_msgs header for point cloud2
#include <sensor_msgs/PointCloud2.h>

main(int argc, char **argv){
    ros::init(argc, argv, "pcl_create");
    ROS_INFO("Started PCL Publishing node");

    ros::NodeHandle nh;

    //Creating publisher object for point cloud 
    ros::Publisher pcl_pub = nh.advertise<sensor_msgs::PointCloud2>("pcl_output",1);

    //Creating a cloud object 
    pcl::PointCloud<pcl::PointXYZ> cloud;

    //Creating a sensor_msg of point Cloud
    sensor_msgs::PointCloud2 output;

    //insert a cloud data 
    cloud.width = 50000;
    cloud.height = 2;
    cloud.points.resize(cloud.width * cloud.height);

    //insert random points on the clouds

    for(size_t i = 0; i < cloud.points.size();++i){
        cloud.points[i].x = 512 * rand () / (RAND_MAX + 1.0f);
        cloud.points[i].y = 512 * rand () / (RAND_MAX + 1.0f);
        cloud.points[i].z = 512 * rand () / (RAND_MAX + 1.0f);
    }

    // Convert the cloud to ROS message
    pcl::toROSMsg(cloud, output);
    output.header.frame_id = "point_cloud";

    ros::Rate loop_rate(1);
    while(ros::ok()){
        //Publishing point cloud data
        pcl_pub.publish(output);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
