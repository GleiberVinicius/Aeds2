#include <stdio.h> 
#include <stdlib.h> 
#include <limits.h> 
#include <stdbool.h>
#include "pilha.h"

// Nó da lista encadeada: cada elemento da pilha vira um nó,
// alocado dinamicamente com malloc, e aponta para o próximo nó abaixo dele.
struct no {
	int valor;
	struct no* prox;
};

struct pilha { 
	struct no* topo;     // ponteiro para o nó do topo (NULL quando vazia)
	int tamanho;          // capacidade máxima da pilha
	int quantidade;        // quantidade de elementos atualmente na pilha
}; 

struct pilha* criar_pilha(int tamanho) { 
	// Aloca a estrutura de dados pilha
    struct pilha* pilha = (struct pilha*)malloc(sizeof(struct pilha)); 
	// Armazena o tamanho (capacidade máxima) que ela foi criada
    pilha->tamanho = tamanho; 
    // Diz que não existe nenhum elemento no topo (lista vazia)
	pilha->topo = NULL; 
    // Nenhum elemento inserido ainda
	pilha->quantidade = 0;
    // Retorna a pilha
	return pilha; 
} 

bool ehCheia(struct pilha* pilha) { 
    // Se a quantidade de elementos já alocados == tamanho, a pilha é cheia
	if (pilha->quantidade == pilha->tamanho) {
        return (true);
    }
    // Caso contrário, ela não está cheia
    return (false);
} 

bool ehVazia(struct pilha* pilha) { 
	// Se o topo aponta para NULL, então a pilha é vazia
    if (pilha->topo == NULL) {
        return (true);
    } 
    return (false);
} 

void push(struct pilha* pilha, int item) { 
	// Se a pilha estiver cheia, não tem como inserir nenhum novo elemento
    if (ehCheia(pilha)) {
		printf("\nPilha cheia. Impossível inserir elementos");
        return; 
    }
    // Aloca dinamicamente um novo nó para o elemento
    struct no* novo = (struct no*)malloc(sizeof(struct no));
    novo->valor = item;
    // O novo nó passa a apontar para o antigo topo
    novo->prox = pilha->topo;
    // O novo nó se torna o topo da pilha
    pilha->topo = novo;
    // Atualiza a quantidade de elementos
    pilha->quantidade++;
} 


int pop(struct pilha* pilha) { 
	// Se a pilha estiver vazia, não tem como remover nenhum elemento
    if (ehVazia(pilha)) {
		printf("\nPilha  vazia. Impossível remover elementos");
        return -1; 
    }
    // Guarda o nó do topo e o valor que será retornado
    struct no* removido = pilha->topo;
    int valor = removido->valor;
    // O topo passa a ser o próximo nó da lista
    pilha->topo = removido->prox;
    // Libera a memória do nó removido
    free(removido);
    // Atualiza a quantidade de elementos
    pilha->quantidade--;
    return (valor);
} 

int obtem_elemento(struct pilha* pilha) { 
	// Se a pilha estiver vazia, não tem como obter nenhum elemento
    if (ehVazia(pilha)) {
		printf("\nPilha  vazia. Impossível obter elementos");
        return INT_MIN; 
    }
    // Retorna o valor armazenado no nó do topo
	return pilha->topo->valor; 
} 


int main() { 
    int val, n;
    bool aux;
    // Cria pilha com 5 posições
    struct pilha* pilha = criar_pilha(5); 
    do {
        printf("\n************************* MENU ************************");
	    printf("\n1. Push");
	    printf("\n2. Pop");
	    printf("\n3. Obtém elemento");
	    printf("\n4. Pilha vazia?");
        printf("\n5. Pilha cheia?");
	    printf("\n6. Sair");
	    printf("\n Digite sua escolha : ");
	    scanf("%d",&n);
	    switch(n) {
            case 1: 
		        printf("\nDigite o valor ");
                scanf("%d",&val);
                push(pilha , val);
                break;
            case 2: 
                printf("\nElemento retirado : %d",pop(pilha));
                break;
            case 3: 
                printf("\nElemento do topo: %d",obtem_elemento(pilha));
                break;
            case 4: 
                aux = ehVazia(pilha);
                if (aux) {
                    printf("\nPilha vazia");
                } else {
                    printf("\nPilha não está vazia");
                }
                break;
            case 5: 
                aux = ehCheia(pilha);
                if (aux) {
                    printf("\nPilha cheia");
                } else {
                    printf("\nPilha não está cheia");
                }
                break;         
            case 6:
                return (1);
                break;
            default: printf("\nOpção errada!");
                break;
        }
	} while(1);
    
    return (0);
 }
