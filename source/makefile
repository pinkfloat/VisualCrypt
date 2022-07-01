PROGRAM = visualCrypt

src = $(wildcard *.c)
obj = $(src:.c=.o)

CC=gcc

LDLIBS += -lm
CFLAGS += -Wall -Wextra -pedantic-errors

release: CFLAGS += -O3
release: $(PROGRAM)

debug: LDLIBS += -fsanitize=address
debug: CFLAGS += -O0 -fsanitize=address
debug: $(PROGRAM)

$(PROGRAM): $(obj)

run:
	./$(PROGRAM)

.PHONY: clean
clean:
	rm -f $(obj) $(PROGRAM)
