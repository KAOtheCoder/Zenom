figure(1);
plot(e_t, e(:,1));
ylim([-1 1])
l = xlabel("time[sec]");
set(l,'FontSize',14);
l = ylabel('e_1(t)[deg]');
set(l,'FontSize',14);
set(gcf, 'Position',  [100, 100, 750, 500])
grid on;
grid minor;
saveas(gcf,'npiPC2e1','eps')

figure(2);
plot(e_t, e(:,2));
ylim([-1 1])
l = xlabel("time[sec]");
set(l,'FontSize',14);
l = ylabel('e_2(t)[deg]');
set(l,'FontSize',14);
set(gcf, 'Position',  [100, 100, 750, 500])
grid on;
grid minor;
saveas(gcf,'npiPC2e2','eps')

figure(3);
plot(tau_t, tau(:,1));
ylim([-10 10])
l = xlabel("time[sec]");
set(l,'FontSize',14);
l = ylabel('\tau_1(t)[deg]');
set(l,'FontSize',14);
set(gcf, 'Position',  [100, 100, 750, 500])
grid on;
grid minor;
saveas(gcf,'npiPC2tau1','eps')

figure(4);
plot(tau_t, tau(:,2));
ylim([-10 10])
l = xlabel("time[sec]");
set(l,'FontSize',14);
l = ylabel('{\tau}_2(t)[deg]');
set(l,'FontSize',14);
set(gcf, 'Position',  [100, 100, 750, 500])
grid on;
grid minor;
saveas(gcf,'npiPC2tau2','eps')

figure(5);
plot(ed_t, ed(:,1));
ylim([-100 100])
l = xlabel("time[sec]");
set(l,'FontSize',14);
l = ylabel('ed_1(t)[deg]');
set(l,'FontSize',14);
set(gcf, 'Position',  [100, 100, 750, 500])
grid on;
grid minor;
saveas(gcf,'npiPC2ed1','eps')

figure(6);
plot(ed_t, ed(:,2));
ylim([-300 300])
l = xlabel("time[sec]");
set(l,'FontSize',14);
l = ylabel('ed_2(t)[deg]');
set(l,'FontSize',14);
set(gcf, 'Position',  [100, 100, 750, 500])
grid on;
grid minor;
saveas(gcf,'npiPC2ed2','eps')

figure(7);
plot(r_t, r(:,1));
ylim([-100 100])
l = xlabel("time[sec]");
set(l,'FontSize',14);
l = ylabel('r_1(t)[deg]');
set(l,'FontSize',14);
set(gcf, 'Position',  [100, 100, 750, 500])
grid on;
grid minor;
saveas(gcf,'npiPC2r1','eps')

figure(8);
plot(r_t, r(:,2));
ylim([-300 300])
l = xlabel("time[sec]");
set(l,'FontSize',14);
l = ylabel('r_2(t)[deg]');
set(l,'FontSize',14);
set(gcf, 'Position',  [100, 100, 750, 500])
grid on;
grid minor;
saveas(gcf,'npiPC2r2','eps')