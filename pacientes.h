#ifndef PACIENTES_H
#define PACIENTES_H

typedef struct {
    int id;
    char nome[50];
    char cpf[15];
    int idMedico;
    char estado[20];
} Paciente;

void cadastrarPacientes();

#endif
