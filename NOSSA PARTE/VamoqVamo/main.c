#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define true 1
#define false 0

typedef int bool;
typedef int TIPOPESO;

//UTILIZA LISTA LIGADA
//ESTRUTURA ADJACENCIA
typedef struct adjacencia
{
    int vertice; //VERTICE DE DESTINO
    TIPOPESO peso; //PESO ASSOCIADO A ARESTA QUE LEVA AO VERTICE DE DESTINO
    struct adjacencia *prox; //PROXIMO ELEMENTO DA LISTA DE ADJACENCIA
}ADJACENCIA;

//ESTRUTURA VERTICE
typedef struct vertice
{
    //dados armazenados aqui
    //GUARDA ONDE O VERTICE CHEGA
    ADJACENCIA *cab; //CABEÇA DA LISTA DE ADJACENCIA

}VERTICE;

//ESTRUTURA GRAFO
typedef struct grafo
{
    int vertices; //NUMERO DE VERTICES
    int arestas;  //NUMERO DE ARESTAS
    VERTICE *adj; //ARRANJO DE VERTICES
}GRAFO;

//orig = origem
//pres = predecessor
//d = distancia,"peso"
//g = vertices

void inicializaD(GRAFO *g, int *dis, int *pres, int orig)
{
    int v;
    for(v=0;v<g->vertices;v++)
        {
            dis[v] = INT_MAX/2;
            pres[v] = -1;

        }
    dis[orig] = 0;
}

//FUNÇAO QUE CRIA GRAFOS
GRAFO *criaGrafo(int v)
{
    //ALOCA ESPAÇO NA MEMORIA PARA GUARDAR O GRAFO QUE CRIAMOS
    GRAFO *g = (GRAFO *)malloc(sizeof(GRAFO));
    g->vertices = v;
    g->arestas = 0;
    //ALOCA ARRANJO NA MEMORIA PARA OS V VERTICES E COLOCA ENDEREÇO DO ADJACENCIA NO GRAFO
    g->adj = (VERTICE *)malloc(v*sizeof(VERTICE));
    //INICIALIZA LISTA DE ADJACENCIA NULL
    int i;
    for(i=0; i<v; i++)
        g->adj[i].cab = NULL;
    return(g);
}
//NO FINAL DA ARESTA E O PESO DA ARESTA
ADJACENCIA *criaAdj(int v,int peso)
{
    //GUARDA NA MEMORIA OS NOS ADJACENTES DE UM NO
    ADJACENCIA *temp = (ADJACENCIA *)malloc(sizeof(ADJACENCIA));
    temp->vertice = v;
    temp->peso = peso;
    temp->prox = NULL;
    return(temp);
}

//CRIAMOS UMA ARESTA QUANDO ADICIONAMOS UMA ADJACENCIA A LISTA DE UM VERTICE
//ARESTA COMEÇA DE VI E VAI ATE VF, ARETA TEM UM PESO P
bool criaAresta(GRAFO *gr, int vi,int vf, TIPOPESO p)
{
    //SE GRAFO FOR VAZIO,NAO FAZ NADA
    if(!gr)
        return false;

    //NOS INVALIDOS
    if((vf<0) || (vf >= gr->vertices))
        return false;
    if((vf<0) || (vi >= gr->vertices))
        return false;

    //CRIADA A ADJACENCIA, RECEBE VERTICE FINAL E PESO
    ADJACENCIA *novo = criaAdj(vf,p);
    //COLOCAMOS A ADJACENCIA NA LISTA DE ADJACENCIA DO VERTICE INICIAL
    //NESSE CASO COLOCAMOS NO INICIO DA LISTA, A CABEÇA DA LISTA PASSA A SER O NOVO ELEMENTO
    novo->prox = gr->adj[vi].cab;
    gr->adj[vi].cab = novo;
    //AUMENTAMOS O NUMERO DE ARESTAS JA QUE INCLUIMOS UMA ARESTA
    gr->arestas++;
    return(true);
}

/*
//UMA GRAFO NAO DIGIRIDO E UM GRAFO DIRIGIDO COM ARESTAS INDO E VOLTANDO
bool criaArestaDupla(GRAFO *gr, int vf,int vi, TIPOPESO p)
{
    //SE GRAFO FOR VAZIO,NAO FAZ NADA
    if(!gr)
        return false;

    //NOS INVALIDOS
    if((vf<0) || (vf >= gr->vertices))
        return false;
    if((vf<0) || (vi >= gr->vertices))
        return false;

    //CRIADA A ADJACENCIA, RECEBE VERTICE FINAL E PESO
    ADJACENCIA *novo = criaAdj(vi,p);
    //COLOCAMOS A ADJACENCIA NA LISTA DE ADJACENCIA DO VERTICE INICIAL
    //NESSE CASO COLOCAMOS NO INICIO DA LISTA, A CABEÇA DA LISTA PASSA A SER O NOVO ELEMENTO
    novo->prox = gr->adj[vf].cab;
    gr->adj[vf].cab = novo;
    //NÃO AUMENTAMOS A ARESTA POIS AB = BA
    //gr->arestas++;
    return(true);
}

*/

void imprime(GRAFO *gr)
{
    printf("Vertices %d. Arestas: %d.\n", gr->vertices,gr->arestas);
    int i = 0;
    for(i=0;i<gr->vertices;i++)
        {
            printf("v%d: ",i);
            ADJACENCIA *ad = gr->adj[i].cab;
            while(ad)
                {
                    printf("v%d(%d) ",ad->vertice,ad->peso);
                    ad = ad->prox;
                }
            printf("\n");
        }
}
//FUNÇÃO RELAXAMENTO
//VERIFICA SE A MENOR DISTANCIA ATUAL É MAIOR QUE A DISTANCIA ATUAL + O PESO DO NO ATUAL
void relaxa(GRAFO *g,int *dis,int*pres, int u, int v)
{
    //U É O VERTICE ATUAL, V É O VERTICE ADJACENTE SENDO COMPARADO
    //VOU NO GRAFO G, PEGO A CABEÇA(INICIO) DA LISTA DE ADJACENCIA DE U
    ADJACENCIA *ad = g->adj[u].cab;
    //BUSCAR O VERTICE ADJACENTE A V (U) ATÉ PERCORRER TUDO
    while(ad && ad->vertice != v)
        {
        ad = ad->prox;
        }
        if(ad)
            {
                //FAZ O RELAXAMENTO, SE O CAMINHO É MENOR
               if(dis[v] > dis[u] + ad->peso)
                  {
                     dis[v] = dis[u] + ad->peso;
                     pres[v] = u;
                  }
            }
}

//FUNCAO QUE NOS DIZ SE AINDA EXISTE NO ABERTO
bool existeAberto(GRAFO *g, int *aberto)
{
    int i;
    for(i=0;i<g->vertices;i++)
    {
        if(aberto[i]) return(true);
    }
    return(false);
}



//FUNÇÃO QUE INFORMA QUAL A MENOR DISTANCIA DENTRE TODOS OS VERTICES ABERTOS
int menorDist(GRAFO *g,int *aberto,int *dis)
{
    int i;
    for(i=0; i<g->vertices;i++)
        if(aberto[i]) break;
    if (i==g->vertices) return(-1);
    int menor = i;
    for(i=menor+1; i<g->vertices;i++)
        if(aberto[i] && (dis[menor]>dis[i]))
          menor = i;
    return(menor);
}


int *dijkstra(GRAFO *g, int orig)
{
    //ALOCA ESPAÇO PARA O ARRNJO DAS DISTANCIAS
    int *dis = (int *)malloc(g->vertices*sizeof(int));
    int pres[g->vertices];
    //ALOCA ARRANJO PARA OS PREDECESSORES
    bool aberto[g->vertices];
    //INICIALIZA AS DISTANCIAS E OS PREDECESSORES, BASEADO NO NO S COMO ORIGEM
    inicializaD(g,dis,pres,orig);

    //SETAR TODOS OS NOS COMO ABERTOS
    int i;
    for(i=0;i<g->vertices;i++)
        aberto[i] = true;

        //ENQUANTO EXISTIR ABERTOS, OLHAR NO GRAFO QUAL TEM A MENOR DISTANCIA
        //FECHAR ESSE VERTICE
        //PARA CADA VERTICE DESSE [u] QUE ACABAMOS DE FECHAR, PEGA A LISTA LIGADA A ELE(CABEÇA)
        //RELAXA A ARESTA QUE VAI DE U ATE O VIZINHO(v)
        //RETORNAMOS AS DISTANCIAS APOS TODO O GRAFO FOR PERCORRIDO
        while (existeAberto(g,aberto))
            {
                int u = menorDist(g,aberto,dis);
                aberto[u] = false;
                ADJACENCIA *ad = g->adj[u].cab;
                while(ad)
                    {
                        relaxa(g,dis,pres,u,ad->vertice);
                        ad = ad->prox;
                    }
            }
        return(dis);
}

int main()
{

    //List de Adjacência
    //Arranjo de n elementos, n= numero de nos. Aponta para lista ligada dos nós que se conectam ao arranjo
    //ou seja, os nós adjacentes

    //CRIA UM GRAFO COM 6 NOS
     GRAFO *gr = criaGrafo(7);

    //DIGITAMOS AS ARESTAS
    //QUEM ESTA LIGADO A QUEM E QUAL O PESO
    //CRIA UMA ARESTA DO VERTICE 0 AO VERTICE 1, COM PESO 10
    criaAresta(gr, 0, 1, 15);
    //CRIAMOS OUTRA ARESTA NO SENTIDO CONTRARIO
    criaAresta(gr, 1, 0, 15);

    criaAresta(gr, 0, 3, 20);
    criaAresta(gr, 3, 0, 20);

    criaAresta(gr, 1, 2, 25);
    criaAresta(gr, 2, 1, 25);

    criaAresta(gr, 1, 3, 35);
    criaAresta(gr, 3, 1, 35);

    criaAresta(gr, 1, 4, 28);
    criaAresta(gr, 4, 1, 28);

    criaAresta(gr, 2, 4, 20);
    criaAresta(gr, 4, 2, 20);

    criaAresta(gr, 3, 4, 100);
    criaAresta(gr, 4, 3, 100);

    criaAresta(gr, 3, 5, 30);
    criaAresta(gr, 5, 3, 30);

    criaAresta(gr, 4, 5, 70);
    criaAresta(gr, 5, 4, 70);

    criaAresta(gr, 5, 6, 42);
    criaAresta(gr, 6, 5, 42);

    criaAresta(gr, 6, 4, 40);
    criaAresta(gr, 4, 6, 40);

    imprime(gr);


    int inicio, chegada;
    scanf("%i", &inicio);
    printf("\n");
    scanf("%i", &chegada);
    //AQUI DEFINIMOS A ORIGEM DO GRAFO
    int *r = dijkstra(gr,inicio);

    int i;
    for(i=0;i<gr->vertices;i++)
        printf("D(v%d -> v%d) = %d\n",inicio,i,r[i]);

    return 0;
}
