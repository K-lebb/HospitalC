#include <stdio.h>
#include <stdlib.h>
#include "pacientes.h"
#include "medicos.h"

int main() {
    int opcaoAtendente;
    int operacao;

    printf("Digite se deseja entrar na area dos Pacientes(1), Medicos(2), Fila(3): ");
    scanf("%d", &opcaoAtendente);

    switch (opcaoAtendente) {
        case 1:
            printf("Escolha uma operacao:\n");
            printf(" 1 - Cadastrar paciente\n 2 - Consultar por ID\n 3 - Modificar\n 4 - Apagar\n");
            scanf("%d", &operacao);

            if (operacao == 1) {
                cadastrarPacientes();

            } else if (operacao == 2) {
                char idBuscado[50];
                printf("Digite o ID do paciente que deseja buscar: ");
                scanf("%s", idBuscado);
                Paciente pacienteEncontrado = buscarPacientePorID("registroPaciente.txt", idBuscado);
                if (pacienteEncontrado.id != 0) {
                    printf("Paciente encontrado:\n");
                    printf("ID: %s\n", pacienteEncontrado.id);
                    printf("Nome: %s\n", pacienteEncontrado.nome);
                    printf("CPF: %s \n", pacienteEncontrado.cpf);
                    printf("Estado: %d\n", pacienteEncontrado.estado);
                } else {
                    printf("Paciente não encontrado.\n");
                }
                
            } else if (operacao == 3) {
                char idBuscado[50];
                printf("Digite o ID do paciente que deseja modificar: ");
                scanf("%s", idBuscado);

                Paciente pacienteAlterado = modificarPaciente("registroPaciente.txt", idBuscado);

            } else if (operacao == 4) {
                int idRemover;
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
                char idBuscado[50];
                printf("Digite o ID do medico que deseja buscar: ");
                scanf("%s", idBuscado);
                Medico medicoEncontrado = buscarMedicoPorID("registroMedico.txt", idBuscado);
                if (medicoEncontrado.id != 0) {
                    printf("Médico encontrado:\n");
                    printf("Nome: %s\n", medicoEncontrado.nome);
                    printf("CRM: %s\n", medicoEncontrado.crm);
                    printf("ID: %s\n", medicoEncontrado.id);
                    printf("Plantão: %s\n", medicoEncontrado.plantao ? "Sim" : "Não");
                } else {
                    printf("Médico não encontrado.\n");
                }
                
            } else if (operacao == 3) {
                char idBuscado[50];
                printf("Digite o ID do médico que deseja modificar: ");
                scanf("%s", idBuscado);

                Medico medicoAlterado = modificarMedico("registroMedico.txt", idBuscado);

            } else if (operacao == 4) {
                
                int idParaRemover;
                printf("Digite o id que voce deseja apagar: \n");
                scanf("%d", idParaRemover);
                apagarMedico("registroMedico.txt", idParaRemover);

            } else {
                printf("Opcao invalida\n");
            }
            break;

        case 3:
            // Fila (não implementado)
            break;

        default:
            printf("Opção invalida\n");
            abort();  // Termina o programa com erro
            break;
    }

    return 0;
}
