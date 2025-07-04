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
void listarMedicos();
Medico buscarMedicoPorID(const char *nomeArquivo, const char *idBuscado);
Medico modificarMedico(const char *nomeArquivo, const char *idBuscado);

#endif