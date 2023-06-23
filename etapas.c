#include "moves.c"
#include <math.h>

bool InArray(short *array, short num) {
    for (short i = 0; array[i] ; i++) {
        if (num == array[i]) {
            return 1;
        }
    }
    return 0;
}


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
        while (InArray(solucao, aleatorio)) {
            aleatorio = (rand() % (dimensao_cidades - 2)) + 2;
        }
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

void ordenarCustosInsercao(InsertionInfo *custos, short size) {
    double menor_custo;
    short pos_menor;
    int i, j;
    InsertionInfo temp;

    for (i = 0; i < size-1; i++) {
        pos_menor = i;
        menor_custo = custos[i].custo_insercao;
        for (j = i+1; j < size; j++) {
            if (custos[j].custo_insercao < menor_custo) {
                pos_menor = j;
                menor_custo = custos[j].custo_insercao;
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
    short i, j, o;
    int l, selecionado;
    short sol_insercao[dimensao_cidades+1];
    float custo_inserido;
    float menor_custo = INFINITY;
    short cidade_menor;
    short pos_cidade_menor;
    double alpha;
    int tamanho_array_custos = dimensao_cidades*dimensao_cidades/4 + 1;

    short c_restantes[dimensao_cidades];
    for (i = 1; i <= dimensao_cidades; i++) {
        c_restantes[i-1] = i;
    }

    inserir3aleatorios(solucao, dimensao_cidades);
    
    for (i = 0; i < 5; i++) {
        c_restantes[solucao[i]-1] = 0;
    }

    while(!solucao[dimensao_cidades]) {
        l = 0;
        InsertionInfo custos[tamanho_array_custos];
        for (i = 2; i <= dimensao_cidades; i++) {
            if (c_restantes[i-1]) {
                for (j = 1; solucao[j] && j < dimensao_cidades; j++) {
                    InsertionInfo insercao_info = custo_insercao(solucao, i, j, distMatrix);
                    custos[l] = insercao_info;
                    l++;
                }
            }
        }
        ordenarCustosInsercao(custos, l);
        alpha = (double) rand() / RAND_MAX;
        selecionado = rand() % ((int) ceil(alpha * l));
        inserir_cidade(solucao, dimensao_cidades+1, custos[selecionado].cidade_insercao, custos[selecionado].index_insercao);
        c_restantes[custos[selecionado].cidade_insercao - 1] = 0;
    }
    
}

void BuscaLocal(short *solucao, short dimensao_cidades, double *custo_solucao_atual, double **distMatrix) {
    bool nova_solucao = 0, continuar = 1;

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

            if (nova_solucao) {
                continuar = 1;
                break;
            }
        }

    }

}

void Perturbacao(short *sol, short dimensao_cidades) {
    short i;

    short t1 = rand() % (dimensao_cidades/10) + 2;
    short t2 = rand() % (dimensao_cidades/10) + 2;
    short pos1 = rand() % (dimensao_cidades - t1 - t2 - 1) + 1;
    short pos2 = rand() % (dimensao_cidades - t2 - pos1 - t1) + pos1 + t1;

    // printf("%hi %hi %hi %hi\n", t1, t2, pos1, pos2);

    short ar1[t1], ar2[t2];

    for (i = 0; i < t1; i++) {
        ar1[i] = sol[pos1 + i];
    }

    for (i = 0; i < t2; i++) {
        ar2[i] = sol[pos2 + i];
    }

    // printf("%hi, %hi/n", ar1[0], ar2[0]);

    if (t1 == t2) {
        preenche_sequencia_array(sol, ar2, t2, pos1);
        preenche_sequencia_array(sol, ar1, t1, pos2);
    }
    else if (t1 > t2) {
        short diferenca = t1 - t2;

        for (i = pos1 + t2; i < pos2; i++) {
            sol[i] = sol[i + diferenca];
        }

        preenche_sequencia_array(sol, ar2, t2, pos1);
        preenche_sequencia_array(sol, ar1, t1, pos2-diferenca);
    }
    else {
        short diferenca = t2 - t1;

        for (i = pos2 + diferenca - 1; i >= pos1 + t2; i--) {
            sol[i] = sol[i - diferenca];
        }

        preenche_sequencia_array(sol, ar1, t1, pos2 + diferenca);
        preenche_sequencia_array(sol, ar2, t2, pos1);
    }
}