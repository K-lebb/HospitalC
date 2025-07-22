#include <stdio.h>
#include <stdlib.h>
#include "pacientes.h"
#include "medicos.h"
#include "fila.h"

int main() {
    int opcaoAtendente;
    int operacao;
    int idRemover;
    int idParaAlterar;
    char idBuscado[50];
    

    printf("Digite se deseja entrar na area dos Pacientes(1), Medicos(2), Fila(3) ou Sair(0): ");
    scanf("%d", &opcaoAtendente);

    switch (opcaoAtendente) {

        case 0:
            printf("Delisgando Sistema e gerando relatorio[...] \n");

            break;

        case 1:

            printf("Escolha uma operacao:\n");
            printf(" 1 - Cadastrar paciente\n 2 - Consultar por ID\n 3 - Modificar\n 4 - Apagar\n");
            scanf("%d", &operacao);

            if (operacao == 1) {
                cadastrarPacientes();
            } else if (operacao == 2) {

                printf("Digite o ID do paciente que deseja buscar: ");
                scanf("%s", idBuscado);
                Paciente pacienteEncontrado = buscarPacientePorID("registroPaciente.txt", idBuscado);

                if (pacienteEncontrado.id != 0) {

                    printf("Paciente encontrado:\n");
                    printf("ID: %s\n", pacienteEncontrado.id);
                    printf("Nome: %s\n", pacienteEncontrado.nome);
                    printf("CPF: %s \n", pacienteEncontrado.cpf);
                    printf("ID do medico responsavel: %s \n", pacienteEncontrado.idMedico);
                    printf("Estado: %d\n", pacienteEncontrado.estado);

                } else {

                    printf("Paciente não encontrado.\n");

                }
            } else if (operacao == 3) {

                printf("Digite o ID do paciente que deseja modificar: ");
                scanf("%d", &idParaAlterar);
                modificarPaciente("registroPaciente.txt", idParaAlterar);
            } else if (operacao == 4) {
                
                printf("Digite o id que voce deseja apagar: \n");
                scanf("%d", &idRemover);
                apagarPaciente("registroPaciente.txt", idRemover);
            } else {
                printf("Opcao invalida\n");
            }

            break;

        case 2:

            printf("Escolha uma operacao:\n");
            printf(" 1 - Cadastrar medico\n 2 - Consultar por ID\n 3 - Modificar\n 4 - Apagar\n");
            scanf("%d", &operacao);

            if (operacao == 1) {

                cadastrarMedico();

            } else if (operacao == 2) {

                printf("Digite o ID do medico que deseja buscar: ");
                scanf("%s", idBuscado);

                Medico medicoEncontrado = buscarMedicoPorID("registroMedico.txt", idBuscado);

                if (medicoEncontrado.id != 0) {

                    printf("Medico encontrado:\n");
                    printf("Nome: %s\n", medicoEncontrado.nome);
                    printf("CRM: %s\n", medicoEncontrado.crm);
                    printf("ID: %s\n", medicoEncontrado.id);
                    printf("Plantão: %s\n", medicoEncontrado.plantao ? "Sim" : "Não");

                } else {

                    printf("Medico não encontrado.\n");

                }

                
            } else if (operacao == 3) {

                printf("Digite o ID do Medico que deseja modificar: ");
                scanf("%d", &idParaAlterar);

                modificarMedico("registroMedico.txt", idParaAlterar);

            } else if (operacao == 4) {
                
                int idParaRemover;
                printf("Digite o id que voce deseja apagar: \n");
                scanf("%d", &idParaRemover);
                apagarMedico("registroMedico.txt", idParaRemover);

            } else {
                printf("Opcao invalida\n");
            }
            break;

        case 3:
            printf("Escolha uma operacao:\n");
            printf(" 1 - Listar o Medico Designado de Cada Paciente\n 2 - Trocar de Medico\n 3 - Alterar Estado do Paciente\n ");
            scanf("%d", &operacao);


            if (operacao == 1) {

                listarMedicosDesignados();

            } else if (operacao == 2) {

                printf("Digite o ID do Paciente que deseja trocar o Medico: ");
                scanf("%d", &idParaAlterar);

                trocarMedico(idParaAlterar);

            } else if (operacao == 3) {

                printf("Digite o ID do Paciente que deseja trocar o estado: ");
                scanf("%d", &idParaAlterar);

                trocarEstado(idParaAlterar);

            } else {
                printf("Opcao invalida\n");
            }

            
            break;

        default:
            printf("Opcao invalida\n");
            abort();  
            break;
    }

    esperarEnter();
    relatorio();


    return 0;
}
