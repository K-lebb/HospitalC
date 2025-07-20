#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "medicos.h"
#include "pacientes.h"
#include "fila.h"

void esperarEnter() {
    printf("\n Pressione Enter para continuar...\n");
    while (getchar() != '\n'); 
    getchar(); 
}

int gerarProximoID(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
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
        perror("Erro ao abrir médicos");
        return;
    }

    char linhaMed[150];
    Medico medico;

    while (fgets(linhaMed, sizeof(linhaMed), arqMed)) {
        int plantaoInt;

        if (sscanf(linhaMed, "%9[^;];%49[^;];%49[^;];%d",
                   medico.id, medico.nome, medico.crm, &plantaoInt) == 4) {

            medico.plantao = (plantaoInt != 0);

            printf("\n=== Médico ===\n");
            printf("ID: %s\n", medico.id);
            printf("Nome: %s\n", medico.nome);
            printf("CRM: %s\n", medico.crm);
            printf("Plantão: %s\n", medico.plantao ? "Sim" : "Não");
            printf("------------------------------\n");

            // Abre pacientes
            FILE *arqPac = fopen("registroPaciente.txt", "r");
            if (!arqPac) {
                perror("Erro ao abrir pacientes");
                continue;
            }

            char linhaPac[150];
            Paciente paciente;

            // Vetor para armazenar pacientes associados ao médico
            Paciente pacientes[100];
            int qtdPacientes = 0;

            while (fgets(linhaPac, sizeof(linhaPac), arqPac)) {
                if (sscanf(linhaPac, "%9[^;];%49[^;];%49[^;];%49[^;];%d",
                           paciente.id, paciente.nome, paciente.cpf,
                           paciente.idMedico, &paciente.estado) == 5) {

                    if (strcmp(paciente.idMedico, medico.id) == 0) {
                        pacientes[qtdPacientes++] = paciente;
                    }
                }
            }

            fclose(arqPac);

            // Ordenar pacientes por gravidade (estado decrescente)
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
                printf("ID do médico vinculado: %s\n", p.idMedico);
                printf("Estado do Paciente: %d\n", p.estado);

                switch (p.estado) {
                    case 3:
                        printf("Estado: INTERNAÇÃO\n");
                        break;
                    case 2:
                        printf("Estado: GRAVE\n");
                        break;
                    case 1:
                        printf("Estado: MÉDIO\n");
                        break;
                    case 0:
                        printf("Estado: LEVE\n");
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

    printf("Novo ID do Médico responsável: ");
    scanf(" %49[^\n]", pacienteAlterado.idMedico);

    Medico medicoEncontrado = buscarMedicoPorID("registroMedico.txt", pacienteAlterado.idMedico);
    if (medicoEncontrado.id[0] == '\0') {
        printf("Médico com ID %s não encontrado.\n", pacienteAlterado.idMedico);
        printf("Por favor, cadastre o médico antes.\n");
        return;
    }

    printf("Novo estado do paciente (Grave(3), Moderado(2), Leve(1), Alta(0)): ");
    scanf("%d", &pacienteAlterado.estado);

    if (pacienteAlterado.estado == 3) {
        printf("Ele não pode ser trocado de médico.\n");
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
    printf("Médico do paciente atualizado com sucesso!\n");
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
    printf("ID do médico responsável: %s\n", paciente.idMedico);
    printf("Estado atual: %d\n", paciente.estado);

    int novoEstado;
    printf("Novo estado do paciente (Grave(3), Moderado(2), Leve(1), Alta(0)): ");
    scanf("%d", &novoEstado);

    paciente.estado = novoEstado;

    if (novoEstado < 0 || novoEstado > 3) {
        printf("Estado inválido. Operação cancelada.\n");
        return;

    }else if(novoEstado == 0){
        printf("Paciente está de alta! Vamos tirar ele do Banco de Dados [...] \n");
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
            paciente.estado);

        fclose(arquivo);
        printf("Estado do paciente atualizado com sucesso!\n");
        
        return;

    }

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

    int totalMedicos = 0;
    int totalPacientes = 0;

    listarMedicos("registroMedico.txt", arquivo, &totalMedicos);
    listarPacientes("registroPaciente.txt", arquivo, &totalPacientes);

    fprintf(arquivo,
        "\n===== RESUMO FINAL =====\n"
        "Total de Médicos: %d\n"
        "Total de Pacientes: %d\n",
        totalMedicos, totalPacientes);

    fclose(arquivo);
    printf("Relatório gerado com sucesso!\n");
}


