#include <stdio.h>
#include <windows.h>

typedef struct
{
    int chave; // Identificador sem ambiguidade[cite: 2]
    int valor;
} No;

int remover(No v[], int n, int x)
{
    int indice;

    if (n != 0)
    {
        indice = busca(v, n, x);

        if (indice != 0)
        {
            for (int i = indice; i <= n - 1; i++)
            {
                v[i] = v[i + 1];
            }
            n = n - 1;
        }
    }

    return n;
}

void imprimir(No v[], int n)
{
    if (n == 0)
    {
        printf("A lista esta vazia!\n");
        return;
    }

    printf("\n--- CONTEUDO DO VETOR (Total: %d) ---\n", n);
    for (int i = 1; i <= n; i++)
    {
        printf("Posicao [%d] -> Chave: %d\n", i, v[i].chave);
    }
    printf("------------------------------------\n");
}

int insere(No v[], int n, int x)
{
    int insere;
    if (n < 6)
    {
        if (busca(v, n, x) == 0)
        {
            v[n + 1].chave = x;
            n = n + 1;
            insere = n;
        }
        else
        {
            insere = 0;
        }
    }
    else
    {
        insere = -1;
    }
    return insere;
}

int busca(No v[], int n, int x)
{
    v[n + 1].chave = x; // Insere o sentinela na posição n+1[cite: 2]
    int i = 1;

    // A busca começa no índice 1[cite: 2]
    while (v[i].chave != x)
    {
        i = i + 1;
    }

    if (i != n + 1)
    {
        return i; // Encontrou o elemento
    }
    else
    {
        return 0; // Retorna 0 indicando que não encontrou[cite: 2]
    }
}

int main()
{
    SetConsoleOutputCP(65001);
    // Criamos um vetor com 7 posições (índices 0 a 6) para acomodar 5 itens (1 a 5) mais o sentinela (6)
    No v[7] = {
        {0, 0},  // Índice 0: ignorado para bater com a lógica do material de i = 1
        {5, 0},  // Índice 1
        {9, 0},  // Índice 2
        {10, 0}, // Índice 3
        {8, 0},  // Índice 4
    };

    int n = 5;       // Tamanho de elementos válidos no vetor
    int buscar = 10; // Faltava declarar o valor que você quer buscar
    int opcao = 0;
    int inserir = 2;
    int retirar = 5;
    int resultado3, resultado2;

    // Chamada da função passando o vetor, o tamanho e o valor buscado
    int resultado = busca(v, n, buscar);
    do
    {
        printf("\n===== MENU =====\n");
        printf("1 - Busca\n");
        printf("2 - Inserção\n");
        printf("3 - Remoção\n");
        printf("4 - Mostrar o Vetor\n");
        printf("0 - Sair do programa\n");
        printf("Digite uma opção!\n");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            if (resultado != 0)
            {
                printf("O valor %d foi encontrado no indice %d.\n", buscar, resultado);
            }
            else
            {
                printf("O valor %d nao foi encontrado no vetor.\n", buscar);
            }
            break;
        case 2:
            // Executa a inserção no exato momento em que a opção 2 é escolhida
            resultado2 = insere(v, n, inserir);

            if (resultado2 > 0)
            {
                n = resultado2; // Atualiza o tamanho da lista no main (n passa a ser 6)
                printf("O valor %d foi inserido com sucesso.\n", inserir);
            }
            else if (resultado2 == 0)
            {
                printf("O valor %d ja existe no vetor.\n", inserir);
            }
            else
            {
                printf("Nao foi possivel inserir o valor %d. O vetor esta cheio.\n", inserir);
            }
            break;
        case 3:
            // Executa a inserção no exato momento em que a opção 2 é escolhida
            resultado3 = remover(v, n, retirar);

            if (resultado3 < n)
            {
                n = resultado3; // Atualiza o tamanho da lista no main (n passa a ser 6)
                printf("O valor %d foi removido com sucesso.\n", retirar);
            }
            else if (resultado3 == n)
            {
                printf("O valor %d nao existe no vetor.\n", retirar);
            }
            break;
        case 4:
            printf("\n--- CONTEUDO DA LISTA (Total: %d) ---\n", n);
            if (n == 0)
            {
                printf("A lista esta vazia.\n");
            }
            else
            {
                for (int i = 1; i <= n; i++)
                {
                    printf("Posicao [%d] -> Chave: %d\n", i, v[i].chave);
                }
            }
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida.");
            break;
        }
    } while (opcao != 0);
    printf("Você saiu do programa!");
    return 0;
}