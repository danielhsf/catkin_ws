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
  float sinr_cosp = +2.0 * (sensorimu->orientation.w * sensorimu->orientation.x + sensorimu->orientation.y * sensorimu->orientation.z);
  float cosr_cosp = +1.0 - 2.0 * (sensorimu->orientation.x * sensorimu->orientation.x + sensorimu->orientation.y * sensorimu->orientation.y);
  float roll = atan2f(sinr_cosp, cosr_cosp);
  printf("Roll = %f \n", roll*180/M_PI);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "vision_node");

  ros::NodeHandle nh;

  message_filters::Subscriber<PointCloud2> image_sub(nh, "/camera/depth_registered/points", 1);
  message_filters::Subscriber<Imu> info_sub(nh, "/sensorimu", 1);
  TimeSynchronizer<Imu,PointCloud2> sync(info_sub,image_sub , 10);
  sync.registerCallback(boost::bind(&callback, _1, _2));

  ros::spin();

  return 0;
}