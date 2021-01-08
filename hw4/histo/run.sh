# Mingxiang Dai (md382), Jiawei Sun (js896)
if [ "$1" = "histo" ]
then
    ./histo "$2" > histo_result.out
else
    OMP_NUM_THREADS="$3" ./"$1" "$2" > "$1"_result.out
fi