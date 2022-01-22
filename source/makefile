PROGRAMM = main

src = $(wildcard *.c)
obj = $(src:.c=.o)

CC=gcc

LDLIBS += -lm -fsanitize=address
CFLAGS += -Wall -Wextra -pedantic-errors -fsanitize=address

all: $(PROGRAMM)

run: all
	./$(PROGRAMM)

$(PROGRAMM): $(obj)

.PHONY: clean
clean:
	rm -f $(obj) $(PROGRAMM)
