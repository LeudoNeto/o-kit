#include "Data.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

struct InsertionInfo
{
    int noInserido;      // no k a ser inserido
    int arestaRemovida;  // aresta {i,j} na qual o no k sera inserido
    double custo;        // delta ao inserir k na aresta {i,j}
};

struct Solution
{
    std::vector<int> sequence;
    double cost;
};

std::vector<int> escolher3NosAleatorios()
{
    std::vector<int> nosAleatorios = {1, 1};
    short i = 3;
    while(i--) {
        short aleatorio = (rand() % (dimensao_cidades - 2)) + 2;
        while (InArray(solucao, aleatorio)) {
            aleatorio = (rand() % (dimensao_cidades - 2)) + 2;
        }
        nosAleatorios.insert(nosAleatorios.begin() + 1, aleatorio)
    }
    return nosAleatorios;
}

std::vector<int> nosRestantes()
{
    // Implemente a lógica para retornar os nós restantes
    // que não estão na sequência atual.
    // Exemplo:
    std::vector<int> nosRestantes = {4, 5, 6};
    return nosRestantes;
}

void ordenarEmOrdemCrescente(std::vector<InsertionInfo>& custoInsercao)
{
    // Implemente a lógica para ordenar o vetor custoInsercao
    // em ordem crescente com base no custo.
}

void inserirNaSolucao(Solution& s, int no)
{
    // Implemente a lógica para inserir o nó 'no' na solução 's'.
}

std::vector<InsertionInfo> calcularCustoInsercao(Solution& s, std::vector<int>& CL, double **c)
{
    std::vector<InsertionInfo> custoInsercao((s.sequence.size() - 1) * CL.size());

    int l = 0;
    for (int a = 0; a < s.sequence.size() - 1; a++) {
        int i = s.sequence[a];
        int j = s.sequence[a + 1];
        for (auto k : CL) {
            custoInsercao[l].custo = c[i][k] + c[j][k] - c[i][j];
            custoInsercao[l].noInserido = k;
            custoInsercao[l].arestaRemovida = a;
            l++;
        }
    }
    return custoInsercao;
}

Solution Construcao(double **c)
{
    Solution s;
    s.sequence = escolher3NosAleatorios();
    std::vector<int> CL = nosRestantes();

    while (!CL.empty()) {
        std::vector<InsertionInfo> custoInsercao = calcularCustoInsercao(s, CL, c);
        ordenarEmOrdemCrescente(custoInsercao);
        double alpha = (double)rand() / RAND_MAX;
        int selecionado = rand() % ((int)ceil(alpha * custoInsercao.size()));
        inserirNaSolucao(s, custoInsercao[selecionado].noInserido);
    }

    return s;
}

int main(int argc, char** argv)
{
    // Inicializar a semente do gerador de números aleatórios
    std::srand(std::time(0));

    auto data = Data(argc, argv[1]);
    data.read();
    size_t dimensao_cidades = data.getDimension();
    short i, j, temp, o;
    
    clock_t inicio = clock();

    double **distMatrix = data.distMatrix;

    Solution solucaoConstruida = Construcao(distMatrix);

    // Faça algo com a solução construída...

    return 0;
}
