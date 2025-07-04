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
Medico BuscarMedicoPorID(const char *nomeArquivo, const char *idBuscado);

#endif