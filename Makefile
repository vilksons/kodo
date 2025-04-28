CC = gcc
CFLAGS = -fPIE -D_GNU_SOURCE -g -O2 -pipe -s -flto
LDFLAGS = -lm -lcurl -lncurses -lreadline -larchive -Wl,-O1

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