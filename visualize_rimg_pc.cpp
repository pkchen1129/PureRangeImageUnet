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

typedef pcl::PointXYZ PointType;

// --------------------
// -----Parameters-----
// --------------------
float angular_resolution_x = (float) (0.09f * (M_PI/180.0f));  //   1.0 degree in radians
float angular_resolution_y = angular_resolution_x;
pcl::RangeImage::CoordinateFrame coordinate_frame = pcl::RangeImage::LASER_FRAME;
bool live_update = false;

// --------------
// -----Help-----
// --------------
void printUsage (const char* progName) {
  std::cout << "\n\nUsage: "<<progName<<" [options] <scene.pcd>\n\n"
            << "Options:\n"
            << "-------------------------------------------\n"
            << "-rx <float>  angular resolution in degrees (default "<<angular_resolution_x<<")\n"
            << "-ry <float>  angular resolution in degrees (default "<<angular_resolution_y<<")\n"
            << "-c <int>     coordinate frame (default "<< (int)coordinate_frame<<")\n"
            << "-l           live update - update the range image according to the selected view in the 3D viewer.\n"
            << "-h           this help\n"
            << "\n\n";
}

void setViewerPose (pcl::visualization::PCLVisualizer& viewer, const Eigen::Affine3f& viewer_pose) {
  Eigen::Vector3f pos_vector = viewer_pose * Eigen::Vector3f(0, 0, 0);
  Eigen::Vector3f look_at_vector = viewer_pose.rotation () * Eigen::Vector3f(0, 0, 1) + pos_vector;
  Eigen::Vector3f up_vector = viewer_pose.rotation () * Eigen::Vector3f(0, -1, 0);
  viewer.setCameraPosition (pos_vector[0], pos_vector[1], pos_vector[2],
                            look_at_vector[0], look_at_vector[1], look_at_vector[2],
                            up_vector[0], up_vector[1], up_vector[2]);
}

// --------------
// -----Main-----
// --------------

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

	// -----------------------------------------------
	// -----Create RangeImage from the PointCloud-----
	// -----------------------------------------------
	float noise_level = 0.0;
	float min_range = 0.0f;
	int border_size = 1;
	pcl::RangeImage::Ptr range_image_ptr(new pcl::RangeImage);
	pcl::RangeImage& range_image = *range_image_ptr;   
	range_image.createFromPointCloud (pointCloud, angular_resolution_x, angular_resolution_y,
										pcl::deg2rad (360.0f), pcl::deg2rad (180.0f),
										scene_sensor_pose, coordinate_frame, noise_level, min_range, border_size);
	
	// --------------------------------------------
	// -----Open 3D viewer and add point cloud-----
	// --------------------------------------------
	pcl::visualization::PCLVisualizer viewer ("3D Viewer");
	viewer.setBackgroundColor (1, 1, 1);
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointWithRange> range_image_color_handler (range_image_ptr, 0, 0, 0);
	viewer.addPointCloud (range_image_ptr, range_image_color_handler, "range image");
	viewer.setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "range image");
	//viewer.addCoordinateSystem (1.0f, "global");
	//PointCloudColorHandlerCustom<PointType> point_cloud_color_handler (pointCloud, 150, 150, 150);
	//viewer.addPointCloud (pointCloud_ptr, point_cloud_color_handler, "original point cloud");
	viewer.initCameraParameters ();
	setViewerPose(viewer, range_image.getTransformationToWorldSystem ());
	
	// --------------------------
	// -----Show range image-----
	// --------------------------
	pcl::visualization::RangeImageVisualizer range_image_widget ("Range image");
	range_image_widget.showRangeImage (range_image);
	
	//--------------------
	// -----Main loop-----
	//--------------------
	while (!viewer.wasStopped ())
	{
		range_image_widget.spinOnce ();
		viewer.spinOnce ();
		pcl_sleep (0.01);
		
		if (live_update)
		{
		scene_sensor_pose = viewer.getViewerPose();
		range_image.createFromPointCloud (pointCloud, angular_resolution_x, angular_resolution_y,
											pcl::deg2rad (360.0f), pcl::deg2rad (180.0f),
											scene_sensor_pose, pcl::RangeImage::LASER_FRAME, noise_level, min_range, border_size);
		range_image_widget.showRangeImage (range_image);
		}
	}
}
