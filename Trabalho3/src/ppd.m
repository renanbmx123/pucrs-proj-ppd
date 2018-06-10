x= 4:4:32
su_bub= [1.266380 2.951709 4.404753 5.989174 6.168365 6.370211 6.458801 6.483555]
ef_bub= [0.316595 0.368964 0.367063 0.374323 0.308418 0.265425 0.230671 0.202611]
su_q= [1.792433 3.807778 4.081270 4.200765 4.137171 4.276391 3.948507 3.960099]
ef_q= [0.448108 0.475972 0.340106 0.262548 0.206859 0.178183 0.141018 0.123753]
plot_su_bub2 = plot(x,su_bub,'--r','marker','o','markerfacecolor','r','markersize',10);
hold on
plot_su_q2 = plot(x,su_q,'--b','marker','o','markerfacecolor','b','markersize',10);
labelx_h = xlabel('Número de processos','fontsize',25);
labelx_h = ylabel('Speed-Up','fontsize',25);
legend_h = legend('Speed-Up Bubble Sort', 'Speed-Up Quick Sort', 'location','southeast','orientation','horizontal');
figure
plot_ef_bub2 = plot(x,ef_bub,'--r','marker','o','markerfacecolor','r','markersize',10);
hold on
plot_ef_q2 = plot(x,ef_q,'--b','marker','o','markerfacecolor','b','markersize',10);
labelx_h = xlabel('Número de processos','fontsize',25);
labelx_h = ylabel('Eficiencia','fontsize',25);
legend_h = legend('Eficiencia Bubble Sort', 'Eficiencia Quick Sort','location','southwest','orientation','horizontal');