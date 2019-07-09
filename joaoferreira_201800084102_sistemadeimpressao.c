#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct elemPilha *Pilha;

struct elemPilha
{
    char x[51];
    struct elemPilha *proxPilha;
};

typedef struct elemPilha ElemPilha;

typedef struct
{
    char nome[51];
    char x[51];
    int32_t impressos;
} Arquivo;

typedef struct
{
    char nome[51];
    Pilha *pilha;
    Arquivo arquivo;
    int32_t qntd;
} Impressora;

Pilha *cria_Pilha()
{
    Pilha *pi = (Pilha *)malloc(sizeof(Pilha));
    if (pi != NULL)
        *pi = NULL;
    return pi;
}

int32_t insere_Pilha(Pilha *pi, char x[51])
{
    if (pi == NULL)
        return 0;
    ElemPilha *no = (ElemPilha *)malloc(sizeof(ElemPilha));
    if (no == NULL)
        return 0;
    strcpy(no->x, x);
    no->proxPilha = (*pi);
    *pi = no;
    return 1;
}

void imprime(Pilha *pi, int32_t x, FILE **output)
{
    ElemPilha *no = *pi;
    fprintf(*output, "%s-", no->x);
    no = no->proxPilha;
    fprintf(*output, "%sp", no->x);
    no = no->proxPilha;
    while (no != NULL)
    {
        fprintf(*output, ", %s-", no->x);
        no = no->proxPilha;
        fprintf(*output, "%sp", no->x);
        no = no->proxPilha;
    }
    fprintf(*output, "\n");
}

int32_t consulta(Pilha *pi, char nome[51])
{
    if (pi == NULL)
        return 0;
    if ((*pi) == NULL)
        return 0;
    ElemPilha *no = *pi;
    if (strcmp(no->x, nome) == 0)
    {
        return 1;
    }
    else
    {
        while (no != NULL)
        {
            if (strcmp(no->x, nome) == 0)
            {
                return 1;
            }
            else
            {
                no = no->proxPilha;
            }
        }
    }
    return 0;
}

int32_t main(int32_t argc, char *argv[])
{
    Pilha *pi = cria_Pilha();

    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    char *dados = malloc(51 * (sizeof(char))), *dds = malloc(51 * (sizeof(char)));
    char *d = malloc(51 * (sizeof(char)));

    int32_t qtd = 0, pgns = 0;

    fscanf(input, "%s", dados);

    Impressora impressoras[atoi(dados)];

    for (int32_t i = 0; i < atoi(dados); i++)
    {
        fscanf(input, "%s", dds);
        strcpy(impressoras[i].nome, dds);
        impressoras[i].arquivo.impressos = 0;
        impressoras[i].pilha = cria_Pilha();
        impressoras[i].qntd = 0;
        qtd++;
    }

    fscanf(input, "%s", d);

    int32_t tmnhpilha = 0, lidas = 0;

    while (tmnhpilha != atoi(d))
    {
        for (int32_t x = 0; x < atoi(dados); x++)
        {
            if (lidas != atoi(d) && impressoras[x].arquivo.impressos == 0)
            {
                fscanf(input, "%s", impressoras[x].arquivo.nome);
                fscanf(input, "%s", impressoras[x].arquivo.x);
                insere_Pilha(impressoras[x].pilha, impressoras[x].arquivo.x);
                insere_Pilha(impressoras[x].pilha, impressoras[x].arquivo.nome);
                impressoras[x].arquivo.impressos = atoi(impressoras[x].arquivo.x);
                impressoras[x].qntd++;
                impressoras[x].qntd++;
                fprintf(output, "[%s] ", impressoras[x].nome);
                imprime(impressoras[x].pilha, impressoras[x].qntd, &output);
                lidas++;
                pgns += atoi(impressoras[x].arquivo.x);
            }
            if (impressoras[x].arquivo.impressos > 0)
            {
                impressoras[x].arquivo.impressos--;
            }
            if (impressoras[x].arquivo.impressos == 0 && consulta(pi, impressoras[x].arquivo.nome) == 0)
            {
                insere_Pilha(pi, impressoras[x].arquivo.x);
                insere_Pilha(pi, impressoras[x].arquivo.nome);
                tmnhpilha++;
            }
        }
    }

    fprintf(output, "%ip\n", pgns);

    ElemPilha *no = *pi;
    while (no != NULL)
    {
        fprintf(output, "%s-", no->x);
        no = no->proxPilha;
        fprintf(output, "%sp\n", no->x);
        no = no->proxPilha;
    }

    fclose(input);
    fclose(output);

    return 0;
}