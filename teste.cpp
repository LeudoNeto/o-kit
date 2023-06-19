custo_solucao_vizinha = custo_solucao_atual - distMatrix[solucao[i-1]-1][solucao[i]-1] - distMatrix[solucao[i+1]-1][solucao[i]-1]
                                            + distMatrix[solucao[i-1]-1][solucao[j]-1] + distMatrix[solucao[i+1]-1][solucao[j]-1]
                                            
                                            - distMatrix[solucao[j-1]-1][solucao[j]-1] - distMatrix[solucao[j+1]-1][solucao[j]-1]
                                            + distMatrix[solucao[j-1]-1][solucao[i]-1] + distMatrix[solucao[j+1]-1][solucao[i]-1];
                
                if (custo_solucao_vizinha < custo_melhor_solucao) {
                    copia_array(melhor_solucao, solucao_vizinha, dimensao_cidades+1);
                    custo_melhor_solucao = custo_solucao_vizinha;
                    nova_solucao = 1;
                }