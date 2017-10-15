# Compilador
CC = gcc

# Arquivo principal
OBJ_FILE = main.c

# Arquivos auxiliares
AUX_FILES = register.c

# Flags do compilador
CC_FLAGS = -g -Wall

# Arquivo de saída
OUT_FILE = ./archiver

all:
	$(CC) $(OBJ_FILE) $(AUX_FILES) $(CC_FLAGS) -o $(OUT_FILE)

clean:
	rm $(OUT_FILE)
