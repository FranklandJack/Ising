./ising -a -T 2.2 -s 100 -g -o "animationRun" &

gnuplot -e "filename='animationRun/spins.dat" animate.gp 