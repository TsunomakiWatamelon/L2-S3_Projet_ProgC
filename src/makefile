all: chips_vs_virus
chips_vs_virus: chips_vs_virus.o main.o
    clang -std=c17 chips_vs_virus.o -o chip_vs_virus main.o
chips_vs_virus.o: chips_vs_virus.c
    clang -std=c17 -Wall -Wfatal-errors chips_vs_virus.c -o chips_vs_virus.o 

main.o: main.c chips_vs_virus.h
    clang -std=c17 -Wall -Wfatal-errors main.c -o main.o 

clean:
    rm -rf *.o

mrproper: clean
    rm -rf chips_vs_virus