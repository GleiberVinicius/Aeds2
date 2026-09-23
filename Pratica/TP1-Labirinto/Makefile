# =============================================================================
#
# Alvos disponiveis:
#   make            compila o projeto e gera o executavel "labirinto"
#   make executar   compila e roda a instancia padrao (instancias/labirinto1.txt)
#   make testes     roda o programa em todas as instancias fornecidas
#   make exemplo    roda o exemplo 5 x 5 que aparece no enunciado
#   make limpar     remove os arquivos objeto e o executavel
#   make ajuda      mostra esta lista de alvos
# =============================================================================

# ------------------------------- Ferramentas ---------------------------------
CC       = gcc
# -Wall -Wextra -pedantic: liga os avisos do compilador; o codigo compila
#                          sem nenhum deles
# -std=c99              : fixa a versao da linguagem
# -O2                   : otimizacao padrao
CFLAGS   = -Wall -Wextra -pedantic -std=c99 -O2

# -------------------------------- Diretorios ---------------------------------
DIR_SRC  = src
DIR_OBJ  = obj
DIR_INST = instancias

# -------------------------- Arquivos do projeto ------------------------------
# Todo .c dentro de src/ vira um .o dentro de obj/, sem precisar listar na mao.
FONTES   = $(wildcard $(DIR_SRC)/*.c)
OBJETOS  = $(patsubst $(DIR_SRC)/%.c,$(DIR_OBJ)/%.o,$(FONTES))

# No Windows o gcc gruda um ".exe" no nome sozinho. Sem essa distincao o make
# nao acha o arquivo que ele mesmo gerou e refaz a ligacao toda vez. No Linux e
# no macOS o nome continua sendo so "labirinto".
ifeq ($(OS),Windows_NT)
    EXEC = labirinto.exe
else
    EXEC = labirinto
endif

INST_PADRAO = $(DIR_INST)/labirinto1.txt

# As instancias que tem caminho entre E e S.
INSTANCIAS = $(DIR_INST)/labirinto1.txt \
             $(DIR_INST)/labirinto2.txt \
             $(DIR_INST)/labirinto3.txt \
             $(DIR_INST)/labirinto4.txt

# -----------------------------------------------------------------------------
# Regras
# -----------------------------------------------------------------------------

# Alvo padrao: e o que roda quando se digita so "make".
all: $(EXEC)

# Junta todos os objetos num executavel so.
$(EXEC): $(OBJETOS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Compilacao concluida. Execute com: ./$(EXEC) $(INST_PADRAO)"

# Cada .c gera o seu .o. O "| $(DIR_OBJ)" so garante que a pasta obj/ exista
# antes, sem mandar recompilar tudo quando a data da pasta muda.
$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c | $(DIR_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_OBJ):
	mkdir $(DIR_OBJ)

# Se estruturas.h mudar, os dois objetos tem que ser refeitos.
$(DIR_OBJ)/main.o:       $(DIR_SRC)/estruturas.h
$(DIR_OBJ)/estruturas.o: $(DIR_SRC)/estruturas.h

# ------------------------------- Execucao ------------------------------------

executar: $(EXEC)
	./$(EXEC) $(INST_PADRAO)

exemplo: $(EXEC)
	./$(EXEC) $(DIR_INST)/exemplo5x5.txt

# Roda as instancias com caminho e, no fim, a que nao tem solucao, so pra
# mostrar que o programa trata esse caso.
testes: $(EXEC)
	@for instancia in $(INSTANCIAS); do \
		echo "===== $$instancia ====="; \
		./$(EXEC) $$instancia; \
		echo ""; \
	done
	@echo "===== $(DIR_INST)/labirinto5-sem-solucao.txt (deve falhar) ====="
	@./$(EXEC) $(DIR_INST)/labirinto5-sem-solucao.txt || true

# Igual ao "testes", mas com as informacoes extras do modo verboso.
testes-verboso: $(EXEC)
	@for instancia in $(INSTANCIAS); do \
		echo "===== $$instancia ====="; \
		./$(EXEC) $$instancia -v; \
		echo ""; \
	done

# ------------------------------- Utilidades ----------------------------------

limpar:
	rm -rf $(DIR_OBJ) labirinto labirinto.exe

# "clean" fica como apelido de "limpar", que e o nome que todo mundo tenta.
clean: limpar

ajuda:
	@echo "Alvos disponiveis:"
	@echo "  make             compila o projeto e gera o executavel '$(EXEC)'"
	@echo "  make executar    compila e roda a instancia padrao"
	@echo "  make exemplo     roda o exemplo 5 x 5 do enunciado"
	@echo "  make testes      roda todas as instancias fornecidas"
	@echo "  make limpar      remove os arquivos objeto e o executavel"

# Esses alvos nao sao arquivos de verdade no disco.
.PHONY: all executar exemplo testes testes-verboso limpar clean ajuda
