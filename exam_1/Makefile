RM = rm -f
CC = gcc
#C++ = g++
#CFLAGS = -I/work/barrier_breaker/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/include
#CFLAGS += -I/work/barrier_breaker/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/include/libxml2
#LDFLAGS = -L/work/barrier_breaker/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/lib
LDFLAGS += -lxml2

1:1.c
	$(CC) -g -o 1 1.c -I /usr/include/libxml2/ $(LDFLAGS) $(CFLAGS)
	$(CC) -g -o 2 2.c -I /usr/include/libxml2/ $(LDFLAGS) $(CFLAGS)
	$(CC) -g -o test test.c -I /usr/include/libxml2/ $(LDFLAGS) $(CFLAGS)

clean:
	$(RM) 1 2 test

