#include <ros/ros.h>
#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <sensor_msgs/PointCloud2.h>
//#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/PolygonMesh.h>
#include <pcl_msgs/PolygonMesh.h>


class cloudHandler
{
public:
    cloudHandler()
    {
        pcl_sub = nh.subscribe("pcl_output", 10, &cloudHandler::cloudCB, this);
        pcl_pub = nh.advertise<sensor_msgs::PointCloud2>("pcl_filtered", 1);
    }

    void cloudCB(const sensor_msgs::PointCloud2& input)
    {
        pcl::PolygonMesh mesh;
        pcl::PointCloud<pcl::PointXYZRGBA> cloud;
        sensor_msgs::PointCloud2 output;
        pcl::fromROSMsg(input, cloud);
        pcl::toPCLPointCloud2(cloud, mesh.cloud);
        pcl::toROSMsg(cloud, output);
    	output.header.frame_id = "point_cloud";

        pcl_pub.publish(output);
    }

protected:
    ros::NodeHandle nh;
    ros::Subscriber pcl_sub;
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
