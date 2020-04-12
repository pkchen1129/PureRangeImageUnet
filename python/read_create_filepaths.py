import pdb
import glob 
import os

file_KITTI = open("../filepaths/KIITI_filepaths.txt", "w")

data_file_path = '/media/thomas/Samsung_T5/_data/KITTI_SegmentedData/KITTI_Segmantic_Segmentation/dataset/sequences/'
# Gives the full path to subfolder
# seq_paths = glob.glob(data_file_path+'/*')
seq_paths = [f.path for f in os.scandir(data_file_path) if f.is_dir()]
seq_paths = sorted(seq_paths)
# Gives the name of subfolder
seq_names = [f.name for f in os.scandir(data_file_path) if f.is_dir()]
seq_names = sorted(seq_names)

for seq_path, seq_name in zip(seq_paths, seq_names):
    image_type_paths = [f.path for f in os.scandir(seq_path) if f.is_dir()]
    image_type_names = [f.name for f in os.scandir(seq_path) if f.is_dir()]
    for image_type_path, image_type_names in zip(image_type_paths, image_type_names):
        if (image_type_path.endswith('velodyne')):
            image_paths = [f.path for f in os.scandir(image_type_path) if f.is_file()]
            image_paths = sorted(image_paths)
            image_names = [f.name for f in os.scandir(image_type_path) if f.is_file()]
            image_names = sorted(image_names)
            for image_path, image_name in zip(image_paths, image_names):
                if (image_name.endswith('.bin')):
                    print(image_path)
                    file_KITTI.write(image_path + "\n")

file_KITTI.close()