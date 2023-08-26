function DICO_calib = DICOtoLabel(i)
list = ['A','B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'];
DICO_calib = list(2*i-1:2*i);
if i <= 4
    DICO_calib = [DICO_calib, list(2*i-1+8)];
else
    DICO_calib = [DICO_calib, list(2*i-1-8)];
end