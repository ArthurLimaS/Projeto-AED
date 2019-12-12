#include <stdio.h>>
#include <stdlib.h>


/*
3 vetores de saida e 3 de entrada, pra garantir que o grafo nao seja orientado
id descreve o caminho que foi feito pra chegar em algum no
*/
typedef struct No{
    int id;
    int val;
    int valPri;
    int valSeg;
    int valTer;
    struct No* pri;
    struct No* seg;
    struct No* ter;
    int valRetPri;
    int valRetSeg;
    int valRetTer;
    struct No* retPri;
    struct No* retSeg;
    struct No* retTer;
}no;

no* buscar(no* atual, int id)
{
    if(id == atual->id)
    {
        return atual;
    }else if(id!=0)
    {
        if(atual->pri != NULL)
        {
            no* teste = buscar(atual->pri, id);
            if(teste != NULL)
            {
                return teste;
            }
        } else if(atual->seg != NULL)
        {
            no* teste = buscar(atual->seg, id);
            if(teste != NULL)
            {
                return teste;
            }
        }else if(atual->ter != NULL)
        {
            no* teste = buscar(atual->ter, id);
            if(teste != NULL)
            {
                return teste;
            }
        }
        return NULL;
    }
    return atual;
}



no* inserir(no* atual, int custo, int idin, int id)
{
    if(idin == atual->id)
    {
        no* novo = (no*)malloc(sizeof(no));
        novo->pri = NULL;
        novo->seg = NULL;
        novo->ter = NULL;
        novo->retPri = atual;
        novo->id = id;
        novo->valPri = custo;
        if(atual->pri == NULL)
        {
            atual->pri = novo;
            atual->valPri = custo;
            return atual;
        }else if(atual->seg == NULL)
        {
            atual->valSeg = custo;
            atual->seg = novo;
            return atual;
        }else if(atual->ter == NULL)
        {
            atual->valTer = custo;
            atual->ter = novo;
            return atual;
        }

    }else if(id!=0)
    {
        if(atual->pri != NULL)
        {
            no* teste = buscar(atual->pri, id);
            if(teste != NULL)
            {
                atual->pri = teste;
                return atual;
            }
        } else if(atual->seg != NULL)
        {
            no* teste = buscar(atual->seg, id);
            if(teste != NULL)
            {
                atual->seg = teste;
                return atual;
            }
        }else if(atual->ter != NULL)
        {
            no* teste = buscar(atual->ter, id);
            if(teste != NULL)
            {
                atual->ter = teste;
                return atual;
            }
        }
        return NULL;
    }


}



int main(void)
{
    no* primeiro, *a, *b, *c, *d, *e, *f, *g, *h, *i, *j, *k, *l, *m, *n, *o;

    //Primeiro aponta para A , A aponta para Primeiro
    primeiro->id = 0;
    a->id = 3;
    primeiro->pri = a;
    a->retPri = primeiro;
    primeiro->valPri = 8;
    a ->valRetPri = 8;

    //Primeiro aponta para B, B aponta para Primeiro
    b->id = 7;
    primeiro->seg = b;
    b->retSeg = primeiro;
    b->valRetPri = 4;
    primeiro->valSeg = 4;

    //Primeiro aponta para C, C aponta para Primeiro
    primeiro->ter = c;
    c->retTer = primeiro;
    c->id = 11;
    primeiro->valTer=6;
    c->valRetPri = 6;

    //B aponta para D , D aponta para B

    b->pri = d;
    d->retPri = b;
    d->id = 19;
    b->valPri = 9;
    d->valRetPri = 9;

    // D aponta para E, E aponta para D

    d->pri = e;
    e->retPri = d;
    e->id = 29;
    d->valPri = 4;
    e->valRetPri = 4;

   //C aponta para F, F aponta para C

   c->pri = f;
   f->retPri = c;
   f->id = 37;
   c->valPri = 4;
   f->valRetPri = 4;

   //F aponta para G, G aponta para f

   f->pri = g;
   g->retPri = f;
   g->id = 41;
   f->valPri = 6;
   g->valRetPri = 6;

   //G aponta para H, H aponta para G

   g->pri = h;
   h->retPri = g;
   h->id = 43;
   g->valPri = 9;
   h->valRetPri = 9;

   //H aponta para I, I aponta para H
   h->pri = i;
   i->retPri = h;
   i->id = 47;
   h->valPri = 10;
   i->valRetPri = 10;

   //I APONTA PARA J, J APONTA PARA I

   i->pri = j;
   j->retPri = i;
   j-> id = 53;
   i->valPri = 5;
   j->valRetPri=5;

  //J aponta para K, K aponta para J

   j->pri = k;
   k->retPri = j;
   k->id = 59;
   j->valPri = 8;
   k->valRetPri = 8;

  //J aponta L, L aponta para J

   j->seg = l;
   l->retPri = j;
   l->id = 61;
   j->valSeg = 10;
   l->valRetSeg = 10;

  //L aponta para M, M aponta para L

    l->pri = m;
    m->retPri = l;
    m->id = 63;
    l->valPri = 7;
    m->valRetPri = 7;


   //M aponta para N, N aponta para M

    m->pri = n;
    n->retPri = m;
    n->id = 67;
    m->valPri = 10;
    n->valRetPri = 10;


   //M aponta para O, O aponta para M

    m->seg = o;
    o->retPri=m;
    o->id = 71;
    m->valSeg = 4;
    o->valRetSeg = 4;

   // N aponta para O, O aponta para N

    n->pri = o;
    o->retPri = n;
    n->valPri = 6;
    o->valRetPri = 6;


    // D aponta para G, G aponta para D

    d-> seg = g;
    g->retSeg = d;
    d->valSeg = 2;
    g->valRetSeg = 2;


    // F aponta para I, I aponta para F

    f-> seg = i;
    i-> retSeg = f;
    f->valSeg = 4;
    i->valRetSeg = 4;



    return 0;
}
