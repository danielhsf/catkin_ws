#include <octomap_server/OctomapServer.h>
#include <octomap_ros/conversions.h>


class cloudHandler
{
public:
    cloudHandler()
    {
        pcl_sub = nh.subscribe("pcl_filtered", 10, &cloudHandler::cloudCB, this);
        pcl_pub = nh.advertise<octomap::Pointcloud>("octomap", 1);
    }

    void cloudCB(const sensor_msgs::PointCloud2& input)
    {
        octomap::Pointcloud octomapCloud;
        octomap::pointCloud2ToOctomap(input,octomapCloud);
        octomap::Pointcloud output;
        //pcl::fromROSMsg(input, cloud);

        //pcl::toROSMsg(cloud_filtered, output);
    	//output.header.frame_id = "octo_map";

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

