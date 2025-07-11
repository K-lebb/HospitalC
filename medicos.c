#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h> 
#include "medicos.h"
#include "fila.h"

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

    printf("O Médico esta de plantao? (Sim/Nao): ");
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

void listarMedicos(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char linha[150];
    Medico medico;
    while (fgets(linha, sizeof(linha), arquivo)) {
        int plantaoInt;
        if (sscanf(linha, "%9[^;];%49[^;];%49[^;];%d",
                   medico.id, medico.nome, medico.crm, &plantaoInt) == 4) {
            medico.plantao = (plantaoInt != 0);
            printf("ID: %s\nNome: %s\nCRM: %s\nPlantao: %s\n\n",
                   medico.id, medico.nome, medico.crm,
                   medico.plantao ? "Sim" : "Nao");
        }
    }
    fclose(arquivo);
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
    printf("O Médico está de plantao? (Sim/Nao): ");
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
