#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

/* Uma posicao do labirinto. Linha 0 e a de cima, coluna 0 e a da esquerda. */
typedef struct {
    int linha;
    int coluna;
} Celula;

/* ------------------------------- Fila (FIFO) -------------------------------
 * Fila em vetor circular. Circular porque, quando o indice chega no fim do
 * vetor, ele volta pro comeco. Assim nao precisa empurrar todo mundo uma
 * posicao pra tras a cada remocao. E a fila que faz a busca em largura andar. */
typedef struct {
    Celula *itens;
    int capacidade;
    int inicio;      /* primeiro da fila, o proximo a sair */
    int fim;         /* onde o proximo elemento vai entrar  */
    int tamanho;
} Fila;

/* Devolve NULL se faltar memoria. */
Fila *fila_criar(int capacidade);

int fila_vazia(const Fila *fila);
int fila_cheia(const Fila *fila);

/* Devolvem 1 se deu certo e 0 se a fila estava cheia (ou vazia). */
int fila_enfileirar(Fila *fila, Celula item);
int fila_desenfileirar(Fila *fila, Celula *saida);

void fila_destruir(Fila *fila);

/* ------------------------------ Pilha (LIFO) -------------------------------
 * Pilha em vetor simples. O topo aponta pra proxima posicao livre, entao
 * topo == 0 quer dizer vazia e topo == capacidade quer dizer cheia. Serve pra
 * virar o caminho do avesso, ja que ele e remontado da saida para a entrada. */
typedef struct {
    Celula *itens;
    int capacidade;
    int topo;
} Pilha;

/* Devolve NULL se faltar memoria. */
Pilha *pilha_criar(int capacidade);

int pilha_vazia(const Pilha *pilha);
int pilha_cheia(const Pilha *pilha);

/* Devolvem 1 se deu certo e 0 se a pilha estava cheia (ou vazia). */
int pilha_empilhar(Pilha *pilha, Celula item);
int pilha_desempilhar(Pilha *pilha, Celula *saida);

/* Zera a pilha, mas guarda a memoria que ja foi alocada. */
void pilha_limpar(Pilha *pilha);

void pilha_destruir(Pilha *pilha);

#endif /* ESTRUTURAS_H */
