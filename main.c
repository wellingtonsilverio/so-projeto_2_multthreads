#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct ARRAY{
    int *matriz;
    int tamanho;
}array;

typedef struct POPULAR{
	int maxNum;
    int* matriz_cortada;
    array* arr;
} popular;

//Calcula o tempo final
double calculaTempo(struct timespec tIni, struct timespec tFim)
{
  return ((double)tFim.tv_sec + 1.0e-9 * tFim.tv_nsec) -
         ((double)tIni.tv_sec + 1.0e-9 * tIni.tv_nsec);
}

//Fun��o utilizada no qsort
int cmpFunc (const void *x, const void *y)
{
    int p = *(int*)x;
    int q = *(int*)y;
    return (p - q);
}

//Fun��o que utiliza thread para ordenar a um vetor
void* sort(void* arg) {
    array* arr = (array*)arg;

    qsort(arr->matriz, arr->tamanho, sizeof(int), cmpFunc);

    pthread_exit("Sort finalizado!");
    return NULL;
}

//Fun��o que utiliza thread para popular a matriz final
void* popularMatrix(void* arg) {
    popular* pop = (popular*)arg;

	int i;
    for (i = 0; i < pop->arr->tamanho; i++)
			pop->matriz_cortada[i] = pop->arr->matriz[i];

	for (; i < pop->maxNum; i++)
		pop->matriz_cortada[i] = 0;

    pthread_exit("Constru��o da matrix finalizado!");
    return NULL;
}

//Fun��o que mostra uma mensagem de erro
//Arg1 - numero do erro ao sair
//Arg2 - Menssagem a ser mostrada antes de sair
void thereAnError(int status, char* message) {
    if (status != 0) {
        printf("%s\n", message);
        exit(status);
    }
}

//Fun��o com a utilidade de realizar um pthread_join para todas as threads
//Arg1 - Quantidade de threads a serem resetadas e reseta seu valor ao final
//Arg2 - Vetor de threds a serem dados join
void join_Threads(int* thread_atual, pthread_t* thread_id) {
  for (int j = 0; j < *thread_atual; j++) {
      int thread_status = pthread_join(*(thread_id + j), NULL);
      thereAnError(thread_status, "Error join!");
  }
  *thread_atual = 0;
}

//Fun��o que le os arquivos de entrada
// Arg1 - arquivo file j� aberto
// Arg2 - array que ser� salvo as informa��es
void LerArquivo(FILE *file, array *arr)
{
	int contador = 0, temp;
    while (!feof (file))
    {
        fscanf (file, "%d", &temp);
        contador++;
    }

    arr->matriz = (int*) calloc(contador, sizeof(int));
    arr->tamanho = contador;

    rewin(file);
    for(int i = 0; i < arr->tamanho; i++)
        fscanf (file, "%d", &arr->matriz[i]);
}

//Salva a matriz no arquivo de sa�da
// Arg1 - arquivo file j� aberto
// Arg2 - numero de linhas da matriz
// Arg3 - numero de colunas de matriz
// Arg4 - matriz a ser usada para escrever
void SalvaMatriz_File (FILE *file, int row, int col, int matriz[row][col])
{
    if (file == NULL)
        thereAnError(1, "Erro ao criar arquivo");

    for (int i = 0; i < row; i++)
    {
        for (int e = 0; e < col; e++)
        {
            fprintf(file, "%-5d", matriz[i][e]);
        }
        fprintf(file, "\n");
    }
}

int main(int argc, char *argv[])
{
	//Declaracao das variaveis
    int i;

    FILE* file;
    pthread_t* thread_id;
    int thread_status;
    int thread_atual = 0;

    int numThreads;
    int numFiles;
    int maxNum_inFiles = 0;

    struct timespec tInicio={0,0}, tFinal={0,0};

	//Adiquirindo o numero de threads e arquivos de entrada
    numThreads = atoi(argv[1]);
	numFiles = argc-4;

	//Nome da saida
    char *outFile = argv[argc-1];

	//Nomes das entradas
    char *inFiles[numFiles];
    for(i = 0; i < numFiles; i++)
        inFiles[i] = argv[i+2];

	//Variavel para salvar os valores das entradas
	array dats[numFiles];

	//Le todas as entradas enquanto, acha a que tem a maior quantidade de inteiros
	for (i = 0; i < numFiles; i++)
	{
		file = fopen(inFiles[i], "r");
		if(file != NULL){
			LerArquivo(file, &dats[i]);
			fclose(file);
			if (dats[i].tamanho > maxNum_inFiles)
				maxNum_inFiles = dats[i].tamanho;
		}
		else
			thereAnError(-1,"Ler Arquivo");
	}

	// Inicializa a matriz final
	int matrix_resultado[numFiles][maxNum_inFiles];

	//Tempo inicial das threads
    clock_gettime(CLOCK_MONOTONIC, &tInicio);

    //alloca threads
    thread_id = malloc(numThreads * sizeof(pthread_t));

    // Realiza um qsort nas arrays obtidas anteriormente atraves de threads
    for (i = 0; i < numFiles; i++)
	  {
        thread_status = pthread_create((thread_id + thread_atual++), NULL, sort, (void*)&dats[i]);
        thereAnError(thread_status, "Error create!");

        if (thread_atual >= numThreads)
            join_Threads(&thread_atual, thread_id);
    }
    if (thread_atual > 0)
        join_Threads(&thread_atual, thread_id);

    // Popula a matriz resultado com threads
    if (numFiles > 0) {
        for (i = 0; i < numFiles; i++) {
            popular *pop = malloc(sizeof(popular));
            pop->maxNum = maxNum_inFiles;
            pop->matriz_cortada = matrix_resultado[i];
            pop->arr = &dats[i];

            thread_status = pthread_create((thread_id + thread_atual), NULL, popularMatrix, (void*)(pop));
            thread_atual++;
            thereAnError(thread_status, "Error create!");

            if (thread_atual >= numThreads){
                join_Threads(&thread_atual, thread_id);
            }
        }
    }
	if (thread_atual > 0) {
		join_Threads(&thread_atual, thread_id);
	}

	//Tempo final das threads
    clock_gettime(CLOCK_MONOTONIC, &tFinal);

    //Chama a fun��o que ir� escrever a sa�da
    file = fopen(outFile, "w");
    SalvaMatriz_File(file, numFiles, maxNum_inFiles, matrix_resultado);
    fclose(file);

    //Printa o tempo da aplica��o
    printf("Tempo com thread %d: %lf ms\n", numThreads, calculaTempo(tInicio, tFinal));

    return 0;
}
