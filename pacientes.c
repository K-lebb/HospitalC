#include <stdio.h>
#include "pacientes.h"
#include <string.h>
#include <stdlib.h>

static Paciente paciente;

void cadastrarPacientes() {
    printf("Digite o nome do paciente: ");
    scanf("%s", paciente.nome);

    printf("Digite o CPF do paciente: ");
    scanf("%s", paciente.cpf);

    printf("Digite o estado do paciente: ");
    scanf("%s", paciente.estado);

    printf("Paciente cadastrado com sucesso!\n");

    FILE *arq = fopen("bancoDeDados.txt", "a");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        abort();
    }

    fwrite(&paciente, sizeof(Paciente), 1, arq);
    fclose(arq);
    printf("Paciente salvo no arquivo bancoDeDados.txt\n");
}
