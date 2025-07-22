#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h> 
#include "medicos.h"
#include "fila.h"
#include "pacientes.h"   // Inclui struct Paciente

static Medico medico;

bool interpretarPlantao(const char *entrada, bool *plantaoOut) {
    if (strcasecmp(entrada, "sim") == 0) {
        *plantaoOut = true;
        return true;
    } else if (strcasecmp(entrada, "nao") == 0 || strcasecmp(entrada, "não") == 0) {
        *plantaoOut = false;
        return true;
    }
    return false;
}

void cadastrarMedico() {
    char entradaPlantao[10];

    printf("Digite o nome do Medico: ");
    scanf(" %[^\n]", medico.nome);

    printf("Digite o CRM do Medico: ");
    scanf(" %[^\n]", medico.crm);

    printf("O Medico esta de plantao? (Sim/Nao): ");
    scanf(" %9s", entradaPlantao);

    if (!interpretarPlantao(entradaPlantao, &medico.plantao)) {
        printf("Entrada inválida para plantao. Use 'Sim' ou 'Nao'. Encerrando...\n");
        abort();
    }

    int proximoID = gerarProximoID("registroMedico.txt");
    sprintf(medico.id, "%d", proximoID);

    FILE *arq = fopen("registroMedico.txt", "a");
    if (!arq) {
        perror("Erro ao abrir o arquivo");
        abort();
    }

    fprintf(arq, "%s;%s;%s;%d\n",
            medico.id, medico.nome, medico.crm, medico.plantao ? 1 : 0);
    fclose(arq);

    printf("Medico cadastrado com sucesso! ID gerado: %s\n", medico.id);
}

Medico buscarMedicoPorID(const char *nomeArquivo, const char *idBuscado) {
    Medico resultado = {"", "", "", false};
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return resultado;
    }

    char linha[150];
    while (fgets(linha, sizeof(linha), arquivo)) {
        char *token = strtok(linha, ";\n");
        if (token && strcmp(token, idBuscado) == 0) {
            strcpy(resultado.id, token);

            token = strtok(NULL, ";\n");
            if (token) strcpy(resultado.nome, token);

            token = strtok(NULL, ";\n");
            if (token) strcpy(resultado.crm, token);

            token = strtok(NULL, ";\n");
            resultado.plantao = token && (strcmp(token, "1") == 0);

            break;
        }
    }
    fclose(arquivo);
    return resultado;
}

void listarMedicos(FILE *saida, int *total) {
    FILE *arquivo = fopen("registroMedico.txt", "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de medico");
        return;
    }

    char linha[150];
    Medico medico;
    int numeroMedicos = 0;

    if (saida) fprintf(saida, "\n===== Lista de Medicos =====\n");

    while (fgets(linha, sizeof(linha), arquivo)) {
        int plantaoInt;
        if (sscanf(linha, "%9[^;];%49[^;];%49[^;];%d",
                   medico.id, medico.nome, medico.crm, &plantaoInt) == 4) {
            medico.plantao = (plantaoInt != 0);
            if (saida)
                fprintf(saida,
                        "\n=== Medico ===\nID: %s\nNome: %s\nCRM: %s\nPlantão: %s\n------------------------------\n",
                        medico.id, medico.nome, medico.crm, medico.plantao ? "Sim" : "Não");
            numeroMedicos++;
        }
    }

    if (saida) fprintf(saida, "\n=== Total de Medicos: %d ===\n", numeroMedicos);
    fclose(arquivo);
    if (total) *total = numeroMedicos;
}

void apagarMedico(const char *nomeArquivo, const int idParaRemover){
    FILE *arquivoOriginal = fopen(nomeArquivo, "r");
    FILE *arquivoTemp = fopen("Temp.txt", "w");
    
    if (!arquivoOriginal || !arquivoTemp){
        printf("Erro ao abrir arquivos.\n");
        abort();
    }

    char linha[1024];
    char idStr[5];
    int encontrou = 0;

    while (fgets(linha, sizeof(linha), arquivoOriginal)) {
        if (sscanf(linha, "%5[^;]", idStr) == 1) {
            int idAtual = atoi(idStr);
            if (idAtual != idParaRemover) {
                fputs(linha, arquivoTemp);
            } else {
                encontrou = 1;
            }
        }
    }

    fclose(arquivoOriginal);
    fclose(arquivoTemp);

    remove(nomeArquivo);
    rename("Temp.txt", nomeArquivo);

    if (encontrou) {
        printf("Medico com id %d removido com sucesso.\n", idParaRemover);
    } else {
        printf("Medico com id %d nao foi encontrado.\n", idParaRemover);
    }
}

void modificarMedico(const char *nomeArquivo, const int idParaAlterar) {
    char idChar[50];
    sprintf(idChar, "%d", idParaAlterar);

    Medico medicoAntigo = buscarMedicoPorID(nomeArquivo, idChar);

    if (medicoAntigo.id[0] == '\0') {
        printf("Medico nao existe.\n");
        abort();
    }

    printf("Nome do Medico: %s\n", medicoAntigo.nome);
    printf("CRM do Medico: %s\n", medicoAntigo.crm);

    apagarMedico(nomeArquivo, idParaAlterar);

    Medico medicoAlterado;
    strcpy(medicoAlterado.id, medicoAntigo.id);
    strcpy(medicoAlterado.crm, medicoAntigo.crm);

    printf("\nDigite os novos dados para o medico:\n");

    printf("Novo nome: ");
    scanf(" %[^\n]", medicoAlterado.nome);

    char entradaPlantao[10];
    printf("O Medico está de plantao? (Sim/Nao): ");
    scanf(" %9s", entradaPlantao);

    if (!interpretarPlantao(entradaPlantao, &medicoAlterado.plantao)) {
        printf("Entrada inválida para plantao. Use 'Sim' ou 'Nao'. Encerrando...\n");
        abort();
    }

    FILE *arquivo = fopen(nomeArquivo, "a");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(arquivo, "%s;%s;%s;%d\n",
            medicoAlterado.id,
            medicoAlterado.nome,
            medicoAlterado.crm,
            medicoAlterado.plantao ? 1 : 0);

    fclose(arquivo);
    printf("Medico atualizado com sucesso!\n");
}

void contarPacientesPorMedico(Medico *medicos, int totalMedicos) {
    FILE *arqPac = fopen("registroPaciente.txt", "r");
    if (!arqPac) {
        printf("Erro ao abrir registroPaciente.txt\n");
        return;
    }

    // Inicializa a contagem para cada médico
    for (int i = 0; i < totalMedicos; i++) {
        medicos[i].totalPacientes = 0;
    }

    char linha[150];
    while (fgets(linha, sizeof(linha), arqPac)) {
        Paciente p;
        if (sscanf(linha, "%9[^;];%49[^;];%49[^;];%49[^;];%d",
                   p.id, p.nome, p.cpf, p.idMedico, &p.estado) == 5) {
            for (int i = 0; i < totalMedicos; i++) {
                if (strcmp(p.idMedico, medicos[i].id) == 0) {
                    medicos[i].totalPacientes++;
                    break;
                }
            }
        }
    }

    fclose(arqPac);
}

int carregarMedicos(const char *nomeArquivo, Medico *medicos, int maxMedicos) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo de médicos");
        return 0;
    }

    int count = 0;
    char linha[150];
    int plantaoInt;

    while (fgets(linha, sizeof(linha), arquivo) && count < maxMedicos) {
        if (sscanf(linha, "%9[^;];%49[^;];%49[^;];%d",
                   medicos[count].id,
                   medicos[count].nome,
                   medicos[count].crm,
                   &plantaoInt) == 4) {
            medicos[count].plantao = (plantaoInt != 0);
            medicos[count].totalPacientes = 0; 
            count++;
        }
    }

    fclose(arquivo);
    return count;
}
