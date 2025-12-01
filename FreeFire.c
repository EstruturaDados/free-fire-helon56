#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ITENS 20

// Struct do item
typedef struct {
    char nome[30];
    char tipo[10];
    int quantidade;
    int prioridade;
} Item;

// Enum para critério de ordenação
typedef enum {
    ORDENAR_NOME = 1,
    ORDENAR_TIPO,
    ORDENAR_PRIORIDADE
} Criterio;

// Protótipos
void adicionarItem(Item mochila[], int *total);
void removerItem(Item mochila[], int *total);
void listarItens(Item mochila[], int total);
void buscarSequencial(Item mochila[], int total);
void ordenarItens(Item mochila[], int total, Criterio criterio);
int compararItens(Item a, Item b, Criterio criterio);
void buscaBinaria(Item mochila[], int total, char nome[], bool ordenada);

// Função principal
int main() {
    Item mochila[MAX_ITENS];
    int totalItens = 0;
    int opcao;
    bool ordenadaPorNome = false;

    do {
        printf("\nMOCHILA DE SOBREVIVENCIA\n");
        printf("Adicionar item\n");
        printf("Remover item\n");
        printf("Listar itens\n");
        printf("Ordenar mochila\n");
        printf("Buscar item por nome\n");
        printf("Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                adicionarItem(mochila, &totalItens);
                ordenadaPorNome = false;
                break;

            case 2:
                removerItem(mochila, &totalItens);
                ordenadaPorNome = false;
                break;

            case 3:
                listarItens(mochila, totalItens);
                break;

            case 4: {
                if (totalItens == 0) {
                    printf("Não há item\n");
                    break;
                }

                int escolha;
                printf("\nEscolha o critério de ordenação\n");
                printf("Nome\nTipo\nPrioridade\n");
                printf("Opção: ");
                scanf("%d", &escolha);
                getchar();

                if (escolha < 1 || escolha > 3) {
                    printf("Critério não é válido!\n");
                    break;
                }

                ordenarItens(mochila, totalItens, (Criterio)escolha);
                ordenadaPorNome = (escolha == ORDENAR_NOME);
                break;
            }

            case 5: {
                if (totalItens == 0) {
                    printf("A Mochila está vazia.\n");
                    break;
                }

                if (!ordenadaPorNome) {
                    printf("A mochila precisa estar ordenada por nome antes da busca binária!\n");
                    break;
                }

                char nomeBusca[30];
                printf("\nDigite o nome do item: ");
                fgets(nomeBusca, 30, stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

                buscaBinaria(mochila, totalItens, nomeBusca, ordenadaPorNome);
                break;
            }

            case 0:
                printf("\nEncerrando o programa... \n");
                break;

            default:
                printf("A Opção não é válida!\n");
        }
    } while (opcao != 0);

    return 0;
}

void adicionarItem(Item mochila[], int *total) {
    if (*total >= MAX_ITENS) {
        printf("A Mochila está cheia! (máx. %d itens)\n", MAX_ITENS);
        return;
    }

    printf("\n--- Adicionar Item ---\n");
    printf("Nome: ");
    fgets(mochila[*total].nome, 30, stdin);
    mochila[*total].nome[strcspn(mochila[*total].nome, "\n")] = '\0';

    printf("Tipo: ");
    fgets(mochila[*total].tipo, 30, stdin);
    mochila[*total].tipo[strcspn(mochila[*total].tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &mochila[*total].quantidade);
    getchar();

    do {
        printf("Prioridade (1 a 5): ");
        scanf("%d", &mochila[*total].prioridade);
        getchar();
    } while (mochila[*total].prioridade < 1 || mochila[*total].prioridade > 5);

    (*total)++;
    printf("Item adicionado\n");
}

void removerItem(Item mochila[], int *total) {
    if (*total == 0) {
        printf("Nenhum item\n");
        return;
    }

    char nomeRemover[30];
    printf("\nDigite o nome do item: ");
    fgets(nomeRemover, 30, stdin);
    nomeRemover[strcspn(nomeRemover, "\n")] = '\0';

    for (int i = 0; i < *total; i++) {
        if (strcmp(mochila[i].nome, nomeRemover) == 0) {
            for (int j = i; j < *total - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            (*total)--;
            printf("Item removido\n");
            return;
        }
    }

    printf("Item nao encontrado!\n");
}

void listarItens(Item mochila[], int total) {
    if (total == 0) {
        printf("\nA Mochila está vazia.\n");
        return;
    }

    printf("\n--- Itens na Mochila ---\n");
    printf("%-20s %-15s %-12s %-10s\n", "Nome", "Tipo", "Quantidade", "Prioridade");
    for (int i = 0; i < total; i++) {
        printf("%-20s %-15s %-12d %-10d\n",
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade,
               mochila[i].prioridade);
    }
}

int compararItens(Item a, Item b, Criterio criterio) {
    switch (criterio) {
        case ORDENAR_NOME:
            return strcmp(a.nome, b.nome);
        case ORDENAR_TIPO:
            return strcmp(a.tipo, b.tipo);
        case ORDENAR_PRIORIDADE:
            return a.prioridade - b.prioridade;
        default:
            return 0;
    }
}

void ordenarItens(Item mochila[], int total, Criterio criterio) {
    int comparacoes = 0;

    for (int i = 1; i < total; i++) {
        Item chave = mochila[i];
        int j = i - 1;

        while (j >= 0 && compararItens(mochila[j], chave, criterio) > 0) {
            mochila[j + 1] = mochila[j];
            j--;
            comparacoes++;
        }
        mochila[j + 1] = chave;
    }

    printf("\nA Mochila está ordenada\n");
    printf("Total de comparações: %d\n", comparacoes);
}

void buscaBinaria(Item mochila[], int total, char nome[], bool ordenada) {
    if (!ordenada) {
        printf("Lista não está ordenada por nome!\n");
        return;
    }

    int inicio = 0, fim = total - 1, meio;
    bool encontrado = false;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        int cmp = strcmp(mochila[meio].nome, nome);

        if (cmp == 0) {
            printf("\nO Item foi encontrado!\n");
            printf("Nome: %s\n", mochila[meio].nome);
            printf("Tipo: %s\n", mochila[meio].tipo);
            printf("Quantidade: %d\n", mochila[meio].quantidade);
            printf("Prioridade: %d\n", mochila[meio].prioridade);
            encontrado = true;
            break;
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    if (!encontrado) {
        printf("O Item '%s' não foi encontrado\n", nome);
    }
}