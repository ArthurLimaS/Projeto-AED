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
            no* teste = busca(atual->pri, id);
            if(teste != NULL)
            {
                return teste;
            }
        } else if(atual->seg != NULL)
        {
            no* teste = busca(atual->seg, id);
            if(teste != NULL)
            {
                return teste;
            }
        }else if(atual->ter != NULL)
        {
            no* teste = busca(atual->ter, id);
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
        if(atual->pri == NULL)
        {
            atual->pri = novo;
            return atual;
        }else if(atual->seg = NULL)
        {
            atual->seg = novo;
            return atual;
        }else if(atual->ter = NULL)
        {
            atual->ter = novo;
            return atual;
        }

    }else if(id!=0)
    {
        if(atual->pri != NULL)
        {
            no* teste = busca(atual->pri, id);
            if(teste != NULL)
            {
                atual->pri = teste;
                return atual;
            }
        } else if(atual->seg != NULL)
        {
            no* teste = busca(atual->seg, id);
            if(teste != NULL)
            {
                atual->seg = teste;
                return atual;
            }
        }else if(atual->ter != NULL)
        {
            no* teste = busca(atual->ter, id);
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






    return 0;
}
