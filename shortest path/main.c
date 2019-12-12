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
    no* primeiro, *a, *b, *c, *d, *e, *f, *g, *h, *i, *j;
    primeiro->id = 0;
    primeiro->pri = a;
    a->id = 3;
    a->retPri = primeiro;
    primeiro->valPri = 8;
    a->valRetPri = 8;
    b->id = 7;
    primeiro->seg = b;
    b->retPri = primeiro;
    b->valRetPri = 4;
    primeiro->valSeg = 4;
    primeiro->ter = c;
    c->retPri = primeiro;
    c->id = 11;
    primeiro->valTer = 6;
    c->valRetTer = 6;
    a->pri = d;
    d->retPri = a;
    d->id = 19;
    a->valPri = 9;
    d->valRetPri = 9;
    a->seg = e;
    e->retPri = a;
    e->id = 29;
    a->valSeg = 3;
    e->valRetPri = 3;
    b->pri = e;
    e->retSeg = b;
    b->valPri = 4;
    e->valRetSeg = 4;
    b->seg = f;
    f->retPri = b;
    f->id = 37;
    b->valSeg = 5;
    f->valRetPri = 5;
    c->pri = g;
    g->retPri = c;
    g->id = 41;
    c->valPri = 13;
    g->valRetPri = 13;
    e->pri = h;
    h->retPri = e;
    h->id = 43;
    e->valPri = 7;
    h->valRetPri = 7;
    h->pri = i;
    i->retPri = h;
    i->id = 47;
    i->valRetPri = 6;
    h->valPri = 6;
    h->seg = j;
    j->retPri = h;
    j->id = 53;
    j->valRetPri = 6;
    h->valSeg = 6;





    return 0;
}
