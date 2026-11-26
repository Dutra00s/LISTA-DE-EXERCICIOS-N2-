#include <stdio.h>
#include <string.h>

#define MAX_PROD 10
#define MAX_VENDAS 20

typedef struct {
    int codigo;
    char nome[50];
    float preco;
    int estoque;
} Produto;

/* A venda guarda os dados do produto vendido, a quantidade e o valor total */
typedef struct {
    Produto produto;
    int quantidade;
    float valorTotal;
} Venda;

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void lerTexto(char *dest, int tam) {
    fgets(dest, tam, stdin);
    dest[strcspn(dest, "\n")] = '\0';
}

int buscarProduto(Produto p[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (p[i].codigo == codigo) return i;
    }
    return -1;
}

void cadastrarProduto(Produto p[], int *total) {
    int codigo;
    if (*total >= MAX_PROD) {
        printf("Limite de %d produtos atingido.\n", MAX_PROD);
        return;
    }
    printf("Codigo: ");
    scanf("%d", &codigo);
    limparBuffer();
    if (buscarProduto(p, *total, codigo) != -1) {
        printf("Erro: codigo ja cadastrado.\n");
        return;
    }
    p[*total].codigo = codigo;
    printf("Nome: ");
    lerTexto(p[*total].nome, 50);
    printf("Preco: ");
    scanf("%f", &p[*total].preco);
    printf("Estoque: ");
    scanf("%d", &p[*total].estoque);
    limparBuffer();
    if (p[*total].preco < 0 || p[*total].estoque < 0) {
        printf("Erro: valores negativos nao sao permitidos.\n");
        return;
    }
    (*total)++;
    printf("Produto cadastrado!\n");
}

void listarProdutos(Produto p[], int total) {
    int i;
    if (total == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    printf("\n%-8s %-25s %10s %8s\n", "Codigo", "Nome", "Preco", "Estoque");
    for (i = 0; i < total; i++) {
        printf("%-8d %-25s %10.2f %8d\n", p[i].codigo, p[i].nome, p[i].preco, p[i].estoque);
    }
}

void registrarVenda(Produto p[], int totalProd, Venda v[], int *totalVendas) {
    int codigo, qtd, pos;

    if (*totalVendas >= MAX_VENDAS) {
        printf("Limite de %d vendas atingido.\n", MAX_VENDAS);
        return;
    }
    printf("Codigo do produto: ");
    scanf("%d", &codigo);
    pos = buscarProduto(p, totalProd, codigo);
    if (pos == -1) {
        printf("Produto nao cadastrado.\n");
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
    if (qtd > p[pos].estoque) {
        printf("Venda negada: estoque disponivel = %d.\n", p[pos].estoque);
        return;
    }

    v[*totalVendas].produto = p[pos];            /* associa a venda ao produto */
    v[*totalVendas].quantidade = qtd;
    v[*totalVendas].valorTotal = qtd * p[pos].preco;
    p[pos].estoque -= qtd;                       /* atualiza o estoque */

    printf("Venda registrada! Total: R$ %.2f\n", v[*totalVendas].valorTotal);
    (*totalVendas)++;
}

void listarVendas(Venda v[], int total) {
    int i;
    if (total == 0) {
        printf("Nenhuma venda registrada.\n");
        return;
    }
    printf("\n%-4s %-25s %6s %12s\n", "N", "Produto", "Qtd", "Total");
    for (i = 0; i < total; i++) {
        printf("%-4d %-25s %6d %12.2f\n", i + 1, v[i].produto.nome,
               v[i].quantidade, v[i].valorTotal);
    }
}

void relatorio(Produto p[], int totalProd, Venda v[], int totalVendas) {
    int i, j, iMaior = -1;
    float faturamento = 0, fatProd, maior = 0;

    for (i = 0; i < totalVendas; i++) {
        faturamento += v[i].valorTotal;
    }
    printf("Faturamento total: R$ %.2f\n", faturamento);

    /* para cada produto, soma as vendas que pertencem a ele */
    for (i = 0; i < totalProd; i++) {
        fatProd = 0;
        for (j = 0; j < totalVendas; j++) {
            if (v[j].produto.codigo == p[i].codigo) fatProd += v[j].valorTotal;
        }
        if (fatProd > maior) {
            maior = fatProd;
            iMaior = i;
        }
    }

    if (iMaior == -1) printf("Ainda nao ha vendas.\n");
    else printf("Produto com maior faturamento: %s (R$ %.2f)\n", p[iMaior].nome, maior);
}

int main(void) {
    Produto produtos[MAX_PROD];
    Venda vendas[MAX_VENDAS];
    int totalProd = 0, totalVendas = 0, opcao;

    do {
        printf("\n===== LOJA =====\n");
        printf("1 - Cadastrar produto\n");
        printf("2 - Listar produtos\n");
        printf("3 - Registrar venda\n");
        printf("4 - Listar vendas\n");
        printf("5 - Faturamento e produto campeao\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: cadastrarProduto(produtos, &totalProd); break;
            case 2: listarProdutos(produtos, totalProd); break;
            case 3: registrarVenda(produtos, totalProd, vendas, &totalVendas); break;
            case 4: listarVendas(vendas, totalVendas); break;
            case 5: relatorio(produtos, totalProd, vendas, totalVendas); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}
