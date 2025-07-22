#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "medicos.h"
#include "pacientes.h"
#include "fila.h"

void esperarEnter() {
    printf("\nPressione Enter para continuar...\n");
    while (getchar() != '\n'); // consome até o fim da linha atual
}

// Gera o próximo ID baseado no maior existente no arquivo
int gerarProximoID(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        return 1;
    }

    char linha[150];
    char idStr[20];
    int maiorID = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (sscanf(linha, "%19[^;];", idStr) == 1) {
            int idAtual = atoi(idStr);
            if (idAtual > maiorID) {
                maiorID = idAtual;
            }
        }
    }

    fclose(arquivo);
    return maiorID + 1;
}

void listarMedicosDesignados() {
    FILE *arqMed = fopen("registroMedico.txt", "r");
    if (!arqMed) {
        perror("Erro ao abrir medicos");
        return;
    }

    char linhaMed[150];
    Medico medico;

    while (fgets(linhaMed, sizeof(linhaMed), arqMed)) {
        int plantaoInt;

        if (sscanf(linhaMed, "%9[^;];%49[^;];%49[^;];%d",
                   medico.id, medico.nome, medico.crm, &plantaoInt) == 4) {

            medico.plantao = (plantaoInt != 0);

            printf("\n=== Medico ===\n");
            printf("ID: %s\n", medico.id);
            printf("Nome: %s\n", medico.nome);
            printf("CRM: %s\n", medico.crm);
            printf("Plantão: %s\n", medico.plantao ? "Sim" : "Não");
            printf("------------------------------\n");

            FILE *arqPac = fopen("registroPaciente.txt", "r");
            if (!arqPac) {
                perror("Erro ao abrir pacientes");
                continue;
            }

            char linhaPaciente[150];
            Paciente paciente;

            Paciente pacientes[100];
            int qtdPacientes = 0;

            while (fgets(linhaPaciente, sizeof(linhaPaciente), arqPac)) {
                if (sscanf(linhaPaciente, "%9[^;];%49[^;];%49[^;];%49[^;];%d",
                           paciente.id, paciente.nome, paciente.cpf,
                           paciente.idMedico, &paciente.estado) == 5) {

                    if (strcmp(paciente.idMedico, medico.id) == 0) {
                        if (qtdPacientes < 100) { // evitar overflow
                            pacientes[qtdPacientes++] = paciente;
                        }
                    }
                }
            }

            fclose(arqPac);

            // Ordena pacientes pelo estado em ordem decrescente (3 a 0)
            for (int i = 0; i < qtdPacientes - 1; i++) {
                for (int j = i + 1; j < qtdPacientes; j++) {
                    if (pacientes[i].estado < pacientes[j].estado) {
                        Paciente temp = pacientes[i];
                        pacientes[i] = pacientes[j];
                        pacientes[j] = temp;
                    }
                }
            }

            printf("=== Pacientes Relacionados ===\n");

            for (int i = 0; i < qtdPacientes; i++) {
                Paciente p = pacientes[i];
                printf("\n=== Paciente ===\n");
                printf("ID: %s\n", p.id);
                printf("Nome: %s\n", p.nome);
                printf("CPF: %s\n", p.cpf);
                printf("ID do medico vinculado: %s\n", p.idMedico);
                printf("Estado do Paciente: %d\n", p.estado);

                switch (p.estado) {
                    case 3:
                        printf("Estado: GRAVE (Internação)\n");
                        break;
                    case 2:
                        printf("Estado: MODERADO\n");
                        break;
                    case 1:
                        printf("Estado: LEVE\n");
                        break;
                    case 0:
                        printf("Estado: ALTA\n");
                        break;
                    default:
                        printf("Estado: DESCONHECIDO\n");
                }

                printf("------------------------------\n");
            }
        }
    }

    fclose(arqMed);
}

void trocarMedico(const int idParaAlterar) {
    char idChar[50];
    sprintf(idChar, "%d", idParaAlterar);

    Paciente paciente = buscarPacientePorID("registroPaciente.txt", idChar);

    if (paciente.id[0] == '\0') {
        printf("Paciente não existe.\n");
        return;
    }

    printf("Nome do Paciente: %s\n", paciente.nome);
    printf("CPF do Paciente: %s\n", paciente.cpf);

    Paciente pacienteAlterado;
    strcpy(pacienteAlterado.nome, paciente.nome);
    strcpy(pacienteAlterado.idMedico, paciente.idMedico);
    strcpy(pacienteAlterado.id, paciente.id);
    strcpy(pacienteAlterado.cpf, paciente.cpf);

    printf("Novo ID do Medico responsável: ");
    scanf(" %49[^\n]", pacienteAlterado.idMedico);

    Medico medicoEncontrado = buscarMedicoPorID("registroMedico.txt", pacienteAlterado.idMedico);
    if (medicoEncontrado.id[0] == '\0') {
        printf("Medico com ID %s não encontrado.\n", pacienteAlterado.idMedico);
        printf("Por favor, cadastre o medico antes.\n");
        return;
    }

    printf("Novo estado do paciente (Grave(3), Moderado(2), Leve(1), Alta(0)): ");
    scanf("%d", &pacienteAlterado.estado);

    if (pacienteAlterado.estado == 3) {
        printf("Ele não pode ser trocado de medico.\n");
        return;
    }

    apagarPaciente("registroPaciente.txt", idParaAlterar);

    FILE *arquivo = fopen("registroPaciente.txt", "a");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(arquivo, "%s;%s;%s;%s;%d\n",
            pacienteAlterado.id,
            pacienteAlterado.nome,
            pacienteAlterado.cpf,
            pacienteAlterado.idMedico,
            pacienteAlterado.estado);

    fclose(arquivo);
    printf("Medico do paciente atualizado com sucesso!\n");
}

void trocarEstado(const int idParaAlterar) {
    char idChar[50];
    sprintf(idChar, "%d", idParaAlterar);

    Paciente paciente = buscarPacientePorID("registroPaciente.txt", idChar);

    if (paciente.id[0] == '\0') {
        printf("Paciente não encontrado.\n");
        return;
    }

    printf("Paciente encontrado:\n");
    printf("ID: %s\n", paciente.id);
    printf("Nome: %s\n", paciente.nome);
    printf("CPF: %s\n", paciente.cpf);
    printf("ID do medico responsável: %s\n", paciente.idMedico);
    printf("Estado atual: %d\n", paciente.estado);

    int novoEstado;
    printf("Novo estado do paciente (Grave(3), Moderado(2), Leve(1), Alta(0)): ");
    scanf("%d", &novoEstado);

    if (novoEstado < 0 || novoEstado > 3) {
        printf("Estado inválido. Operação cancelada.\n");
        return;
    }

    if (novoEstado == 0) {
        printf("Paciente está de alta! Removendo do Banco de Dados principal...\n");
        apagarPaciente("registroPaciente.txt", idParaAlterar);

        FILE *arquivo = fopen("registroAlta.txt", "a");
        if (!arquivo) {
            perror("Erro ao abrir o arquivo para escrita");
            return;
        }

        fprintf(arquivo, "%s;%s;%s;%s;%d\n",
                paciente.id,
                paciente.nome,
                paciente.cpf,
                paciente.idMedico,
                novoEstado);

        fclose(arquivo);
        printf("Paciente arquivado com sucesso!\n");
        return;
    }

    // Atualiza estado do paciente
    paciente.estado = novoEstado;

    apagarPaciente("registroPaciente.txt", idParaAlterar);

    FILE *arquivo = fopen("registroPaciente.txt", "a");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }

    fprintf(arquivo, "%s;%s;%s;%s;%d\n",
            paciente.id,
            paciente.nome,
            paciente.cpf,
            paciente.idMedico,
            paciente.estado);

    fclose(arquivo);
    printf("Estado do paciente atualizado com sucesso!\n");
}

void relatorio() {
    FILE *arquivo = fopen("relatorio.txt", "w");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }

    Medico medicos[100];
    int totalMedicos = carregarMedicos("registroMedico.txt", medicos, 100); // carrega médicos

    contarPacientesPorMedico(medicos, totalMedicos); // conta pacientes para cada médico

    fprintf(arquivo, "\n===== PACIENTES POR MÉDICO =====\n");
    for (int i = 0; i < totalMedicos; i++) {
        fprintf(arquivo, "Médico %s (%s) - Total de Pacientes: %d\n",
                medicos[i].id, medicos[i].nome, medicos[i].totalPacientes);
    }

    int totalPacientes = 0, totalAltas = 0, totalInternados = 0, totalModerados = 0, totalLeves = 0;

    listarPacientes(arquivo, &totalPacientes);
    ambulatorio(&totalAltas, &totalInternados, &totalModerados, &totalLeves);

    fprintf(arquivo,
        "\n===== RESUMO FINAL =====\n"
        "Total de Medicos: %d\n"
        "Total de Pacientes: %d\n"
        "Total de Pacientes em Alta: %d\n"
        "Total de Pacientes Internados: %d\n"
        "Total de Pacientes em estado Moderado: %d\n"
        "Total de Pacientes em estado Leve: %d\n",
        totalMedicos, totalPacientes, totalAltas,
        totalInternados, totalModerados, totalLeves);

    fclose(arquivo);
    printf("Relatório gerado com sucesso!\n");
}
