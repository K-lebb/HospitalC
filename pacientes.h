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
void consultarPacientePorID();
Paciente buscarPacientePorID(const char *nomeArquivo, const char *idBuscadoPaciente); 
modificarPaciente(const char *nomeArquivo, const int idParaAlterar);
void apagarPaciente(const char *nomeArquivo, const int idParaRemover);

#endif
