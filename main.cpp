#include "Data.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

double custo_solucao(short *sol, short dimensao_cidades, double **distMatrix) {
    double custo = 0;
    for (short i = 0; i < dimensao_cidades; i++) {
        custo += distMatrix[sol[i]-1][sol[i+1]-1];
        
    }
    return custo;
}

void copia_array(short *array_destino, short *array_origem, short size) {
    for (short i = 0; i < size; i++) {
        array_destino[i] = array_origem[i];
    }
}

int main(int argc, char** argv) {

    auto data = Data(argc, argv[1]);
    data.read();
    size_t dimensao_cidades = data.getDimension();
    short i, j, temp, o;
    
    clock_t inicio = clock();

    double **distMatrix = data.distMatrix;

    /* Gerando uma solução com início e fim 1, e indíces aleatórios no meio */
    short solucao[dimensao_cidades+1] = { 1 };
    solucao[dimensao_cidades] = 1;
    
    for (i = 1; i < dimensao_cidades; i++) {
        solucao[i] = i+1;
    }
    
    
    for (i = dimensao_cidades-1; i > 1; i--) {
        srand(time(NULL));
        
        j = rand() % (i-1) + 1;
        temp = solucao[i];
        solucao[i] = solucao[j];
        solucao[j] = temp;
    }
    
    printf("Solução inicial: ");
    for (i = 0; i < dimensao_cidades; i++) {
        printf("%hi -> ", solucao[i]);
    }
    printf("1\n");
    
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
        for (i = 1; i < dimensao_cidades; i++) {
            for (j = i+1; j < dimensao_cidades; j++) {
                copia_array(solucao_vizinha, solucao, dimensao_cidades+1);
                
                short temp = solucao_vizinha[i];
                solucao_vizinha[i] = solucao_vizinha[j];
                solucao_vizinha[j] = temp;

                custo_solucao_vizinha = custo_solucao(solucao_vizinha, dimensao_cidades, distMatrix);
                if (custo_solucao_vizinha < custo_melhor_solucao) {
                    copia_array(melhor_solucao, solucao_vizinha, dimensao_cidades+1);
                    custo_melhor_solucao = custo_solucao_vizinha;
                    nova_solucao = 1;
                }

            }
        }
        
        /* 2-OPT */
        
        /* REINSERTION */
        for (i = 1; i < dimensao_cidades - 1; i++) {
            for (j = i+1; j < dimensao_cidades; j++) {
                copia_array(solucao_vizinha, solucao, dimensao_cidades+1);
                short vertice_reinserido = solucao[i];
                
                for (o = i; o < j; o++) {
                    solucao_vizinha[o] = solucao_vizinha[o+1];
                }
                solucao_vizinha[o] = vertice_reinserido;
                
                custo_solucao_vizinha = custo_solucao(solucao_vizinha, dimensao_cidades, distMatrix);
                
                if (custo_solucao_vizinha < custo_melhor_solucao) {
                    copia_array(melhor_solucao, solucao_vizinha, dimensao_cidades+1);
                    custo_melhor_solucao = custo_solucao_vizinha;
                    nova_solucao = 1;
                }
            }
        }
        
        /* OR-OPT-2 */
        for (i = 1; i < dimensao_cidades - 3; i++) {
            for (j = i+1; j < dimensao_cidades-1; j++) {
                copia_array(solucao_vizinha, solucao, dimensao_cidades+1);
                short vertice_reinserido1 = solucao[i];
                short vertice_reinserido2 = solucao[i+1];
                
                for (o = i; o < j; o++) {
                    solucao_vizinha[o] = solucao_vizinha[o+2];
                }
                solucao_vizinha[o] = vertice_reinserido1;
                solucao_vizinha[o+1] = vertice_reinserido2;
                
                custo_solucao_vizinha = custo_solucao(solucao_vizinha, dimensao_cidades, distMatrix);
                
                if (custo_solucao_vizinha < custo_melhor_solucao) {
                    copia_array(melhor_solucao, solucao_vizinha, dimensao_cidades+1);
                    custo_melhor_solucao = custo_solucao_vizinha;
                    nova_solucao = 1;
                }
            }
        }
        
        /* OR-OPT-3 */
        for (i = 1; i < dimensao_cidades - 5; i++) {
            for (j = i+1; j < dimensao_cidades-2; j++) {
                copia_array(solucao_vizinha, solucao, dimensao_cidades+1);
                short vertice_reinserido1 = solucao[i];
                short vertice_reinserido2 = solucao[i+1];
                short vertice_reinserido3 = solucao[i+2];
                
                for (o = i; o < j; o++) {
                    solucao_vizinha[o] = solucao_vizinha[o+3];
                }
                solucao_vizinha[o] = vertice_reinserido1;
                solucao_vizinha[o+1] = vertice_reinserido2;
                solucao_vizinha[o+2] = vertice_reinserido3;
                
                custo_solucao_vizinha = custo_solucao(solucao_vizinha, dimensao_cidades, distMatrix);
                
                if (custo_solucao_vizinha < custo_melhor_solucao) {
                    copia_array(melhor_solucao, solucao_vizinha, dimensao_cidades+1);
                    custo_melhor_solucao = custo_solucao_vizinha;
                    nova_solucao = 1;
                }
            }
        }

        /* printf("Custo solucao vizinha: %lf, Custo solucao atual: %lf\n", custo_solucao_vizinha, custo_melhor_solucao); */
        
        printf("Custo da nova solucao: %.2lf\n", custo_melhor_solucao);

        /* Se não tiver achado solução nova, mas ainda não tiver tentado uma perturbação, vai perturbar a atual */
        if (!nova_solucao && !perturbada) {
            perturbada = 1;
            copia_array(solucao_final, melhor_solucao, dimensao_cidades+1);
            custo_solucao_final = custo_melhor_solucao;
            
            for (i = dimensao_cidades/3; i < dimensao_cidades/3 + 5; i++) {
                temp = melhor_solucao[i];
                melhor_solucao[i] = melhor_solucao[dimensao_cidades - dimensao_cidades/5 + i];
                melhor_solucao[dimensao_cidades - dimensao_cidades/5 + i] = temp;
            }
            
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

    }
    
    printf("Custo da solução antiga: %lf\nCusto da solução perturbada final: %lf\n", custo_solucao_final, custo_melhor_solucao);
    if (custo_melhor_solucao < custo_solucao_final) {
        puts("A solução perturbada foi melhor que a antiga.");
        copia_array(solucao_final, melhor_solucao, dimensao_cidades+1);
        custo_solucao_final = custo_melhor_solucao;
    }
    
    printf("Solução final: ");
    for (i = 0; i < dimensao_cidades; i++) {
        printf("%hi -> ", solucao_final[i]);
    }
    printf("1\n");
    
    printf("Custo da solucao: ");
    printf("%.2lf\n", custo_solucao_final);
    printf("Tempo de execução: %.2f", (double)(clock() - inicio)/ CLOCKS_PER_SEC);

    return 0;
}