RM = rm -f
CC = gcc
LDFLAGS += -lxml2

glcli:glcli.c
	$(CC) -g -o glcli glcli.c -I /usr/include/libxml2/ $(LDFLAGS) $(CFLAGS)

clean:
	$(RM) glcli

