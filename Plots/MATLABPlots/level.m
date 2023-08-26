function P = level(list, Volt)
i = 1;
while i <= size(list,1) && list(i,2) > Volt 
    i = i + 1;
end
if i >= size(list,1)
    P = "Nan";
else
    Pb = list(i,1);
    Pa = list(i+1,1);
    Vb = list(i,2);
    Va = list(i+1,2);
    a = (Vb-Va)/(Pb-Pa);
    b = Vb - a*Pb;
    P = (Volt-b)/a;
end
end