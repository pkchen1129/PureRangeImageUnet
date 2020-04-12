#include <stdio.h>
#include <pcl/range_image/range_image.h>

// For Range Viewer
#include <pcl/pcl_config.h>
#include <pcl/common/common_headers.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/range_image_visualizer.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>
// For Cloud Viewer
#include <pcl/visualization/cloud_viewer.h>

#include <iostream>
#include <fstream>
#include <string>
#include <thread>

pcl::visualization::PCLVisualizer::Ptr simpleVis (pcl::PointCloud<pcl::PointXYZ>::ConstPtr cloud)
{
  // --------------------------------------------
  // -----Open 3D viewer and add point cloud-----
  // --------------------------------------------
  pcl::visualization::PCLVisualizer::Ptr viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));
  viewer->setBackgroundColor (0, 0, 0);
  viewer->addPointCloud<pcl::PointXYZ> (cloud, "sample cloud");
  viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "sample cloud");
  viewer->addCoordinateSystem (1.0);
  viewer->initCameraParameters ();
  return (viewer);
}

pcl::visualization::PCLVisualizer::Ptr rgbVis (pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr cloud)
{
  // --------------------------------------------
  // -----Open 3D viewer and add point cloud-----
  // --------------------------------------------
  pcl::visualization::PCLVisualizer::Ptr viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));
  viewer->setBackgroundColor (0, 0, 0);
  pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(cloud);
  viewer->addPointCloud<pcl::PointXYZRGB> (cloud, rgb, "sample cloud");
  viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "sample cloud");
  viewer->addCoordinateSystem (1.0);
  viewer->initCameraParameters ();
  return (viewer);
}

int main (int argc, char** argv) {
	std::cout << "PCL Version: " << PCL_VERSION << std::endl;

	// pcl::PointCloud
	pcl::PointCloud<pcl::PointXYZ> pointCloud;
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr rgb_pointCloud_ptr(new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr pointCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
	// Eigen::Affine3f scene_sensor_pose (Eigen::Affine3f::Identity ());
	Eigen::Affine3f scene_sensor_pose = (Eigen::Affine3f)Eigen::Translation3f(0.0f, 0.0f, 0.0f);
	
	// Read the .bin file
	// allocate 4 MB buffer (only ~130*4*4 KB are needed)
	int32_t num = 1000000;
	float *data = (float*)malloc(num*sizeof(float));
	// pointers
	float *px = data+0;
	float *py = data+1;
	float *pz = data+2;
	float *pr = data+3;
	// load point cloud
	std::string currFilenameBinary ("/media/thomas/Samsung_T5/_data/KITTI_SegmentedData/KITTI_Segmantic_Segmentation/dataset/sequences/00/velodyne/000000.bin");
	FILE *stream;
	stream = fopen (currFilenameBinary.c_str(),"rb");
	num = fread(data,sizeof(float),num,stream)/4;
	for (int32_t i=0; i<num; i++) {
		// Point
		pcl::PointXYZ point;
		point.x = px[0];
		point.y = py[0];
		point.z = pz[0];
		pointCloud.points.push_back(point);
		pointCloud_ptr->points.push_back(point);
		// Point RGB
		pcl::PointXYZRGB point_rgb;
		point_rgb.x = px[0];
		point_rgb.y = py[0];
		point_rgb.z = pz[0];
		point_rgb.r = 255;
		point_rgb.b = 0;
		point_rgb.g = 0;
		rgb_pointCloud_ptr->points.push_back(point_rgb);

		px+=4; py+=4; pz+=4; pr+=4;
	}
	pointCloud.width = (std::uint32_t) pointCloud.points.size();
	pointCloud.height = 1;

	pointCloud_ptr->width = (std::uint32_t) rgb_pointCloud_ptr->points.size();
	pointCloud_ptr->height = 1;

	rgb_pointCloud_ptr->width = (std::uint32_t) rgb_pointCloud_ptr->points.size();
	rgb_pointCloud_ptr->height = 1;
	fclose(stream);

	pcl::visualization::PCLVisualizer::Ptr pviewer;
	// pviewer = simpleVis(pointCloud_ptr);
	pviewer = rgbVis(rgb_pointCloud_ptr);
	while (!pviewer->wasStopped ()) {
	    pviewer->spinOnce (100);
		pcl_sleep (0.01);
    }
}
