function dataMatrix = CSVRead(folder)
listDir = dir(folder);
cmpt = 1;
for i = 1:length(listDir)
    fileName = listDir(i).name;
    path = fullfile(folder, fileName);
    if ~listDir(i).isdir
        if path(end-2:end) == 'csv'
            data = readmatrix(path);
            if cmpt == 1
                dataMatrix(:,cmpt) = data(:,1);
                cmpt = cmpt + 1;
            end
            s = size(data,2) - 1;
            dataMatrix(:,cmpt:cmpt + s - 1) = data(:,2:end);
            cmpt = cmpt + s;
        end
    end
end
end




















