for temp in $(seq 1 0.1 3);
do
	./ising -T $temp -k -b 10000
done
