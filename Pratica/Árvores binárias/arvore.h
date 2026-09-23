#ifndef _H_ARVORE
#define _H_ARVORE

struct node {
    int valor;
    struct node *esquerda, *direita;
};

struct node* novo_no(int item);
struct node* inserir(struct node* node, int valor);
struct node* buscar(struct node* root, int valor);
struct node* encontrar_maior(struct node* node);
struct node* remover(struct node* root, int valor);
void em_ordem(struct node* root);

#endif // _H_ARVORE