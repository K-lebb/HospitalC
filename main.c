#include <stdio.h>
#include <stdlib.h>
#include "pacientes.h"
#include "medicos.h"

int main() {
    int opcaoAtendente;
    int operacao;

    printf("Digite se deseja entrar na área dos Pacientes(1), Médicos(2), Fila(3): ");
    scanf("%d", &opcaoAtendente);

    switch (opcaoAtendente) {
        case 1:
            printf("Escolha uma operação:\n");
            printf("1 - Cadastrar paciente\n2 - Consultar por ID\n3 - Modificar\n4 - Apagar\n");
            scanf("%d", &operacao);

            if (operacao == 1) {
                cadastrarPacientes();

            } else if (operacao == 2) {

            } else if (operacao == 3) {

            } else if (operacao == 4) {

            } else {
                printf("Opção inválida\n");
            }
            break;

        case 2:
            printf("Escolha uma operação:\n");
            printf("1 - Cadastrar médico\n2 - Consultar por ID\n3 - Modificar\n4 - Apagar\n");
            scanf("%d", &operacao);

            if (operacao == 1) {
                cadastrarMedico();
            } else if (operacao == 2) {
                
            } else if (operacao == 3) {
                
            } else if (operacao == 4) {

            } else {
                printf("Opção inválida\n");
            }
            break;

        case 3:
            // Fila (não implementado)
            break;

        default:
            printf("Opção inválida\n");
            abort();  // Termina o programa com erro
            break;
    }

    return 0;
}
