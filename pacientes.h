#ifndef PACIENTES_H
#define PACIENTES_H

typedef struct {
    char id[50];
    char nome[50];
    char cpf[15];
    char idMedico[50];
    int estado;
} Paciente;

void cadastrarPacientes();

#endif
