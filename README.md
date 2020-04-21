# RangeImageUnet
U of M ROB530: Mobile Robotics Project for Winter 2019 
The purpose of this project is to perform label segmentation on a range image extracted from a point cloud using a deep learning approach.  There were several steps that we took to accomplish this which will be outlined in this repository.  
* Generating Range Images
* Deep Neural Network (Unet)
* Evaluation of Labeled Output
* Visualize map using BKISemanticMapping

## Prerequisites
* Docker (Optional, Recommended as their might be version conflicts with pre-existing packages installed in your computer)
* Ubuntu 16.04
* Google Colab
* Eigen 3.2.5
* VTK-7.1.0
* Flann 1.8
* Boost 1.58

There are several other prerequisites for the prepocessing and running the RangeImageUnet code which are described below.

### Preprocess (Range Image)
The range images were created using PCL (Point Cloud Library) which is an open-source library of algorithms for point cloud processing tasks. PCL can be run using either C++ or python and for this project our team compiled using C++. Outlined below are the necesarry files and libraries that need to be installed:  

* Install oracle-java8-jdk:
```
sudo add-apt-repository -y ppa:webupd8team/java && sudo apt update && sudo apt -y install oracle-java8-installer
```
* Install universal pre-requisites:
```
sudo apt -y install g++ cmake cmake-gui doxygen mpi-default-dev openmpi-bin openmpi-common libusb-1.0-0-dev libqhull* libusb-dev libgtest-dev
sudo apt -y install git-core freeglut3-dev pkg-config build-essential libxmu-dev libxi-dev libphonon-dev libphonon-dev phonon-backend-gstreamer
sudo apt -y install phonon-backend-vlc graphviz mono-complete qt-sdk libflann-dev   
```
* Install libflann and libboost
```
sudo apt -y install libflann1.8 libboost1.58-all-dev  
```

* Install libeigen
```
cd ~/Downloads
wget http://launchpadlibrarian.net/209530212/libeigen3-dev_3.2.5-4_all.deb
sudo dpkg -i libeigen3-dev_3.2.5-4_all.deb
sudo apt-mark hold libeigen3-dev
```

* Install VTK
```
wget http://www.vtk.org/files/release/7.1/VTK-7.1.0.tar.gz
tar -xf VTK-7.1.0.tar.gz
cd VTK-7.1.0 && mkdir build && cd build
cmake ..
make                                                                   
sudo make install
```

* Install PCL (This takes a while ...)
```
cd ~/Downloads
wget https://github.com/PointCloudLibrary/pcl/archive/pcl-1.8.0.tar.gz
tar -xf pcl-1.8.0.tar.gz
cd pcl-pcl-1.8.0 && mkdir build && cd build
cmake ..
make
sudo make install
```  

## Generating Range Images
The .cpp files specifically write_rimg_bin.cpp and write_rimg_labels.cpp are used to generate the range images and the parameters will be explained below.
### Angular Resolution
The angular resolution for both x and y axis have been set to 0.31 degrees to limit the size of the range image. The KITTI dataset contain lidar point cloud data with an angular resolution of 0.09 degrees. However this created a range image that was nearly 4000 x 530 pixels. Due to the limitation of Google Colab in terms of GPU RAM and disk space, the range images generated needed to be tuned to 0.31 degrees to generate an image that was 1280 x 128 pixels.
### Coordinate Frame
LASER_FRAME was used instead of IMAGE_FRAME
### Angular
360.0f 180.0f

