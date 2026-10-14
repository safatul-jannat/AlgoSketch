# AlgoSketch Makefile
# Supports both Windows (win_flex/win_bison) and Linux (flex/bison).
#
# On Windows, invoke via build.bat, or:
#   make BISON=tools/win_bison.exe FLEX=tools/win_flex.exe
# On Linux:
#   make

CC      = gcc
CFLAGS  ?= -O2 -Wall

BISON   ?= bison
FLEX    ?= flex

ifeq ($(OS),Windows_NT)
DELCMD   := del /Q
GEN_FILES := src\parser.c src\parser.h src\lexer.c
else
DELCMD   := rm -f
GEN_FILES := src/parser.c src/parser.h src/lexer.c
endif

SRC      = src
OUT      = algosketch

CSRCS    = $(SRC)/main.c \
           $(SRC)/ast.c \
           $(SRC)/error.c \
           $(SRC)/symbol_table.c \
           $(SRC)/semantic.c \
           $(SRC)/tac.c \
           $(SRC)/codegen.c \
           $(SRC)/parser.c \
           $(SRC)/lexer.c

HDRS     = $(SRC)/ast.h \
           $(SRC)/error.h \
           $(SRC)/parser.h \
           $(SRC)/lexer.h \
           $(SRC)/symbol_table.h \
           $(SRC)/semantic.h \
           $(SRC)/tac.h \
           $(SRC)/codegen.h

.PHONY: all clean

all: $(OUT)

$(SRC)/parser.c $(SRC)/parser.h: $(SRC)/parser.y
	$(BISON) -d -o $(SRC)/parser.c $(SRC)/parser.y

$(SRC)/lexer.c: $(SRC)/lexer.l $(SRC)/parser.h
	$(FLEX) -o $(SRC)/lexer.c $(SRC)/lexer.l

$(OUT): $(CSRCS) $(HDRS)
	$(CC) $(CFLAGS) -I$(SRC) -o $(OUT) $(CSRCS)

clean:
	-$(DELCMD) $(OUT)
	-$(DELCMD) $(GEN_FILES)
