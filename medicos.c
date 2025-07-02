#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "medicos.h"

static Medico medico;

Medico BuscarPorID(const char *nomeArquivo, const char *idBuscado){

    FILE *arquivo = fopen(nomeArquivo, "r");
    char linha[150];
    Medico resultado = {"", "", 0, false};
    
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return resultado;
    }

    while (fgets(linha, sizeof(linha), arquivo)) {
        char *nome, *CRM;
        int *ID;
        bool *plantao;

        linha[strcspn(linha, "\n")] = '\0';
        
    } 
    
}