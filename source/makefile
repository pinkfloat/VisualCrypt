PROGRAMM = main

src = $(wildcard *.c)
obj = $(src:.c=.o)

CC=gcc

LDLIBS += -lm
CFLAGS += -Wall -Wextra -pedantic-errors

release: CFLAGS += -O3
release: $(PROGRAMM)

debug: LDLIBS += -fsanitize=address
debug: CFLAGS += -O0 -fsanitize=address
debug: $(PROGRAMM)

all: release

run: all
	./$(PROGRAMM)

$(PROGRAMM): $(obj)

.PHONY: clean
clean:
	rm -f $(obj) $(PROGRAMM)
