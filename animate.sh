./ising -a -s 1000000000 -g -o "animationRun"

gnuplot -e "filename='animationRun/spins.dat" animate.gp 