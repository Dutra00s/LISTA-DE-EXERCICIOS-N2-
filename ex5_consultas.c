#include <stdio.h>
#include <string.h>

#define MAX_PAC 15
#define MAX_CONS 30

typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    int codigo;
    char nome[50];
    int idade;
} Paciente;

typedef struct {
    Paciente paciente;
    Data data;
    char descricao[100];
} Consulta;

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void lerTexto(char *dest, int tam) {
    fgets(dest, tam, stdin);
    dest[strcspn(dest, "\n")] = '\0';
}

int buscarPaciente(Paciente p[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (p[i].codigo == codigo) return i;
    }
    return -1;
}

int dataIgual(Data a, Data b) {
    return a.dia == b.dia && a.mes == b.mes && a.ano == b.ano;
}

/* Le uma data e repete ate ser valida (validacao simples) */
Data lerData(void) {
    Data d;
    do {
        printf("Data (dd mm aaaa): ");
        scanf("%d %d %d", &d.dia, &d.mes, &d.ano);
        limparBuffer();
        if (d.dia < 1 || d.dia > 31 || d.mes < 1 || d.mes > 12 || d.ano < 1900)
            printf("Data invalida, tente de novo.\n");
    } while (d.dia < 1 || d.dia > 31 || d.mes < 1 || d.mes > 12 || d.ano < 1900);
    return d;
}

void imprimirConsulta(Consulta c) {
    printf("%02d/%02d/%04d | %s (cod %d) | %s\n",
           c.data.dia, c.data.mes, c.data.ano,
           c.paciente.nome, c.paciente.codigo, c.descricao);
}

int contarConsultas(Consulta c[], int total, int codigoPaciente) {
    int i, cont = 0;
    for (i = 0; i < total; i++) {
        if (c[i].paciente.codigo == codigoPaciente) cont++;
    }
    return cont;
}

void cadastrarPaciente(Paciente p[], int *total) {
    int codigo;
    if (*total >= MAX_PAC) {
        printf("Limite de %d pacientes atingido.\n", MAX_PAC);
        return;
    }
    printf("Codigo: ");
    scanf("%d", &codigo);
    limparBuffer();
    if (buscarPaciente(p, *total, codigo) != -1) {
        printf("Erro: codigo ja cadastrado.\n");
        return;
    }
    p[*total].codigo = codigo;
    printf("Nome: ");
    lerTexto(p[*total].nome, 50);
    printf("Idade: ");
    scanf("%d", &p[*total].idade);
    limparBuffer();
    (*total)++;
    printf("Paciente cadastrado!\n");
}

void registrarConsulta(Paciente p[], int totalPac, Consulta c[], int *totalCons) {
    int codigo, pos;
    if (*totalCons >= MAX_CONS) {
        printf("Limite de %d consultas atingido.\n", MAX_CONS);
        return;
    }
    printf("Codigo do paciente: ");
    scanf("%d", &codigo);
    limparBuffer();
    pos = buscarPaciente(p, totalPac, codigo);
    if (pos == -1) {
        printf("Paciente nao encontrado.\n");
        return;
    }
    c[*totalCons].paciente = p[pos];
    c[*totalCons].data = lerData();
    printf("Descricao do atendimento: ");
    lerTexto(c[*totalCons].descricao, 100);
    (*totalCons)++;
    printf("Consulta registrada!\n");
}

void pesquisarPorPaciente(Consulta c[], int total) {
    int codigo, i, achou = 0;
    printf("Codigo do paciente: ");
    scanf("%d", &codigo);
    limparBuffer();
    for (i = 0; i < total; i++) {
        if (c[i].paciente.codigo == codigo) {
            imprimirConsulta(c[i]);
            achou = 1;
        }
    }
    if (!achou) printf("Nenhuma consulta para esse paciente.\n");
}

void listarPorData(Consulta c[], int total) {
    int i, achou = 0;
    Data d = lerData();
    for (i = 0; i < total; i++) {
        if (dataIgual(c[i].data, d)) {
            imprimirConsulta(c[i]);
            achou = 1;
        }
    }
    if (!achou) printf("Nenhuma consulta nessa data.\n");
}

void consultasPorPaciente(Paciente p[], int totalPac, Consulta c[], int totalCons) {
    int i;
    if (totalPac == 0) {
        printf("Nenhum paciente cadastrado.\n");
        return;
    }
    for (i = 0; i < totalPac; i++) {
        printf("%-25s (cod %d): %d consulta(s)\n", p[i].nome, p[i].codigo,
               contarConsultas(c, totalCons, p[i].codigo));
    }
}

void pacienteMaisAtendido(Paciente p[], int totalPac, Consulta c[], int totalCons) {
    int i, qtd, maior = 0;
    for (i = 0; i < totalPac; i++) {
        qtd = contarConsultas(c, totalCons, p[i].codigo);
        if (qtd > maior) maior = qtd;
    }
    if (maior == 0) {
        printf("Ainda nao ha consultas registradas.\n");
        return;
    }
    printf("Maior numero de atendimentos: %d\n", maior);
    for (i = 0; i < totalPac; i++) {   /* lista tambem os empatados */
        if (contarConsultas(c, totalCons, p[i].codigo) == maior)
            printf("-> %s (cod %d)\n", p[i].nome, p[i].codigo);
    }
}

int main(void) {
    Paciente pacientes[MAX_PAC];
    Consulta consultas[MAX_CONS];
    int totalPac = 0, totalCons = 0, opcao;

    do {
        printf("\n===== CLINICA =====\n");
        printf("1 - Cadastrar paciente\n");
        printf("2 - Registrar consulta\n");
        printf("3 - Pesquisar consultas de um paciente\n");
        printf("4 - Listar consultas de uma data\n");
        printf("5 - Consultas por paciente\n");
        printf("6 - Paciente com mais atendimentos\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: cadastrarPaciente(pacientes, &totalPac); break;
            case 2: registrarConsulta(pacientes, totalPac, consultas, &totalCons); break;
            case 3: pesquisarPorPaciente(consultas, totalCons); break;
            case 4: listarPorData(consultas, totalCons); break;
            case 5: consultasPorPaciente(pacientes, totalPac, consultas, totalCons); break;
            case 6: pacienteMaisAtendido(pacientes, totalPac, consultas, totalCons); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}
