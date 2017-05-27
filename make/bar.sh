nasm -f elf64 ../src/mem1.asm -o mem1.o && 
nasm -f elf64 ../src/mem4.asm -o mem4.o && 
nasm -f elf64 ../src/mem8.asm -o mem8.o && 
nasm -f elf64 ../src/mem16.asm -o mem16.o && 
nasm -f elf64 ../src/mem16nt.asm -o mem16nt.o && 
g++ -std=c++11 -o app ../src/main.cpp mem1.o mem4.o mem8.o mem16.o mem16nt.o &&
./app;
