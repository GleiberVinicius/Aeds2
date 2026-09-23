/* ==========================================================================
 * UNIVERSIDADE FEDERAL DE ALFENAS (UNIFAL-MG)
 * Bacharelado em Ciencia da Computacao
 * DCE792 - Algoritmos e Estruturas de Dados 2 (AEDS2)
 * Professor: Iago Augusto de Carvalho
 *
 * Trabalho Pratico 1 - Saida do Labirinto
 *
 * Integrantes do grupo:
 *   - Joao Victor Martins Hollanda de Oliveira
 *   - Joao Gabriel de Oliveira Megda
 *   - Gleiber Vinicius Domingos de Souza
 *
 * Uso:
 *     ./labirinto <arquivo_de_instancia> [-v]
 *
 *     -v   modo verboso: joga na saida de erro (stderr) as dimensoes, umas
 *          estatisticas da busca e o caminho desenhado em cima da matriz. A
 *          saida padrao continua so com o caminho no formato que o enunciado
 *          pede, entao da pra redirecionar pra um arquivo sem sujeira.
 * ========================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "estruturas.h"

/* O trabalho pede 10 x 10, mas o tamanho e descoberto lendo o proprio arquivo,
 * ate esse limite. Assim da pra rodar tambem o exemplo 5 x 5 do enunciado. */
#define MAX_DIM 100

#define SIMBOLO_ENTRADA 'E'
#define SIMBOLO_SAIDA   'S'
#define SIMBOLO_PAREDE  'X'
#define SIMBOLO_LIVRE   '0'
#define SIMBOLO_CAMINHO '*'

#define INSTANCIA_PADRAO "instancias/labirinto1.txt"
#define NUM_DIRECOES 4

typedef enum {
    LAB_OK = 0,
    LAB_ERRO_ARQUIVO,      /* arquivo inexistente ou sem permissao   */
    LAB_ERRO_VAZIO,
    LAB_ERRO_DIMENSAO,     /* linhas de tamanhos diferentes ou > MAX */
    LAB_ERRO_SIMBOLO,
    LAB_ERRO_ENTRADA,      /* nenhuma ou mais de uma entrada 'E'     */
    LAB_ERRO_SAIDA
} ResultadoLeitura;

typedef struct {
    int  linhas;
    int  colunas;
    char grade[MAX_DIM][MAX_DIM + 1];   /* +1 para o terminador '\0' */
    Celula entrada;
    Celula saida;
} Labirinto;

typedef struct {
    int encontrou;        /* 1 se achou caminho entre E e S             */
    int comprimento;      /* quantas casas tem o caminho, contando E e S */
    int casasVisitadas;   /* quantas casas a busca abriu (modo verboso)  */
} ResultadoBusca;

/* ==========================================================================
 * Leitura e validacao da instancia
 * ========================================================================== */

/* Tira o "\r\n" do fim da linha, que vem junto quando o arquivo foi
 * salvo no Windows, e de quebra qualquer espaco sobrando. */
static void remover_brancos_finais(char *texto)
{
    size_t tamanho = strlen(texto);

    while (tamanho > 0 && isspace((unsigned char) texto[tamanho - 1])) {
        texto[tamanho - 1] = '\0';
        tamanho--;
    }
}

/* Traduz o caractere lido para um dos simbolos que o programa usa, ou '\0'
 * se for alguma coisa invalida. Aceita a letra 'O' valendo zero porque no
 * exemplo do enunciado as duas formas aparecem misturadas. */
static char normalizar_simbolo(char c)
{
    switch (c) {
        case 'E': case 'e': return SIMBOLO_ENTRADA;
        case 'S': case 's': return SIMBOLO_SAIDA;
        case 'X': case 'x': return SIMBOLO_PAREDE;
        case '0': case 'O': case 'o': case '.': return SIMBOLO_LIVRE;
        default:  return '\0';
    }
}

static ResultadoLeitura labirinto_carregar(Labirinto *lab, const char *caminho)
{
    FILE *arquivo;
    char  buffer[MAX_DIM + 16];
    int   linhaAtual = 0;
    int   entradasEncontradas = 0;
    int   saidasEncontradas   = 0;

    arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        return LAB_ERRO_ARQUIVO;
    }

    lab->linhas  = 0;
    lab->colunas = 0;

    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        int coluna;
        int comprimento;

        remover_brancos_finais(buffer);
        comprimento = (int) strlen(buffer);

        if (comprimento == 0) {
            continue;                   /* linha em branco: pula */
        }

        if (linhaAtual >= MAX_DIM || comprimento > MAX_DIM) {
            fclose(arquivo);
            return LAB_ERRO_DIMENSAO;
        }

        /* A primeira linha e quem diz quantas colunas o labirinto tem. Da
         * segunda em diante, todas precisam ter esse mesmo tamanho. */
        if (linhaAtual == 0) {
            lab->colunas = comprimento;
        } else if (comprimento != lab->colunas) {
            fclose(arquivo);
            return LAB_ERRO_DIMENSAO;
        }

        for (coluna = 0; coluna < comprimento; coluna++) {
            char simbolo = normalizar_simbolo(buffer[coluna]);

            if (simbolo == '\0') {
                fclose(arquivo);
                return LAB_ERRO_SIMBOLO;
            }

            if (simbolo == SIMBOLO_ENTRADA) {
                entradasEncontradas++;
                lab->entrada.linha  = linhaAtual;
                lab->entrada.coluna = coluna;
            } else if (simbolo == SIMBOLO_SAIDA) {
                saidasEncontradas++;
                lab->saida.linha  = linhaAtual;
                lab->saida.coluna = coluna;
            }

            lab->grade[linhaAtual][coluna] = simbolo;
        }

        lab->grade[linhaAtual][comprimento] = '\0';
        linhaAtual++;
    }

    fclose(arquivo);

    lab->linhas = linhaAtual;

    if (lab->linhas == 0 || lab->colunas == 0) {
        return LAB_ERRO_VAZIO;
    }
    if (entradasEncontradas != 1) {
        return LAB_ERRO_ENTRADA;
    }
    if (saidasEncontradas != 1) {
        return LAB_ERRO_SAIDA;
    }

    return LAB_OK;
}

static const char *labirinto_mensagem_erro(ResultadoLeitura codigo)
{
    switch (codigo) {
        case LAB_ERRO_ARQUIVO:
            return "nao foi possivel abrir o arquivo da instancia";
        case LAB_ERRO_VAZIO:
            return "o arquivo da instancia esta vazio";
        case LAB_ERRO_DIMENSAO:
            return "as linhas do labirinto possuem tamanhos diferentes ou excedem o limite";
        case LAB_ERRO_SIMBOLO:
            return "o arquivo contem um simbolo invalido (use apenas E, S, X e 0)";
        case LAB_ERRO_ENTRADA:
            return "o labirinto deve possuir exatamente uma entrada 'E'";
        case LAB_ERRO_SAIDA:
            return "o labirinto deve possuir exatamente uma saida 'S'";
        default:
            return "erro desconhecido";
    }
}

/* Da pra pisar em qualquer casa que exista e nao seja parede, inclusive no
 * 'E' e no 'S'. */
static int labirinto_livre(const Labirinto *lab, int linha, int coluna)
{
    if (linha < 0 || linha >= lab->linhas || coluna < 0 || coluna >= lab->colunas) {
        return 0;
    }
    return (lab->grade[linha][coluna] != SIMBOLO_PAREDE);
}

static void labirinto_imprimir(const Labirinto *lab, FILE *fluxo)
{
    int linha;

    for (linha = 0; linha < lab->linhas; linha++) {
        fprintf(fluxo, "%s\n", lab->grade[linha]);
    }
}

/* ==========================================================================
 * Busca do caminho
 * ========================================================================== */

/* Cima, baixo, esquerda e direita. Nao anda na diagonal. */
static const int DESLOCAMENTO_LINHA[NUM_DIRECOES]  = { -1,  1,  0,  0 };
static const int DESLOCAMENTO_COLUNA[NUM_DIRECOES] = {  0,  0, -1,  1 };

/*
 * Busca em largura (BFS). A ideia e olhar o labirinto como um grafo: cada casa
 * livre e um vertice, ligado as casas livres vizinhas. Como todo passo custa a
 * mesma coisa, a primeira vez que a busca chega na saida ja e pelo caminho
 * mais curto.
 *
 * No fim a pilha "caminho" fica montada de um jeito que desempilhar devolve as
 * casas na ordem entrada -> saida. Custa O(L * C) de tempo e de espaco.
 */
static ResultadoBusca busca_encontrar_caminho(const Labirinto *lab, Pilha *caminho)
{
    static int    visitado[MAX_DIM][MAX_DIM];
    static Celula anterior[MAX_DIM][MAX_DIM];   /* de onde a busca chegou em cada casa */

    ResultadoBusca resultado;
    Fila   *fila;
    Celula  atual;
    Celula  passo;
    int     linha;
    int     coluna;
    int     direcao;

    resultado.encontrou      = 0;
    resultado.comprimento    = 0;
    resultado.casasVisitadas = 0;

    for (linha = 0; linha < lab->linhas; linha++) {
        for (coluna = 0; coluna < lab->colunas; coluna++) {
            visitado[linha][coluna]        = 0;
            anterior[linha][coluna].linha  = -1;
            anterior[linha][coluna].coluna = -1;
        }
    }

    pilha_limpar(caminho);

    /* Cada casa entra na fila no maximo uma vez, entao linhas * colunas
     * posicoes sao de sobra e a fila nunca enche. */
    fila = fila_criar(lab->linhas * lab->colunas);
    if (fila == NULL) {
        return resultado;
    }

    visitado[lab->entrada.linha][lab->entrada.coluna] = 1;
    fila_enfileirar(fila, lab->entrada);

    while (!fila_vazia(fila)) {
        fila_desenfileirar(fila, &atual);
        resultado.casasVisitadas++;

        if (atual.linha == lab->saida.linha && atual.coluna == lab->saida.coluna) {
            resultado.encontrou = 1;
            break;
        }

        for (direcao = 0; direcao < NUM_DIRECOES; direcao++) {
            Celula vizinho;

            vizinho.linha  = atual.linha  + DESLOCAMENTO_LINHA[direcao];
            vizinho.coluna = atual.coluna + DESLOCAMENTO_COLUNA[direcao];

            if (!labirinto_livre(lab, vizinho.linha, vizinho.coluna)) {
                continue;
            }
            /* Marca ja na hora de descobrir, e nao quando for expandir. Se
             * deixar pra depois, a mesma casa entra na fila mais de uma vez. */
            if (visitado[vizinho.linha][vizinho.coluna]) {
                continue;
            }

            visitado[vizinho.linha][vizinho.coluna] = 1;
            anterior[vizinho.linha][vizinho.coluna] = atual;
            fila_enfileirar(fila, vizinho);
        }
    }

    fila_destruir(fila);

    if (!resultado.encontrou) {
        return resultado;      /* E e S estao em pedacos separados do labirinto */
    }

    /* Volta da saida ate a entrada seguindo os predecessores e vai empilhando
     * cada casa. Como a pilha inverte a ordem, na hora de desempilhar o caminho
     * sai no sentido certo. A entrada tem predecessor (-1, -1), que e o que
     * segura o laco. */
    passo = lab->saida;
    while (passo.linha != -1) {
        pilha_empilhar(caminho, passo);
        resultado.comprimento++;
        passo = anterior[passo.linha][passo.coluna];
    }

    return resultado;
}

/* ==========================================================================
 * Saida
 * ========================================================================== */

/*
 * Aqui dentro o labirinto e uma matriz (linha, coluna) com a linha 0 la em
 * cima, mas o enunciado pede o par "x,y" no estilo cartesiano:
 *
 *     x = coluna
 *     y = (numero de linhas - 1) - linha
 *
 * Ou seja, o y conta as linhas de baixo pra cima. E por isso que, no exemplo
 * 5 x 5, o 'E' da primeira linha sai como "0,4" e o 'S', que esta na terceira
 * linha e quinta coluna, sai como "4,2".
 */
static void imprimir_coordenada(const Labirinto *lab, Celula casa)
{
    int x = casa.coluna;
    int y = (lab->linhas - 1) - casa.linha;

    printf("%d,%d\n", x, y);
}

static void mostrar_uso(const char *nomePrograma)
{
    fprintf(stderr, "Uso: %s <arquivo_de_instancia> [-v]\n", nomePrograma);
    fprintf(stderr, "  <arquivo_de_instancia>  caminho do arquivo texto com o labirinto\n");
    fprintf(stderr, "  -v                      modo verboso (informacoes extras em stderr)\n");
    fprintf(stderr, "\nExemplo: %s %s\n", nomePrograma, INSTANCIA_PADRAO);
}

/* Desenha a matriz com o caminho marcado. Mexe numa copia pra nao estragar o
 * labirinto original. */
static void desenhar_caminho(const Labirinto *lab, const Celula *casas, int total)
{
    Labirinto copia = *lab;
    int i;

    for (i = 0; i < total; i++) {
        char atual = copia.grade[casas[i].linha][casas[i].coluna];

        if (atual != SIMBOLO_ENTRADA && atual != SIMBOLO_SAIDA) {
            copia.grade[casas[i].linha][casas[i].coluna] = SIMBOLO_CAMINHO;
        }
    }

    labirinto_imprimir(&copia, stderr);
}

/* ========================================================================== */

int main(int argc, char *argv[])
{
    Labirinto        labirinto;
    ResultadoLeitura leitura;
    ResultadoBusca   busca;
    Pilha           *caminho;
    Celula          *casas;
    const char      *arquivo = INSTANCIA_PADRAO;
    int              verboso = 0;
    int              total   = 0;
    int              i;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verboso") == 0) {
            verboso = 1;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--ajuda") == 0) {
            mostrar_uso(argv[0]);
            return EXIT_SUCCESS;
        } else {
            arquivo = argv[i];
        }
    }

    leitura = labirinto_carregar(&labirinto, arquivo);
    if (leitura != LAB_OK) {
        fprintf(stderr, "Erro ao ler \"%s\": %s.\n",
                arquivo, labirinto_mensagem_erro(leitura));
        mostrar_uso(argv[0]);
        return EXIT_FAILURE;
    }

    if (verboso) {
        fprintf(stderr, "Instancia....: %s\n", arquivo);
        fprintf(stderr, "Dimensoes....: %d x %d\n", labirinto.linhas, labirinto.colunas);
        fprintf(stderr, "Entrada (E)..: linha %d, coluna %d\n",
                labirinto.entrada.linha, labirinto.entrada.coluna);
        fprintf(stderr, "Saida  (S)...: linha %d, coluna %d\n",
                labirinto.saida.linha, labirinto.saida.coluna);
        fprintf(stderr, "\nLabirinto lido:\n");
        labirinto_imprimir(&labirinto, stderr);
    }

    caminho = pilha_criar(labirinto.linhas * labirinto.colunas);
    if (caminho == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente para executar a busca.\n");
        return EXIT_FAILURE;
    }

    busca = busca_encontrar_caminho(&labirinto, caminho);

    if (!busca.encontrou) {
        fprintf(stderr,
                "Nao existe caminho entre a entrada e a saida neste labirinto.\n");
        pilha_destruir(caminho);
        return EXIT_FAILURE;
    }

    casas = (Celula *) malloc(sizeof(Celula) * (size_t) busca.comprimento);
    if (casas == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente para armazenar o caminho.\n");
        pilha_destruir(caminho);
        return EXIT_FAILURE;
    }

    /* Desempilhando, as casas saem na ordem entrada -> saida. */
    while (pilha_desempilhar(caminho, &casas[total])) {
        total++;
    }

    for (i = 0; i < total; i++) {
        imprimir_coordenada(&labirinto, casas[i]);
    }

    if (verboso) {
        fprintf(stderr, "\nCasas do caminho.........: %d\n", total);
        fprintf(stderr, "Passos (movimentos)......: %d\n", total - 1);
        fprintf(stderr, "Casas expandidas pela BFS: %d\n", busca.casasVisitadas);
        fprintf(stderr, "\nCaminho sobre o labirinto ('*' marca as casas percorridas):\n");
        desenhar_caminho(&labirinto, casas, total);
    }

    free(casas);
    pilha_destruir(caminho);

    return EXIT_SUCCESS;
}
