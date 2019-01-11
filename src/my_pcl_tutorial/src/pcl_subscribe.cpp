#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>

#include <pcl_ros/point_cloud.h>
#include <pcl_ros/transforms.h>

#include <pcl/point_types.h>
#include <pcl/filters/filter.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/common/transforms.h>

//Plane detection
#include <pcl/ModelCoefficients.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/filters/extract_indices.h>

typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

Eigen::Quaternionf orientation;

void orientationcallback(std_msgs::Float32MultiArray sensorimu){
orientation.w() = sensorimu.data[0];
orientation.y() = sensorimu.data[1];
orientation.x() = sensorimu.data[2];
orientation.z() = sensorimu.data[3];
}

void callback(const PointCloud::ConstPtr& msg){
  std::vector<int> indices;
  // Removendo NaNs
  PointCloud::Ptr RemovedNaNs (new PointCloud);
  pcl::removeNaNFromPointCloud(*msg,*RemovedNaNs,indices);
  //printf ("Original Point Cloud Size = %d\n After filtering nans = %d\n",msg->width*msg->height, RemovedNaNs->width*RemovedNaNs->height);
  //PassTroughfilter
  PointCloud::Ptr PassTroughfilter (new PointCloud);
  // Create the filtering object
  pcl::PassThrough<pcl::PointXYZ> pass;
  pass.setInputCloud(RemovedNaNs);
  pass.setFilterFieldName ("z");
  pass.setFilterLimits (0.0, 2.0);
  pass.filter (*PassTroughfilter);
  //printf ("After PassThrough Filter = %d\n",PassTroughfilter->width*PassTroughfilter->height);
  //Voxel Filter
  PointCloud::Ptr cloud_filtered (new PointCloud);
  //Create the filtering Voxel
  pcl::VoxelGrid<pcl::PointXYZ> sor;
  sor.setInputCloud (PassTroughfilter);
  sor.setLeafSize (0.01f, 0.01f, 0.01f);
  sor.filter (*cloud_filtered);
  //printf ("After Voxel Grid Filter = %d\n",cloud_filtered->width*cloud_filtered->height);

  // Apllying transform view
  cloud_filtered->sensor_orientation_ = orientation;
  //printf("Quaternions : qw = %f, qx = %f, qy = %f, qz = %f\n", cloud_filtered->sensor_orientation_.w(),cloud_filtered->sensor_orientation_.y(),cloud_filtered->sensor_orientation_.x(),cloud_filtered->sensor_orientation_.z());
  //printf("Points : x = %f, y = %f e z = %f\n", cloud_filtered->points[0].x,cloud_filtered->points[0].y,cloud_filtered->points[0].z);
  
  Eigen::Vector4f translation = Eigen::Vector4f::Zero ();
  Eigen::Quaternionf origin = Eigen::Quaternionf::Identity ();
  PointCloud::Ptr cloud_out (new PointCloud);

  pcl::transformPointCloud(*cloud_filtered, *cloud_out, translation.head<3> (), orientation);
  //printf("Quaternions_transform : qw = %f, qx = %f, qy = %f, qz = %f\n", cloud_out->sensor_orientation_.w(),cloud_out->sensor_orientation_.y(),cloud_out->sensor_orientation_.x(),cloud_filtered->sensor_orientation_.z());
  //printf("Points_transform : x = %f, y = %f e z = %f\n", cloud_out->points[0].x,cloud_out->points[0].y,cloud_out->points[0].z);
  
  //translation.z() = 0.8;
  //pcl::transformPointCloud(*cloud_filtered, *cloud_filtered, translation.head<3> (), orientation);
  //cloud_filtered->sensor_orientation_ = orientation;
  //cloud_filtered->sensor_origin_ = translation;
  //printf ("Quaternions2: qw = %f, qy = %f, qx = %f, qz = %f\n", cloud_filtered->sensor_orientation_.w(),cloud_filtered->sensor_orientation_.y(),cloud_filtered->sensor_orientation_.x(),cloud_filtered->sensor_orientation_.z());
  //pcl::PCDWriter::writeASCII("teste.pcd", cloud_filtered, translation, orientation);
  //Eigen::Affine3f transform_2 = Eigen::Affine3f::Identity();
  //PointCloud::Ptr cloud_out (new PointCloud);
  //Eigen::Vector4f translation = Eigen::Vector4f::Zero ();
  //Eigen::Quaternionf origin = Eigen::Quaternionf::Identity ();
  //pcl_ros::transformPointCloud
  // You can either apply transform_1 or transform_2; they are the same
  //pcl_ros::transformPointCloud(*cloud_filtered, *cloud_out);Eigen::Matrix4f::Identity()
  //pcl::transformPointCloud<pcl::PointXYZ>(*cloud_filtered,*cloud_out, translation.head<3> (), origin);
  //printf ("Quaternions: qw = %f, qy = %f, qx = %f, qz = %f\n", cloud_filtered->sensor_orientation_.w(),cloud_filtered->sensor_orientation_.y(),cloud_filtered->sensor_orientation_.x(),cloud_filtered->sensor_orientation_.z());
  //printf ("Quaternions 2 : qw = %f, qy = %f, qx = %f, qz = %f\n", cloud_out->sensor_orientation_.w(),cloud_out->sensor_orientation_.y(),cloud_out->sensor_orientation_.x(),cloud_out->sensor_orientation_.z());
  //printf ("After Transform View = %d\n",cloud_out->width*cloud_out->height);
  //Eigen::Vector4f translation;
  //translation = Eigen::Vector4f::Zero ();
  //pcl::transformPointCloud(*cloud_filtered, *cloud_out, translation.head<3> (), orientation);
  //pcl::transformPointCloud(*cloud_filtered, *xyz, translation.head<3> (), orientation);
  //printf ("Quaternions: qw = %f, qy = %f, qx = %f, qz = %f\n", cloud_out->sensor_orientation_.w(),cloud_out->sensor_orientation_.y(),cloud_out->sensor_orientation_.x(),cloud_out->sensor_orientation_.z());
  //orientation = Eigen::Quaternionf::Identity ();

  //Plane Detection
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_p (new pcl::PointCloud<pcl::PointXYZ>);
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_f (new pcl::PointCloud<pcl::PointXYZ>);
  pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients ());
  pcl::PointIndices::Ptr inliers (new pcl::PointIndices ());
  // Create the segmentation object
  pcl::SACSegmentation<pcl::PointXYZ> seg;
  // Optional
  seg.setOptimizeCoefficients (true);
  // Mandatory
  seg.setModelType (pcl::SACMODEL_PLANE);
  seg.setMethodType (pcl::SAC_RANSAC);
  seg.setMaxIterations (1000);
  seg.setDistanceThreshold (0.005);

  // Create the filtering object
  pcl::ExtractIndices<pcl::PointXYZ> extract;

  int i = 0, nr_points = (int) cloud_out->points.size ();
  printf("Número de Pontos: %d\n", nr_points);
  // While 30% of the original cloud is still there
  while (cloud_out->points.size () > 0.05 * nr_points)
  {
    
    // Segment the largest planar component from the remaining cloud
    seg.setInputCloud (cloud_out);
    seg.segment (*inliers, *coefficients);
    printf("Número de inliers: %d\n",(int) inliers->indices.size());
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
    extract.setInputCloud (cloud_out);
    extract.setIndices (inliers);
    extract.setNegative (false);
    extract.filter (*cloud_p);
    std::cerr << "Pontos que pertencem ao plano " << cloud_p->width * cloud_p->height << " data points." << std::endl;
	
	  //std::stringstream ss;
    //ss << "5vrep_" << i << ".pcd";
    //writer.write<pcl::PointXYZ> (ss.str (), *cloud_p, false);

    // Create the filtering object
    extract.setNegative (true);
    extract.filter (*cloud_f);
    cloud_out.swap (cloud_f);
    i++;
    printf("Número de Pontos em cloud_out: %d\n ", cloud_out->width*cloud_out->height);
  }

  printf("Planos detectados = %d\n",i);
  
}
int main(int argc, char** argv)
{
  ros::init(argc, argv, "sub_pcl");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe<PointCloud>("/camera/depth/points", 1, callback);
  ros::NodeHandle ni;
  ros::Subscriber sub2 = ni.subscribe("/sensorimu", 1, orientationcallback);
  ros::spin();
}