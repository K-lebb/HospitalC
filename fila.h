#ifndef FILA_H
#define FILA_H
#include <stdbool.h>

int gerarProximoID(const char *nomeArquivo);
void listarMedicosDesignados();
void trocarMedico(const int idParaAlterar);
void trocarEstado(const int idParaAlterar);

#endif
