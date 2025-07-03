#ifndef MEDICO_H
#define MEDICO_H
#include <stdbool.h>

typedef struct {
    char nome[50];
    char crm[50];
    int id;
    bool plantao;
} Medico;

void cadastrarMedico();

#endif