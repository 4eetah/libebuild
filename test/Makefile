PYTEST=$(wildcard test_*.py)
PYMOD=$(patsubst %.py,%,$(PYTEST))
pytest: $(PYTEST) 
	python -m unittest $(PYMOD)


SRC_DIR=..
CFLAGS=-I$(SRC_DIR) -O0 -g
MYTEST=$(wildcard mytest_*.c)
MYMOD=$(patsubst %.c,%,$(MYTEST))
SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(patsubst %.c,%.o,$(SRC))

mytest: $(MYMOD) 

%.o: %.c $(SRC_DIR)/common.h
	$(CC) -c -o $@ $< $(CFLAGS)

mytest_cpv: $(SRC) mytest_cpv.o
	$(CC) -o $@ $^ $(CFLAGS)

mytest_atom: $(SRC) mytest_atom.o
	$(CC) -o $@ $^ $(CFLAGS)

mytest_validate: $(SRC) mytest_validate.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(SRC_DIR)/*.o *.o $(MYMOD)
