СС = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
LDFLAGS = -lncurses
SRC = brick_game/tetris/backend.c \
	brick_game/main.c \
	gui/cli/ui.c
OBJ = $(SRC:.c=.o)
BINDIR = /usr/local/bin
DESTDIR ?= /


DOC_SRC = dvi/Documentation.tex
DVI_OUTPUT_DIR = dvi/docs/
PDF_OUTPUT_DIR = dvi/docs/

ifeq ($(OS),Linux)
	TEST_LIBS = -lsubunit -lrt -lm -pthread
endif
ifeq ($(OS),Darwin)
	TEST_LIBS = 
endif

all: Tetris

Tetris: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

install: Tetris
	mkdir -p $(DESTDIR)$(BINDIR)
	install -m 755 Tetris $(DESTDIR)$(BINDIR)/Tetris

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/Tetris

clean:
	rm -f $(OBJ) Tetris high_score.txt $(TEST_OBJ) dvi/docs/* tetris.tar.gz

dvi: $(DOC_SRC)
	latex $(DOC_SRC)
	mv Documentation.aux $(DVI_OUTPUT_DIR)
	mv Documentation.log $(DVI_OUTPUT_DIR)
	mv Documentation.dvi $(DVI_OUTPUT_DIR)

pdf: $(DOC_SRC)
	pdflatex $(DOC_SRC)
	mv Documentation.pdf $(PDF_OUTPUT_DIR)
	mv Documentation.aux $(PDF_OUTPUT_DIR)
	mv Documentation.log $(PDF_OUTPUT_DIR)

dist: install
	tar -czvf tetris.tar.gz Makefile brick_game gui KA_diagram.png

style:
	cp ../materials/linters/.clang-format ./
	clang-format -i brick_game/tetris/backend.c
	clang-format -i brick_game/tetris/backend.h
	clang-format -i gui/cli/ui.c
	clang-format -i gui/cli/ui.h
	clang-format -i brick_game/main.c
	clang-format -i brick_game/tetris/unit_tests/tests.c
	rm .clang-format

check:
	cp ../materials/linters/.clang-format ./
	clang-format -n brick_game/tetris/backend.c
	clang-format -n brick_game/tetris/backend.h
	clang-format -n gui/cli/ui.c
	clang-format -n gui/cli/ui.h
	clang-format -n brick_game/main.c
	clang-format -n brick_game/tetris/unit_tests/tests.c
	rm .clang-format

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all install uninstall clean dvi pdf dist style check