CC = gcc
CFLAGS = -fPIE -D_GNU_SOURCE -g -Os -s
LDFLAGS = -lm -lcurl -lncurses -lreadline -larchive

TARGET = kodo

SRCS = kodo.c utils.c package.c server.c tomlc99/toml.c cJson/cJSON.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
