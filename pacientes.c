#include <stdio.h>
#include "pacientes.h"
#include <string.h>
#include <stdlib.h>

static Paciente paciente;

void cadastrarPacientes() {
    printf("Digite o nome do paciente: ");
    scanf(" %[^\n]", paciente.nome);

    printf("Digite o CPF do paciente: ");
    scanf("%s", paciente.cpf);

    printf("Digite o estado do paciente (Grave(3), Moderado(2), Leve(1)): ");
    scanf("%d", &paciente.estado);

    if (paciente.estado != 1 && paciente.estado != 2 && paciente.estado != 3){
        printf("Você anexou um estado que não computamos no sistema!\n");
        system("pause");
        abort();
}


    printf("Paciente cadastrado com sucesso!\n");

    FILE *arq = fopen("registroPaciente.txt", "a");

    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        abort();
    }

    fwrite(&paciente, sizeof(Paciente), 1, arq);
    fclose(arq);
    printf("Paciente salvo no arquivo bancoDeDados.txt\n");
}

