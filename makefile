CC=gcc
CFLAGS=-g -Wall
LIB_DIR=lib
SRC_DIR=src
INC_DIR=include
TEST_DIR=test

# Fichiers de la librairie
LIBRARY=$(LIB_DIR)/libfs.a
OBJECTS=$(SRC_DIR)/func.o

# Fichier de test
TEST_SRC=$(TEST_DIR)/main.c
TEST_BIN=$(TEST_DIR)/test_program

# Cibles
all: $(LIBRARY) $(TEST_BIN)

$(LIBRARY): $(OBJECTS)
	ar rcs $(LIBRARY) $(OBJECTS)

$(OBJECTS): $(SRC_DIR)/func.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/func.c -o $(SRC_DIR)/func.o

$(TEST_BIN): $(TEST_SRC) $(LIBRARY)
	$(CC) $(CFLAGS) -I$(INC_DIR) $(TEST_SRC) -L$(LIB_DIR) -lfs -o $(TEST_BIN)

clean:
	rm -f $(SRC_DIR)/*.o $(LIBRARY) $(TEST_BIN)

run: $(TEST_BIN)
	./$(TEST_BIN)

