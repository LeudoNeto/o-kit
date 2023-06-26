#include "Data.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "etapas.c"

#define maxIter 50
#define maxIterILS 50

using namespace std;

int main(int argc, char** argv) {

    auto data = Data(argc, argv[1]);
    data.read();
    size_t dimensao_cidades = data.getDimension();
    short i, j, temp, o;
    
    clock_t inicio = clock();

    double **distMatrix = data.distMatrix;

    short solucao_final[dimensao_cidades+1];
    double custo_solucao_final = INFINITY;

    srand(time(NULL));
    
    for (int i = 0; i < maxIter; i++) {
        short solucao[dimensao_cidades+1] = { 1, 1 };

        Construcao(solucao, dimensao_cidades, distMatrix);

        double custo_solucao_atual = custo_solucao(solucao, dimensao_cidades, distMatrix);

        printf("Solução inicial: ");
        for (int o = 0; o < dimensao_cidades; o++) {
            printf("%hi -> ", solucao[o]);
        }
        printf("%hi\n", solucao[dimensao_cidades]);
        
        printf("Custo da solucao: %.2lf\n", custo_solucao_atual);

        printf("Terminou a construcao: %.2f\n", (double)(clock() - inicio)/ CLOCKS_PER_SEC);

        short melhor_solucao[dimensao_cidades+1];
        copia_array(melhor_solucao, solucao, dimensao_cidades+1);
        double custo_melhor_solucao = custo_solucao_atual;

        int iterILS = 0;

        while (iterILS < maxIterILS) {
            BuscaLocal(solucao, dimensao_cidades, &custo_solucao_atual, distMatrix);
            if (custo_solucao_atual < custo_melhor_solucao) {
                copia_array(melhor_solucao, solucao, dimensao_cidades+1);
                custo_melhor_solucao = custo_solucao_atual;
                iterILS = 0;
            }
            else if (custo_solucao_atual > custo_melhor_solucao) {
                copia_array(solucao, melhor_solucao, dimensao_cidades+1);
                custo_solucao_atual = custo_melhor_solucao;
            }
            Perturbacao(solucao, dimensao_cidades);
            custo_solucao_atual = custo_solucao(solucao, dimensao_cidades, distMatrix);
            iterILS++;
        }

        if (custo_melhor_solucao < custo_solucao_final) {
            copia_array(solucao_final, melhor_solucao, dimensao_cidades+1);
            custo_solucao_final = custo_melhor_solucao;
        }
        printf("Custo da nova solucao: %.2lf\n", custo_melhor_solucao);
    }

    printf("Solução final: ");
    for (i = 0; i < dimensao_cidades; i++) {
        printf("%hi -> ", solucao_final[i]);
    }
    printf("%hi\n", solucao_final[i]);
    
    printf("Custo da solucao: ");
    printf("%.2lf\n", custo_solucao_final);
    printf("Tempo de execução: %.2f\n", (double)(clock() - inicio)/ CLOCKS_PER_SEC);

    return 0;
}