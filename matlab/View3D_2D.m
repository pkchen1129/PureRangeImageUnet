%% Looking at point cloud .bin data
clc, clear
% fid = fopen('data/000000.bin', 'r');
sequence = '00';
datanum = '000001';
datapath = ['/media/thomas/Samsung_T51/_data/KITTI_SegmentedData/' ...
            'KITTI_Segmantic_Segmentation/dataset/sequences/' ...
            sequence, '/velodyne'];
fid = fopen([datapath, '/' datanum '.bin'],'r');
data = fread(fid, '*float32');
fclose(fid);

num = length(data)/4;
x = zeros(num,1);
y = zeros(num,1);
z = zeros(num,1);
r = zeros(num,1);
for i = 1:num
   x(i) = data(4*(i-1)+1);
   y(i) = data(4*(i-1)+2);
   z(i) = data(4*(i-1)+3);
   r(i) = data(4*(i-1)+4);
end

%% Looking at labels
% fid = fopen('data/000000.label', 'r');
labelpath =  ['/media/thomas/Samsung_T51/_data/KITTI_SegmentedData/' ...
              'KITTI_Segmantic_Segmentation/dataset/sequences/' ...
              sequence, '/labels_uint32_range_images'];
fid = fopen([labelpath, '/' datanum '.label'],'r');
labels = fread(fid, '*int32');
fclose(fid);

%% Create label structures
data_labeled.unlabeled              = struct('x',x(find(labels == 0)),'y', y(find(labels == 0)), 'z', z(find(labels == 0)));
data_labeled.outlier                = struct('x',x(find(labels == 1)),'y', y(find(labels == 1)), 'z', z(find(labels == 1)));
data_labeled.car                    = struct('x',x(find(labels == 10)),'y', y(find(labels == 10)), 'z', z(find(labels == 10)));
data_labeled.bicycle                = struct('x',x(find(labels == 11)),'y', y(find(labels == 11)), 'z', z(find(labels == 11)));
data_labeled.bus                    = struct('x',x(find(labels == 13)),'y', y(find(labels == 13)), 'z', z(find(labels == 13)));
data_labeled.motorcycle             = struct('x',x(find(labels == 15)),'y', y(find(labels == 15)), 'z', z(find(labels == 15)));
data_labeled.on_rails               = struct('x',x(find(labels == 16)),'y', y(find(labels == 16)), 'z', z(find(labels == 16)));
data_labeled.truck                  = struct('x',x(find(labels == 18)),'y', y(find(labels == 18)), 'z', z(find(labels == 18)));
data_labeled.other_vehicle          = struct('x',x(find(labels == 20)),'y', y(find(labels == 20)), 'z', z(find(labels == 20)));
data_labeled.person                 = struct('x',x(find(labels == 30)),'y', y(find(labels == 30)), 'z', z(find(labels == 30)));
data_labeled.bicyclist              = struct('x',x(find(labels == 31)),'y', y(find(labels == 31)), 'z', z(find(labels == 31)));
data_labeled.motorcyclist           = struct('x',x(find(labels == 32)),'y', y(find(labels == 32)), 'z', z(find(labels == 32)));
data_labeled.road                   = struct('x',x(find(labels == 40)),'y', y(find(labels == 40)), 'z', z(find(labels == 40)));
data_labeled.parking                = struct('x',x(find(labels == 44)),'y', y(find(labels == 44)), 'z', z(find(labels == 44)));
data_labeled.sidewalk               = struct('x',x(find(labels == 48)),'y', y(find(labels == 48)), 'z', z(find(labels == 48)));
data_labeled.other_ground           = struct('x',x(find(labels == 49)),'y', y(find(labels == 49)), 'z', z(find(labels == 49)));
data_labeled.building               = struct('x',x(find(labels == 50)),'y', y(find(labels == 50)), 'z', z(find(labels == 50)));
data_labeled.fence                  = struct('x',x(find(labels == 51)),'y', y(find(labels == 51)), 'z', z(find(labels == 51)));
data_labeled.other_structure        = struct('x',x(find(labels == 52)),'y', y(find(labels == 52)), 'z', z(find(labels == 52)));
data_labeled.lane_marking           = struct('x',x(find(labels == 60)),'y', y(find(labels == 60)), 'z', z(find(labels == 60)));
data_labeled.vegetation             = struct('x',x(find(labels == 70)),'y', y(find(labels == 70)), 'z', z(find(labels == 70)));
data_labeled.trunk                  = struct('x',x(find(labels == 71)),'y', y(find(labels == 71)), 'z', z(find(labels == 71)));
data_labeled.terrain                = struct('x',x(find(labels == 72)),'y', y(find(labels == 72)), 'z', z(find(labels == 72)));
data_labeled.pole                   = struct('x',x(find(labels == 80)),'y', y(find(labels == 80)), 'z', z(find(labels == 80)));
data_labeled.traffic_sign           = struct('x',x(find(labels == 81)),'y', y(find(labels == 81)), 'z', z(find(labels == 81)));
data_labeled.other_object           = struct('x',x(find(labels == 99)),'y', y(find(labels == 99)), 'z', z(find(labels == 99)));
data_labeled.moving_car             = struct('x',x(find(labels == 252)),'y', y(find(labels == 252)), 'z', z(find(labels == 252)));
data_labeled.moving_bicyclist       = struct('x',x(find(labels == 253)),'y', y(find(labels == 253)), 'z', z(find(labels == 253)));
data_labeled.moving_person          = struct('x',x(find(labels == 254)),'y', y(find(labels == 254)), 'z', z(find(labels == 254)));
data_labeled.moving_motorcyclist    = struct('x',x(find(labels == 255)),'y', y(find(labels == 255)), 'z', z(find(labels == 255)));
data_labeled.moving_on_rails        = struct('x',x(find(labels == 256)),'y', y(find(labels == 256)), 'z', z(find(labels == 256)));
data_labeled.moving_bus             = struct('x',x(find(labels == 257)),'y', y(find(labels == 257)), 'z', z(find(labels == 257)));
data_labeled.moving_truck           = struct('x',x(find(labels == 258)),'y', y(find(labels == 258)), 'z', z(find(labels == 258)));
data_labeled.moving_other_vehicle   = struct('x',x(find(labels == 259)),'y', y(find(labels == 259)), 'z', z(find(labels == 259)));
%% Color Specifications
colorstring = {'Black', 'Blue', 'BlueViolet', 'Brown', 'Cyan', 'DarkBlue', ...
    'DarkGreen', 'DarkMagenta', 'DarkSalmon', 'DarkYellow', 'Gray', 'Green', ...
    'LightBlue', 'LightSeaGreen', 'Magenta', 'Orange', 'PaleYellow', 'Purple',...
    'Red', 'SmahtGuide', 'White', 'Yellow'};
colors = distinguishable_colors(34);
%% 3D Plot
fnames = fieldnames(data_labeled);
used_fnames3 = {};
figure 
hold on
for i = 1:length(fieldnames(data_labeled))
    if ~isempty(data_labeled.(fnames{i}).x)
        used_fnames3{end+1} = fnames{i};
        plot3(data_labeled.(fnames{i}).x, data_labeled.(fnames{i}).y, data_labeled.(fnames{i}).z, '*', 'Color', colors(i,:))
    end
end
xlabel('x')
ylabel('y')
zlabel('z')
legend(used_fnames3{1:length(used_fnames3)})
axis equal
title('Ground Truth')


%% PREDICTED
% fid = fopen('data/000000.bin', 'r');
sequence = '04';
datanum = '000000';
datapath = ['/media/thomas/Samsung_T51/_data/KITTI_SegmentedData/' ...
            'KITTI_Segmantic_Segmentation/dataset/sequences/' ...
            sequence, '/predicted/velodyne'];
fid = fopen([datapath, '/' datanum '.bin'],'r');
data = fread(fid, '*float32');
fclose(fid);

num = length(data)/4;
x = zeros(num,1);
y = zeros(num,1);
z = zeros(num,1);
r = zeros(num,1);
for i = 1:num
   x(i) = data(4*(i-1)+1);
   y(i) = data(4*(i-1)+2);
   z(i) = data(4*(i-1)+3);
   r(i) = data(4*(i-1)+4);
end

%% Looking at labels
% fid = fopen('data/000000.label', 'r');
labelpath =  ['/media/thomas/Samsung_T51/_data/KITTI_SegmentedData/' ...
              'KITTI_Segmantic_Segmentation/dataset/sequences/' ...
              sequence, '/predicted/labels'];
fid = fopen([labelpath, '/' datanum '.label'],'r');
labels = fread(fid, '*int8');
fclose(fid);

%% Create label structures
data_labeled.unlabeled              = struct('x',x(find(labels == 0)),'y', y(find(labels == 0)), 'z', z(find(labels == 0)));
data_labeled.outlier                = struct('x',x(find(labels == 1)),'y', y(find(labels == 1)), 'z', z(find(labels == 1)));
data_labeled.car                    = struct('x',x(find(labels == 2)),'y', y(find(labels == 2)), 'z', z(find(labels == 2)));
data_labeled.bicycle                = struct('x',x(find(labels == 3)),'y', y(find(labels == 3)), 'z', z(find(labels == 3)));
data_labeled.bus                    = struct('x',x(find(labels == 4)),'y', y(find(labels == 4)), 'z', z(find(labels == 4)));
data_labeled.motorcycle             = struct('x',x(find(labels == 5)),'y', y(find(labels == 5)), 'z', z(find(labels == 5)));
data_labeled.on_rails               = struct('x',x(find(labels == 6)),'y', y(find(labels == 6)), 'z', z(find(labels == 6)));
data_labeled.truck                  = struct('x',x(find(labels == 7)),'y', y(find(labels == 7)), 'z', z(find(labels == 7)));
data_labeled.other_vehicle          = struct('x',x(find(labels == 8)),'y', y(find(labels == 8)), 'z', z(find(labels == 8)));
data_labeled.person                 = struct('x',x(find(labels == 9)),'y', y(find(labels == 9)), 'z', z(find(labels == 9)));
data_labeled.bicyclist              = struct('x',x(find(labels == 10)),'y', y(find(labels == 10)), 'z', z(find(labels == 10)));
data_labeled.motorcyclist           = struct('x',x(find(labels == 11)),'y', y(find(labels == 11)), 'z', z(find(labels == 11)));
data_labeled.road                   = struct('x',x(find(labels == 12)),'y', y(find(labels == 12)), 'z', z(find(labels == 12)));
data_labeled.parking                = struct('x',x(find(labels == 13)),'y', y(find(labels == 13)), 'z', z(find(labels == 13)));
data_labeled.sidewalk               = struct('x',x(find(labels == 14)),'y', y(find(labels == 14)), 'z', z(find(labels == 14)));
data_labeled.other_ground           = struct('x',x(find(labels == 15)),'y', y(find(labels == 15)), 'z', z(find(labels == 15)));
data_labeled.building               = struct('x',x(find(labels == 16)),'y', y(find(labels == 16)), 'z', z(find(labels == 16)));
data_labeled.fence                  = struct('x',x(find(labels == 17)),'y', y(find(labels == 17)), 'z', z(find(labels == 17)));
data_labeled.other_structure        = struct('x',x(find(labels == 18)),'y', y(find(labels == 18)), 'z', z(find(labels == 18)));
data_labeled.lane_marking           = struct('x',x(find(labels == 19)),'y', y(find(labels == 19)), 'z', z(find(labels == 19)));
data_labeled.vegetation             = struct('x',x(find(labels == 20)),'y', y(find(labels == 20)), 'z', z(find(labels == 20)));
data_labeled.trunk                  = struct('x',x(find(labels == 21)),'y', y(find(labels == 21)), 'z', z(find(labels == 21)));
data_labeled.terrain                = struct('x',x(find(labels == 22)),'y', y(find(labels == 22)), 'z', z(find(labels == 22)));
data_labeled.pole                   = struct('x',x(find(labels == 23)),'y', y(find(labels == 23)), 'z', z(find(labels == 23)));
data_labeled.traffic_sign           = struct('x',x(find(labels == 24)),'y', y(find(labels == 24)), 'z', z(find(labels == 24)));
data_labeled.other_object           = struct('x',x(find(labels == 25)),'y', y(find(labels == 25)), 'z', z(find(labels == 25)));
data_labeled.moving_car             = struct('x',x(find(labels == 26)),'y', y(find(labels == 26)), 'z', z(find(labels == 26)));
data_labeled.moving_bicyclist       = struct('x',x(find(labels == 27)),'y', y(find(labels == 27)), 'z', z(find(labels == 27)));
data_labeled.moving_person          = struct('x',x(find(labels == 28)),'y', y(find(labels == 28)), 'z', z(find(labels == 28)));
data_labeled.moving_motorcyclist    = struct('x',x(find(labels == 29)),'y', y(find(labels == 29)), 'z', z(find(labels == 29)));
data_labeled.moving_on_rails        = struct('x',x(find(labels == 30)),'y', y(find(labels == 30)), 'z', z(find(labels == 30)));
data_labeled.moving_bus             = struct('x',x(find(labels == 31)),'y', y(find(labels == 31)), 'z', z(find(labels == 31)));
data_labeled.moving_truck           = struct('x',x(find(labels == 32)),'y', y(find(labels == 32)), 'z', z(find(labels == 32)));
data_labeled.moving_other_vehicle   = struct('x',x(find(labels == 33)),'y', y(find(labels == 33)), 'z', z(find(labels == 33)));
%% Color Specifications
colorstring = {'Black', 'Blue', 'BlueViolet', 'Brown', 'Cyan', 'DarkBlue', ...
    'DarkGreen', 'DarkMagenta', 'DarkSalmon', 'DarkYellow', 'Gray', 'Green', ...
    'LightBlue', 'LightSeaGreen', 'Magenta', 'Orange', 'PaleYellow', 'Purple',...
    'Red', 'SmahtGuide', 'White', 'Yellow'};
colors = distinguishable_colors(34);
%% 3D Plot
fnames = fieldnames(data_labeled);
used_fnames3 = {};
figure 
hold on
for i = 1:length(fieldnames(data_labeled))
    if ~isempty(data_labeled.(fnames{i}).x)
        used_fnames3{end+1} = fnames{i};
        plot3(data_labeled.(fnames{i}).x, data_labeled.(fnames{i}).y, data_labeled.(fnames{i}).z, '*', 'Color', colors(i,:))
    end
end
xlabel('x')
ylabel('y')
zlabel('z')
legend(used_fnames3{1:length(used_fnames3)})
axis equal
title('Predicted')
%%
% %% 2D Plot
% used_fnames2 = {};
% figure
% hold on
% for i = 1:length(fieldnames(data_labeled))
%     if ~isempty(data_labeled.(fnames{i}).x)
%         image = pointcloud2image(data_labeled.(fnames{i}).x, data_labeled.(fnames{i}).y, data_labeled.(fnames{i}).z,100,1000);
%         if ~any(isnan(image),'all')
% %             if strcmp(fnames{i},'trunk')
%                 used_fnames2{end+1} = fnames{i};
%                 [row, col] = find(image ~= 1);
%                 plot(col,row,'.', 'Color', colors(i,:))
% %             end
%         end
%     end
% end
% ax = gca;
% ax.XDir = 'reverse';
% 
% legend(used_fnames2{1:length(used_fnames2)})
% axis equal
% % %%
% % figure
% % image = pointcloud2image(x,y,z,100,1000);
% % imshow(image,[])
