clear
reset
unset key
# Give the bars a plain fill pattern, and draw a solid line around them.
set style fill solid border

set xtics font "Bold,8"
set ytics font "Bold,8"
set term pdfcairo
set output "apple-1_bruit_ind8.pdf"
set title "Indicateur 8 en fonction du bruit"
set xlabel "Bruit"
set ylabel "Valeur de l'indicateur 8"
set pointsize 2
set style fill solid

plot "apple-1_bruit_ind8.dat" using 1:2 title columnheader(2),
