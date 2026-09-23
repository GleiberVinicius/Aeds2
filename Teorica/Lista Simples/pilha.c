#include <stdio.h>
#include <windows.h>

int M = 100;
int topo = 0;

typedef struct
{
    int chave; // Identificador sem ambiguidade[cite: 2]
    int valor;
} No;

No P[101];
No remover(No P[])
{
    No removido = {-1, -1}; // Cria um nó "nulo" para caso de erro[cite: 1]

    if (topo == 0)
    {
        printf("A pilha esta vazia!\n");
        return removido; // Retorna nulo no caso de pilha vazia[cite: 1]
    }

    if (topo != 0)
    {
        removido = P[topo]; // O nó inteiro é resgatado[cite: 1]
        topo = topo - 1;    // O topo desce uma posição[cite: 1]
    }

    return removido; // Retorna o nó resgatado
}
void imprimir_pilha(No P[])
{
    // Se o topo for 0, não tem nada para mostrar
    if (topo == 0)
    {
        printf("A pilha esta vazia!\n");
        return;
    }

    printf("\n--- CONTEUDO DA PILHA (Total de itens: %d) ---\n", topo);
    // Imprimimos do 'topo' descendo até '1'
    for (int i = topo; i >= 1; i--)
    {
        printf("Posicao [%d] -> Chave: %d | Valor: %d\n", i, P[i].chave, P[i].valor);
    }
    printf("----------------------------------------------\n");
}

int insere(No P[], No no)
{
    int resultado = -1;
    if (topo != M)
    {
        topo = topo + 1;
        P[topo] = no;
        resultado = topo;
    }
    return resultado;
}

int main()
{
    SetConsoleOutputCP(65001);
    // 1. A pilha começa vazia. Vamos printar para ver:
    printf("Teste 1:");
    imprimir_pilha(P);

    // 2. Criando alguns nós para testar
    No no1 = {15, 200};
    No no2 = {30, 500};
    No no3 = {99, 1000};

    // 3. Inserindo na pilha
    insere(P, no1);
    insere(P, no2);
    insere(P, no3); // O no3 será o novo topo!

    // 4. Printando para ver se inseriu certinho
    printf("\nTeste 2:");
    imprimir_pilha(P);

    remover(P);

    printf("\n Teste3:");
    imprimir_pilha(P);

    return 0;
}
