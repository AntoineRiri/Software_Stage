listDICO = list_DICO(PATH);
path = 'DICO';
path = fullfile(PATH, path);
for i = 1:length(listDICO)

    dico = listDICO(i);
    
    folder = sprintf('DICO%d', dico);
    folder = fullfile(path, folder);

    folderOpen = fullfile(folder,'Open');
    openData = CSVRead(folderOpen);
    openData = meanSplit(openData,2);
    openData = extrapolPin(openData, dico,PATH);

    folderLoad = fullfile(folder, 'Load');
    loadData = CSVRead(folderLoad);
    loadData = meanSplit(loadData,2);
    loadData = extrapolPin(loadData, dico,PATH);

    %folderMeter = fullfile(folder, 'RFmeter');
    %RFData = CSVRead(folderMeter);
    %RFData = meanSplit(RFData,3);
    %RFData = extrapolPin(RFData, dico,PATH);

    
    Plot(openData, dico, 1);
    Plot(loadData, dico, 2);
    %Plot(RFData, dico, 3);
  
end



