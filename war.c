#include <stdio.h>
#include <string.h>

#define MAX_TERRITORIOS 5
#define MAX_NOME 50

// Definição da struct
typedef struct {
    char nome[MAX_NOME];
    char corExercito[MAX_NOME];
    int tropas;
} Territorio;

int main() {
    Territorio mapa[MAX_TERRITORIOS]; // vetor estático
    int i;

    printf("=== Cadastro de Territórios ===\n");

    for (i = 0; i < MAX_TERRITORIOS; i++) {
        printf("\nTerritório %d:\n", i + 1);

        // Nome do território
        printf("Nome: ");
        fgets(mapa[i].nome, MAX_NOME, stdin);
        // remover '\n' do final caso exista
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        // Cor do exército
        printf("Cor do Exército: ");
        fgets(mapa[i].corExercito, MAX_NOME, stdin);
        mapa[i].corExercito[strcspn(mapa[i].corExercito, "\n")] = '\0';

        // Número de tropas
        printf("Número de Tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar(); // consumir o '\n' que sobra do scanf
    }

    // Exibição organizada
    printf("\n=== Estado Atual do Mapa ===\n");
    printf("%-3s %-20s %-15s %-6s\n", "ID", "Nome", "Exército", "Tropas");
    printf("-----------------------------------------------------\n");

    for (i = 0; i < MAX_TERRITORIOS; i++) {
        printf("%-3d %-20s %-15s %-6d\n",
               i + 1,
               mapa[i].nome,
               mapa[i].corExercito,
               mapa[i].tropas);
    }

    return 0;
}
