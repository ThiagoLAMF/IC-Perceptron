#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// DEFINES:
#define N_ENTRADAS 31
#define N_EPOCAS 2

#define FLAG_ALEATORIO 1

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

void calculaPeso(Neuronio *neuro, int *entrada,float e)
{
    int i;

    neuro->entrada = e;
    for(i=0;i<N_ENTRADAS;i++)
    {
        neuro->peso[i] += neuro->entrada * 1 * entrada[i];
        //printf("\nNovo Peso1: %f",neuro->peso[i]);
    }
}

int main()
{
    srand(time(NULL));
    int i,j;
    //inicia neuronio
    //Neuronio **n = (Neuronio**)malloc(sizeof(Neuronio*)*2);
    //n[0] = (Neuronio*)malloc(sizeof(Neuronio));
    //n[1] = (Neuronio*)malloc(sizeof(Neuronio));
    Neuronio n[6];
    iniciaPesos(&n[0],FLAG_ALEATORIO);
    iniciaPesos(&n[1],FLAG_ALEATORIO);
    iniciaPesos(&n[2],FLAG_ALEATORIO);
    iniciaPesos(&n[3],FLAG_ALEATORIO);
    iniciaPesos(&n[4],FLAG_ALEATORIO);
    iniciaPesos(&n[5],FLAG_ALEATORIO);

    Entrada **entradas = (Entrada**) malloc(sizeof(Entrada*)*6);
    entradas[0] = leArquivo("0.txt",10);
    entradas[1] = leArquivo("1.txt",10);
    entradas[2] = leArquivo("2.txt",10);
    entradas[3] = leArquivo("3.txt",10);
    entradas[4] = leArquivo("4.txt",10);
    entradas[5] = leArquivo("5.txt",10);

    //imprimePesos(&n[0]);
    //imprimePesos(&n[1]);


    int desejado[6][6] =
    {
        {1,0,0,0,0,0}, //primeiro neuronio, treina para 0
        {0,1,0,0,0,0}, //segundo neuronio, treina para 1
        {0,0,1,0,0,0}, // ...
        {0,0,0,1,0,0},
        {0,0,0,0,1,0},
        {0,0,0,0,0,1}
    };
    //inicia treinamento
    int flag0=0,flag1=0;
    int flag[6] = {0,0,0,0,0,0};
    int epoch =0;
    int acertos = 0;

    while(acertos < 6)
    {
        printf("\nEPOCA: %d",epoch);
        acertos = 0;
        for(i=0;i<6;i++)
        {
            for(j=0;j<6;j++)
            {
                flag[j] = calculaNeuronio(&n[j],entradas[i][0].valores);
                calculaPeso(&n[j],entradas[i][0].valores,(desejado[i][j]-flag[j])); //desejado - obtido
            }
            //printf("\nNovo Peso: %f",n[1].peso[i]);
            //printf("\n %d || %d \n",flag[0],flag[1]);

            if(flag[0] == desejado[i][0] && flag[1] == desejado[i][1] &&
               flag[2] == desejado[i][2] && flag[3] == desejado[i][3] &&
               flag[4] == desejado[i][4] && flag[5] == desejado[i][5]) //verifica se a regra está ok
                acertos++;
        }
        //imprimePesos(&n[0]);
    //imprimePesos(&n[1]);
        //printf("\n\n0:%f\n",n[0].peso[0]);

        //getchar();
        epoch++;
    }

    imprimePesos(&n[0]);
    imprimePesos(&n[1]);
    imprimePesos(&n[2]);
    imprimePesos(&n[3]);
    imprimePesos(&n[4]);
    imprimePesos(&n[5]);

    //Testa 0s
    for(i=0;i<6;i++)
    {
        for(j=0;j<10;j++)
        {
            printf("-----------\n");
            printf("TESTE[%d]:%d",i,calculaNeuronio(&n[0],entradas[i][j].valores));
            printf(" %d",calculaNeuronio(&n[1],entradas[i][j].valores));
            printf(" %d",calculaNeuronio(&n[2],entradas[i][j].valores));
            printf(" %d",calculaNeuronio(&n[3],entradas[i][j].valores));
            printf(" %d",calculaNeuronio(&n[4],entradas[i][j].valores));
            printf(" %d\n",calculaNeuronio(&n[5],entradas[i][j].valores));
        }
    }

    return 0;
}
