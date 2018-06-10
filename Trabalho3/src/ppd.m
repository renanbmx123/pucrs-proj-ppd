x= 4:4:32
y= [149.432170 64.111334 42.962223 31.596678 30.678795 29.706708 29.299245 29.187384]
z= [2.743756 1.291567 1.205017 1.170739 1.188735 1.150035 1.245534 1.241888]
plot_bub = plot(x,y,'linewidth',10);
plot_bub2 = plot(x,y,'--r','marker','o','markerfacecolor','r','markersize',10);
labelx_h = xlabel('número de processos','fontsize',25);
labely_h = ylabel('tempo em segundos','fontsize',25);
legend_h = legend('BubbleSort','location','southwest','orientation','horizontal');
figure
plot_quick = plot(x,z,'linewidth',10);
plot_quick2 = plot(x,z,'--b','marker','o','markerfacecolor','b','markersize',10);
labelx_h = xlabel('número de processos','fontsize',25);
labely_h = ylabel('tempo em segundos','fontsize',25);
legend_h = legend('QuickSort','location','southwest','orientation','horizontal');