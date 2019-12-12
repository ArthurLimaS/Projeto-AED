#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <windows.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <math.h>

const float FPS = 60;

typedef struct{
    int valor;
    struct Lista* conexao;
    struct No* matchingPrincipal;
    int tipo;
}No;

typedef struct{
    struct Lista* proximo;
    struct No* ponto;
}Lista;

typedef struct{
    int width,height;
    ALLEGRO_BITMAP *img;
}SIMP_OBJECT;

SIMP_OBJECT build_Simple_Object(char path[]){
    SIMP_OBJECT object;
    object.img = al_load_bitmap(path);
    object.width = al_get_bitmap_width(object.img);
    object.height = al_get_bitmap_height(object.img);
    return object;
}

//LISTA , INSERÇÃO , REMOÇÃO , BUSCA;
Lista* inserir(Lista* atual , No* ponto){
    if(atual == NULL){
        Lista* novo = malloc(sizeof(Lista));
        novo->proximo = NULL;
        novo->ponto = ponto;
        return novo;
    }else{
        atual->proximo = inserir(atual->proximo,ponto);
        return atual;
    }
}

Lista* remover(Lista* atual, No* ponto){
    if(atual == NULL){
        return NULL;
    }else {
        if(ponto == atual->ponto){
            Lista* aux = atual->proximo;
            //free(atual);
            return aux;
        }else{
            atual->proximo = remover(atual->proximo, ponto);
            return atual;
        }
    }
}

Lista* buscar(Lista* atual,No* ponto){
    if(atual == NULL){
        return NULL;
    }else{
        if(atual->ponto == ponto){
            return atual;
        }else{
            buscar(atual->proximo,ponto);
        }
    }
}

No* buscarPorValor(Lista* atual,int valor){
    if(atual == NULL){
        return NULL;
    }else{
        No* aux = atual->ponto;
        if(aux->valor == valor){
        return atual;
        }else{
            buscarPorValor(atual->proximo,valor);
        }
    }
}

No* buscarUltimoElemento(Lista* atual,No* ultimo){
    if(atual->proximo == NULL){
        No* aux = atual->ponto;
        if(aux->tipo == 0){
        ultimo = aux;
        }
        return ultimo;
    }else{
        No* aux = atual->ponto;
        if(aux->tipo == 0){
            No* fresco = aux;
            buscarUltimoElemento(atual->proximo,fresco);
        }else{
        buscarUltimoElemento(atual->proximo,ultimo);
        }
    }
}

No* inserirNo(int valor , Lista* lista, int tipo){
    No* novo = (No*)malloc(sizeof(No));
    novo->valor = valor;
    novo->conexao = lista;
    novo->matchingPrincipal = NULL;
    novo->tipo = tipo;

    return novo;
}

void conectarNos(No* no1 , No* no2){
    no1->conexao = inserir(no1->conexao,no2);
    no2->conexao = inserir(no2->conexao,no1);
}

void desconectarNos(No* no1,No* no2){
    no1->conexao = remover(no1->conexao,no2);
    no2->conexao = remover(no2->conexao,no1);
}

No* removerNo(No* no){
    if(no->conexao != NULL){
        Lista* aux = no->conexao;
        aux->proximo = remover(aux->proximo,aux->ponto);
        desconectarNos(no,aux->ponto);
        removerNo(no);
        return aux;
        }else{
        return NULL;
        }
    }


//MATCHING

int contador(Lista* lista){
    int total;
    if(lista != NULL){
        total = 1 + contador(lista->proximo);
    }else{
        return 0;
    }

    return total;
}

Lista* reorganizarMatching(Lista* grafoMatching,Lista* grafoMatchingImutavel){
    //CRIANDO AUXÍLIARES
    Lista* auxLista = NULL;
    No* auxNo = NULL;

    if(grafoMatching->proximo != NULL){
        auxLista = grafoMatching->proximo;
        auxNo = auxLista->ponto;
    }

    No* auxNoPricipal = grafoMatching->ponto;

    if(auxNoPricipal->tipo == 0 && auxLista == NULL){
        Lista* conectores = auxNoPricipal->conexao;
        //INSERINDO E REMOVENDO O PRIMEIRO DA LISTA DE CONEXOES DO NO FALHO
        grafoMatching = inserir(grafoMatching, conectores->ponto);

        return grafoMatching;
    }
    // VERIFICANDO SE O PRIXIMO É DESTINATÁRIO
    else if(auxNoPricipal->tipo == 0 && auxNo->tipo == 0){
        Lista* conectores = auxNoPricipal->conexao;
        //INSERINDO E REMOVENDO O PRIMEIRO DA LISTA DE CONEXOES DO NO FALHO
        conectores = inserir(conectores,conectores->ponto);
        conectores = remover(conectores,conectores->ponto);


    while(0==0){
        if(conectores == NULL){
        conectores = auxNoPricipal->conexao;
        //INSERINDO E REMOVENDO PONTO FALHO
        grafoMatching = inserir(grafoMatching,auxNoPricipal);
        grafoMatching = remover(grafoMatching,auxNoPricipal);

        grafoMatching = inserir(grafoMatching,conectores->ponto);
        grafoMatching = remover(grafoMatching,conectores->ponto);

        break;
        }else if(buscar(grafoMatching,conectores->ponto) != NULL){
            conectores = conectores->proximo;
        }else{
            //INSERINDO E REMOVENDO PONTO FALHO
            grafoMatching = inserir(grafoMatching,auxNoPricipal);
            grafoMatching = remover(grafoMatching,auxNoPricipal);
            grafoMatching = inserir(grafoMatching,conectores->ponto);
            break;
        }

    }

        imprimirLista(grafoMatching);
        printf("\n\n");
        return grafoMatching;
        }else{
            imprimirLista(grafoMatching);
            printf("\n\n");
            grafoMatching->proximo = reorganizarMatching(grafoMatching->proximo,grafoMatchingImutavel);
            if(auxNoPricipal->tipo == 1 && buscarPorValor(grafoMatching->proximo,auxNoPricipal->valor)!= NULL){
                grafoMatching = remover(grafoMatching,auxNoPricipal);
            }

            imprimirLista(grafoMatching);
            printf("\n\n");
            return grafoMatching;
        }

}

Lista* matching(Lista* grafoPrincipal,Lista* grafoMatching){
    if(grafoMatching == NULL){
        grafoMatching = inserir(grafoMatching,grafoPrincipal->ponto);
        No* aux = grafoMatching->ponto;
        Lista* auxL = aux->conexao;
        grafoMatching = inserir(grafoMatching,auxL->ponto);
    }else if(buscar(grafoMatching,grafoPrincipal->ponto)!= NULL){
        grafoMatching = matching(grafoPrincipal->proximo,grafoMatching);
    }else{
        grafoMatching = inserir(grafoMatching,grafoPrincipal->ponto);
        No* aux = grafoPrincipal->ponto;
        Lista* auxL = aux->conexao;
        while(0==0){
           if(buscar(grafoMatching,auxL->ponto) == NULL){
                grafoMatching = inserir(grafoMatching,auxL->ponto);
                break;
           }else if(auxL->proximo == NULL){
                break;
           }else{
                auxL = auxL->proximo;
           }
        }
    }

    if(buscar(grafoMatching,buscarUltimoElemento(grafoPrincipal,NULL)) != NULL){
        return grafoMatching;
    }else{
        grafoMatching = matching(grafoPrincipal,grafoMatching);
    }
}


void imprimirLista(Lista* atual){
    if(atual != NULL){
        No* aux = atual->ponto;
        printf("%i=>",aux->valor);
        imprimirLista(atual->proximo);
    }else{
    printf("NULL");
    }
}


void imprimirConecNo(No* no){
    imprimirLista(no->conexao);
}


int main()
{
    //VARIAVEIS ALLEGRO
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_DISPLAY_MODE disp_data;

    //addons
    al_init();
    al_init_image_addon();
    al_install_mouse();

    al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    display = al_create_display(disp_data.width, disp_data.height);
    int SCREEN_W = al_get_display_width(display);
    int SCREEN_H = al_get_display_height(display);
    int pos_x = 0, pos_y = 0;
    int fechar = 0;
    int but_matching = 0;

    int but_1 = 0,but_2 = 0,but_3 = 0,but_4 = 0,but_5 = 0,but_6 = 0,but_7 = 0,but_8 = 0,but_9 = 0,but_10 = 0;

    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue,al_get_mouse_event_source());
    al_flip_display();

    ALLEGRO_BITMAP *image = al_load_bitmap("images/menu/download.jpg");
    ALLEGRO_BITMAP *start = al_load_bitmap("images/menu/start.png");
    ALLEGRO_BITMAP *shortest = al_load_bitmap("images/menu/shortest.png");
    ALLEGRO_BITMAP *fundo_matching = al_load_bitmap("images/matchingPage/fundo.jpg");
     ALLEGRO_BITMAP *butimage_vermelho= al_load_bitmap("images/matchingPage/butVermelho.png");
      ALLEGRO_BITMAP *butimage_verde = al_load_bitmap("images/matchingPage/butVerde.png");

    while(1){
        while(fechar == 0){
            ALLEGRO_EVENT ev;
            al_wait_for_event(event_queue,&ev);

            if(ev.type == ALLEGRO_EVENT_MOUSE_AXES){
                pos_x = ev.mouse.x;
                pos_y = ev.mouse.y;
            }else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                fechar = 1;
                break;
            }else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                if(pos_x >= 610 && pos_x <740 && pos_y > 260 && pos_y < 380){
                    fechar = 1;
                    but_matching = 1;
                    break;
                }
            }

            if(al_is_event_queue_empty(event_queue)){
                al_draw_scaled_bitmap(image,0,0,al_get_bitmap_width(image),al_get_bitmap_height(image),0,0,SCREEN_W,SCREEN_H,0);
                al_draw_scaled_bitmap(start,0,0,al_get_bitmap_width(start),al_get_bitmap_height(start),630,280,100,100,0);
                al_draw_scaled_bitmap(shortest,0,0,al_get_bitmap_width(shortest),al_get_bitmap_height(shortest),630,450,100,100,0);

                //desenha com as cordenada
                if(pos_x >= 610 && pos_x <740 && pos_y > 260 && pos_y < 380){
                    al_draw_scaled_bitmap(start,0,0,al_get_bitmap_width(start),al_get_bitmap_height(start),610,260,130,130,0);

                }else if(pos_x >= 610 && pos_x <740 && pos_y > 420 && pos_y < 540){
                    al_draw_scaled_bitmap(shortest,0,0,al_get_bitmap_width(shortest),al_get_bitmap_height(shortest),610,430,130,130,0);
                }
            }

            al_flip_display();
        }

        while(but_matching){
            ALLEGRO_EVENT ev2;
            al_wait_for_event(event_queue,&ev2);

            if(ev2.type == ALLEGRO_EVENT_MOUSE_AXES){
                pos_x = ev2.mouse.x;
                pos_y = ev2.mouse.y;
            }else if(ev2.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                but_matching = 0;
                break;
            }else if(ev2.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                if(pos_x >= 230 && pos_x <270 && pos_y > 180 && pos_y < 210){
                    if(but_1 == 0){
                    but_1 = 1;
                    }else{
                    but_1 = 0;
                    }
                }
            }

            if(al_is_event_queue_empty(event_queue)){
                al_draw_scaled_bitmap(fundo_matching,0,0,al_get_bitmap_width(fundo_matching),al_get_bitmap_height(fundo_matching),0,0,SCREEN_W,SCREEN_H,0);
                if(but_1 == 0){
                al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,180,40,40,0);
                }else{
                al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),230,180,40,40,0);
                }
                al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,230,40,40,0);
                al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,280,40,40,0);
                al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,330,40,40,0);
                al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,380,40,40,0);
                al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,430,40,40,0);
                al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,480,40,40,0);
                al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,530,40,40,0);
                al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,580,40,40,0);
                al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,630,40,40,0);

                //desenha com as cordenada
                if(pos_x >= 230 && pos_x <270 && pos_y > 180 && pos_y < 210){
                    //al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,180,60,60,0);

                }else if(pos_x >= 610 && pos_x <740 && pos_y > 420 && pos_y < 540){
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),610,430,130,130,0);
                }
            }

            al_flip_display();
        }


    }

    //Screen Rest Time


    /*Lista* lista1 = NULL;
    No* no1 = inserirNo(1,lista1,0);

    Lista* lista2 = NULL;
    No* no2 = inserirNo(2,lista2,0);

    Lista* lista3 = NULL;
    No* no3 = inserirNo(3,lista3,0);

    Lista* lista4 = NULL;
    No* re4 = inserirNo(4,lista4,1);

    Lista* lista5 = NULL;
    No* re5 = inserirNo(5,lista5,1);

    Lista* lista6 = NULL;
    No* re6 = inserirNo(6,lista6,1);

    Lista* lista7 = NULL;
    No* re7 = inserirNo(7,lista7,1);

    Lista* lista8 = NULL;
    No* re8 = inserirNo(8,lista8,1);

    Lista* lista9 = NULL;
    No* no9 = inserirNo(9,lista9,0);

    Lista* lista10 = NULL;
    No* no10 = inserirNo(10,lista10,0);

    conectarNos(no1,re4);
    conectarNos(no1,re5);
    conectarNos(no1,re6);
    conectarNos(no1,re7);
    conectarNos(no1,re8);
    conectarNos(no2,re4);
    conectarNos(no3,re5);
    conectarNos(no3,re6);
    conectarNos(no9,re5);
    conectarNos(no10,re6);
    conectarNos(no10,re7);

    imprimirConecNo(no1);
    printf("\n\n");
    imprimirConecNo(no2);

    printf("\n\n");

    imprimirConecNo(no3);

    printf("\n\n");
    imprimirConecNo(re6);

    Lista* grafoPrincipal = NULL;
    Lista* grafoMatching = NULL;
    grafoPrincipal = inserir(grafoPrincipal,no1);
    grafoPrincipal = inserir(grafoPrincipal,no2);
    grafoPrincipal = inserir(grafoPrincipal,no3);
    grafoPrincipal = inserir(grafoPrincipal,no9);
    grafoPrincipal = inserir(grafoPrincipal,no10);
    grafoPrincipal = inserir(grafoPrincipal,re4);
    grafoPrincipal = inserir(grafoPrincipal,re5);
    grafoPrincipal = inserir(grafoPrincipal,re6);
    grafoPrincipal = inserir(grafoPrincipal,re7);
    grafoPrincipal = inserir(grafoPrincipal,re8);

    printf("ULTIMO %i",buscarUltimoElemento(grafoPrincipal,NULL)->valor);

    printf("\n\n");
    grafoMatching = matching(grafoPrincipal,grafoMatching);
    imprimirLista(grafoMatching);

    while(contador(grafoMatching) != contador(grafoPrincipal)){
    grafoMatching = reorganizarMatching(grafoMatching,grafoMatching);
    }

    imprimirLista(grafoMatching);*/

}
