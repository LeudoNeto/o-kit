#include "Data.h"
#include "etapas.cpp"


#define max_iter 50
#define max_iter_ils 50

using namespace std;

Solution ILS(int maxIter, int maxIterILS, int dimensao_cidades, double **distMatrix) {
    Solution solucao_final;
    solucao_final.cost = INFINITY;
    for (int i = 0; i < maxIter; i++) {
        Solution solucao = Construcao(dimensao_cidades, distMatrix);
    }
    return solucao_final;
}

int main(int argc, char** argv) {
    auto data = Data(argc, argv[1]);
    data.read();
    size_t dimensao_cidades = data.getDimension();
    double **distMatrix = data.distMatrix;
    int i, j, temp, o;

    srand(time(NULL));
    
    clock_t inicio = clock();

    Solution solucao_final = ILS(max_iter, max_iter_ils, dimensao_cidades, distMatrix);

    printf("Tempo de execução: %lf\n", (double)(clock() - inicio)/ CLOCKS_PER_SEC);

    std::cout << "Sequência final: ";
    for (size_t i = 0; i < solucao_final.sequence.size(); ++i) {
        std::cout << solucao_final.sequence[i];
        if (i != solucao_final.sequence.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;



    return 0;    
}