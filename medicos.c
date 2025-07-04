#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "medicos.h"

static Medico medico;

void cadastrarMedico() {
    char servico;

    printf("Digite o nome do Médico: ");
    scanf(" %[^\n]", medico.nome);

    printf("Digite o CRM do médico: ");
    scanf("%s", medico.crm);

    printf("O médio está de plantão: [s][n]");
    scanf("%s", servico);

    if (servico == 's'){
        medico.plantao = true;
    }else{
        medico.plantao = false;
    }

    printf("Médico cadastrado com sucesso!\n");

    FILE *arq = fopen("bancoDeDados.txt", "a");

    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        abort();
    }

    fwrite(&medico, sizeof(Medico), 1, arq);
    fclose(arq);
    printf("Paciente salvo no arquivo bancoDeDados.txt\n");
}

Medico BuscarPorID(const char *nomeArquivo, const char *idBuscado){

    FILE *arquivo = fopen(nomeArquivo, "r");
    char linha[150];
    Medico resultado = {"", "", 0, false};
    
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return resultado;
    }

    while (fgets(linha, sizeof(linha), arquivo)) {
        char *nome, *crm, linhaCopia[150], *token;
        int *id;
        bool *plantao;

        nome = resultado.nome;
        crm = resultado.crm;
        id = &resultado.id;
        plantao = &resultado.plantao;

        linha[strcspn(linha, "\n")] = '\0';

        strcpy(linhaCopia, linha);

        token = strtok(linhaCopia, ";");

        if (token != NULL && strcmp(linhaCopia, idBuscado) == 0){
            printf("Pessoa encontrada");
            fclose(arquivo);
        }

    } 
    
}