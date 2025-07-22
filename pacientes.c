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

    printf("Digite o ID do Medico responsável pelo paciente: ");
    scanf("%s", paciente.idMedico);

    Medico medicoEncontrado = buscarMedicoPorID("registroMedico.txt", paciente.idMedico);
    if (medicoEncontrado.id[0] == '\0') {
        printf("Medico com ID %s não encontrado.\n", paciente.idMedico);
        printf("Por favor, cadastre o Medico antes de cadastrar o paciente.\n");
        abort();
    }

    printf("Digite o estado do paciente (Grave(3), Moderado(2), Leve(1)): ");
    scanf("%d", &paciente.estado);

    if (paciente.estado < 1 || paciente.estado > 3) {
        printf("Você anexou um estado que não computamos no sistema!\n");
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

void consultarPacientePorID() {
    char idBuscadoPaciente[50];
    printf("Digite o ID do paciente que deseja buscar: ");
    scanf("%s", idBuscadoPaciente);

    Paciente pacienteEncontrado = buscarPacientePorID("registroPaciente.txt", idBuscadoPaciente);
    if (pacienteEncontrado.id[0] != '\0') {
        printf("Paciente encontrado:\n");
        printf("ID: %s\n", pacienteEncontrado.id);
        printf("Nome: %s\n", pacienteEncontrado.nome);
        printf("CPF: %s\n", pacienteEncontrado.cpf);
        printf("ID do Medico responsável: %s \n", pacienteEncontrado.idMedico);
        printf("Estado do paciente: ");
        if (pacienteEncontrado.estado == 3) {
            printf("Grave \n");
        } else if (pacienteEncontrado.estado == 2) {
            printf("Moderado \n");
        } else {
            printf("Leve \n");
        }
    } else {
        printf("Paciente não encontrado.\n");
    }
}

Paciente buscarPacientePorID(const char *nomeArquivo, const char *idBuscado) {
    Paciente resultado = {"", "", "", "", 0};
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
            if (token) strcpy(resultado.cpf, token);

            token = strtok(NULL, ";\n");
            if (token) strcpy(resultado.idMedico, token);

            token = strtok(NULL, ";\n");
            if (token) resultado.estado = atoi(token);

            break;
        }
    }
    fclose(arquivo);
    return resultado;
}

void modificarPaciente(const char *nomeArquivo, const int idParaAlterar) {
    char idChar[50];
    sprintf(idChar, "%d", idParaAlterar);

    Paciente pacienteAntigo = buscarPacientePorID(nomeArquivo, idChar);

    if (pacienteAntigo.id[0] == '\0') {
        printf("Paciente nao existe.\n");
        return;
    }

    printf("Nome do Paciente: %s\n", pacienteAntigo.nome);
    printf("CPF do Paciente: %s\n", pacienteAntigo.cpf);

    apagarPaciente(nomeArquivo, idParaAlterar);

    Paciente pacienteAlterado;
    strcpy(pacienteAlterado.id, pacienteAntigo.id);
    strcpy(pacienteAlterado.cpf, pacienteAntigo.cpf); 

    printf("\nDigite os novos dados para o paciente:\n");

    printf("Novo nome: ");
    scanf(" %[^\n]", pacienteAlterado.nome);

    printf("Novo ID do Medico responsável: ");
    scanf("%s", pacienteAlterado.idMedico);

    Medico medicoEncontrado = buscarMedicoPorID("registroMedico.txt", pacienteAlterado.idMedico);
    if (medicoEncontrado.id[0] == '\0') {
        printf("Medico com ID %s não encontrado.\n", pacienteAlterado.idMedico);
        printf("Por favor, cadastre o Medico antes.\n");
        return;
    }

    printf("Novo estado do paciente (Grave(3), Moderado(2), Leve(1)): ");
    scanf("%d", &pacienteAlterado.estado);

    FILE *arquivo = fopen(nomeArquivo, "a");
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
    printf("Paciente atualizado com sucesso!\n");
}

void apagarPaciente(const char *nomeArquivo, const int idParaRemover){
    FILE *arquivoOriginal = fopen(nomeArquivo, "r");
    FILE *arquivoTemp = fopen("Temp.txt", "w");
    
    if (!arquivoOriginal || !arquivoTemp){
        printf("Erro ao abrir arquivos.\n");
        abort();
    }

    char linha[1024];
    char idStr[10];
    int encontrou = 0;

    while(fgets(linha, sizeof(linha), arquivoOriginal)){
        if (sscanf(linha, "%9[^;]", idStr) == 1){
            int idAtual = atoi(idStr);
            if (idAtual != idParaRemover){
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

    if (encontrou){
        printf("Paciente com id %d removido com sucesso.\n", idParaRemover);
    } else {
        printf("Paciente com id %d nao foi encontrado.\n", idParaRemover);
    }
}

void listarPacientes(FILE *saida, int *total) {
    FILE *arquivo = fopen("registroPaciente.txt", "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de pacientes");
        return;
    }

    int numeroPacientes = 0;
    char linha[150];
    Paciente paciente;

    if (saida) fprintf(saida, "\n===== Lista de Pacientes =====\n");

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (sscanf(linha, "%9[^;];%49[^;];%49[^;];%49[^;];%d",
                   paciente.id, paciente.nome, paciente.cpf, paciente.idMedico, &paciente.estado) == 5) {
            if (saida)
                fprintf(saida,
                        "\n=== Paciente ===\nID: %s\nNome: %s\nCPF: %s\nEstado do Paciente: %d\nID do medico vinculado: %s\n------------------------------\n",
                        paciente.id, paciente.nome, paciente.cpf,
                        paciente.estado, paciente.idMedico);
            numeroPacientes++;
        }
    }

    if (saida) fprintf(saida, "\n=== Total de Pacientes: %d ===\n", numeroPacientes);
    fclose(arquivo);
    if (total) *total = numeroPacientes;
}

void ambulatorio(int *totalAltas, int *totalInternados, int *totalModerados, int *totalLeves) {
    *totalAltas = 0;
    *totalInternados = 0;
    *totalModerados = 0;
    *totalLeves = 0;

    FILE *arquivoAltas = fopen("registroAlta.txt", "r");
    if (arquivoAltas) {
        char linha[150];
        while (fgets(linha, sizeof(linha), arquivoAltas)) {
            (*totalAltas)++;
        }
        fclose(arquivoAltas);
    }

    FILE *arquivoPacientes = fopen("registroPaciente.txt", "r");
    if (!arquivoPacientes) {
        printf("Erro ao abrir registro dos pacientes!\n");
        return;
    }

    Paciente paciente;
    char linha[150];
    while (fgets(linha, sizeof(linha), arquivoPacientes)) {
        if (sscanf(linha, "%9[^;];%49[^;];%49[^;];%49[^;];%d",
                   paciente.id, paciente.nome, paciente.cpf, paciente.idMedico, &paciente.estado) == 5) {
            if (paciente.estado == 3) (*totalInternados)++;
            else if (paciente.estado == 2) (*totalModerados)++;
            else if (paciente.estado == 1) (*totalLeves)++;
        }
    }
    fclose(arquivoPacientes);
}
