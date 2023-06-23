#include <stdio.h>
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

bool swap(short *solucao, short dimensao_cidades, double *custo_solucao_atual, double **distMatrix) {
    short i, j, m_i, m_j;
    short solucao_vizinha[dimensao_cidades+1];
    double custo_solucao_vizinha, custo_swap, melhor_custo = INFINITY;
    bool nova_solucao = 0;

    for (i = 1; i < dimensao_cidades; i++) {
        for (j = i+2; j < dimensao_cidades; j++) {
            custo_swap = distMatrix[solucao[i-1]-1][solucao[j]-1] + distMatrix[solucao[i+1]-1][solucao[j]-1]
                       + distMatrix[solucao[j-1]-1][solucao[i]-1] + distMatrix[solucao[j+1]-1][solucao[i]-1]
                       - distMatrix[solucao[i-1]-1][solucao[i]-1] - distMatrix[solucao[i+1]-1][solucao[i]-1]
                       - distMatrix[solucao[j-1]-1][solucao[j]-1] - distMatrix[solucao[j+1]-1][solucao[j]-1];

            if (custo_swap < melhor_custo) {
                melhor_custo = custo_swap;
                m_i = i;
                m_j = j;
            }
        }
    }
    
    if (melhor_custo < 0) {
        short temp = solucao[m_i];
        solucao[m_i] = solucao[m_j];
        solucao[m_j] = temp;
        *custo_solucao_atual += melhor_custo;
        nova_solucao = 1;
    }
    return nova_solucao;
}

bool two_opt(short *solucao, short dimensao_cidades, double *custo_solucao_atual, double **distMatrix) {
    short i, j, m_i, m_j, temp, o;
    short solucao_vizinha[dimensao_cidades+1];
    double custo_solucao_vizinha, custo_opt, melhor_custo = INFINITY;
    bool nova_solucao = 0;

    for (i = 1; i < dimensao_cidades - 3; i++) {
        for (j = i+3; j < dimensao_cidades; j++) {

            custo_opt = - distMatrix[solucao[i-1]-1][solucao[i]-1]
                        - distMatrix[solucao[j]-1][solucao[j+1]-1]
                        + distMatrix[solucao[i-1]-1][solucao[j]-1]
                        + distMatrix[solucao[i]-1][solucao[j+1]-1];
            
            if (custo_opt < melhor_custo) {
                melhor_custo = custo_opt;
                m_i = i;
                m_j = j;
            }
        }
    }

    if (melhor_custo < 0) {
        for (o = 0; o < (m_j-m_i+1)/2; o++) {
            temp = solucao[m_i+o];
            solucao[m_i+o] = solucao[m_j-o];
            solucao[m_j-o] = temp;
        }
        *custo_solucao_atual += melhor_custo;
        nova_solucao = 1;
    }
    return nova_solucao;
}

bool or_opt(short *solucao, short dimensao_cidades, double *custo_solucao_atual, double **distMatrix, short opt_type) {
    short i, j, m_i, m_j, temp, o;
    short solucao_vizinha[dimensao_cidades+1];
    double custo_solucao_vizinha, custo_opt, melhor_custo = INFINITY;
    bool nova_solucao = 0;

    for (i = 1; i < dimensao_cidades - (1 + 2*opt_type); i++) {
        for (j = i+opt_type+2; j < dimensao_cidades-opt_type; j++) {

            custo_opt = - distMatrix[solucao[i-1]-1][solucao[i]-1]
                        - distMatrix[solucao[i+opt_type]-1][solucao[i+opt_type+1]-1]
                        - distMatrix[solucao[j+opt_type]-1][solucao[j+opt_type+1]-1]
                        + distMatrix[solucao[j+opt_type]-1][solucao[i]-1]
                        + distMatrix[solucao[i+opt_type]-1][solucao[j+opt_type+1]-1]
                        + distMatrix[solucao[i-1]-1][solucao[i+opt_type+1]-1];

            if (custo_opt < melhor_custo) {
                melhor_custo = custo_opt;
                m_i = i;
                m_j = j;
            }

        }
    }

    if (melhor_custo < 0) {
        short temp_vertices[opt_type+1];

        for (temp = 0; temp <= opt_type; temp++) {
            temp_vertices[temp] = solucao[m_i+temp];
        }
        
        for (o = m_i; o < m_j; o++) {
            solucao[o] = solucao[o+opt_type+1];
        }

        for (temp = 0; temp <= opt_type; temp++) {
            solucao[o+temp] = temp_vertices[temp];
        }

        *custo_solucao_atual += melhor_custo;
        nova_solucao = 1;
    }

    return nova_solucao;
}

