#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// DEFINES:
#define N_ENTRADAS 31
#define N_EPOCAS 2

#define FLAG_ALEATORIO 0

typedef struct neuro
{
    float peso[N_ENTRADAS];
    float entrada;
}Neuronio;

typedef struct entrada
{
    int valores[N_ENTRADAS];
    int numero;// -1 = indefinido
}Entrada;

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
    printf("\nPESOS:\n");
    int i;
    printf("%f\n",n->peso[0]);
    for(i=1;i<N_ENTRADAS;i++)
    {
        printf("%f\t",n->peso[i]);
        if(i % 3 == 0) printf("\n");
    }
}
void imprimeEntradas(Entrada *entradas,int totalEntradas)
{
    int i;
    int j;
    printf("\nENTRADAS:\n");
    for(i=0;i<totalEntradas;i++)
    {
        printf("%d\n",entradas->valores[0]);
        for(j=1;j<N_ENTRADAS;j++)
        {
            printf("%d",entradas[i].valores[j]);
            if(j % 5 == 0) printf("\n");
        }
        printf("\n\n");
    }
}
/**
    Lê entradas no arquivo
**/
Entrada *leArquivo(char *arquivo,int totalEntradasArquivo)
{
    FILE *f;

    f = fopen(arquivo,"r");

    int i;
    int j;
    char corrente;
    Entrada *entradas = (Entrada*) malloc(sizeof(Entrada)*totalEntradasArquivo);
    //Entrada entradas[totalEntradasArquivo];
    //int *entradas = (int*) malloc(sizeof(int)*N_ENTRADAS);
    for(i=0;i<totalEntradasArquivo;i++)
    {
        j = 0;
        while(j<N_ENTRADAS)
        {
            corrente = fgetc(f);
            if(corrente != '0' && corrente != '1')
            {
                continue;
            }
            //fscanf(f,"%c",&entradas[i].valores[j]);
            entradas[i].valores[j] = corrente - '0';
            //printf( " %d",entradas[i].valores[j]);
            j++;
        }
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
    if(resultado > 0)
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
        neuro->peso[i] += neuro->entrada * 1 * entrada[i];
        //printf("\nNovo Peso: %f",neuro->peso[i]);
    }
}



int main()
{
    srand(time(NULL));

    //inicia neuronio
    Neuronio *n = (Neuronio*)malloc(sizeof(Neuronio));
    iniciaPesos(n,FLAG_ALEATORIO);

    Entrada **entradas = (Entrada**) malloc(sizeof(Entrada*)*6);
    entradas[0] = leArquivo("0.txt",10);
    entradas[1] = leArquivo("1.txt",10);
    entradas[2] = leArquivo("2.txt",1);
    entradas[3] = leArquivo("3.txt",1);
    entradas[4] = leArquivo("4.txt",1);
    entradas[5] = leArquivo("5.txt",1);
    //Entrada *entradasZero = leArquivo("0.txt",10);
    //Entrada *entradasUm = leArquivo("1.txt",10);
    //imprimeEntradas(entradas[0],10);

    //inicia treinamento
    int flag0 = 0, flag1 = 0;
    int epoch =0;
    while(!flag0 || !flag1)
    {
        printf("\nEPOCA: %d",epoch);
        //treina 0:
        flag0 = 0;
        flag1 = 0;
        int flag;
        flag = calculaNeuronio(n,entradas[0][0].valores);
        calculaPeso(n,entradas[0][0].valores,(0-flag)); //desejado - obtido
        if(flag == 0) flag0 = 1;

        //treina 1:
        flag = calculaNeuronio(n,entradas[1][0].valores);
        calculaPeso(n,entradas[1][0].valores,(1-flag));
        if(flag == 1) flag1= 1;
        epoch++;
    }
    imprimePesos(n);

    //Testa 0s
    int i;
    for(i=0;i<10;i++)
    {
        printf("TESTE0:%d\n",calculaNeuronio(n,entradas[0][i].valores));
    }
    //Testa 1s
    for(i=0;i<10;i++)
    {
        printf("TESTE1:%d\n",calculaNeuronio(n,entradas[1][i].valores));
    }
    printf("TESTE2:%d\n",calculaNeuronio(n,entradas[2][0].valores));
    printf("TESTE3:%d\n",calculaNeuronio(n,entradas[3][0].valores));
    printf("TESTE4:%d\n",calculaNeuronio(n,entradas[4][0].valores));
    printf("TESTE5:%d\n",calculaNeuronio(n,entradas[5][0].valores));
    //Testa demais


    return 0;
}
