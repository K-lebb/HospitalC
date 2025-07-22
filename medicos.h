#ifndef MEDICO_H
#define MEDICO_H
#include <stdbool.h>

typedef struct {
    char nome[50];
    char crm[50];
    char id[50];
    bool plantao;
    int totalPacientes;
} Medico;

void cadastrarMedico();
void listarMedicos(FILE *saida, int *total);
Medico buscarMedicoPorID(const char *nomeArquivo, const char *idBuscado);
void apagarMedico(const char *nomeArquivo, const int idParaRemover);
void modificarMedico(const char *nomeArquivo, const int idParaAlterar);
void contarPacientesPorMedico(Medico *medicos, int totalMedicos);
int carregarMedicos(const char *nomeArquivo, Medico *medicos, int maxMedicos);

#endif