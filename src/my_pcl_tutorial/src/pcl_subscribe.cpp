#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/filter.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/voxel_grid.h>

typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

void callback(const PointCloud::ConstPtr& msg)
{
  std::vector<int> indices;
  // Removendo NaNs
  PointCloud::Ptr RemovedNaNs (new PointCloud);
  pcl::removeNaNFromPointCloud(*msg,*RemovedNaNs,indices);
  printf ("Original Point Cloud Size = %d\n After filtering nans = %d\n",msg->width*msg->height, RemovedNaNs->width*RemovedNaNs->height);
  //PassTroughfilter
  PointCloud::Ptr PassTroughfilter (new PointCloud);
  // Create the filtering object
  pcl::PassThrough<pcl::PointXYZ> pass;
  pass.setInputCloud(RemovedNaNs);
  pass.setFilterFieldName ("z");
  pass.setFilterLimits (0.0, 1.0);
  pass.filter (*PassTroughfilter);
  printf ("After PassThrough Filter = %d\n",PassTroughfilter->width*PassTroughfilter->height);
  //Voxel Filter
  PointCloud::Ptr cloud_filtered (new PointCloud);
  //Create the filtering Voxel
  pcl::VoxelGrid<pcl::PointXYZ> sor;
  sor.setInputCloud (PassTroughfilter);
  sor.setLeafSize (0.02f, 0.02f, 0.02f);
  sor.filter (*cloud_filtered);
  printf ("After Voxel Grid Filter = %d\n",cloud_filtered->width*cloud_filtered->height);
}
int main(int argc, char** argv)
{
  ros::init(argc, argv, "sub_pcl");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe<PointCloud>("/camera/depth/points", 1, callback);
  ros::spin();
}