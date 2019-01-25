#
# Makefile fitxategia
# Cache Simuladorea
# 

CC = gcc
INCLUDES = -Iheaders/

default: sim

sim: src/sim.c src/commands.c src/cache.c
	$(CC) -o sim src/sim.c src/commands.c src/cache.c $(INCLUDES)

sim32: src/sim.c src/commands.c src/cache.c
	$(CC) -m32 -o sim32 src/sim.c src/commands.c src/cache.c $(INCLUDES)

sim64: src/sim.c src/commands.c src/cache.c
	$(CC) -m64 -o sim64 src/sim.c src/commands.c src/cache.c $(INCLUDES)




clean: 
	$(RM) sim
