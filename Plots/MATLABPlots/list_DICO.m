function listDICO = list_DICO(PATH_)
listDICO = [];
for i = 1:8
    path = 'DICO';
    path = fullfile(PATH_, path);
    folder = sprintf('DICO%d', i);
    folder = fullfile(path, folder);
    if exist(folder,'dir') == 7
        listDICO = [listDICO, i];
    end
end
listDICO = 7;
end