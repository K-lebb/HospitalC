#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fila.h"
#include "medicos.h"
#include "pacientes.h"

static Paciente paciente;

void cadastrarPacientes() {
    int proximoID = gerarProximoID("registroPaciente.txt");
    sprintf(paciente.id, "%d", proximoID);

    printf("Digite o nome do paciente: ");
    scanf(" %[^\n]", paciente.nome);

    printf("Digite o CPF do paciente: ");
    scanf("%s", paciente.cpf);

    printf("Digite o ID do médico responsável pelo paciente: ");
    scanf("%s", paciente.idMedico);

    Medico medicoEncontrado = BuscarMedicoPorID("registroMedico.txt", paciente.idMedico);
    if (medicoEncontrado.id[0] == '\0') {
        printf("Médico com ID %s não encontrado.\n", paciente.idMedico);
        printf("Por favor, cadastre o médico antes de cadastrar o paciente.\n");
        system("pause");
        abort();
    }

    printf("Digite o estado do paciente (Grave(3), Moderado(2), Leve(1)): ");
    scanf("%d", &paciente.estado);

    if (paciente.estado < 1 || paciente.estado > 3) {
        printf("Você anexou um estado que não computamos no sistema!\n");
        system("pause");
        abort();
    }

    FILE *arq = fopen("registroPaciente.txt", "a");
    if (!arq) {
        perror("Erro ao abrir o arquivo");
        abort();
    }

    fprintf(arq, "%s;%s;%s;%s;%d\n",
            paciente.id, paciente.nome, paciente.cpf, paciente.idMedico, paciente.estado);
    fclose(arq);

    printf("Paciente cadastrado com sucesso!\n");
    printf("Paciente salvo no arquivo registroPaciente.txt\n");
}
