#include <tf/transform_listener.h>
#include <tf/message_filter.h>
#include <sensor_msgs/PointCloud2.h>

#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>

//Basic ROS package
#include <ros/ros.h>
#include <pcl_conversions/pcl_conversions.h>
// Point Cloud headers
#include <pcl/point_cloud.h>

class PointReader
{ 
  public:
    PointReader(){
        //initialisation
    cloud2_sub_.subscribe(n, "/camera/depth_registered/points", 10);
    tf_filter_ = new tf::MessageFilter<sensor_msgs::PointCloud2>(cloud2_sub_, tf_, "/tf", 10);
    tf_filter_->registerCallback( boost::bind(&PointReader::pointCloudCallback, this, _1) );
    }

    //callback
    void pointCloudCallback(const sensor_msgs::PointCloud2::ConstPtr& input)
    {
        //pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr cloud;
        //pcl::fromROSMsg(input, cloud);
        //ROS_INFO("Message received");
        //pcl::fromROSMsg (*cloud_, original_pcl_cloud_);
        //pcl::transformPointCloud("/map", original_pcl_cloud_, transformed_pcl_cloud_, tf_);
        //ROS_INFO("Message passed");
        //transformed_pcl_cloud_ contains the projected cloud       
    }

protected:
    //Variables
    sensor_msgs::PointCloud2::ConstPtr& cloud_;
    ros::NodeHandle n;
    tf::TransformListener tf_;
    tf::MessageFilter<sensor_msgs::PointCloud2> * tf_filter_;
    message_filters::Subscriber<sensor_msgs::PointCloud2> cloud2_sub_;
};

main(int argc, char** argv)
{
    ros::init(argc, argv, "pcl_filter");
    ROS_INFO("Started Filter Node");
    PointReader reader;
    ros::spin();
    return 0;
}