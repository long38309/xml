RM = rm -f
CC = gcc
#C++ = g++
#CFLAGS = -I/work/barrier_breaker/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/include
#CFLAGS += -I/work/barrier_breaker/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/include/libxml2
#LDFLAGS = -L/work/barrier_breaker/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/lib
LDFLAGS += -lxml2

glcli:glcli.c
	$(CC) -g -o glcli glcli.c -I /usr/include/libxml2/ $(LDFLAGS) $(CFLAGS)

clean:
	$(RM) glcli

