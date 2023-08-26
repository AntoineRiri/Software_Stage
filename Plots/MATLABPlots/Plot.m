function Plot(dataMatrix, dico, n)
figure;
hold on;
label = DICOtoLabel(dico);
fTi = sprintf('Fwrd : %s', label(1));
rTi = sprintf('Rev : %s', label(2));
RfTi = sprintf('Pfwrd/Pin (dB)');
RrTi = sprintf('Prev/Pin (dB)');

if n == 1 || n == 2
    if n == 1
        Tit = sprintf('Calibration DICO %d : open', dico);
    else
        Tit = sprintf('Calibration DICO %d (separated tasks): loaded', dico);
    end
    X = dataMatrix(:,1);
    Yfwrd = dataMatrix(:,2);
    Yrev = dataMatrix(:,3);
    rfwrd = Yfwrd - X;
    rrev = Yrev - X;

    yyaxis left;
    ylabel('Pout (dBm)');
    plot(X, Yfwrd, 'DisplayName', fTi);
    plot(X, Yrev, 'DisplayName', rTi);
    plot(X, Yfwrd-Yrev,'DisplayName','Pfwrd/Prev');

    yyaxis right;
    ylabel('P/Pin (%)');
    plot(X,rfwrd, 'DisplayName', RfTi);
    plot(X,rrev, 'DisplayName',RrTi);

end
if n == 3
    tTi = sprintf('Trans : %s', label(3));
    RtTi = sprintf('Ptrans/Pin (dBm)');
    X = dataMatrix(:,1);
    Yfwrd = dataMatrix(:,2);
    Yrev = dataMatrix(:,3);
    Ytrans = dataMatrix(:,4);
    rfwrd = Yfwrd - X;
    rrev = Yrev - X;
    rTrans = Ytrans - X;

    yyaxis left;
    ylabel('Pout (dBm)');
    plot(X, Yfwrd, 'DisplayName', fTi);
    plot(X, Yrev, 'DisplayName', rTi);
    plot(X, Ytrans, 'DisplayName', tTi);

    yyaxis right;
    ylabel('P/Pin (%)');
    plot(X, rfwrd, 'DisplayName',RfTi);
    plot(X, rrev, 'DisplayName',RrTi);
    plot(X, rTrans, 'DisplayName', RtTi);
    
    Tit = sprintf('Calibration DICO %d : power out measurement', dico);
end

hold off;
grid on;
title(Tit);
xlabel('Pin (dBm)');
legend('Location','bestoutside');
end