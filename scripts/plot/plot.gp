set terminal 'png'
set output 'time.png'
plot "transpose_time.dat" using 1:2 with lines title 'naive',\
     "transpose_time.dat" using 1:3 with lines title 'sse',\
     "transpose_time.dat" using 1:4 with lines title 'sse_prefetch'
