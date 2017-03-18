CFLAGS = -msse2 --std gnu99 -O0 -Wall -Wextra

GIT_HOOKS := .git/hooks/applied

all: $(GIT_HOOKS) main.c
	$(CC) $(CFLAGS) -o main main.c

individual:
	$(CC) $(CFLAGS) main.c -DNAIVE -o naive
	$(CC) $(CFLAGS) main.c -DSSE -o sse
	$(CC) $(CFLAGS) main.c -DSSE_PREFETCH -o sse_prefetch

data: inividual
	./scripts/plot/gendata.sh	

plot: data	
	gnuplot scripts/plot/plot.gp

perf_data: individual
	perf stat --repeat 5 -e \
	cache-misses,cache-references,instructions,cycles \
	./naive
	perf stat --repeat 5 -e \
	cache-misses,cache-references,instructions,cycles \
	./sse
	perf stat --repeat 5 -e \
	cache-misses,cache-references,instructions,cycles \
	./sse_prefetch


$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

clean:
	$(RM) main
	rm -f naive sse sse_prefetch time.png transpose_time.dat
