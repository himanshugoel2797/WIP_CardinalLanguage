TARGET=interpreter

CFLAGS=-std=c11 -I inc

OBJS=src/tokenizer.o src/parse_tree.o src/main.o

.phony:all

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

clean:
	rm -rf $(OBJS)
	rm -rf $(TARGET)