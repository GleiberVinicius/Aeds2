#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"
 
// Cria um novo nó na árvore
struct node* novo_no(int item) {
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->valor = item;
    temp->esquerda = temp->direita = NULL;
    return temp;
}
 
// Inserir novo nó com determinado
// valor na árvore binária
struct node* inserir(struct node* node, int valor) {
    // Se a árvore é vazia, então cria um novo nó
    if (node == NULL) {
        return novo_no(valor);
    }
 
    // Caso contrário, faz uma busca pelo local adequado de inserção
    if (valor < node->valor) {
        node->esquerda = inserir(node->esquerda, valor);
    } else if (valor > node->valor) {
        node->direita = inserir(node->direita, valor);
    }
    
    // Retorna o ponteiro para o nó
    return node;
}
 
// Procura por um valor na árvore
struct node* buscar(struct node* root, int valor) {
    // Caso base - a raiz é vazia ou o valor está na raiz
    if (root == NULL || root->valor == valor) {
        return root;
    }

    // Valor é maior que a raiz
    if (root->valor < valor) {
        return buscar(root->direita, valor);
    }

    // Valor é menor que a raiz
    return buscar(root->esquerda, valor);
}

// Encontra o nó com o maior valor de uma subárvore
// (usado para achar o predecessor, indo sempre para a direita)
struct node* encontrar_maior(struct node* node) {
    struct node* atual = node;
    while (atual != NULL && atual->direita != NULL) {
        atual = atual->direita;
    }
    return atual;
}

// Remove o nó com o valor informado da árvore
struct node* remover(struct node* root, int valor) {
    // Caso base: árvore vazia ou valor não encontrado
    if (root == NULL) {
        return root;
    }

    // Procura o nó a ser removido, como numa busca normal
    if (valor < root->valor) {
        root->esquerda = remover(root->esquerda, valor);
    } else if (valor > root->valor) {
        root->direita = remover(root->direita, valor);
    } else {
        // Achou o nó a ser removido

        // Caso 1: nó folha (nenhum filho)
        // Caso 2: nó com apenas um filho
        // Em ambos, basta "pular" o nó e liberar sua memória
        if (root->esquerda == NULL) {
            struct node* temp = root->direita;
            free(root);
            return temp;
        } else if (root->direita == NULL) {
            struct node* temp = root->esquerda;
            free(root);
            return temp;
        }

        // Caso 3: nó com dois filhos
        // 1. Encontra o predecessor (maior valor da subárvore esquerda)
        struct node* predecessor = encontrar_maior(root->esquerda);

        // 2. Troca o valor do predecessor pelo do item alvo
        root->valor = predecessor->valor;

        // 3. Deleta o predecessor (que agora está duplicado),
        //    que cai no caso 1 ou 2 acima
        root->esquerda = remover(root->esquerda, predecessor->valor);
    }

    return root;
}

// Percorre a árvore em ordem (esquerda, raiz, direita) e imprime os valores
void em_ordem(struct node* root) {
    if (root == NULL) {
        return;
    }
    em_ordem(root->esquerda);
    printf("%d ", root->valor);
    em_ordem(root->direita);
}

int main() {
    // Define a árvore como uma estrutura vazia
    struct node* root = NULL;

    // Insere o nó inicial (raiz)
    root = inserir(root, 50);

    // Insere outros nós
    inserir(root, 30);
    inserir(root, 20);
    inserir(root, 40);
    inserir(root, 70);
    inserir(root, 60);
    inserir(root, 80);
 
    // Valor a ser buscado
    int valor = 6;
 
    // Buscando na árvore
    if (buscar(root, valor) == NULL) {
        printf("%d não encontrado\n", valor);
    } else {
        printf("%d encontrado\n", valor);
    }
 
    // Novo valor a ser buscado
    valor = 60;
 
    // Buscando na árvore
    if (buscar(root, valor) == NULL) {
        printf("%d não encontrado\n", valor);
    } else {
        printf("%d encontrado\n", valor);
    }

    // Árvore atual:
    //          50
    //        30  70
    //       20 40 60 80

    printf("\nÁrvore em ordem antes das remoções: ");
    em_ordem(root);
    printf("\n");

    // Caso 1: remover uma folha (80)
    root = remover(root, 80);
    printf("Após remover 80 (folha):           ");
    em_ordem(root);
    printf("\n");

    // Caso 2: remover um nó com um filho (70, que só tem o filho 60)
    root = remover(root, 70);
    printf("Após remover 70 (um filho):         ");
    em_ordem(root);
    printf("\n");

    // Caso 3: remover um nó com dois filhos (30, filhos 20 e 40)
    root = remover(root, 30);
    printf("Após remover 30 (dois filhos):       ");
    em_ordem(root);
    printf("\n");

    return 0;
}