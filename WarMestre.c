#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

typedef struct {
    char nome[30];
    char cor[10];
    char *missao;
} Jogador;

// Funções
void atribuirMissao(char *destino, char *missoes[], int totalMissoes);
int verificarMissao(char *missao, Territorio *mapa, int tamanho);
void atacar(Territorio *atacante, Territorio *defensor);
void exibirMapa(Territorio *mapa, int tamanho);
void liberarMemoria(Territorio *mapa, Jogador *jogadores, int totalJogadores);

int main() {
    srand(time(NULL));

    int numTerritorios;
    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &numTerritorios);
    getchar();

    Territorio *mapa = (Territorio *)malloc(numTerritorios * sizeof(Territorio));

    for (int i = 0; i < numTerritorios; i++) {
        printf("\nNome do territorio %d: ", i + 1);
        fgets(mapa[i].nome, 30, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do exercito (ex: Vermelho, Verde, Azul): ");
        fgets(mapa[i].cor, 10, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar();
    }

    int numJogadores;
    printf("\nQuantos jogadores? ");
    scanf("%d", &numJogadores);
    getchar();

    Jogador *jogadores = (Jogador *)malloc(numJogadores * sizeof(Jogador));

    char *missoes[] = {
        "Conquistar 3 territorios seguidos",
        "Controlar 5 territorios",
        "Eliminar todas as tropas de uma cor",
        "Destruir o exercito", // <- essa ganha cor aleatória
        "Controlar todos os territorios azuis"
    };
    int totalMissoes = 5;

    // Atribuir missões
    for (int i = 0; i < numJogadores; i++) {
        printf("\nNome do jogador %d: ", i + 1);
        fgets(jogadores[i].nome, 30, stdin);
        jogadores[i].nome[strcspn(jogadores[i].nome, "\n")] = '\0';

        printf("Cor do jogador %d: ", i + 1);
        fgets(jogadores[i].cor, 10, stdin);
        jogadores[i].cor[strcspn(jogadores[i].cor, "\n")] = '\0';

        jogadores[i].missao = (char *)malloc(100 * sizeof(char));
        atribuirMissao(jogadores[i].missao, missoes, totalMissoes);

        printf("🎯 Missao do jogador %d: %s\n", i + 1, jogadores[i].missao);
    }

    int turno = 1;
    while (1) {
        printf("\n===== MAPA ATUAL =====\n");
        exibirMapa(mapa, numTerritorios);

        int atacante, defensor;
        printf("\nEscolha o territorio atacante (numero): ");
        scanf("%d", &atacante);
        printf("Escolha o territorio defensor (numero): ");
        scanf("%d", &defensor);

        atacante--;
        defensor--;

        if (atacante < 0 || defensor < 0 || atacante >= numTerritorios || defensor >= numTerritorios) {
            printf("❌ Territorio invalido!\n");
            continue;
        }

        if (strcmp(mapa[atacante].cor, mapa[defensor].cor) == 0) {
            printf("⚠️ Voce nao pode atacar um territorio aliado!\n");
            continue;
        }

        atacar(&mapa[atacante], &mapa[defensor]);

        // Verificar vitória
        for (int j = 0; j < numJogadores; j++) {
            if (verificarMissao(jogadores[j].missao, mapa, numTerritorios)) {
                printf("\n🏆 O jogador %s venceu! Missao cumprida: %s\n", jogadores[j].nome, jogadores[j].missao);
                liberarMemoria(mapa, jogadores, numJogadores);
                return 0;
            }
        }

        turno++;
    }

    liberarMemoria(mapa, jogadores, numJogadores);
    return 0;
}

// ===============================================
// Funções auxiliares
// ===============================================

// Sorteia e atribui missão ao jogador
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    char cores[][10] = {"Vermelho", "Verde", "Azul", "Amarelo", "Roxo"};
    char buffer[100];

    if (strcmp(missoes[sorteio], "Destruir o exercito") == 0) {
        int corSorteada = rand() % 5;
        sprintf(buffer, "Destruir o exercito %s", cores[corSorteada]);
        strcpy(destino, buffer);
    } else {
        strcpy(destino, missoes[sorteio]);
    }
}

// Verifica se missão foi cumprida
int verificarMissao(char *missao, Territorio *mapa, int tamanho) {
    // Exemplo simples de lógica
    if (strstr(missao, "Destruir o exercito")) {
        char corAlvo[10];
        sscanf(missao, "Destruir o exercito %s", corAlvo);

        int existe = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corAlvo) == 0 && mapa[i].tropas > 0)
                existe = 1;
        }
        if (!existe)
            return 1;
    }

    if (strcmp(missao, "Controlar 5 territorios") == 0) {
        int total = 0;
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas > 0)
                total++;
        }
        if (total >= 5)
            return 1;
    }

    return 0;
}

// Simula ataque com dados
void atacar(Territorio *atacante, Territorio *defensor) {
    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;

    printf("\n🎲 Dado atacante (%s): %d\n", atacante->nome, dadoA);
    printf("🎲 Dado defensor (%s): %d\n", defensor->nome, dadoD);

    if (dadoA > dadoD) {
        printf("💥 O atacante venceu a batalha!\n");
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas /= 2;
    } else {
        printf("❌ O defensor resistiu! O atacante perde uma tropa.\n");
        atacante->tropas--;
        if (atacante->tropas < 0)
            atacante->tropas = 0;
    }
}

// Mostra o mapa atual
void exibirMapa(Territorio *mapa, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("%d. %s | Cor: %s | Tropas: %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Libera memória
void liberarMemoria(Territorio *mapa, Jogador *jogadores, int totalJogadores) {
    for (int i = 0; i < totalJogadores; i++) {
        free(jogadores[i].missao);
    }
    free(mapa);
    free(jogadores);
}
