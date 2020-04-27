%% Looking at range image .bin data
clc, clear
sequence = '01';
% datanum = '000000';
datanum = '000000';

is_windows = 0;

if (is_windows == 1)
    datapath = ['D:\_data/KITTI_SegmentedData/' ...
                'KITTI_Segmantic_Segmentation/dataset/sequences/' ...
                sequence, '/range_images'];
else
    datapath = ['/media/thomas/Samsung_T51/_data/KITTI_SegmentedData/' ...
                'KITTI_Segmantic_Segmentation/dataset/sequences/' ...
                sequence, '/range_images'];
end
fid = fopen([datapath, '/' datanum '.bin'],'r');
data = fread(fid, '*int16');
fclose(fid);

num = length(data)/5;
idx_x = zeros(num,1);
idx_y = zeros(num,1);
x = zeros(num,1);
y = zeros(num,1);
z = zeros(num,1);
for i = 1:num
    idx_x(i)  = data(5*(i-1)+1);
    idx_y(i)  = data(5*(i-1)+2);
    x(i)      = double(data(5*(i-1)+3))/100;
    y(i)      = double(data(5*(i-1)+4))/100;
    z(i)      = double(data(5*(i-1)+5))/100;
end

%% Looking at range image .label data
if (is_windows == 1)
    labelpath = ['D:\_data/KITTI_SegmentedData/' ...
            'KITTI_Segmantic_Segmentation/dataset/sequences/' ...
            sequence, '/labels_range_images'];
else
    labelpath = ['/media/thomas/Samsung_T51/_data/KITTI_SegmentedData/' ...
            'KITTI_Segmantic_Segmentation/dataset/sequences/' ...
            sequence, '/labels_range_images'];
end

fid_label = fopen([labelpath, '/' datanum '.label'],'r');
data = fread(fid_label, '*int16');
rimage_height = double(data(1));
rimage_width = double(data(2));
labels = double(data(3:end));
fclose(fid_label);
num_label = length(data);

%% Create label structures
% idx_x = (short int) (i % range_image.width);
% idx_y = (short int) (i / range_image.width);
                    
data_labeled.unlabeled              = struct('x',idx_x(find(labels == 0)),'y', idx_y(find(labels == 0)) );
data_labeled.outlier                = struct('x',idx_x(find(labels == 1)),'y', idx_y(find(labels == 1)) );
data_labeled.car                    = struct('x',idx_x(find(labels == 10)),'y', idx_y(find(labels == 10)) );
data_labeled.bicycle                = struct('x',idx_x(find(labels == 11)),'y', idx_y(find(labels == 11)) );
data_labeled.bus                    = struct('x',idx_x(find(labels == 13)),'y', idx_y(find(labels == 13)) );
data_labeled.motorcycle             = struct('x',idx_x(find(labels == 15)),'y', idx_y(find(labels == 15)) );
data_labeled.on_rails               = struct('x',idx_x(find(labels == 16)),'y', idx_y(find(labels == 16)) );
data_labeled.truck                  = struct('x',idx_x(find(labels == 18)),'y', idx_y(find(labels == 18)) );
data_labeled.other_vehicle          = struct('x',idx_x(find(labels == 20)),'y', idx_y(find(labels == 20)) );
data_labeled.person                 = struct('x',idx_x(find(labels == 30)),'y', idx_y(find(labels == 30)) );
data_labeled.bicyclist              = struct('x',idx_x(find(labels == 31)),'y', idx_y(find(labels == 31)) );
data_labeled.motorcyclist           = struct('x',idx_x(find(labels == 32)),'y', idx_y(find(labels == 32)) );
data_labeled.road                   = struct('x',idx_x(find(labels == 40)),'y', idx_y(find(labels == 40)) );
data_labeled.parking                = struct('x',idx_x(find(labels == 44)),'y', idx_y(find(labels == 44)) );
data_labeled.sidewalk               = struct('x',idx_x(find(labels == 48)),'y', idx_y(find(labels == 48)) );
data_labeled.other_ground           = struct('x',idx_x(find(labels == 49)),'y', idx_y(find(labels == 49)) );
data_labeled.building               = struct('x',idx_x(find(labels == 50)),'y', idx_y(find(labels == 50)) );
data_labeled.fence                  = struct('x',idx_x(find(labels == 51)),'y', idx_y(find(labels == 51)) );
data_labeled.other_structure        = struct('x',idx_x(find(labels == 52)),'y', idx_y(find(labels == 52)) );
data_labeled.lane_marking           = struct('x',idx_x(find(labels == 60)),'y', idx_y(find(labels == 60)) );
data_labeled.vegetation             = struct('x',idx_x(find(labels == 70)),'y', idx_y(find(labels == 70)) );
data_labeled.trunk                  = struct('x',idx_x(find(labels == 71)),'y', idx_y(find(labels == 71)) );
data_labeled.terrain                = struct('x',idx_x(find(labels == 72)),'y', idx_y(find(labels == 72)) );
data_labeled.pole                   = struct('x',idx_x(find(labels == 80)),'y', idx_y(find(labels == 80)) );
data_labeled.traffic_sign           = struct('x',idx_x(find(labels == 81)),'y', idx_y(find(labels == 81)) );
data_labeled.other_object           = struct('x',idx_x(find(labels == 99)),'y', idx_y(find(labels == 99)) );
data_labeled.moving_car             = struct('x',idx_x(find(labels == 252)),'y', idx_y(find(labels == 252)) );
data_labeled.moving_bicyclist       = struct('x',idx_x(find(labels == 253)),'y', idx_y(find(labels == 253)) );
data_labeled.moving_person          = struct('x',idx_x(find(labels == 254)),'y', idx_y(find(labels == 254)) );
data_labeled.moving_motorcyclist    = struct('x',idx_x(find(labels == 255)),'y', idx_y(find(labels == 255)) );
data_labeled.moving_on_rails        = struct('x',idx_x(find(labels == 256)),'y', idx_y(find(labels == 256)) );
data_labeled.moving_bus             = struct('x',idx_x(find(labels == 257)),'y', idx_y(find(labels == 257)) );
data_labeled.moving_truck           = struct('x',idx_x(find(labels == 258)),'y', idx_y(find(labels == 258)) );
data_labeled.moving_other_vehicle   = struct('x',idx_x(find(labels == 259)),'y', idx_y(find(labels == 259)) );

%% Plot the range image by depth
norm3 = @(x,y,z) norm([x,y,z]);
range = zeros(size(x));
for i = 1:length(x)
    range(i) = norm3(x(i),y(i),z(i));
end
max_range = max(range);
color_factor = 256 / max_range;
% figure
ax1 = subplot(2,1,1);
scatter(idx_x, -idx_y, 10, round(color_factor*range))
xlabel('x')
ylabel('y')
title({'Range Image by Range','Sequence 01 000000'})
axis equal

%% Plot the range image by classification
% figure 
ax2 = subplot(2,1,2);
hold on
colors = distinguishable_colors(34);

fnames = fieldnames(data_labeled);
used_fnames3 = {};
for i = 1:length(fieldnames(data_labeled))
    if ~isempty(data_labeled.(fnames{i}).x)
        used_fnames3{end+1} = fnames{i};
        scatter(data_labeled.(fnames{i}).x, -data_labeled.(fnames{i}).y, 10, colors(i,:));
    end
end
xlabel('x')
ylabel('y')
title({'Range Image by Label','Sequence 01 000000'})
axis equal
linkaxes([ax1,ax2], 'xy')