errN1 = 0;
tauN1 = 0;
errN2 = 0;
tauN2 = 0;
for i = 1:30000
    errN1 = errN1 + e(i,1)^2;
    tauN1 = tauN1 + tau(i,1)^2;
    errN2 = errN2 + e(i,2)^2;
    tauN2 = tauN2 + tau(i,2)^2;
end
errN1 = sqrt(errN1);
tauN1 = sqrt(tauN1);
errN2 = sqrt(errN2);
tauN2 = sqrt(tauN2);

str = ['E1:', num2str(errN1), ' E2:', num2str(errN2), ' T1:', num2str(tauN1), ' T2:', num2str(tauN2)];
disp(str)