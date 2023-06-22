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
    int tamanho_array_custos = dimensao_cidades*dimensao_cidades;

    srand(time(NULL));
    inserir3aleatorios(solucao, dimensao_cidades);

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
        ordernarCustosInsercao(custos, l);
        alpha = (double) rand() / RAND_MAX;
        selecionado = rand() % ((int) ceil(alpha * l));
        inserir_cidade(solucao, dimensao_cidades+1, custos[selecionado].cidade_insercao, custos[selecionado].index_insercao);
    }
    
}