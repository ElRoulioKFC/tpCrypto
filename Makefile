FLAGS = -std=c99 -Wall -Wextra -pedantic -Werror -O4
# FLAGS = -std=c99 -Wall -Wextra -pedantic -Werror -Wno-unused-parameter -O4
# FLAGS = -std=c99 -Wall -Wextra -Werror -pedantic -O0 -pg -no-pie
LFLAGS = -lcrypto -lssl -lm

EXE = exeMignon

GCC = gcc

FILES = utils.c config.c functions.c hash.c test.c stats.c tables.c brute-force.c crack.c main.c launcher.c
O_FILES = $(FILES:.c=.o)

all: $(EXE)

%.o: %.c test.h
	$(GCC) $(FLAGS) -c $<

$(EXE): $(O_FILES)
	$(GCC) $(FLAGS) $(O_FILES) -o $(EXE) $(LFLAGS)

clean:
	rm -f *.o a.out gmon.out

veryclean: clean
	rm -f $(EXE)

.PHONY: all clean veryclean
