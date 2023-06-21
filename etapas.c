#include "moves.c"

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

void preenche_sequencia_array(short *array_destino, short *array_origem, short size, short pos_inicial) {
    for (int i = 0; i < size; i++) {
        array_destino[pos_inicial + i] = array_origem[i];
    }
}

void Construcao(short *solucao, short dimensao_cidades, double **distMatrix) {
    short i, j;
    short sol_insercao[dimensao_cidades+1];
    float custo_inserido;
    float menor_custo = INFINITY;
    short cidade_menor;
    short pos_cidade_menor;
    
    while (!solucao[dimensao_cidades]) {
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
}

void BuscaLocal(short *solucao, short *melhor_solucao, short dimensao_cidades, double *custo_melhor_solucao, double **distMatrix) {
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

        printf("%hi %hi %hi %hi %hi\n", ordem[0], ordem[1], ordem[2], ordem[3], ordem[4]);

        for (short i = 0; i < 5; i++) {
            switch (ordem[i]) {
                case 1:
                    nova_solucao = swap(solucao, melhor_solucao, dimensao_cidades, custo_melhor_solucao, distMatrix);
                    break;
                case 2:
                    nova_solucao = two_opt(solucao, melhor_solucao, dimensao_cidades, custo_melhor_solucao, distMatrix);
                    break;
                case 3:
                    nova_solucao = or_opt(solucao, melhor_solucao, dimensao_cidades, custo_melhor_solucao, distMatrix, 0);
                    break;
                case 4:
                    nova_solucao = or_opt(solucao, melhor_solucao, dimensao_cidades, custo_melhor_solucao, distMatrix, 1);
                    break;
                case 5:
                    nova_solucao = or_opt(solucao, melhor_solucao, dimensao_cidades, custo_melhor_solucao, distMatrix, 2);
                    break;
            }

            printf("%hi executado e retornou %d\n", ordem[i], nova_solucao);

            if (nova_solucao) {
                printf("Novo custo: %.2lf\n", *custo_melhor_solucao);
                continuar = 1;
                break;
            }
        }

    }

    puts("Nenhuma fez melhoria.");
    // /* 1-SWAP */
    // if (swap(solucao, melhor_solucao, dimensao_cidades, custo_melhor_solucao, distMatrix))
    //     nova_solucao = 1;
    
    // /* 2-OPT */
    // if (two_opt(solucao, melhor_solucao, dimensao_cidades, custo_melhor_solucao, distMatrix))
    //     nova_solucao = 1;
    
    // /* REINSERTION */
    // if (or_opt(solucao, melhor_solucao, dimensao_cidades, custo_melhor_solucao, distMatrix, 0))
    //     nova_solucao = 1;
    
    // /* OR-OPT-2 */
    // if (or_opt(solucao, melhor_solucao, dimensao_cidades, custo_melhor_solucao, distMatrix, 1))
    //     nova_solucao = 1;
    
    // /* OR-OPT-3 */
    // if (or_opt(solucao, melhor_solucao, dimensao_cidades, custo_melhor_solucao, distMatrix, 2))
    //     nova_solucao = 1;
}

void Perturbacao(short *melhor_solucao, short dimensao_cidades) {
    short i;

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
}