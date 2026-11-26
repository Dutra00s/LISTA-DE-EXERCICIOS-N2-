#include <stdio.h>
#include <string.h>

#define MAX 8
#define HORAS_MES 160

typedef struct {
    int codigo;
    char nome[50];
    char cargo[30];
    float salarioBase;
    int horasExtras;
    float salarioBruto;
    float salarioLiquido;
} Funcionario;

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void lerTexto(char *dest, int tam) {
    fgets(dest, tam, stdin);
    dest[strcspn(dest, "\n")] = '\0';
}

float calcularValorHora(float salarioBase) {
    return salarioBase / HORAS_MES;
}

/* hora extra = valor da hora + 50% */
float calcularValorHoraExtra(float valorHora) {
    return valorHora * 1.5f;
}

float calcularBruto(float salarioBase, float valorHoraExtra, int horasExtras) {
    return salarioBase + valorHoraExtra * horasExtras;
}

/* desconto ficticio de 10% */
float calcularLiquido(float bruto) {
    return bruto - bruto * 0.10f;
}

int main(void) {
    Funcionario func[MAX];
    int qtd, i, iMaior = 0;
    float custoTotal = 0;

    do {
        printf("Quantos funcionarios deseja cadastrar (1 a %d)? ", MAX);
        scanf("%d", &qtd);
        limparBuffer();
    } while (qtd < 1 || qtd > MAX);

    for (i = 0; i < qtd; i++) {
        printf("\nFuncionario %d de %d\n", i + 1, qtd);
        printf("Codigo: ");
        scanf("%d", &func[i].codigo);
        limparBuffer();
        printf("Nome: ");
        lerTexto(func[i].nome, 50);
        printf("Cargo: ");
        lerTexto(func[i].cargo, 30);
        printf("Salario-base: ");
        scanf("%f", &func[i].salarioBase);
        printf("Horas extras: ");
        scanf("%d", &func[i].horasExtras);
        limparBuffer();

        float valorHora = calcularValorHora(func[i].salarioBase);
        float valorHoraExtra = calcularValorHoraExtra(valorHora);
        func[i].salarioBruto = calcularBruto(func[i].salarioBase, valorHoraExtra, func[i].horasExtras);
        func[i].salarioLiquido = calcularLiquido(func[i].salarioBruto);

        custoTotal += func[i].salarioLiquido;
        if (func[i].salarioLiquido > func[iMaior].salarioLiquido) iMaior = i;
    }

    printf("\n=== FOLHA DE PAGAMENTO ===\n");
    printf("%-8s %-20s %-15s %12s %12s\n", "Codigo", "Nome", "Cargo", "Bruto", "Liquido");
    for (i = 0; i < qtd; i++) {
        printf("%-8d %-20s %-15s %12.2f %12.2f\n",
               func[i].codigo, func[i].nome, func[i].cargo,
               func[i].salarioBruto, func[i].salarioLiquido);
    }

    printf("\nMaior salario liquido: %s (R$ %.2f)\n",
           func[iMaior].nome, func[iMaior].salarioLiquido);
    printf("Custo total da folha: R$ %.2f\n", custoTotal);

    return 0;
}
