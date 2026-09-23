#include <stdlib.h>
#include "estruturas.h"

/* ------------------------------- Fila (FIFO) ------------------------------ */

Fila *fila_criar(int capacidade)
{
    Fila *fila;

    if (capacidade <= 0) {
        return NULL;
    }

    fila = (Fila *) malloc(sizeof(Fila));
    if (fila == NULL) {
        return NULL;
    }

    fila->itens = (Celula *) malloc(sizeof(Celula) * (size_t) capacidade);
    if (fila->itens == NULL) {
        free(fila);
        return NULL;
    }

    fila->capacidade = capacidade;
    fila->inicio     = 0;
    fila->fim        = 0;
    fila->tamanho    = 0;

    return fila;
}

int fila_vazia(const Fila *fila)
{
    return (fila == NULL || fila->tamanho == 0);
}

int fila_cheia(const Fila *fila)
{
    return (fila != NULL && fila->tamanho == fila->capacidade);
}

int fila_enfileirar(Fila *fila, Celula item)
{
    if (fila == NULL || fila_cheia(fila)) {
        return 0;
    }

    fila->itens[fila->fim] = item;
    fila->fim = (fila->fim + 1) % fila->capacidade;   /* da a volta no vetor */
    fila->tamanho++;

    return 1;
}

int fila_desenfileirar(Fila *fila, Celula *saida)
{
    if (fila_vazia(fila) || saida == NULL) {
        return 0;
    }

    *saida = fila->itens[fila->inicio];
    fila->inicio = (fila->inicio + 1) % fila->capacidade;
    fila->tamanho--;

    return 1;
}

void fila_destruir(Fila *fila)
{
    if (fila == NULL) {
        return;
    }
    free(fila->itens);
    free(fila);
}

/* ------------------------------ Pilha (LIFO) ------------------------------ */

Pilha *pilha_criar(int capacidade)
{
    Pilha *pilha;

    if (capacidade <= 0) {
        return NULL;
    }

    pilha = (Pilha *) malloc(sizeof(Pilha));
    if (pilha == NULL) {
        return NULL;
    }

    pilha->itens = (Celula *) malloc(sizeof(Celula) * (size_t) capacidade);
    if (pilha->itens == NULL) {
        free(pilha);
        return NULL;
    }

    pilha->capacidade = capacidade;
    pilha->topo       = 0;

    return pilha;
}

int pilha_vazia(const Pilha *pilha)
{
    return (pilha == NULL || pilha->topo == 0);
}

int pilha_cheia(const Pilha *pilha)
{
    return (pilha != NULL && pilha->topo == pilha->capacidade);
}

int pilha_empilhar(Pilha *pilha, Celula item)
{
    if (pilha == NULL || pilha_cheia(pilha)) {
        return 0;
    }

    pilha->itens[pilha->topo] = item;
    pilha->topo++;

    return 1;
}

int pilha_desempilhar(Pilha *pilha, Celula *saida)
{
    if (pilha_vazia(pilha) || saida == NULL) {
        return 0;
    }

    pilha->topo--;
    *saida = pilha->itens[pilha->topo];

    return 1;
}

void pilha_limpar(Pilha *pilha)
{
    if (pilha != NULL) {
        pilha->topo = 0;
    }
}

void pilha_destruir(Pilha *pilha)
{
    if (pilha == NULL) {
        return;
    }
    free(pilha->itens);
    free(pilha);
}
