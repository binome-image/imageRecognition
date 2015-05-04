clear
reset
unset key
# Give the bars a plain fill pattern, and draw a solid line around them.
set style fill solid border

set xtics font "Bold,8"
set ytics font "Bold,8"
set term pdfcairo
set output "watch-4_scale_ind6.pdf"
set title "Indicateur 6 en fonction de l'échelle"
set xlabel "Echelle"
set ylabel "Valeur de l'indicateur 6"
set pointsize 2
set style fill solid

plot "watch-4_scale_ind6.dat" using 1:2 title columnheader(2),
