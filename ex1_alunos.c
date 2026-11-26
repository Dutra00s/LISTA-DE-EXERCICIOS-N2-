#include <stdio.h>
#include <string.h>

#define N 10

typedef struct {
    int matricula;
    char nome[50];
    float nota1, nota2, media;
} Aluno;

/* Limpa o '\n' que o scanf deixa no buffer */
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Le uma linha (aceita espacos) e tira o '\n' do final */
void lerTexto(char *dest, int tam) {
    fgets(dest, tam, stdin);
    dest[strcspn(dest, "\n")] = '\0';
}

/* Funcao que calcula a media individual */
float calcularMedia(float n1, float n2) {
    return (n1 + n2) / 2;
}

int main(void) {
    Aluno turma[N];
    int i, iMaior = 0, iMenor = 0, acima = 0;
    float soma = 0, mediaGeral;

    printf("=== CADASTRO DE %d ALUNOS ===\n", N);
    for (i = 0; i < N; i++) {
        printf("\nAluno %d de %d\n", i + 1, N);
        printf("Matricula: ");
        scanf("%d", &turma[i].matricula);
        limparBuffer();
        printf("Nome: ");
        lerTexto(turma[i].nome, 50);
        printf("Nota 1: ");
        scanf("%f", &turma[i].nota1);
        printf("Nota 2: ");
        scanf("%f", &turma[i].nota2);
        limparBuffer();

        turma[i].media = calcularMedia(turma[i].nota1, turma[i].nota2);
        soma += turma[i].media;

        /* guarda o INDICE do maior e do menor */
        if (turma[i].media > turma[iMaior].media) iMaior = i;
        if (turma[i].media < turma[iMenor].media) iMenor = i;
    }

    mediaGeral = soma / N;

    printf("\n=== SITUACAO DOS ALUNOS ===\n");
    for (i = 0; i < N; i++) {
        printf("%d - %-20s Media: %.2f - %s\n",
               turma[i].matricula, turma[i].nome, turma[i].media,
               turma[i].media >= 6.0 ? "APROVADO" : "REPROVADO");
        if (turma[i].media > mediaGeral) acima++;
    }

    printf("\nMaior media: %s (%.2f)\n", turma[iMaior].nome, turma[iMaior].media);
    printf("Menor media: %s (%.2f)\n", turma[iMenor].nome, turma[iMenor].media);
    printf("Media geral da turma: %.2f\n", mediaGeral);
    printf("Alunos acima da media geral: %d\n", acima);

    return 0;
}
