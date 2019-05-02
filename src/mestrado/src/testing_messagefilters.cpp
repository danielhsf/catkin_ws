#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>

#include <tf/transform_listener.h>
//sensor_msgs header for point cloud2
#include <sensor_msgs/PointCloud2.h>

#include <sensor_msgs/Imu.h>
#include <cmath>

using namespace sensor_msgs;
using namespace message_filters;
using namespace tf;

void callback(const ImuConstPtr& sensorimu, const PointCloud2ConstPtr& nuvemdepontos)
{
  printf("teste");
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "vision_node");

  ros::NodeHandle nh;

  message_filters::Subscriber<PointCloud2> image_sub(nh, "/camera/depth_registered/points", 1);
  message_filters::Subscriber<Imu> info_sub(nh, "/sensorimu", 1);
  TimeSynchronizer<Imu,PointCloud2> sync(info_sub,image_sub , 1);
  sync.registerCallback(boost::bind(&callback, _1, _2));

  ros::spin();

  return 0;
}