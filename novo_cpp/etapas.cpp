#include "moves.cpp"

struct InsertionInfo {
    int num;
    int index;
    double custo;
};

Solution escolhe3aleatorios(int dimensao_cidades, double **c) {
    Solution solucao;
    solucao.sequence.push_back(1);  // Adiciona o valor inicial

    for (int i = 0; i < 3; i++) {
        short aleatorio = (rand() % (dimensao_cidades - 2)) + 2;
        while (std::find(solucao.sequence.begin(), solucao.sequence.end(), aleatorio) != solucao.sequence.end()) {
            aleatorio = (rand() % (dimensao_cidades - 2)) + 2;
        }
        solucao.sequence.push_back(aleatorio);
    }
    
    solucao.sequence.push_back(1);  // Adiciona o valor final

    for (int i = 0; i < 4; i++) {
        solucao.cost += c[solucao.sequence[i]-1][solucao.sequence[i+1]-1];
    }

    return solucao;
}

std::vector<int> nos_restantes(Solution sol, int dimensao_cidades) {
    std::vector<int> numerosAusentes;

    for (int i = 2; i <= dimensao_cidades; ++i) {
        numerosAusentes.push_back(i);
    }

    for (int num : sol.sequence) {
        auto iter = std::find(numerosAusentes.begin(), numerosAusentes.end(), num);
        if (iter != numerosAusentes.end()) {
            numerosAusentes.erase(iter);
        }
    }

    return numerosAusentes;
}

std::vector<InsertionInfo> calcularCustosInsercao(Solution& s, std::vector<int> CL, double **c) {
    std::vector<InsertionInfo> custos((s.sequence.size() - 1) * CL.size());

    int l = 0;
    for (int a = 0; a < s.sequence.size() - 1; a++) {
        for (int k : CL)  {
            custos[l].num = k;
            custos[l].index = a;
            custos[l].custo = c[s.sequence[a]-1][k-1] + c[s.sequence[a + 1]-1][k-1] - c[s.sequence[a]-1][s.sequence[a + 1]-1];
            l++;
        }
    }
    return custos;
}

bool compareInsertionInfo(const InsertionInfo& info1, const InsertionInfo& info2) {
    return info1.custo < info2.custo;
}

void ordenarCustos(std::vector<InsertionInfo>& custos) {
    std::sort(custos.begin(), custos.end(), compareInsertionInfo);
}

Solution Construcao(int dimensao_cidades, double **c) {
    Solution solucao;
    solucao = escolhe3aleatorios(dimensao_cidades, c);
    std::vector<int> CL = nos_restantes(solucao, dimensao_cidades);

    while (!CL.empty()) {
        std::vector<InsertionInfo> custos = calcularCustosInsercao(solucao, CL, c);
        ordenarCustos(custos);
        double alfa = (double) rand() / RAND_MAX;
        int aleatorio = rand() % ((int) ceil(alfa * custos.size()));
        solucao.sequence.insert(solucao.sequence.begin() + custos[aleatorio].index + 1, custos[aleatorio].num);
        solucao.cost += custos[aleatorio].custo;
        CL.erase(std::find(CL.begin(), CL.end(), custos[aleatorio].num));
    }

    return solucao;
}

void BuscaLocal(Solution& s, double **c) {
    std::vector<int> NL = {1, 2, 3, 4, 5};
    bool melhorou = false;
    int n;

    while(!NL.empty()) {
        n = rand() % NL.size();
        switch (NL[n]) {
            case 1:
            // cout << "Sequencia antes do swap: ";
            // for (int i = 0; i < s.sequence.size(); i++) {
            //     cout << s.sequence[i] << " ";
            // }
            // cout << endl;
            // cout << "Custo da sequencia: " << s.cost << endl;

                melhorou = swap(s, c);
                

            // cout << "Sequencia depois do swap: ";
            // for (int i = 0; i < s.sequence.size(); i++) {
            //     cout << s.sequence[i] << " ";
            // }
            // cout << endl;
            // cout << "Custo da sequencia: " << s.cost << endl;
            // s.sequence[3233223] = 3;
                break;
            case 2:
                melhorou = two_opt(s, c);
                break;
            case 3:
                melhorou = or_opt(s, c, 1);
                break;
            case 4:
                melhorou = or_opt(s, c, 2);
                break;
            case 5:
                melhorou = or_opt(s, c, 3);
                break;
        }
        if (melhorou) {
            NL = {1, 2, 3, 4, 5};
        }
        else {
            NL.erase(NL.begin() + n);
        }
    }
}