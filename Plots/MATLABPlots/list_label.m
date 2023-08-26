function listLab = list_label(PATH_)
listLab = [];
list = ['A','B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P']; 
for i = 1:length(list)
    path = 'Calibration';
    path = fullfile(PATH_, path);
    folder = sprintf('Calibration%s', list(i));
    folder = fullfile(path, folder);
    if exist(folder,'dir') == 7
        listLab = [listLab, list(i)];
    end
end
end