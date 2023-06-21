#include "Data.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "etapas.c"

using namespace std;

void preenche_sequencia_array(short *array_destino, short *array_origem, short size, short pos_inicial) {
    for (int i = 0; i < size; i++) {
        array_destino[pos_inicial + i] = array_origem[i];
    }
}

bool InArray(short *array, short num) {
    for (short i = 0; array[i] ; i++) {
        if (num == array[i]) {
            return 1;
        }
    }
    return 0;
}

double custo_insercao(short *sol, double **distMatrix) {
    double custo = 0;
    for (short i = 1; sol[i]; i++) {
        custo += distMatrix[sol[i-1]-1][sol[i]-1];
    }
    return custo;
}

void inserir_cidade(short *sol, short size, short num, short pos) {
    for (short i = size - 1; i > pos; i--) {
        sol[i] = sol[i-1];
    }
    sol[pos] = num;
}

int main(int argc, char** argv) {

    auto data = Data(argc, argv[1]);
    data.read();
    size_t dimensao_cidades = data.getDimension();
    short i, j, temp, o;
    
    clock_t inicio = clock();

    double **distMatrix = data.distMatrix;

    /* CONSTRUÇÃO */
    short solucao[dimensao_cidades+1] = { 1, 1 };
    
    short sol_insercao[dimensao_cidades+1];
    float custo_inserido;
    float menor_custo = INFINITY;
    short cidade_menor;
    short pos_cidade_menor;
    
    while (!solucao[dimensao_cidades-1]) {
        for (i = 2; i <= dimensao_cidades; i++) {
            if (!InArray(solucao, i)) {
                for (j = 1; solucao[j]; j++) {
                    copia_array(sol_insercao, solucao, dimensao_cidades+1);
                    inserir_cidade(sol_insercao, dimensao_cidades+1, i, j);
                    custo_inserido = custo_insercao(sol_insercao, distMatrix);
                    if (custo_inserido < menor_custo) {
                        menor_custo = custo_inserido;
                        cidade_menor = i;
                        pos_cidade_menor = j;
                    }
                }
            }
        }
        printf("%hi at %hi, custo %.2lf \n", cidade_menor, pos_cidade_menor, menor_custo);
        inserir_cidade(solucao, dimensao_cidades+1, cidade_menor, pos_cidade_menor);
        menor_custo = INFINITY;
    }
    
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
        swap(solucao, melhor_solucao, dimensao_cidades, custo_melhor_solucao, distMatrix);
        
        /* 2-OPT */
        two_opt(solucao, melhor_solucao, dimensao_cidades, custo_melhor_solucao, distMatrix);
        
        /* REINSERTION */
        or_opt(solucao, melhor_solucao, dimensao_cidades, custo_melhor_solucao, distMatrix, 0);
        
        /* OR-OPT-2 */
        or_opt(solucao, melhor_solucao, dimensao_cidades, custo_melhor_solucao, distMatrix, 1);
        
        /* OR-OPT-3 */
        or_opt(solucao, melhor_solucao, dimensao_cidades, custo_melhor_solucao, distMatrix, 2);

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

                short t1 = rand() % (dimensao_cidades/10) + 2;
                short t2 = rand() % (dimensao_cidades/10) + 2;
                short pos1 = rand() % (dimensao_cidades - t1 - t2 - 1) + 1;
                short pos2 = rand() % (dimensao_cidades - t2 - pos1 - t1) + pos1 + t1;

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
                    preenche_sequencia_array(melhor_solucao, ar2, t2, pos1);
                    preenche_sequencia_array(melhor_solucao, ar1, t1, pos2);
                }
                else if (t1 > t2) {
                    short diferenca = t1 - t2;

                    for (i = pos1 + t2; i < pos2; i++) {
                        melhor_solucao[i] = melhor_solucao[i + diferenca];
                    }

                    preenche_sequencia_array(melhor_solucao, ar2, t2, pos1);
                    preenche_sequencia_array(melhor_solucao, ar1, t1, pos2-diferenca);
                }
                else {
                    short diferenca = t2 - t1;

                    for (i = pos2 + diferenca - 1; i >= pos1 + t2; i--) {
                        melhor_solucao[i] = melhor_solucao[i - diferenca];
                    }

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
                    
                    printf("Solução antes de perturbar: ");
                    for (i = 0; i < dimensao_cidades; i++) {
                        printf("%hi -> ", melhor_solucao[i]);
                    }
                    printf("1\n");

                    short t1 = rand() % (dimensao_cidades/10) + 2;
                    short t2 = rand() % (dimensao_cidades/10) + 2;
                    short pos1 = rand() % (dimensao_cidades - t1 - t2 - 1) + 1;
                    short pos2 = rand() % (dimensao_cidades - t2 - pos1 - t1) + pos1 + t1;

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
                        preenche_sequencia_array(melhor_solucao, ar2, t2, pos1);
                        preenche_sequencia_array(melhor_solucao, ar1, t1, pos2);
                    }
                    else if (t1 > t2) {
                        short diferenca = t1 - t2;

                        for (i = pos1 + t2; i < pos2; i++) {
                            melhor_solucao[i] = melhor_solucao[i + diferenca];
                        }

                        preenche_sequencia_array(melhor_solucao, ar2, t2, pos1);
                        preenche_sequencia_array(melhor_solucao, ar1, t1, pos2-diferenca);
                    }
                    else {
                        short diferenca = t2 - t1;

                        for (i = pos2 + diferenca - 1; i >= pos1 + t2; i--) {
                            melhor_solucao[i] = melhor_solucao[i - diferenca];
                        }

                        preenche_sequencia_array(melhor_solucao, ar1, t1, pos2 + diferenca);
                        preenche_sequencia_array(melhor_solucao, ar2, t2, pos1);
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

    return 0;
}