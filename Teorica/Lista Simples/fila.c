#include <stdio.h>
#include <windows.h>

int M = 100;
int f = 0;
int r = 0;

typedef struct
{
    int chave; // Identificador sem ambiguidade[cite: 2]
    int valor;
} No;

No Fila[101];
No remover(No Fila[])
{
    No removido = {-1, -1}; // Previne retorno de lixo de memória se a fila estiver vazia
    if (f != 0)
    {
        removido = Fila[f];
        if (f == r)
        {
            f = 0;
            r = 0;
        }
        else
        {
            f = (f % M) + 1;
        }
    }
    else
    {
        printf("A fila esta vazia!\n");
    }
    return removido;
}
void imprimir_fila(No Fila[])
{
    // Se o início for 0, não tem nada para mostrar
    if (f == 0)
    {
        printf("A fila esta vazia!\n");
        return;
    }
    printf("\n--- CONTEUDO DA FILA ---\n");
    int i = f; // Começa a ler a partir do início da fila
    while (1)
    {
        printf("Posicao [%d] -> Chave: %d | Valor: %d\n", i, Fila[i].chave, Fila[i].valor);
        // Se o índice de leitura alcançou o final oficial da fila, interrompe o laço
        if (i == r)
        {
            break;
        }
        // Avança o índice em 1 unidade, garantindo que ele dê a volta ao bater no limite M
        i = (i % M) + 1;
    }
    printf("----------------------------------------------\n");
}

int insere(No Fila[], No no)
{
    int resultado = -1;
    int prov = (r % M) + 1;
    if (prov != f)
    {
        r = prov;
        Fila[r] = no;
        resultado = r;
        if (f == 0)
        {
            f = 1;
        }
    }
    return resultado;
}

int main()
{
    SetConsoleOutputCP(65001);

    // 2. Criando alguns nós para testar
    No no1 = {15, 200};
    No no2 = {30, 500};
    No no3 = {99, 1000};

    insere(Fila, no1);
    insere(Fila, no2);
    insere(Fila, no3);

    // 4. Printando para ver se inseriu certinho
    printf("\nTeste 1:");
    imprimir_fila(Fila);

    No no4 = {10, 1000};
    insere(Fila,no4);
    printf("\n Teste2:");
    imprimir_fila(Fila);
    remover(Fila);

    printf("\n Teste3:");
    imprimir_fila(Fila);

    return 0;
}
