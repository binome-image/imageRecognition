clear
reset
unset key
# Give the bars a plain fill pattern, and draw a solid line around them.
set style fill solid border

set xtics font "Bold,8"
set ytics font "Bold,8"
set term pdfcairo
set output "sea_snake-14_bruit_ind5.pdf"
set title "Intervalle résultat pour l'indicateurs 8"
set xlabel "Classes"
set ylabel "Valeur de l'indicateur 8"
set pointsize 2
set style fill solid

plot "sea_snake-14_bruit_ind5.dat" using 1:2 title columnheader(2),
