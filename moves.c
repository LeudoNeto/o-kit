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
    short i, j;
    short solucao_vizinha[dimensao_cidades+1];
    double custo_solucao_vizinha;
    bool nova_solucao = 0;

    for (i = 1; i < dimensao_cidades; i++) {
        for (j = i+1; j < dimensao_cidades; j++) {
            copia_array(solucao_vizinha, solucao, dimensao_cidades+1);
            
            short temp = solucao_vizinha[i];
            solucao_vizinha[i] = solucao_vizinha[j];
            solucao_vizinha[j] = temp;

            custo_solucao_vizinha = custo_solucao(solucao_vizinha, dimensao_cidades, distMatrix);
            if (custo_solucao_vizinha < *custo_solucao_atual) {
                copia_array(solucao, solucao_vizinha, dimensao_cidades+1);
                *custo_solucao_atual = custo_solucao_vizinha;
                nova_solucao = 1;
            }

        }
    }
    return nova_solucao;
}

bool two_opt(short *solucao, short dimensao_cidades, double *custo_solucao_atual, double **distMatrix) {
    short i, j, temp, o;
    short solucao_vizinha[dimensao_cidades+1];
    double custo_solucao_vizinha;
    bool nova_solucao = 0;

    for (i = 1; i < dimensao_cidades - 3; i++) {
        for (j = i+3; j < dimensao_cidades; j++) {
            copia_array(solucao_vizinha, solucao, dimensao_cidades+1);
            for (o = 0; o < (j-i+1)/2; o++) {
                temp = solucao_vizinha[i+o];
                solucao_vizinha[i+o] = solucao_vizinha[j-o];
                solucao_vizinha[j-o] = temp;
            }

            custo_solucao_vizinha = custo_solucao(solucao_vizinha, dimensao_cidades, distMatrix);
            
            if (custo_solucao_vizinha < *custo_solucao_atual) {
                copia_array(solucao, solucao_vizinha, dimensao_cidades+1);
                *custo_solucao_atual = custo_solucao_vizinha;
                nova_solucao = 1;
            }
        }
    }
    return nova_solucao;
}

bool or_opt(short *solucao, short dimensao_cidades, double *custo_solucao_atual, double **distMatrix, short opt_type) {
    short i, j, temp, o;
    short solucao_vizinha[dimensao_cidades+1];
    double custo_solucao_vizinha;
    bool nova_solucao = 0;

    for (i = 1; i < dimensao_cidades - (1 + 2*opt_type); i++) {
        for (j = i+1; j < dimensao_cidades-opt_type; j++) {
            copia_array(solucao_vizinha, solucao, dimensao_cidades+1);
            short temp_vertices[opt_type+1];

            for (temp = 0; temp <= opt_type; temp++) {
                temp_vertices[temp] = solucao[i+temp];
            }
            
            for (o = i; o < j; o++) {
                solucao_vizinha[o] = solucao_vizinha[o+opt_type+1];
            }

            for (temp = 0; temp <= opt_type; temp++) {
                solucao_vizinha[o+temp] = temp_vertices[temp];
            }
            
            custo_solucao_vizinha = custo_solucao(solucao_vizinha, dimensao_cidades, distMatrix);
            
            if (custo_solucao_vizinha < *custo_solucao_atual) {
                copia_array(solucao, solucao_vizinha, dimensao_cidades+1);
                *custo_solucao_atual = custo_solucao_vizinha;
                nova_solucao = 1;
            }
        }
    }
    return nova_solucao;
}

