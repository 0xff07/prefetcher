#! /bin/bash
echo "no naive sse sse_prefetch" > transpose_time.dat
for i in `seq 1 50`;
do 
    printf "$i" >>  transpose_time.dat
    ./naive >>  transpose_time.dat 
    ./sse >>  transpose_time.dat
    ./sse_prefetch >>  transpose_time.dat
    printf "\n" >>  transpose_time.dat
done
