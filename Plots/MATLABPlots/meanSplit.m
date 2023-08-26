function dataMatrix = meanSplit(data, n)
dataMatrix(:,1) = data(:,1);
for i = 1:size(data, 1)
    F = [];
    R = [];
    M = [];
    j = 2;
    while j < size(data,2)
        F = [F, data(i, j)];
        R = [R, data(i, j+1)];
        if n > 2
            M = [M, data(i, j+2)];
        end
        j = j + n;
    end
dataMatrix(i,2) = mean(F);
dataMatrix(i,3) = mean(R);
if n > 2
    dataMatrix(i,4) = mean(M);
end
end
end
