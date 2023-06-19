from random import shuffle

cidades = [1,2,3,4,5,6]

solucao = [1,2,3,4,5,6,7,1]



distancias = [[None, 245, 174, 118, 59, 129],
              [None, None, 250, 226, 186, 147],
              [None, None, None, 274, 169, 114],
              [None, None, None, None, 105, 185],
              [None, None, None, None, None, 87],
              [None, None, None, None, None, None]]

def distancia(a, b):
    a -= 1
    b -= 1

    if a > b:
        return distancias[b][a]
    else:
        return distancias[a][b]

def distancia_total_solucao(solucao):
    total = 0
    for i in range(len(solucao)-1):
        total += distancia(solucao[i], solucao[i+1])
    return total    

def solucoes_vizinhas(solucao):
    vizinhas = []
    for i in range(1, len(solucao)):
        for j in range(i+1, len(solucao)):
            vizinha = solucao[:]
            vizinha[i], vizinha[j] = vizinha[j], vizinha[i]
            vizinhas.append(vizinha)
    return vizinhas


solucao = cidades[:]
shuffle(solucao)

solucao.remove(1)
solucao.insert(0, 1)

print("Solução inicial: ", solucao)

print("len: ", len(solucoes_vizinhas(solucao)))

nova_solucao = True
while nova_solucao:
    nova_solucao = False
    distancia_solucao = distancia_total_solucao(solucao)
    for solucao_vizinha in solucoes_vizinhas(solucao):
        distancia_solucao_vizinha = distancia_total_solucao(solucao_vizinha)
        if distancia_solucao_vizinha < distancia_solucao:
            solucao = solucao_vizinha
            nova_solucao = True
            continue

print("Solução final: ", solucao)