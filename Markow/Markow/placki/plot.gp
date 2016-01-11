set title 'uzytecznosci' font ",20"
set nokey
call "col_counter.gp" "data.dat"
plot for [i=2:col_count] "data.dat" u 1:i w l title columnheader(i) at end
