function plotMat = meanSTD(data)
for i = 1:size(data,1)
    plotMat(i,1) = data(i,1);
    plotMat(i,2) = mean(data(i,2:end));
    plotMat(i,3) = std(data(i,2:end));
end
end