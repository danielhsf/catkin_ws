#include <ros/ros.h>

#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <sensor_msgs/PointCloud2.h>

#include <grid_map_ros/grid_map_ros.hpp>
#include <grid_map_msgs/GridMap.h>
#include <grid_map_pcl/grid_map_pcl.hpp>

#include <pcl/surface/gp3.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/filters/voxel_grid.h>
// Header for removing NaNs
#include <pcl/filters/filter.h>


using namespace grid_map;

class cloudHandler
{
public:
    cloudHandler()
    {
        //pcl_sub = nh.subscribe("/pcl_filtered", 10, &cloudHandler::cloudCB, this);
        pcl_sub = nh.subscribe("/pcl_filtros", 10, &cloudHandler::cloudCB, this);
        pcl_pub = nh.advertise<grid_map_msgs::GridMap>("grid_map", 1, true);
    }

    void cloudCB(const sensor_msgs::PointCloud2& input)
    {
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
        pcl::fromROSMsg(input, *cloud);
        std::vector<int> indices;
        pcl::removeNaNFromPointCloud(*cloud,*cloud,indices);
        // Create the filtering object
        //pcl::VoxelGrid<pcl::PointXYZ> sor;
        //sor.setInputCloud(cloud);
        //sor.setLeafSize(0.01f, 0.01f, 0.01f);
        //sor.filter(*cloud);
        // Normal estimation*
        pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;
        pcl::PointCloud<pcl::Normal>::Ptr normals (new pcl::PointCloud<pcl::Normal>);
        pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
        tree->setInputCloud (cloud);
        n.setInputCloud (cloud);
        n.setSearchMethod (tree);
        n.setKSearch (20);
        n.compute (*normals);
        //* normals should not contain the point normals + surface curvatures

        // Concatenate the XYZ and normal fields*
        pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals (new pcl::PointCloud<pcl::PointNormal>);
        pcl::concatenateFields (*cloud, *normals, *cloud_with_normals);
        //* cloud_with_normals = cloud + normals

        // Create search tree*
        pcl::search::KdTree<pcl::PointNormal>::Ptr tree2 (new pcl::search::KdTree<pcl::PointNormal>);
        tree2->setInputCloud (cloud_with_normals);

        // Initialize objects
        pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;
        pcl::PolygonMesh triangles;

        // Set the maximum distance between connected points (maximum edge length)
        gp3.setSearchRadius (0.025);

        // Set typical values for the parameters
        gp3.setMu (2.5);
        gp3.setMaximumNearestNeighbors (100);
        gp3.setMaximumSurfaceAngle(M_PI/4); // 45 degrees
        gp3.setMinimumAngle(M_PI/18); // 10 degrees
        gp3.setMaximumAngle(2*M_PI/3); // 120 degrees
        gp3.setNormalConsistency(false);

        // Get result
        gp3.setInputCloud (cloud_with_normals);
        gp3.setSearchMethod (tree2);
        gp3.reconstruct (triangles);
        //Grid Map
        GridMap map({"elevation"});
        map.setFrameId("map");
        GridMapPclConverter::initializeFromPolygonMesh(triangles,0.01, map);
        for (GridMapIterator it(map); !it.isPastEnd(); ++it) {
        Position position;
        map.getPosition(*it, position);
        map.at("elevation", *it) = 0;
        }
        GridMapPclConverter::addLayerFromPolygonMesh(triangles,"elevation",map);
        grid_map_msgs::GridMap message;
        GridMapRosConverter::toMessage(map, message);
        pcl_pub.publish(message);
    }

protected:
    ros::NodeHandle nh;
    ros::Subscriber pcl_sub;
    ros::Publisher pcl_pub;
};

main(int argc, char** argv)
{
    ros::init(argc, argv, "grid_map_node");

    ROS_INFO("Started grid map Node");
    cloudHandler handler;
    ros::spin();

    return 0;
}