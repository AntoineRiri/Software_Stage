function P = extrapolPin(data, dico, PATH_)
label = DICOtoLabel(dico);
n = size(data,2) - 1;
P(:,1) = data(:,1);
path = 'Calibration';
path = fullfile(PATH_, path);
folder1 = sprintf('Calibration%s', label(1));
folder1 = fullfile(path, folder1);
folder2 = sprintf('Calibration%s', label(2));
folder2 = fullfile(path, folder2);
levelListFwd = CSVRead(folder1);
levelListRev = CSVRead(folder2);
if n > 2
    folder3 = sprintf('Calibration%s', label(3));
    folder3 = fullfile(path, folder3);
    levelListRF = CSVRead(folder3);
end
for i = 1:size(data(:,1))
    P(i,2) = level(levelListFwd, data(i,2));
    P(i,3) = level(levelListRev, data(i,3));
    if n > 2
        P(i,4) = level(levelListRF, data(i,4));
    end
end
end
