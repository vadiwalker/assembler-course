nasm -f elf64 ../src/mem8.asm -o mem8.o;
nasm -f elf64 ../src/mem16.asm -o mem16.o;
g++ -O2 -o app ../src/main.cpp mem8.o mem16.o;
./app;
