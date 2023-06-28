#include "Data.h"
#include "etapas.cpp"


#define max_iter 50
#define max_iter_ils 50

using namespace std;

Solution ILS(int maxIter, int maxIterILS, int dimensao_cidades, double **c) {
    Solution solucao_final;
    solucao_final.cost = INFINITY;
    clock_t inicio = clock();
    
    for (int i = 0; i < maxIter; i++) {
        cout << "Comecou a construcao em: " << (double)(clock() - inicio)/ CLOCKS_PER_SEC << endl;

        Solution solucao = Construcao(dimensao_cidades, c);
        Solution melhor_solucao = solucao;

        cout << "Terminou a construcao em: " << (double)(clock() - inicio)/ CLOCKS_PER_SEC << endl;

        // cout << "Sequencia construida: ";
        // for (int i = 0; i < solucao.sequence.size(); i++) {
        //     cout << solucao.sequence[i] << " ";
        // }
        // cout << endl;
        // cout << "Custo da sequencia construida: " << solucao.cost << endl;

        int iterILS = 0;
        while (iterILS < maxIterILS) {

            // cout << "Sequencia antes da busca local: ";
            // for (int i = 0; i < solucao.sequence.size(); i++) {
            //     cout << solucao.sequence[i] << " ";
            // }
            // cout << endl;
            // cout << "Custo da sequencia: " << solucao.cost << endl;
            cout << "Comecou a busca local em: " << (double)(clock() - inicio)/ CLOCKS_PER_SEC << endl;
            BuscaLocal(solucao, c);
            cout << "Terminou a busca local em: " << (double)(clock() - inicio)/ CLOCKS_PER_SEC << endl;

            // cout << "Sequencia depois da busca local: ";
            // for (int i = 0; i < solucao.sequence.size(); i++) {
            //     cout << solucao.sequence[i] << " ";
            // }
            // cout << endl;
            // cout << "Custo da sequencia: " << solucao.cost << endl;

            if (solucao.cost < melhor_solucao.cost) {
                melhor_solucao = solucao;
                iterILS = 0;
            }

            // cout << "Sequencia antes da perturbacao: ";
            // for (int i = 0; i < melhor_solucao.sequence.size(); i++) {
            //     cout << melhor_solucao.sequence[i] << " ";
            // }
            // cout << endl;
            // cout << "Custo da sequencia: " << melhor_solucao.cost << endl;

            cout << "Comecou a perturbacao em: " << (double)(clock() - inicio)/ CLOCKS_PER_SEC << endl;
            solucao = Perturbacao(melhor_solucao, c);
            cout << "Terminou a perturbacao em: " << (double)(clock() - inicio)/ CLOCKS_PER_SEC << endl;

            // cout << "Sequencia depois da perturbacao: ";
            // for (int i = 0; i < solucao.sequence.size(); i++) {
            //     cout << solucao.sequence[i] << " ";
            // }
            // cout << endl;
            // cout << "Custo da sequencia: " << solucao.cost << endl;

            iterILS++;
        }
        if (melhor_solucao.cost < solucao_final.cost) {
            solucao_final = melhor_solucao;
        }
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

    cout << "Custo final: " << solucao_final.cost << endl;



    return 0;    
}