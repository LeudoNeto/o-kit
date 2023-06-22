#include "moves.c"

bool InArray(short *array, short num) {
    for (short i = 0; array[i] ; i++) {
        if (num == array[i]) {
            return 1;
        }
    }
    return 0;
}

// double custo_insercao(short *sol, double **distMatrix) {
//     double custo = 0;
//     for (short i = 1; sol[i]; i++) {
//         custo += distMatrix[sol[i-1]-1][sol[i]-1];
//     }
//     return custo;
// }

void inserir_cidade(short *sol, short size, short num, short pos) {
    for (short i = size; i > pos; i--) {
        sol[i] = sol[i-1];
    }
    sol[pos] = num;
}

void inserir3aleatorios(short *solucao, short dimensao_cidades) {
    short i = 3;
    while(i--) {
        short aleatorio = (rand() % (dimensao_cidades - 2)) + 2;
        inserir_cidade(solucao, dimensao_cidades+1, aleatorio, 1);
    }
}

void preenche_sequencia_array(short *array_destino, short *array_origem, short size, short pos_inicial) {
    for (int i = 0; i < size; i++) {
        array_destino[pos_inicial + i] = array_origem[i];
    }
}

typedef struct {
    short index_insercao;
    short cidade_insercao;
    double custo_insercao;
} InsertionInfo;

InsertionInfo custo_insercao(short *sol, short num, short index, double **distMatrix) {
    InsertionInfo insercao;

    insercao.index_insercao = index;
    insercao.cidade_insercao = num;
    insercao.custo_insercao = - distMatrix[sol[index-1]-1][sol[index]-1]
                              + distMatrix[sol[index-1]-1][num-1]
                              + distMatrix[num-1][sol[index]-1];

    return insercao;
}

void ordernarCustosInsercao(InsertionInfo *custos, short size) {
    double menor_custo;
    short pos_menor;
    int i, j;
    InsertionInfo temp;

    for (i = 0; i < size-1; i++) {
        pos_menor = i;
        menor_custo = custos[i];
        for (j = i+1; j < size; j++) {
            if (custos[j] < menor_custo) {
                pos_menor = j;
            }
        }
        if (pos_menor != i) {
            temp = custos[i];
            custos[i] = custos[pos_menor];
            custos[pos_menor] = temp;
        }
    }

}

void Construcao(short *solucao, short dimensao_cidades, double **distMatrix) {
    short i, j;
    int l, selecionado;
    short sol_insercao[dimensao_cidades+1];
    float custo_inserido;
    float menor_custo = INFINITY;
    short cidade_menor;
    short pos_cidade_menor;
    double alpha;
    int tamanho_array_custos = TAMANHO;

    srand(time(NULL));
    inserir3aleatorios(solucao);

    while(!solucao[dimensao_cidades]) {
        l = 0;
        InsertionInfo custos[tamanho_array_custos];
        for (i = 2; i <= dimensao_cidades; i++) {
            if (!InArray(solucao, i)) {
                for (j = 1; solucao[j] && j < dimensao_cidades; j++) {
                    InsertionInfo insercao_info = custo_insercao(solucao, i, j, distMatrix);
                    custos[l] = insercao_info;
                    l++;
                }
            }
        }
        ordernarCustosInsercao(custos, tamanho_array_custos);
        alpha = (double) rand() / RAND_MAX;
        selecionado = rand() % ((int) ceil(alpha * tamanho_array_custos));
        inserir_cidade(solucao, dimensao_cidades+1, custos[selecionado].cidade_insercao, custos[selecionado].index_insercao);
    }
    
}

// void Construcao(short *solucao, short dimensao_cidades, double **distMatrix) {
//     short i, j;
//     short sol_insercao[dimensao_cidades+1];
//     float custo_inserido;
//     float menor_custo = INFINITY;
//     short cidade_menor;
//     short pos_cidade_menor;
    
//     while (!solucao[dimensao_cidades]) {
//         for (i = 2; i <= dimensao_cidades; i++) {
//             if (!InArray(solucao, i)) {
//                 for (j = 1; solucao[j]; j++) {
//                     copia_array(sol_insercao, solucao, dimensao_cidades+1);
//                     inserir_cidade(sol_insercao, dimensao_cidades+1, i, j);
//                     custo_inserido = custo_insercao(sol_insercao, distMatrix);
//                     if (custo_inserido < menor_custo) {
//                         menor_custo = custo_inserido;
//                         cidade_menor = i;
//                         pos_cidade_menor = j;
//                     }
//                 }
//             }
//         }
//         // printf("%hi at %hi, custo %.2lf \n", cidade_menor, pos_cidade_menor, menor_custo);
//         inserir_cidade(solucao, dimensao_cidades+1, cidade_menor, pos_cidade_menor);
//         menor_custo = INFINITY;
//     }
// }

// void Construcao(short *solucao, short dimensao_cidades, double **distMatrix) {
//     short i, j;
//     short sol_insercao[dimensao_cidades+1];
//     float custo_inserido;
//     float menor_custo = INFINITY;
//     short pos_cidade_menor;

//     short cidades[dimensao_cidades-1];
//     for (i = 2; i <= dimensao_cidades; i++) {
//         cidades[i-2] = i;
//     }

//     srand(time(NULL));
//     for (short i = dimensao_cidades-2; i > 0; i--) {
//         short j = rand() % (i+1);
        
//         // Troca os elementos de posição usando uma variável temporária
//         short temp = cidades[i];
//         cidades[i] = cidades[j];
//         cidades[j] = temp;
//     }
    
//     for (i = 2; i <= dimensao_cidades; i++) {
//         for (j = 1; solucao[j]; j++) {
//             copia_array(sol_insercao, solucao, dimensao_cidades+1);
//             inserir_cidade(sol_insercao, dimensao_cidades+1, cidades[i-2], j);
//             custo_inserido = custo_insercao(sol_insercao, distMatrix);
//             if (custo_inserido < menor_custo) {
//                 menor_custo = custo_inserido;
//                 pos_cidade_menor = j;
//             }
//         }
//         inserir_cidade(solucao, dimensao_cidades+1, cidades[i-2], pos_cidade_menor);
//         menor_custo = INFINITY;
//     }

// }

void Construcao(short *solucao, short dimensao_cidades, double **distMatrix) {
    solucao[dimensao_cidades] = 1;

    for (int i = 2; i <= dimensao_cidades; i++) {
        solucao[i-1] = i;
    }

    srand(time(NULL));
    for (int i = dimensao_cidades - 1; i > 0; i--) {
        int j = (rand() % i) + 1;
        
        // Troca os elementos de posição usando uma variável temporária
        int temp = solucao[i];
        solucao[i] = solucao[j];
        solucao[j] = temp;
    }
}

void BuscaLocal(short *solucao, short dimensao_cidades, double *custo_solucao_atual, double **distMatrix) {
    bool nova_solucao = 0, continuar = 1;

    srand(time(NULL));
    while(continuar) {
        continuar = 0;
        short ordem[5] = {1, 2, 3, 4, 5};

        // Randomiza a ordem das funções
        for (short i = 4; i > 0; i--) {
            short j = rand() % (i + 1);
            short temp = ordem[i];
            ordem[i] = ordem[j];
            ordem[j] = temp;
        }

        // printf("%hi %hi %hi %hi %hi\n", ordem[0], ordem[1], ordem[2], ordem[3], ordem[4]);

        for (short i = 0; i < 5; i++) {
            switch (ordem[i]) {
                case 1:
                    nova_solucao = swap(solucao, dimensao_cidades, custo_solucao_atual, distMatrix);
                    break;
                case 2:
                    nova_solucao = two_opt(solucao, dimensao_cidades, custo_solucao_atual, distMatrix);
                    break;
                case 3:
                    nova_solucao = or_opt(solucao, dimensao_cidades, custo_solucao_atual, distMatrix, 0);
                    break;
                case 4:
                    nova_solucao = or_opt(solucao, dimensao_cidades, custo_solucao_atual, distMatrix, 1);
                    break;
                case 5:
                    nova_solucao = or_opt(solucao, dimensao_cidades, custo_solucao_atual, distMatrix, 2);
                    break;
            }

            // printf("%hi executado e retornou %d\n", ordem[i], nova_solucao);

            if (nova_solucao) {
                continuar = 1;
                break;
            }
        }

    }

    // puts("Nenhuma fez melhoria.");
}

void Perturbacao(short *melhor_solucao, short dimensao_cidades) {
    short i;

    short t1 = rand() % (dimensao_cidades/10) + 2;
    short t2 = rand() % (dimensao_cidades/10) + 2;
    short pos1 = rand() % (dimensao_cidades - t1 - t2 - 1) + 1;
    short pos2 = rand() % (dimensao_cidades - t2 - pos1 - t1) + pos1 + t1;

    // printf("%hi %hi %hi %hi\n", t1, t2, pos1, pos2);

    short ar1[t1], ar2[t2];

    for (i = 0; i < t1; i++) {
        ar1[i] = melhor_solucao[pos1 + i];
    }

    for (i = 0; i < t2; i++) {
        ar2[i] = melhor_solucao[pos2 + i];
    }

    // printf("%hi, %hi/n", ar1[0], ar2[0]);

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
}