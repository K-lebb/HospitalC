#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "fila.h"

int gerarProximoID(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        return 1;
    }

    char linha[150];
    char idStr[20];
    int maiorID = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (sscanf(linha, "%19[^;];", idStr) == 1) {
            int idAtual = atoi(idStr); 
            if (idAtual > maiorID) {
                maiorID = idAtual;
            }
        }
    }

    fclose(arquivo);
    return maiorID + 1;
}
