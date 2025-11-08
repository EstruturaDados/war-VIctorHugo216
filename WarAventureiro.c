/*
  jogo_territorios.c
  Implementa simulação de ataque entre territórios usando alocação dinâmica e ponteiros.
  Requisitos atendidos:
   - calloc para alocar vetor de Territorio
   - função void atacar(Territorio* atacante, Territorio* defensor)
   - uso de srand(time(NULL)) e rand() para simulação (dados 1..6)
   - validação: não atacar território da mesma cor; atacante precisa ter >= 2 tropas
   - transferência de cor e metade das tropas do atacante para defensor se atacante vencer
   - se atacante perder, perde 1 tropa
   - modularização e liberação de memória (free)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR 10

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

/* Função: cadastrarTerritorios
   - Aloca e preenche o vetor de territórios (retorna ponteiro para vetor).
   - Parâmetros: quantidade (int).
   - Retorno: Territorio* (ponteiro para vetor alocado).
*/
Territorio* cadastrarTerritorios(int quantidade) {
    if (quantidade <= 0) return NULL;

    // Usando calloc para inicializar memória a zero
    Territorio* mapa = (Territorio*) calloc(quantidade, sizeof(Territorio));
    if (!mapa) {
        fprintf(stderr, "Erro: falha ao alocar memória para territórios.\n");
        exit(EXIT_FAILURE);
    }

    printf("\n--- Cadastro de Territórios ---\n");
    for (int i = 0; i < quantidade; i++) {
        printf("Território %d:\n", i);
        printf("  Nome: ");
        // limpa o buffer antes de ler string com espaço
        scanf(" %29[^\n]", mapa[i].nome);
        printf("  Cor (nome do exército): ");
        scanf(" %9s", mapa[i].cor);
        printf("  Quantidade de tropas (inteiro positivo): ");
        while (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas < 0) {
            printf("    Entrada inválida. Digite um inteiro >= 0: ");
            // limpar buffer de entrada
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
        }
    }
    return mapa;
}

/* Função: exibirMapa
   - Exibe todos os territórios com índice, nome, cor e tropas.
   - Usa ponteiro para o vetor.
*/
void exibirMapa(Territorio* mapa, int quantidade) {
    printf("\n--- Mapa de Territórios ---\n");
    for (int i = 0; i < quantidade; i++) {
        printf("[%d] %s | Cor: %s | Tropas: %d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/* Função: escolherIndice
   - Lê do usuário um índice válido entre 0 e quantidade-1.
*/
int escolherIndice(const char* prompt, int quantidade) {
    int idx;
    printf("%s (0 a %d): ", prompt, quantidade - 1);
    while (scanf("%d", &idx) != 1 || idx < 0 || idx >= quantidade) {
        printf("  Índice inválido. %s (0 a %d): ", prompt, quantidade - 1);
        int c; while ((c = getchar()) != '\n' && c != EOF) {}
    }
    return idx;
}

/* Função: rolarDado
   - Retorna valor aleatório de 1 a 6 (simula um dado).
*/
int rolarDado() {
    return (rand() % 6) + 1;
}

/* Função: atacar
   - Implementa a simulação de ataque entre dois territórios.
   - Parâmetros: ponteiros para atacante e defensor.
   - Regras:
     * Valida que não são da mesma cor antes de chamar (checar também aqui por segurança).
     * Atacante precisa ter pelo menos 2 tropas para atacar (deixa ao menos 1 na origem).
     * Cada lado rola um dado (1-6). Se atacante > defensor, atacante vence.
     * Em vitória: defensor muda de cor para a cor do atacante; defensor recebe (metade inte) das tropas do atacante
                 (defensor->tropas = atacante->tropas / 2) e atacante perde essa quantidade (atacante->tropas -= transfer).
     * Em derrota: atacante perde 1 tropa. Garantimos tropas >= 0 e, na prática, não permitimos ataque se atacante tiver <2 tropas.
*/
void atacar(Territorio* atacante, Territorio* defensor) {
    if (!atacante || !defensor) return;

    // Verificações de segurança
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque inválido: os territórios pertencem ao mesmo exército (%s).\n", atacante->cor);
        return;
    }
    if (atacante->tropas < 2) {
        printf("Ataque inválido: atacante '%s' deve ter pelo menos 2 tropas para iniciar um ataque (atual: %d).\n",
               atacante->nome, atacante->tropas);
        return;
    }

    printf("\n--- Simulação de Ataque ---\n");
    printf("Atacante: %s | Cor: %s | Tropas: %d\n", atacante->nome, atacante->cor, atacante->tropas);
    printf("Defensor:  %s | Cor: %s | Tropas: %d\n", defensor->nome, defensor->cor, defensor->tropas);

    int dadoAtq = rolarDado();
    int dadoDef = rolarDado();
    printf("Rolagem: Atacante -> %d ; Defensor -> %d\n", dadoAtq, dadoDef);

    if (dadoAtq > dadoDef) {
        // atacante vence
        int transferencia = atacante->tropas / 2; // metade inteira
        if (transferencia < 1) transferencia = 1; // garantir transferência mínima 1
        // Atualiza defensor: troca de dono e tropas
        strncpy(defensor->cor, atacante->cor, MAX_COR - 1);
        defensor->cor[MAX_COR - 1] = '\0';
        defensor->tropas = transferencia;
        // Atualiza atacante: perde as tropas transferidas
        atacante->tropas -= transferencia;
        if (atacante->tropas < 0) atacante->tropas = 0;

        printf("Resultado: ATACANTE VENCEU!\n");
        printf("'%s' assumiu o controle de '%s' com %d tropas transferidas.\n",
               atacante->nome, defensor->nome, transferencia);
    } else {
        // atacante perde 1 tropa
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("Resultado: DEFENSOR SE MANTEVE. Atacante perdeu 1 tropa.\n");
    }

    printf("\n--- Estado pós-ataque ---\n");
    printf("Atacante: %s | Cor: %s | Tropas: %d\n", atacante->nome, atacante->cor, atacante->tropas);
    printf("Defensor:  %s | Cor: %s | Tropas: %d\n", defensor->nome, defensor->cor, defensor->tropas);
}

/* Função: liberarMemoria
   - Libera o vetor de territórios alocado dinamicamente.
*/
void liberarMemoria(Territorio* mapa) {
    if (mapa) {
        free(mapa);
    }
}

/* Função principal: menu simples para cadastrar, exibir e atacar até o usuário optar por sair.
*/
int main() {
    srand((unsigned int) time(NULL)); // inicializa gerador de números aleatórios

    int n;
    printf("Quantidade de territórios a cadastrar: ");
    while (scanf("%d", &n) != 1 || n <= 0) {
        printf("Entrada inválida. Digite um inteiro positivo: ");
        int c; while ((c = getchar()) != '\n' && c != EOF) {}
    }

    // Consumir o newline antes das leituras de string no cadastro
    int c; while ((c = getchar()) != '\n' && c != EOF) {}

    Territorio* mapa = cadastrarTerritorios(n);

    int opcao = 0;
    do {
        printf("\n--- Menu ---\n");
        printf("1 - Exibir mapa\n");
        printf("2 - Realizar ataque\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        while (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida. Escolha novamente: ");
            int d; while ((d = getchar()) != '\n' && d != EOF) {}
        }

        switch (opcao) {
            case 1:
                exibirMapa(mapa, n);
                break;
            case 2: {
                exibirMapa(mapa, n);
                int idxAtq = escolherIndice("Selecione o índice do território ATACANTE", n);
                int idxDef = escolherIndice("Selecione o índice do território DEFENSOR", n);

                // validações adicionais
                if (idxAtq == idxDef) {
                    printf("Seleção inválida: atacante e defensor são o mesmo território.\n");
                    break;
                }
                Territorio* atacante = &mapa[idxAtq];
                Territorio* defensor = &mapa[idxDef];

                if (strcmp(atacante->cor, defensor->cor) == 0) {
                    printf("Ataque inválido: ambos os territórios pertencem ao mesmo exército (%s).\n", atacante->cor);
                    break;
                }
                if (atacante->tropas < 2) {
                    printf("Ataque inválido: atacante precisa ter ao menos 2 tropas (atual: %d).\n", atacante->tropas);
                    break;
                }

                atacar(atacante, defensor);
                break;
            }
            case 0:
                printf("Saindo... liberando memória.\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    liberarMemoria(mapa);
    printf("Programa finalizado.\n");
    return 0;
}
