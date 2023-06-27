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
        int aux = s.sequence[m_i];
        s.sequence[m_i] = s.sequence[m_j];
        s.sequence[m_j] = aux;
        s.cost += melhor_delta;
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
        std::reverse(s.sequence.begin() + m_i, s.sequence.begin() + m_j + 1);
        s.cost += melhor_delta;
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
        std::vector<int> aux;
        aux.insert(aux.end(), s.sequence.begin(), s.sequence.begin() + m_i);
        aux.insert(aux.end(), s.sequence.begin() + m_i + opt_type, s.sequence.begin() + m_j + 1);
        aux.insert(aux.end(), s.sequence.begin() + m_i, s.sequence.begin() + m_i + opt_type);
        aux.insert(aux.end(), s.sequence.begin() + m_j + 1, s.sequence.end());
        s.sequence = aux;
        s.cost += melhor_delta;
        return true;
    } else {
        return false;
    }
}