#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// DEFINES:
#DEFINE N_ENTRADAS 31
#DEFINE N_EPOCAS 2

#DEFINE FLAG_ALEATORIO 1

typedef struct neuro
{
    float peso[N_ENTRADAS];
    float entrada;
}Neuronio;



int main()
{
    srand(time(NULL));

    char *arquivos = {"treinamento.txt","teste.txt"};
    //inicia neuronio
    Neuronio n;
    iniciaPesos(&n,FLAG_ALEATORIO);

    //inicia treinamento
    int *entradaTreinamento = leArquivo(arquivos[0]);

    int resultado;
    int flag0 = 0, flag1 = 0;
    int epoch =0;
    while(!flag0 && !flag1)
    {
        //treina 0:
        flag0 = calculaNeuronio(n,entradaTreinamento);

        //treina 1:
        flag1 = calculaNeuronio(n,entradaTreinamento);
        epoch++;
    }


}

/**
    Inicia pesos aleatorios ou zerados
**/
void iniciaPesos(Neuronio *neuronios,int flagAleatorio)
{

    neuronios->entrada = 0;
    int i;

    for (i=0;i<N_ENTRADAS;i++)
    {
        if(flagAleatorio)
        {
            neuronios->peso[i] = ((float)rand()/(float)(RAND_MAX)) * 1;
        }
        else
        {
            neuronios->peso[i] = 0;
        }
    }
}

void imprimePesos(Neuronio *n)
{
    int i;
    for(i=1;i<N_ENTRADAS;i++)
    {
        printf("%f\t");
        if(i % 3 == 0) printf("\n");
    }
}
/**
    Lê entradas no arquivo
**/
int *leArquivo(char *arquivo)
{
    FILE *f;

    f = fopen(arquivo,"r");

    int i;
    int *entradas = (int*) malloc(sizeof(int)*N_ENTRADAS);
    for(i=0;i<N_ENTRADAS;i++)
    {
        fscanf(f,"%d",entradas[i]);
    }

    fclose(f);
    return entradas;
}
/**
    Funções Neuronio
**/

int calculaNeuronio(Neuronio * neuro, int *entrada)
{
    int i;
    float resultado = 0;

    for(i=0;i<N_ENTRADAS;i++)
    {
        resultado += neuro->peso[i] * entrada[i];
    }

    //Função degrau:
    if(resultado >= 1)
    {
        return 1;
    }
    else return 0;
}

void calculaPeso(Neuronio * neuro, int *entrada,float e)
{
    int i;

    neuro->entrada = e;
    for(i=0;i<N_ENTRADAS;i++)
    {
        neuro->peso[i] += neuro->entrada * entrada[i];
    }
}
