#include <stdio.h>
#include <pcl/range_image/range_image.h>

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
        
            // pcl::PointCloud
            pcl::PointCloud<pcl::PointXYZ> pointCloud;
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
                pointCloud.points.push_back(point);
                px+=4; py+=4; pz+=4; pr+=4;
            }
            pointCloud.width = (std::uint32_t) pointCloud.points.size();
            pointCloud.height = 1;
            free(data);
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
            
            FILE * outfile;

            // PARSE THE LINE
            std::string delimeter = "sequences/";
            // std::string token = line.substr(1, line.find(delimeter,0));
            line.erase(0, line.find(delimeter,0) + delimeter.length());
            // FInd sequence number
            delimeter = "/velodyne/";
            std::string seq = line.substr(0, line.find(delimeter,0));
            line.erase(0, line.find(delimeter,0) + delimeter.length());
            // Find bin number
            delimeter = ".bin";
            std::string bin = line.substr(0, line.find(delimeter,0));
            line.erase(0, line.find(delimeter,0) + delimeter.length());
            
            // New text file path for range-image
            std::string kitti_path = "/media/thomas/Samsung_T5/_data/KITTI_SegmentedData/KITTI_Segmantic_Segmentation/dataset/sequences/";
            std::string rimg_path = kitti_path + seq + "/range_images/" + bin + ".bin";
            outfile = fopen (rimg_path.c_str(),"wb");
            short int idx_x, idx_y, x, y, z;
            for (int i = 0; i < range_image.points.size(); i++) {
                // range_image.width = 4002
                // range_image.height = 329
                if (!(std::isnan(range_image.points[i].x))) {
                    idx_x = (short int) (i % range_image.width);
                    idx_y = (short int) (i / range_image.width);
                    x = round(range_image.points[i].x * 100);
                    y = round(range_image.points[i].y * 100);
                    z = round(range_image.points[i].z * 100);
                    // r = round(range_image.points[i].range * 100);
                    fwrite(&idx_x, 1, sizeof(idx_x), outfile);
                    fwrite(&idx_y, 1, sizeof(idx_y), outfile);
                    fwrite(&x, 1, sizeof(x), outfile);
                    fwrite(&y, 1, sizeof(y), outfile);
                    fwrite(&z, 1, sizeof(z), outfile);
                    // fwrite(&r, 1, sizeof(r), outfile);
                }
            }
            fclose(outfile);
        }
        readfile.close();
    }
    
}
