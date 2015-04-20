[[ $# -ne  2 && $# -ne 3 ]] && echo "USAGE:./short_contactmap.sh input.n_contact out.n_contact [distance_between_bin]" && exit

bin_thre=0
i_f=$1
o_f=$2

echo "Input:"$i_f
echo "output:"$o_f

if [ $# == 3 ]; then
    bin_thre=$3
    echo "distance between bins:"$bin_thre
fi
if [ $# == 3 ]; then
	awk -v thre=$bin_thre '{if (($2-$1)>=0 && ($2-$1)<thre) print}' $i_f > $o_f
else
	awk '{if (($2-$1)>=0) print}' $i_f > $o_f
fi
