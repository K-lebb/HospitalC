#ifndef MEDICO_H
#define MEDICO_H
#include <stdbool.h>

typedef struct {
    char nome[50];
    char crm[50];
    char id[50];
    bool plantao;
} Medico;

void cadastrarMedico();
void listarMedicos(const char *nomeArquivo);
Medico buscarMedicoPorID(const char *nomeArquivo, const char *idBuscado);
void apagarMedico(const char *nomeArquivo, const int idParaRemover);
void modificarMedico(const char *nomeArquivo, const int idParaAlterar);

#endif