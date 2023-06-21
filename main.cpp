#include "Data.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "etapas.c"

using namespace std;

// void preenche_sequencia_array(short *array_destino, short *array_origem, short size, short pos_inicial) {
//     for (int i = 0; i < size; i++) {
//         array_destino[pos_inicial + i] = array_origem[i];
//     }
// }

int main(int argc, char** argv) {

    auto data = Data(argc, argv[1]);
    data.read();
    size_t dimensao_cidades = data.getDimension();
    short i, j, temp, o;
    
    clock_t inicio = clock();

    double **distMatrix = data.distMatrix;

    /* CONSTRUÇÃO */
    short solucao[dimensao_cidades+1] = { 1, 1 };
    
    Construcao(solucao, dimensao_cidades, distMatrix);
    
    printf("Solução inicial: ");
    for (i = 0; i < dimensao_cidades; i++) {
        printf("%hi -> ", solucao[i]);
    }
    printf("%hi\n", solucao[dimensao_cidades]);
    
    printf("Custo da solucao: ");
    
    printf("%.2lf\n", custo_solucao(solucao, dimensao_cidades, distMatrix));
    

    /* Busca local: */
    short melhor_solucao[dimensao_cidades+1];
    double custo_melhor_solucao = custo_solucao(solucao, dimensao_cidades, distMatrix);
    copia_array(melhor_solucao, solucao, dimensao_cidades+1);
    
    short nova_solucao = 1;
    short perturbada = 0;
    double custo_solucao_atual;
    
    short solucao_vizinha[dimensao_cidades+1];
    double custo_solucao_vizinha;
    
    short solucao_final[dimensao_cidades+1];
    double custo_solucao_final;
    while(nova_solucao) {
        nova_solucao = 0;
        copia_array(solucao, melhor_solucao, dimensao_cidades+1);
        custo_solucao_atual = custo_melhor_solucao;
        
        srand(time(NULL));
        
        /* 1-SWAP */
        if (swap(solucao, melhor_solucao, dimensao_cidades, &custo_melhor_solucao, distMatrix))
            nova_solucao = 1;
        
        /* 2-OPT */
        if (two_opt(solucao, melhor_solucao, dimensao_cidades, &custo_melhor_solucao, distMatrix))
            nova_solucao = 1;
        
        /* REINSERTION */
        if (or_opt(solucao, melhor_solucao, dimensao_cidades, &custo_melhor_solucao, distMatrix, 0))
            nova_solucao = 1;
        
        /* OR-OPT-2 */
        if (or_opt(solucao, melhor_solucao, dimensao_cidades, &custo_melhor_solucao, distMatrix, 1))
            nova_solucao = 1;
        
        /* OR-OPT-3 */
        if (or_opt(solucao, melhor_solucao, dimensao_cidades, &custo_melhor_solucao, distMatrix, 2))
            nova_solucao = 1;

        /* printf("Custo solucao vizinha: %lf, Custo solucao atual: %lf\n", custo_solucao_vizinha, custo_melhor_solucao); */
        
        printf("Custo da nova solucao: %.2lf\n", custo_melhor_solucao);

        /* Se não tiver achado solução nova, mas ainda não tiver tentado uma perturbação, vai perturbar a atual */
        if (!nova_solucao) {
            if (!perturbada) {
                perturbada = 1;
                copia_array(solucao_final, melhor_solucao, dimensao_cidades+1);
                custo_solucao_final = custo_melhor_solucao;
                

                printf("Solução antes de perturbar: ");
                for (i = 0; i < dimensao_cidades; i++) {
                    printf("%hi -> ", melhor_solucao[i]);
                }
                printf("1\n");

                Perturbacao(melhor_solucao, dimensao_cidades);
                
                custo_melhor_solucao = custo_solucao(melhor_solucao, dimensao_cidades, distMatrix);
                nova_solucao = 1;
                
                printf("Solução perturbada: ");
                for (i = 0; i < dimensao_cidades; i++) {
                    printf("%hi -> ", melhor_solucao[i]);
                }
                printf("1\n");
                
                printf("Custo da solucao perturbada inicial: ");
                printf("%.2lf\n", custo_melhor_solucao);
            }
            else {
                printf("Custo da solução antiga: %lf\nCusto da solução perturbada final: %lf\n", custo_solucao_final, custo_melhor_solucao);
                if (custo_melhor_solucao < custo_solucao_final) {
                    puts("A solução perturbada foi melhor que a antiga.");
                    copia_array(solucao_final, melhor_solucao, dimensao_cidades+1);
                    custo_solucao_final = custo_melhor_solucao;
                    
                    printf("Solução antes de perturbar: ");
                    for (i = 0; i < dimensao_cidades; i++) {
                        printf("%hi -> ", melhor_solucao[i]);
                    }
                    printf("1\n");

                    Perturbacao(melhor_solucao, dimensao_cidades);

                    custo_melhor_solucao = custo_solucao(melhor_solucao, dimensao_cidades, distMatrix);
                    nova_solucao = 1;
                    
                    printf("Nova solução perturbada: ");
                    for (i = 0; i < dimensao_cidades; i++) {
                        printf("%hi -> ", melhor_solucao[i]);
                    }
                    printf("1\n");
                    
                    printf("Custo da nova solucao perturbada: ");
                    printf("%.2lf\n", custo_melhor_solucao);
                }
                else {
                    puts("Solução perturbada não foi melhor que a antiga. Solução final encontrada.");
                }
            }
        }

    }
    
    
    printf("Solução final: ");
    for (i = 0; i < dimensao_cidades; i++) {
        printf("%hi -> ", solucao_final[i]);
    }
    printf("1\n");
    
    printf("Custo da solucao: ");
    printf("%.2lf\n", custo_solucao_final);
    printf("Tempo de execução: %.2f\n", (double)(clock() - inicio)/ CLOCKS_PER_SEC);

    return 0;
}