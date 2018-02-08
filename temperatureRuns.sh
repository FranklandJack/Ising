for i in `seq 10 30`;
do
	temp=$(python -c "print($i/10.0)")
	./ising -T $temp
done