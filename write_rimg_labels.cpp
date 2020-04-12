#include <stdio.h>
#include <pcl/range_image/range_image.h>

#include <pcl/point_cloud.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <vector>

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cmath>

// --------------------
// -----Parameters-----
// --------------------
float angular_resolution_x = (float) (0.09f * (M_PI/180.0f));  //   1.0 degree in radians
float angular_resolution_y = angular_resolution_x;
pcl::RangeImage::CoordinateFrame coordinate_frame = pcl::RangeImage::LASER_FRAME;
bool live_update = false;


// --------------
// -----Main-----
// --------------
int main (int argc, char** argv) {
	std::cout << "PCL Version: " << PCL_VERSION << std::endl;

    std::fstream readfile;
    readfile.open("/home/thomas/ROB530/final_project/filepaths/KIITI_filepaths.txt", std::ios::in);
    if (readfile.is_open()) {
        std::string line;
        while(getline(readfile, line)) {
            std::cout << line << std::endl;

            // Create the path for .label file
            // replace velodyne with labels
            std::string line_label = line;
            std::string delimeter = "velodyne";
            int del_idx = line_label.find(delimeter,0);
            line_label.replace(del_idx,delimeter.length(), "labels");
            // replace .bin with .labels
            delimeter = ".bin";
            del_idx = line_label.find(delimeter,0);
            line_label.replace(del_idx,delimeter.length(), ".label");
            std::cout << line_label << std::endl;

            // pcl::PointCloud
            pcl::PointCloud<pcl::PointXYZ> pointCloud;
            pcl::PointCloud<pcl::PointXYZ>::Ptr pointCloud_ptr (new pcl::PointCloud<pcl::PointXYZ>);
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
            // std::string currFilenameBinary ("/media/thomas/Samsung_T5/_data/KITTI_SegmentedData/KITTI_Segmantic_Segmentation/dataset/sequences/00/velodyne/000000.bin");
            FILE *stream;
            // stream = fopen (currFilenameBinary.c_str(),"rb");
            stream = fopen (line.c_str(),"rb");
            num = fread(data,sizeof(float),num,stream)/4;

            for (int32_t i=0; i<num; i++) {
                // Point
                pcl::PointXYZ point;
                point.x = px[0];
                point.y = py[0];
                point.z = pz[0];
                point.data[4] = i;
                pointCloud.points.push_back(point);
                pointCloud_ptr->points.push_back(point);

                px+=4; py+=4; pz+=4; pr+=4;
            }
            pointCloud.width = (std::uint32_t) pointCloud.points.size();
            pointCloud.height = 1;
            free(data);
            fclose(stream);

            // read the label file
            // allocate 4 MB buffer (only ~130*4*4 KB are needed)
            int32_t num_label = 1000000;
            int *data_label = (int*)malloc(num_label*sizeof(int));
            // pointers
            int *plabel = data_label+0;
            FILE *stream_label;
            stream_label = fopen (line_label.c_str(),"rb");
            num_label = fread(data_label,sizeof(int),num_label,stream_label);
            std::vector<int> labels;
            for (int32_t i=0; i<num_label; i++) {
                labels.push_back(plabel[0]);
                plabel+=1;
            }
            free(data_label);
            fclose(stream_label);

            //  ----------------------------------------------
            //  --------------- Create KD Tree ---------------
            //  ----------------------------------------------
            pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;
            kdtree.setInputCloud (pointCloud_ptr);
            int K = 5;
            
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
            
            FILE * outfile;

            // PARSE THE LINE
            delimeter = "sequences/";
            // std::string token = line.substr(1, line.find(delimeter,0));
            line_label.erase(0, line_label.find(delimeter,0) + delimeter.length());
            // FInd sequence number
            delimeter = "/labels/";
            std::string seq = line_label.substr(0, line_label.find(delimeter,0));
            line_label.erase(0, line_label.find(delimeter,0) + delimeter.length());
            // Find bin number
            delimeter = ".label";
            std::string label = line_label.substr(0, line_label.find(delimeter,0));
            line_label.erase(0, line_label.find(delimeter,0) + delimeter.length());
            
            // New text file path for range-image
            std::string kitti_path = "/media/thomas/Samsung_T5/_data/KITTI_SegmentedData/KITTI_Segmantic_Segmentation/dataset/sequences/";
            std::string rimg_path = kitti_path + seq + "/labels_range_images/" + label + ".label";
            outfile = fopen (rimg_path.c_str(),"wb");

            // write the range image width and height
            short int r_height = range_image.height;
            short int r_width = range_image.width; 
            fwrite(&r_height, 1, sizeof(r_height), outfile);
            fwrite(&r_width, 1, sizeof(r_width), outfile); 

            short int idx, x, y, z;
            for (int i = 0; i < range_image.points.size(); i++) {
                // range_image.width = 4002
                // range_image.height = 329
                if (!(std::isnan(range_image.points[i].x))) {
                    pcl::PointXYZ searchPoint;
                    searchPoint.x = range_image.points[i].x;
                    searchPoint.y = range_image.points[i].y;
                    searchPoint.z = range_image.points[i].z;
                    std::vector<int> pointIdxNKNSearch(K);
                    std::vector<float> pointNKNSquaredDistance(K);

                    // std::cout << "K nearest neighbor search at (" << searchPoint.x 
                    //             << " " << searchPoint.y 
                    //             << " " << searchPoint.z
                    //             << ") with K=" << K << std::endl;
                    // if ( kdtree.nearestKSearch (searchPoint, K, pointIdxNKNSearch, pointNKNSquaredDistance) > 0 ){
                    //     for (std::size_t i = 0; i < pointIdxNKNSearch.size (); ++i) {
                    //     std::cout << "    "  <<   pointCloud_ptr->points[ pointIdxNKNSearch[i] ].x 
                    //                 << " " << pointCloud_ptr->points[ pointIdxNKNSearch[i] ].y 
                    //                 << " " << pointCloud_ptr->points[ pointIdxNKNSearch[i] ].z 
                    //                 << " (squared distance: " << pointNKNSquaredDistance[i] << ")" << std::endl;
                    //     }
                    // }
                    kdtree.nearestKSearch (searchPoint, K, pointIdxNKNSearch, pointNKNSquaredDistance);
                    short int label = labels[pointIdxNKNSearch[0]];
                    fwrite(&label, 1, sizeof(label), outfile);
                }
            }
            fclose(outfile);
        }
        readfile.close();
    }
    
}
