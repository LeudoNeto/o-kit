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

void preenche_sequencia_array(short *array_destino, short *array_origem, short size, short pos_inicial) {
    for (int i = 0; i < size; i++) {
        puts("começo:");
        printf("%hi\n", array_origem[i]);
        printf("%hi\n", array_destino[pos_inicial + i]);
        array_destino[pos_inicial + i] = array_origem[i];
        puts("fim:");
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
        for (i = 1; i < dimensao_cidades - 3; i++) {
            for (j = i+3; j < dimensao_cidades; j++) {
                copia_array(solucao_vizinha, solucao, dimensao_cidades+1);
                for (o = 0; o < (j-i+1)/2; o++) {
                    temp = solucao_vizinha[i+o];
                    solucao_vizinha[i+o] = solucao_vizinha[j-o];
                    solucao_vizinha[j-o] = temp;
                }

                custo_solucao_vizinha = custo_solucao(solucao_vizinha, dimensao_cidades, distMatrix);
                
                if (custo_solucao_vizinha < custo_melhor_solucao) {
                    copia_array(melhor_solucao, solucao_vizinha, dimensao_cidades+1);
                    custo_melhor_solucao = custo_solucao_vizinha;
                    nova_solucao = 1;
                }
            }
        }
        
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

                short t1 = (rand() % dimensao_cidades/10) + 2;
                short t2 = (rand() % dimensao_cidades/10) + 2;
                short pos1 = (rand() % dimensao_cidades - t1 - t2 - 1) + 1;
                short pos2 = (rand() % dimensao_cidades - t2 - pos1 - t1) + pos1 + t1;

                printf("%hi %hi %hi %hi\n", t1, t2, pos1, pos2);

                short ar1[t1], ar2[t2];

                for (i = 0; i < t1; i++) {
                    ar1[i] = melhor_solucao[pos1 + i];
                }

                for (i = 0; i < t2; i++) {
                    ar2[i] = melhor_solucao[pos2 + i];
                }

                printf("%hi, %hi/n", ar1[0], ar2[0]);

                if (t1 == t2) {
                    short ar_temp[t1];
                    copia_array(ar_temp, ar1, t1);
                    preenche_sequencia_array(melhor_solucao, ar2, t2, pos1);
                    preenche_sequencia_array(melhor_solucao, ar_temp, t1, pos2);
                }
                else if (t1 > t2) {
                    short ar_temp[t1];
                    short diferenca = t2 - t1;
                    copia_array(ar_temp, ar1, t1);

                    for (i = pos1 + diferenca; i < pos2; i++) {
                        melhor_solucao[i - diferenca] = melhor_solucao[i];
                    }

                    preenche_sequencia_array(melhor_solucao, ar2, t2, pos1);
                    preenche_sequencia_array(melhor_solucao, ar1, t1, pos2 - (t1 - t2));
                }
                else {
                    short ar_temp[t2];
                    short diferenca = t2 - t1;
                    copia_array(ar_temp, ar2, t2);

                    puts("Copiou o array.");

                    for (i = pos2 + diferenca - 1; i >= pos1 + t2; i--) {
                        melhor_solucao[i - diferenca] = melhor_solucao[i];
                    }
                    puts("Passou os elementos pra esquerda");

                    preenche_sequencia_array(melhor_solucao, ar1, t1, pos2 + diferenca);
                    preenche_sequencia_array(melhor_solucao, ar2, t2, pos1);
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
            else {
                printf("Custo da solução antiga: %lf\nCusto da solução perturbada final: %lf\n", custo_solucao_final, custo_melhor_solucao);
                if (custo_melhor_solucao < custo_solucao_final) {
                    puts("A solução perturbada foi melhor que a antiga.");
                    copia_array(solucao_final, melhor_solucao, dimensao_cidades+1);
                    custo_solucao_final = custo_melhor_solucao;
                    
                    for (i = dimensao_cidades/3; i < dimensao_cidades/3 + 5; i++) {
                        temp = melhor_solucao[i];
                        melhor_solucao[i] = melhor_solucao[dimensao_cidades - dimensao_cidades/5 + i];
                        melhor_solucao[dimensao_cidades - dimensao_cidades/5 + i] = temp;
                    }
                    
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

    double custo = 0;
    for (short i = 0; i < dimensao_cidades; i++) {
        printf("Distância %hi -> %hi: %.2lf === ", solucao_final[i], solucao_final[i+1], distMatrix[solucao_final[i]-1][solucao_final[i+1]-1]);
        custo += distMatrix[solucao_final[i]-1][solucao_final[i+1]-1];
        printf("Distância total: %.2lf\n", custo);
    }
    return custo;

    return 0;
}