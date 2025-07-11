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

    Medico medicoEncontrado = buscarMedicoPorID("registroMedico.txt", paciente.idMedico);
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

void consultarPacientePorID() {
    char idBuscadoPaciente[50];
    printf("Digite o ID do paciente que deseja buscar: ");
    scanf("%s", idBuscadoPaciente);

    Paciente pacienteEncontrado = buscarPacientePorID("registroPaciente.txt", idBuscadoPaciente);
    if (pacienteEncontrado.id != 0) {
        printf("Paciente encontrado:\n");
        printf("ID: %s\n", pacienteEncontrado.id);
        printf("Nome: %s\n", pacienteEncontrado.nome);
        printf("CPF: %s\n", pacienteEncontrado.cpf);
        printf("ID do Médico responsável: %s \n", pacienteEncontrado.idMedico);
        printf("Estado do paciente: ");
        if (pacienteEncontrado.estado == 3) {
        printf("Grave");
        } else if (pacienteEncontrado.estado == 2) {
            printf("Moderado");
        } else {
            printf("Leve");
        }
    } else {
        printf("Paciente não encontrado.\n");
    }
}

Paciente buscarPacientePorID(const char *nomeArquivo, const char *idBuscado) {
    char texto[50];
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

void modificarPaciente(const char *nomeArquivo, const int idParaAlterar){
    char idChar[50];

    sprintf(idChar, "%d", idParaAlterar);

    Paciente pacienteAntigo = buscarPacientePorID("registroPaciente.txt", idChar); 

    if(pacienteAntigo.id == '\0'){
        printf("Paciente não existe");
        abort();
    }

    printf("Nome do Paciente: %s\n", pacienteAntigo.nome);
    printf("CPF do Paciente: %s\n", pacienteAntigo.cpf);

    apagarPaciente("registroPacient.txt", idParaAlterar);

    Paciente pacienteAlterado;
    strcpy(pacienteAlterado.id, pacienteAntigo.id);

    printf("\nDigite os novos dados para o paciente:\n");

    printf("Novo nome: ");
    scanf(" %[^\n]", pacienteAlterado.nome);

    printf("Novo CPF: ");
    scanf("%s", pacienteAlterado.cpf);

    printf("Novo ID do médico responsável: ");
    scanf("%s", pacienteAlterado.idMedico);

    Medico medicoEncontrado = buscarMedicoPorID("registroMedico.txt", pacienteAlterado.idMedico);
    if (medicoEncontrado.id[0] == '\0') {
        printf("Médico com ID %s não encontrado.\n", pacienteAlterado.idMedico);
        printf("Por favor, cadastre o médico antes.\n");
        system("pause");
        return;
    }

    printf("Novo estado do paciente (Grave(3), Moderado(2), Leve(1)): ");
    scanf("%d", &pacienteAlterado.estado);

    FILE *arq = fopen(nomeArquivo, "a");
    if (!arq) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(arq, "%s;%s;%s;%s;%d\n",
            pacienteAlterado.id,
            pacienteAlterado.nome,
            pacienteAlterado.cpf,
            pacienteAlterado.idMedico,
            pacienteAlterado.estado);
    fclose(arq);

    printf("Paciente atualizado com sucesso!\n");
}


void apagarPaciente(const char *nomeArquivo, const int idParaRemover){
    FILE *arquivoOriginal = fopen(nomeArquivo, "r");
    FILE *arquivoTemp = fopen("Temp.txt", "w");
    
    if (!arquivoOriginal || !arquivoTemp){
        printf("Erro ao abrir");
        system("pause");
        abort();
    }

    char linha[1024];
    char idStr[5];
    int encontrou = 0;

    while(fgets(linha, sizeof(linha), arquivoOriginal)){

        if (sscanf(linha, "%5[^;]", idStr) == 1){

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
        printf("Paciente com id &d nao foi encontrado.\n", idParaRemover);
    }
}