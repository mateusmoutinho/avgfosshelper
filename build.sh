
gcc -c dtw.c -o dtw.o
gcc -c sha-256.c -o sha256.o
gcc  main.c dtw.o sha256.o  -o main.out