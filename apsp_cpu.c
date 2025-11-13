#include <stdio.h>  
#include <stdlib.h> 
#include <time.h>   
#include <limits.h> 
#include <string.h>

#define INF INT_MAX

// Função para gerar um grafo ponderado aleatório
void gera_grafo(int* matriz, int N, int densidade) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int idx = i * N + j; // Mapeamento 2D -> 1D

            if (i == j) {
                matriz[idx] = 0; // Custo zero na diagonal 
            } else {
                if ((rand() % 100) < densidade) {
                    matriz[idx] = (rand() % 10) + 1; // Aresta com peso entre 1 e 10
                } else {
                    // Sem aresta
                    matriz[idx] = INF; 
                }
            }
        }
    }
}

// Função para imprimir a matriz (para debug)
void imprime_matriz(int* matriz, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int idx = i * N + j;
            if (matriz[idx] == INF) {
                printf("INF\t");
            } else {
                printf("%d\t", matriz[idx]);
            }
        }
        printf("\n"); 
    }
}

// Implementação Sequencial (CPU) do Floyd-Warshall
void floyd_warshall_cpu(int* matriz, int N) {
    
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                
                int idx_ij = i * N + j;
                int idx_ik = i * N + k;
                int idx_kj = k * N + j;

                // Custo atual para ir de i -> j
                int dist_ij = matriz[idx_ij];
                
                // Custos dos caminhos "alternativos" via k
                int dist_ik = matriz[idx_ik];
                int dist_kj = matriz[idx_kj];
                
                // Evitando Overflow
                if (dist_ik == INF || dist_kj == INF) {
                    continue; 
                }

                int novo_custo_via_k = dist_ik + dist_kj;

                if (novo_custo_via_k < dist_ij) {
                    matriz[idx_ij] = novo_custo_via_k;
                }
            }
        }
    }
}

int main() {
    int N = 4;
    int densidade = 50; // Ex: 50% de chance de ter uma aresta

    size_t tamanho_matriz = (size_t)N * N * sizeof(int);
    int* matriz_adj = (int*)malloc(tamanho_matriz);

    if (matriz_adj == NULL) {
        fprintf(stderr, "Falha ao alocar memoria.\n");
        return 1;
    }

    srand(time(NULL));

   gera_grafo(matriz_adj, N, densidade);

    printf("Matriz de Adjacencia (Grafo de Entrada):\n");
    imprime_matriz(matriz_adj, N);

    int* matriz_dist = (int*)malloc(tamanho_matriz);
    if (matriz_dist == NULL) {
        fprintf(stderr, "Falha ao alocar memoria para matriz de distancias.\n");
        free(matriz_adj); 
        return 1;
    }

    memcpy(matriz_dist, matriz_adj, tamanho_matriz);

    free(matriz_adj);
    return 0;
}