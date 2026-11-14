CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -pedantic
LDLIBS  = -lm
OBJS    = main.o artemis.o validaciones.o
TARGET  = artemis

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS)

%.o: %.c artemis.h validaciones.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
