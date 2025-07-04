#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "medicos.h"
#include "fila.h"

static Medico medico;

void cadastrarMedico() {
    char servico;

    printf("Digite o nome do Médico: ");
    scanf(" %[^\n]", medico.nome);

    printf("Digite o CRM do médico: ");
    scanf(" %[^\n]", medico.crm);

    printf("O médico está de plantão? [s/n]: ");
    scanf(" %c", &servico);
    medico.plantao = (servico == 's' || servico == 'S');

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

    printf("Médico cadastrado com sucesso! ID gerado: %s\n", medico.id);
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

Medico modificarMedico(const char *nomeArquivo, const char *idBuscado){
    char alterar;
    Medico medicoModificado = buscarMedicoPorID(nomeArquivo, idBuscado);

    if(strcmp(medicoModificado.id, "") == 0){
        printf("Não foi encontrado nenhum médico com esse ID");
        abort();
    }

    printf("Você deseja alterar os dados do Dr %s?: [s/n] \n",medicoModificado.nome);
    scanf(" %c", &alterar); 

    if (alterar == 'n') {
    printf("Então vou encerrar a modificação por aqui! \n");
    abort();
}

    

    
}