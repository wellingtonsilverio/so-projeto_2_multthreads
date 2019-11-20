# Projeto 2 - Build Matrix - Sistemas Operacionais TT304A

## Autores

### Deepin

- Andre Sacilotto [Andre S](https://github.com/231396)
- Renan Douglas Holanda [HolandaDEV](https://github.com/HolandaDEV)
- Wellington Silverio [wellingtonsilverio](https://github.com/wellingtonsilverio)

## Compilar e rodar

### Compilar o projeto

`gcc main.c -o main -lpthread`

### Executar o projeto

2 threads
`./main 2 arq1.dat arq2.dat arq3.dat arq4.dat arq5.dat arq6.dat arq7.dat -o saida.dat`
4 threads
`./main 4 arq1.dat arq2.dat arq3.dat arq4.dat arq5.dat arq6.dat arq7.dat -o saida.dat`
8 threads
`./main 8 arq1.dat arq2.dat arq3.dat arq4.dat arq5.dat arq6.dat arq7.dat -o saida.dat`
16 threads
`./main 16 arq1.dat arq2.dat arq3.dat arq4.dat arq5.dat arq6.dat arq7.dat -o saida.dat`
32 threads
`./main 32 arq1.dat arq2.dat arq3.dat arq4.dat arq5.dat arq6.dat arq7.dat -o saida.dat`
