CFLAGS = -msse2 --std gnu99 -O0 -Wall -Wextra

GIT_HOOKS := .git/hooks/applied

all: $(GIT_HOOKS) main.c
	$(CC) $(CFLAGS) -o main main.c

test:
	$(CC) $(CFLAGS) main.c -DNAIVE -o naive
	$(CC) $(CFLAGS) main.c -DSSE -o sse
	$(CC) $(CFLAGS) main.c -DSSE_PREFETCH -o sse_prefetch

data: test
	./scripts/plot/gendata.sh	

plot: data	
	gnuplot scripts/plot/plot.gp

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

clean:
	$(RM) main
	rm -f naive sse sse_prefetch time.png transpose_time.dat
