#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>

struct Solution {
    double cost;
    std::vector<int> sequence;
};

bool swap(Solution& s, double **c) {
    double delta, melhor_delta = INFINITY;
    int i, j, m_i, m_j;

    for (i = 1; i < s.sequence.size() - 2; i++) {
        for (j = i + 2; j < s.sequence.size() - 1; j++) {
            delta = c[s.sequence[i-1]-1][s.sequence[j]-1] + c[s.sequence[i+1]-1][s.sequence[j]-1]
                  + c[s.sequence[j-1]-1][s.sequence[i]-1] + c[s.sequence[j+1]-1][s.sequence[i]-1]
                  - c[s.sequence[i-1]-1][s.sequence[i]-1] - c[s.sequence[i+1]-1][s.sequence[i]-1]
                  - c[s.sequence[j-1]-1][s.sequence[j]-1] - c[s.sequence[j+1]-1][s.sequence[j]-1];

            if (delta < melhor_delta) {
                melhor_delta = delta;
                m_i = i;
                m_j = j;
            }
        }
    }

    if (melhor_delta < 0) {
        // cout << "Sequencia antes do swap: ";
        // for (int i = 0; i < s.sequence.size(); i++) {
        //     cout << s.sequence[i] << " ";
        // }
        // cout << endl;
        // cout << "Custo da sequencia: " << s.cost << endl;

        // cout << "Swap entre " << s.sequence[m_i] << " e " << s.sequence[m_j] << endl;

        int aux = s.sequence[m_i];
        s.sequence[m_i] = s.sequence[m_j];
        s.sequence[m_j] = aux;
        s.cost += melhor_delta;

        // cout << "Sequencia depois do swap: ";
        // for (int i = 0; i < s.sequence.size(); i++) {
        //     cout << s.sequence[i] << " ";
        // }
        // cout << endl;
        // cout << "Custo da sequencia: " << s.cost << endl;
        // s.sequence[3233223] = 3;

        return true;
    } else {
        return false;
    }

}

bool two_opt(Solution& s, double **c) {
    double delta, melhor_delta = INFINITY;
    int i, j, m_i, m_j;

    for (i = 1; i < s.sequence.size() - 4; i++) {
        for (j = i + 3; j < s.sequence.size() - 1; j++) {

            delta = - c[s.sequence[i-1]-1][s.sequence[i]-1]
                    - c[s.sequence[j]-1][s.sequence[j+1]-1]
                    + c[s.sequence[i-1]-1][s.sequence[j]-1]
                    + c[s.sequence[i]-1][s.sequence[j+1]-1];

            if (delta < melhor_delta) {
                melhor_delta = delta;
                m_i = i;
                m_j = j;
            }
        }
    }

    if (melhor_delta < 0) {
        // cout << "Sequencia antes do two_opt: ";
        // for (int i = 0; i < s.sequence.size(); i++) {
        //     cout << s.sequence[i] << " ";
        // }
        // cout << endl;
        // cout << "Custo da sequencia: " << s.cost << endl;

        // cout << "opt em entre " << s.sequence[m_i] << " e " << s.sequence[m_j] << endl;

        std::reverse(s.sequence.begin() + m_i, s.sequence.begin() + m_j + 1);
        s.cost += melhor_delta;

        // cout << "Sequencia depois do two_opt: ";
        // for (int i = 0; i < s.sequence.size(); i++) {
        //     cout << s.sequence[i] << " ";
        // }
        // cout << endl;
        // cout << "Custo da sequencia: " << s.cost << endl;
        return true;
    } else {
        return false;
    }
}


bool or_opt(Solution& s, double **c, int opt_type) {
    opt_type--;
    double delta, melhor_delta = INFINITY;
    int i, j, m_i, m_j;

    for (i = 1; i < s.sequence.size() - 1 - (1 + 2*opt_type); i++) {
        for (j = i + 2 + opt_type; j < s.sequence.size() - 1 - opt_type; j++) {

            delta = - c[s.sequence[i-1]-1][s.sequence[i]-1]
                    - c[s.sequence[i+opt_type]-1][s.sequence[i+1+opt_type]-1]
                    - c[s.sequence[j+opt_type]-1][s.sequence[j+1+opt_type]-1]
                    + c[s.sequence[j+opt_type]-1][s.sequence[i]-1]
                    + c[s.sequence[i+opt_type]-1][s.sequence[j+1+opt_type]-1]
                    + c[s.sequence[i-1]-1][s.sequence[i+1+opt_type]-1];

            if (delta < melhor_delta) {
                melhor_delta = delta;
                m_i = i;
                m_j = j;
            }
        }
    }

    if (melhor_delta < 0) {
        // cout << "Sequencia antes do or_opt: ";
        // for (int i = 0; i < s.sequence.size(); i++) {
        //     cout << s.sequence[i] << " ";
        // }
        // cout << endl;
        // cout << "Custo da sequencia: " << s.cost << endl;

        // cout << "or_opt de " << m_i << " em " << m_j << " tamanho " << opt_type << endl;

        std::vector<int> aux;
        aux.insert(aux.end(), s.sequence.begin(), s.sequence.begin() + m_i);
        aux.insert(aux.end(), s.sequence.begin() + m_i + opt_type + 1, s.sequence.begin() + m_j + opt_type + 1);
        aux.insert(aux.end(), s.sequence.begin() + m_i, s.sequence.begin() + m_i + opt_type + 1);
        aux.insert(aux.end(), s.sequence.begin() + m_j + opt_type + 1, s.sequence.end());
        s.sequence = aux;
        s.cost += melhor_delta;

        // cout << "Sequencia depois do or_opt: ";
        // for (int i = 0; i < s.sequence.size(); i++) {
        //     cout << s.sequence[i] << " ";
        // }
        // cout << endl;
        // cout << "Custo da sequencia: " << s.cost << endl;
        return true;
    } else {
        return false;
    }
}