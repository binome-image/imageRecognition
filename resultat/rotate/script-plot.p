clear
reset
unset key
# Give the bars a plain fill pattern, and draw a solid line around them.
set style fill solid border

set xtics font "Bold,8"
set ytics font "Bold,8"
set term pdfcairo
set output "camel-11_rotate_ind2.pdf"
set title "Indicateur 2 en fonction de l'angle"
set xlabel "Angle"
set ylabel "Valeur de l'indicateur 2"
set pointsize 2
set style fill solid

plot "camel-11_rotate_ind2.dat" using 1:2 title columnheader(2),
