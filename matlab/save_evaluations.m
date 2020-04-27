%%
clc, clear
sequence = '09';
path = ['/media/thomas/Samsung_T51/bki_ws/src/BKISemanticMapping/' ...
             'data/RUNET/semantickitti_', sequence];
gt_labelpath =  [path '/labels'];
pred_labelpath =  [path '/predictions'];
for i = 1:(length(dir(gt_labelpath))-2)
    i
    datanum = sprintf('%06d',(i-1));
    %% Looking at GT labels
    fid = fopen([gt_labelpath, '/' datanum '.label'],'r');
    gt_labels = fread(fid, '*int32');
    fclose(fid);
    %% Looking at labels
    fid = fopen([pred_labelpath, '/' datanum '.label'],'r');
    pred_labels = fread(fid, '*int32');
    fclose(fid);
    %%
    fileID = fopen([path '/evaluations/' datanum '.txt'],'w');
    formatSpec = '%d %d\n';
    fprintf(fileID, formatSpec, gt_labels, pred_labels);
    fclose(fileID);
end