#include <stdio.h>
#include <string.h>

#define MAX 15

typedef struct {
    int codigo;
    char nome[50];
    float preco;
    int quantidade;
} Produto;

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void lerTexto(char *dest, int tam) {
    fgets(dest, tam, stdin);
    dest[strcspn(dest, "\n")] = '\0';
}

/* Retorna a posicao do produto no vetor, ou -1 se nao existir */
int buscarPorCodigo(Produto p[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (p[i].codigo == codigo) return i;
    }
    return -1;
}

/* 'total' vem por ponteiro para a funcao poder aumentar o contador */
void cadastrar(Produto p[], int *total) {
    int codigo;

    if (*total >= MAX) {
        printf("Limite de %d produtos atingido.\n", MAX);
        return;
    }

    printf("Codigo: ");
    scanf("%d", &codigo);
    limparBuffer();

    if (buscarPorCodigo(p, *total, codigo) != -1) {
        printf("Erro: ja existe um produto com esse codigo.\n");
        return;
    }

    p[*total].codigo = codigo;
    printf("Nome: ");
    lerTexto(p[*total].nome, 50);
    printf("Preco: ");
    scanf("%f", &p[*total].preco);
    printf("Quantidade em estoque: ");
    scanf("%d", &p[*total].quantidade);
    limparBuffer();

    if (p[*total].preco < 0 || p[*total].quantidade < 0) {
        printf("Erro: preco e quantidade nao podem ser negativos.\n");
        return;
    }

    (*total)++;
    printf("Produto cadastrado!\n");
}

void listar(Produto p[], int total) {
    int i;
    if (total == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    printf("\n%-8s %-25s %10s %8s\n", "Codigo", "Nome", "Preco", "Estoque");
    for (i = 0; i < total; i++) {
        printf("%-8d %-25s %10.2f %8d\n",
               p[i].codigo, p[i].nome, p[i].preco, p[i].quantidade);
    }
}

void reporEstoque(Produto p[], int total) {
    int codigo, qtd, pos;
    printf("Codigo do produto: ");
    scanf("%d", &codigo);
    pos = buscarPorCodigo(p, total, codigo);
    if (pos == -1) {
        printf("Produto nao encontrado.\n");
        limparBuffer();
        return;
    }
    printf("Quantidade a repor: ");
    scanf("%d", &qtd);
    limparBuffer();
    if (qtd <= 0) {
        printf("Quantidade invalida.\n");
        return;
    }
    p[pos].quantidade += qtd;
    printf("Estoque atualizado: %d unidades.\n", p[pos].quantidade);
}

void registrarVenda(Produto p[], int total) {
    int codigo, qtd, pos;
    printf("Codigo do produto: ");
    scanf("%d", &codigo);
    pos = buscarPorCodigo(p, total, codigo);
    if (pos == -1) {
        printf("Produto nao encontrado.\n");
        limparBuffer();
        return;
    }
    printf("Quantidade vendida: ");
    scanf("%d", &qtd);
    limparBuffer();
    if (qtd <= 0) {
        printf("Quantidade invalida.\n");
        return;
    }
    if (qtd > p[pos].quantidade) {
        printf("Venda negada: so ha %d unidades em estoque.\n", p[pos].quantidade);
        return;
    }
    p[pos].quantidade -= qtd;
    printf("Venda registrada! Total: R$ %.2f | Estoque restante: %d\n",
           qtd * p[pos].preco, p[pos].quantidade);
}

void consultar(Produto p[], int total) {
    int codigo, pos;
    printf("Codigo do produto: ");
    scanf("%d", &codigo);
    limparBuffer();
    pos = buscarPorCodigo(p, total, codigo);
    if (pos == -1) {
        printf("Produto nao encontrado.\n");
        return;
    }
    printf("Codigo: %d\nNome: %s\nPreco: R$ %.2f\nEstoque: %d\n",
           p[pos].codigo, p[pos].nome, p[pos].preco, p[pos].quantidade);
}

float valorTotalEstoque(Produto p[], int total) {
    int i;
    float soma = 0;
    for (i = 0; i < total; i++) {
        soma += p[i].preco * p[i].quantidade;
    }
    return soma;
}

void relatorioEstoque(Produto p[], int total) {
    int i, iMaior = 0;
    if (total == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    for (i = 1; i < total; i++) {
        if (p[i].preco * p[i].quantidade > p[iMaior].preco * p[iMaior].quantidade)
            iMaior = i;
    }
    printf("Valor total em estoque: R$ %.2f\n", valorTotalEstoque(p, total));
    printf("Produto de maior valor em estoque: %s (R$ %.2f)\n",
           p[iMaior].nome, p[iMaior].preco * p[iMaior].quantidade);
}

int main(void) {
    Produto produtos[MAX];
    int total = 0, opcao;

    do {
        printf("\n===== PAPELARIA =====\n");
        printf("1 - Cadastrar produto\n");
        printf("2 - Listar produtos\n");
        printf("3 - Repor estoque\n");
        printf("4 - Registrar venda\n");
        printf("5 - Consultar produto pelo codigo\n");
        printf("6 - Valor total do estoque / maior valor\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: cadastrar(produtos, &total); break;
            case 2: listar(produtos, total); break;
            case 3: reporEstoque(produtos, total); break;
            case 4: registrarVenda(produtos, total); break;
            case 5: consultar(produtos, total); break;
            case 6: relatorioEstoque(produtos, total); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}
