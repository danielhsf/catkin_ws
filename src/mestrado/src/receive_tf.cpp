#include <ros/ros.h>
#include <tf/transform_listener.h>


int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_listener");

  ros::NodeHandle node;

  tf::TransformListener listener;

  ros::Rate rate(10.0);

  while (node.ok()){
    tf::StampedTransform transform;
    try{
      listener.lookupTransform("/camera_link", "/tf",  
                               ros::Time(0), transform);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    }
    printf("w = %f, x = %f, y = %f, z = %f \n", transform.getRotation().w(), transform.getRotation().x(), transform.getRotation().y(), transform.getRotation().z());
    rate.sleep();
  }
  return 0;
};