PYTEST=$(wildcard test_*.py)
PYMOD=$(patsubst %.py,%,$(PYTEST))
pytest: $(PYTEST) 
	python -m unittest $(PYMOD)

PROJ=..
CFLAGS=-I$(PROJ) -O0 -g
mytest: $(PROJ)/common.h $(PROJ)/cpv.c mytest.c
	$(CC) -o $@ $(PROJ)/cpv.c mytest.c $(CFLAGS)

