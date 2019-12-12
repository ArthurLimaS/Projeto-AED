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

void verdarBotoes(int* no, int* receptor)
{
    *no = 1;
    *receptor = 1;
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
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(20);

    //SOUND
    ALLEGRO_SAMPLE *button;
    ALLEGRO_SAMPLE_INSTANCE *inst_button;
    button = al_load_sample("sounds/button.ogg");
    inst_button = al_create_sample_instance(button);
    al_attach_sample_instance_to_mixer(inst_button,al_get_default_mixer());
    al_set_sample_instance_gain(inst_button,1.0);



    al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    display = al_create_display(disp_data.width, disp_data.height);
    int SCREEN_W = al_get_display_width(display);
    int SCREEN_H = al_get_display_height(display);
    int pos_x = 0, pos_y = 0;
    int fechar = 0;
    int but_matching = 0;
    int but_shortest = 0;
    int confirmar = 0 , voltar =0, desligar = 0;
    int but_1 = 0,but_2 = 0,but_3 = 0,but_4 = 0,but_5 = 0,but_6 = 0,but_7 = 0,but_8 = 0,but_9 = 0,but_10 = 0;
    int but_11 = 0,but_12 = 0,but_13 = 0,but_14 = 0,but_15 = 0,but_16 = 0,but_17 = 0,but_18 = 0,but_19 = 0,but_20 = 0;
    int selecionadoEsq = 0 , selecionadoDir = 0;

    /*
    [i][j]:
        i representa os nós 1-10
        j representa os receptores 11-20

        ex:
            [0][0] = 1 -> 11
            [9][9] = 10 -> 20
            etc...
    */
    int linhas[10][10];
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            linhas[i][j] = 0;
        }
    }


    int conec_1_11 = 0;
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue,al_get_mouse_event_source());
    al_flip_display();

    ALLEGRO_BITMAP *image = al_load_bitmap("images/menu/download.jpg");
    ALLEGRO_BITMAP *start = al_load_bitmap("images/menu/start.png");
    ALLEGRO_BITMAP *sair = al_load_bitmap("images/menu/sair.png");
    ALLEGRO_BITMAP *shortest = al_load_bitmap("images/menu/shortest.png");
    ALLEGRO_BITMAP *fundo_matching = al_load_bitmap("images/matchingPage/fundo.jpg");
    ALLEGRO_BITMAP *butimage_vermelho= al_load_bitmap("images/matchingPage/butVermelho.png");
    ALLEGRO_BITMAP *butimage_azul= al_load_bitmap("images/matchingPage/but_azul.png");
    ALLEGRO_BITMAP *linha_1= al_load_bitmap("images/matchingPage/linha_1.png");
    ALLEGRO_BITMAP *linha_1_2= al_load_bitmap("images/matchingPage/linha_1.2.png");
    ALLEGRO_BITMAP *linha_1_3= al_load_bitmap("images/matchingPage/linha1.3.png");
    ALLEGRO_BITMAP *linha_1_4= al_load_bitmap("images/matchingPage/linha_1.4.png");
    ALLEGRO_BITMAP *butimage_verde = al_load_bitmap("images/matchingPage/butVerde.png");
    ALLEGRO_BITMAP *butimage_confirmar= al_load_bitmap("images/matchingPage/but_confirmar.png");
    ALLEGRO_BITMAP *butimage_voltar = al_load_bitmap("images/matchingPage/but_voltar.png");


    while(1)
    {
        while(fechar == 0)
        {
            ALLEGRO_EVENT ev;
            al_wait_for_event(event_queue,&ev);

            if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                pos_x = ev.mouse.x;
                pos_y = ev.mouse.y;
            }
            else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                fechar = 1;
                break;
            }
            else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if(pos_x >= 610 && pos_x <740 && pos_y > 260 && pos_y < 380)
                {
                    fechar = 1;
                    but_matching = 1;
                    al_play_sample_instance(inst_button);
                    break;
                }
                else if(pos_x >= 630 && pos_x <760 && pos_y > 620 && pos_y < 740)
                {
                    al_play_sample_instance(inst_button);
                    fechar = 1;
                    desligar = 1;
                    al_rest(2);
                    break;
                }
                else if(pos_x >= 610 && pos_x <740 && pos_y > 420 && pos_y < 540)
                {
                    fechar = 1;
                    but_shortest = 1;
                    al_play_sample_instance(inst_button);
                    break;
                }
            }

            if(al_is_event_queue_empty(event_queue))
            {
                al_draw_scaled_bitmap(image,0,0,al_get_bitmap_width(image),al_get_bitmap_height(image),0,0,SCREEN_W,SCREEN_H,0);
                al_draw_scaled_bitmap(start,0,0,al_get_bitmap_width(start),al_get_bitmap_height(start),630,280,100,100,0);
                al_draw_scaled_bitmap(shortest,0,0,al_get_bitmap_width(shortest),al_get_bitmap_height(shortest),630,450,100,100,0);
                al_draw_scaled_bitmap(sair,0,0,al_get_bitmap_width(sair),al_get_bitmap_height(sair),630,620,100,100,0);

                //desenha com as cordenada
                if(pos_x >= 610 && pos_x <740 && pos_y > 260 && pos_y < 380)
                {
                    al_draw_scaled_bitmap(start,0,0,al_get_bitmap_width(start),al_get_bitmap_height(start),610,260,130,130,0);
                }
                else if(pos_x >= 610 && pos_x <740 && pos_y > 420 && pos_y < 540)
                {
                    al_draw_scaled_bitmap(shortest,0,0,al_get_bitmap_width(shortest),al_get_bitmap_height(shortest),610,430,130,130,0);
                }
                else if(pos_x >= 630 && pos_x <760 && pos_y > 620 && pos_y < 740)
                {
                    al_draw_scaled_bitmap(sair,0,0,al_get_bitmap_width(sair),al_get_bitmap_height(sair),610,600,130,130,0);
                }
            }

            al_flip_display();
        }

        while(but_matching)
        {
            ALLEGRO_EVENT ev2;
            al_wait_for_event(event_queue,&ev2);

            if(ev2.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                pos_x = ev2.mouse.x;
                pos_y = ev2.mouse.y;
            }
            else if(ev2.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                but_matching = 0;
                break;
            }
            else if(ev2.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if(confirmar == 0)
                {
                    if((pos_x >= 230 && pos_x <270 && pos_y > 180 && pos_y < 210)||(pos_x >= 980 && pos_x <1020 && pos_y > 180 && pos_y < 210))
                    {
                        al_stop_sample_instance(inst_button);
                        al_play_sample_instance(inst_button);
                        if(but_1 == 0)
                        {
                            but_1 = 1;
                            but_11 = 1;
                        }
                        else
                        {
                            but_1 = 0;
                            but_11 = 0;
                        }
                    }
                    else if((pos_x >= 230 && pos_x <270 && pos_y > 230 && pos_y < 260)||(pos_x >= 980 && pos_x <1020 && pos_y > 230 && pos_y < 260))
                    {
                        al_stop_sample_instance(inst_button);
                        al_play_sample_instance(inst_button);
                        if(but_2 == 0)
                        {
                            but_2 = 1;
                            but_12 = 1;
                        }
                        else
                        {
                            but_2 = 0;
                            but_12 = 0;
                        }
                    }
                    else if((pos_x >= 230 && pos_x <270 && pos_y > 280 && pos_y < 310)||(pos_x >= 980 && pos_x <1020 && pos_y > 280 && pos_y < 310))
                    {
                        al_stop_sample_instance(inst_button);
                        al_play_sample_instance(inst_button);
                        if(but_3 == 0)
                        {
                            but_3 = 1;
                            but_13 = 1;
                        }
                        else
                        {
                            but_3 = 0;
                            but_13 = 0;
                        }
                    }
                    else if((pos_x >= 230 && pos_x <270 && pos_y > 330 && pos_y < 360)||(pos_x >= 980 && pos_x <1020 && pos_y > 330 && pos_y < 360))
                    {
                        al_stop_sample_instance(inst_button);
                        al_play_sample_instance(inst_button);
                        if(but_4 == 0)
                        {
                            but_4 = 1;
                            but_14 = 1;
                        }
                        else
                        {
                            but_4 = 0;
                            but_14 = 0;
                        }
                    }
                    else if((pos_x >= 230 && pos_x <270 && pos_y > 380 && pos_y < 410)||(pos_x >= 980 && pos_x <1020 && pos_y > 380 && pos_y < 410))
                    {
                        al_stop_sample_instance(inst_button);
                        al_play_sample_instance(inst_button);
                        if(but_5 == 0)
                        {
                            but_5 = 1;
                            but_15 = 1;
                        }
                        else
                        {
                            but_5 = 0;
                            but_15 = 0;
                        }
                    }
                    else if((pos_x >= 230 && pos_x <270 && pos_y > 430 && pos_y < 460)||(pos_x >= 980 && pos_x <1020 && pos_y > 430 && pos_y < 460))
                    {
                        al_stop_sample_instance(inst_button);
                        al_play_sample_instance(inst_button);
                        if(but_6 == 0)
                        {
                            but_6= 1;
                            but_16 = 1;
                        }
                        else
                        {
                            but_6 = 0;
                            but_16 = 0;
                        }
                    }
                    else if((pos_x >= 230 && pos_x <270 && pos_y > 480 && pos_y < 510)||(pos_x >= 980 && pos_x <1020 && pos_y > 480 && pos_y < 510))
                    {
                        al_stop_sample_instance(inst_button);
                        al_play_sample_instance(inst_button);
                        if(but_7 == 0)
                        {
                            but_7 = 1;
                            but_17 = 1;
                        }
                        else
                        {
                            but_7 = 0;
                            but_17 = 0;
                        }
                    }
                    else if((pos_x >= 230 && pos_x <270 && pos_y > 530 && pos_y < 560)||(pos_x >= 980 && pos_x <1020 && pos_y > 530 && pos_y < 560))
                    {
                        al_stop_sample_instance(inst_button);
                        al_play_sample_instance(inst_button);
                        if(but_8 == 0)
                        {
                            but_8 = 1;
                            but_18 = 1;
                        }
                        else
                        {
                            but_8 = 0;
                            but_18 = 0;
                        }
                    }
                    else if((pos_x >= 230 && pos_x <270 && pos_y > 580 && pos_y < 610)||(pos_x >= 980 && pos_x <1020 && pos_y > 580 && pos_y < 610))
                    {
                        al_stop_sample_instance(inst_button);
                        al_play_sample_instance(inst_button);
                        if(but_9 == 0)
                        {
                            but_9 = 1;
                            but_19 = 1;
                        }
                        else
                        {
                            but_9 = 0;
                            but_19 = 0;
                        }
                    }
                    else if((pos_x >= 230 && pos_x <270 && pos_y > 630 && pos_y < 680)||(pos_x >= 980 && pos_x <1020 && pos_y > 630 && pos_y < 680))
                    {
                        al_stop_sample_instance(inst_button);
                        al_play_sample_instance(inst_button);
                        if(but_10 == 0)
                        {
                            but_10 = 1;
                            but_20 = 1;
                        }
                        else
                        {
                            but_10 = 0;
                            but_20 = 0;
                        }
                    }
                    else if(pos_x >=0 && pos_x <=40 && pos_y >= 0 && pos_y <= 40)
                    {
                        if(voltar == 0)
                        {
                            but_matching = 0;
                            fechar = 0;
                            but_1 = 0,but_2 = 0,but_3 = 0,but_4 = 0,but_5 = 0,but_6 = 0,but_7 = 0,but_8 = 0,but_9 = 0,but_10 = 0;
                            but_11 = 0,but_12 = 0,but_13 = 0,but_14 = 0,but_15 = 0,but_16 = 0,but_17 = 0,but_18 = 0,but_19 = 0,but_20 = 0;
                            selecionadoDir = 0;
                            selecionadoEsq = 0;
                            conec_1_11 = 0;
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            break;
                        }
                    }
                    else if(pos_x >=1180 && pos_x <=1260 && pos_y >= 630 && pos_y <= 665)
                    {
                        confirmar = 1;
                        voltar = 1;
                        al_stop_sample_instance(inst_button);
                        al_play_sample_instance(inst_button);
                    }
                }

                if(confirmar == 1)
                {
                    int* no;
                    int* receptor;

                    if(pos_x >=0 && pos_x <=40 && pos_y >= 0 && pos_y <= 40)
                    {
                        confirmar = 0;
                        voltar = 0;
                        if(but_1 == 2)
                        {
                            selecionadoEsq = 0;
                            but_1 = 1;
                        }
                        else if(but_2 == 2)
                        {
                            selecionadoEsq = 0;
                            but_2 = 1;
                        }
                        else if(but_3 == 2)
                        {
                            selecionadoEsq = 0;
                            but_3 = 1;
                        }
                        else if(but_4 == 2)
                        {
                            selecionadoEsq = 0;
                            but_4 = 1;
                        }
                        else if(but_5 == 2)
                        {
                            selecionadoEsq = 0;
                            but_5 = 1;
                        }
                        else if(but_6 == 2)
                        {
                            selecionadoEsq = 0;
                            but_6 = 1;
                        }
                        else if(but_7 == 2)
                        {
                            selecionadoEsq = 0;
                            but_7 = 1;
                        }
                        else if(but_8 == 2)
                        {
                            selecionadoEsq = 0;
                            but_8 = 1;
                        }
                        else if(but_9 == 2)
                        {
                            selecionadoEsq = 0;
                            but_9 = 1;
                        }
                        else if(but_10 == 2)
                        {
                            selecionadoEsq = 0;
                            but_10 = 1;
                        }

                        if(but_11 == 2)
                        {
                            selecionadoDir = 0;
                            but_11 = 1;
                        }
                        else if(but_12 == 2)
                        {
                            selecionadoDir = 0;
                            but_12 = 1;
                        }
                        else if(but_13 == 2)
                        {
                            selecionadoDir = 0;
                            but_13 = 1;
                        }
                        else if(but_14 == 2)
                        {
                            selecionadoDir = 0;
                            but_14 = 1;
                        }
                        else if(but_15 == 2)
                        {
                            selecionadoDir = 0;
                            but_15 = 1;
                        }
                        else if(but_16 == 2)
                        {
                            selecionadoDir = 0;
                            but_16 = 1;
                        }
                        else if(but_17 == 2)
                        {
                            selecionadoDir = 0;
                            but_17 = 1;
                        }
                        else if(but_18 == 2)
                        {
                            selecionadoDir = 0;
                            but_18 = 1;
                        }
                        else if(but_19 == 2)
                        {
                            selecionadoDir = 0;
                            but_19 = 1;
                        }
                        else if(but_20 == 2)
                        {
                            selecionadoDir = 0;
                            but_20 = 1;
                        }

                        al_stop_sample_instance(inst_button);
                        al_play_sample_instance(inst_button);
                    }
                    else if(pos_x >=1180 && pos_x <=1260 && pos_y >= 630 && pos_y <= 665)
                    {
                        //FAZER CONFIRMAR PARA RODAR O CÓDIGO
                        al_stop_sample_instance(inst_button);
                        al_play_sample_instance(inst_button);
                    }
                    else if(pos_x >= 230 && pos_x <270 && pos_y > 180 && pos_y < 210) // Botão 1
                    {
                        if(but_1 == 1)
                        {
                            if(but_1 != 2 && but_2 != 2 && but_3 != 2 && but_4 != 2 && but_5 != 2 && but_6 != 2 && but_7 != 2 && but_8 != 2 && but_9 != 2 && but_10 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_1 = 2;
                                no = &but_1;
                                selecionadoEsq = 1;
                            }
                        }
                        else if(but_1 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_1 = 1;
                            selecionadoEsq = 0;
                        }
                    }
                    else if(pos_x >= 230 && pos_x <270 && pos_y > 230 && pos_y < 260) // Botão 2
                    {
                        if(but_2 == 1)
                        {
                            if(but_1 != 2 && but_2 != 2 && but_3 != 2 && but_4 != 2 && but_5 != 2 && but_6 != 2 && but_7 != 2 && but_8 != 2 && but_9 != 2 && but_10 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_2 = 2;
                                no = &but_2;
                                selecionadoEsq = 2;
                            }
                        }
                        else if(but_2 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_2 = 1;
                            selecionadoEsq = 0;
                        }
                    }
                    else if(pos_x >= 230 && pos_x <270 && pos_y > 280 && pos_y < 310) // Botão 3
                    {
                        if(but_3 == 1)
                        {
                            if(but_1 != 2 && but_2 != 2 && but_3 != 2 && but_4 != 2 && but_5 != 2 && but_6 != 2 && but_7 != 2 && but_8 != 2 && but_9 != 2 && but_10 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_3 = 2;
                                no = &but_3;
                                selecionadoEsq = 3;
                            }
                        }
                        else if(but_3 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_3 = 1;
                            selecionadoEsq = 0;
                        }
                    }
                    else if(pos_x >= 230 && pos_x <270 && pos_y > 330 && pos_y < 360) // Botão 4
                    {
                        if(but_4 == 1)
                        {
                            if(but_1 != 2 && but_2 != 2 && but_3 != 2 && but_4 != 2 && but_5 != 2 && but_6 != 2 && but_7 != 2 && but_8 != 2 && but_9 != 2 && but_10 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_4 = 2;
                                no = &but_4;
                                selecionadoEsq = 4;
                            }
                        }
                        else if(but_4 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_4 = 1;
                            selecionadoEsq = 0;
                        }
                    }
                    else if(pos_x >= 230 && pos_x <270 && pos_y > 380 && pos_y < 410) // Botão 5
                    {
                        if(but_5 == 1)
                        {
                            if(but_1 != 2 && but_2 != 2 && but_3 != 2 && but_4 != 2 && but_5 != 2 && but_6 != 2 && but_7 != 2 && but_8 != 2 && but_9 != 2 && but_10 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_5 = 2;
                                no = &but_5;
                                selecionadoEsq = 5;
                            }
                        }
                        else if(but_5 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_5 = 1;
                            selecionadoEsq = 0;
                        }
                    }
                    else if(pos_x >= 230 && pos_x <270 && pos_y > 430 && pos_y < 460) // Botão 6
                    {
                        if(but_6 == 1)
                        {
                           if(but_1 != 2 && but_2 != 2 && but_3 != 2 && but_4 != 2 && but_5 != 2 && but_6 != 2 && but_7 != 2 && but_8 != 2 && but_9 != 2 && but_10 != 2)
                           {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_6 = 2;
                                no = &but_6;
                                selecionadoEsq = 6;
                            }
                        }
                        else if(but_6 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_6 = 1;
                            selecionadoEsq = 0;
                        }
                    }
                    else if(pos_x >= 230 && pos_x <270 && pos_y > 480 && pos_y < 510) // Botão 7
                    {
                        if(but_7 == 1)
                        {
                            if(but_1 != 2 && but_2 != 2 && but_3 != 2 && but_4 != 2 && but_5 != 2 && but_6 != 2 && but_7 != 2 && but_8 != 2 && but_9 != 2 && but_10 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_7 = 2;
                                no = &but_7;
                                selecionadoEsq = 7;
                            }
                        }
                        else if(but_7 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_7 = 1;
                            selecionadoEsq = 0;
                        }
                    }
                    else if(pos_x >= 230 && pos_x <270 && pos_y > 530 && pos_y < 560) // Botão 8
                    {
                        if(but_8 == 1)
                        {
                            if(but_1 != 2 && but_2 != 2 && but_3 != 2 && but_4 != 2 && but_5 != 2 && but_6 != 2 && but_7 != 2 && but_8 != 2 && but_9 != 2 && but_10 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_8 = 2;
                                no = &but_8;
                                selecionadoEsq = 8;
                            }
                        }
                        else if(but_8 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_8 = 1;
                            selecionadoEsq = 0;
                        }
                    }
                    else if(pos_x >= 230 && pos_x <270 && pos_y > 580 && pos_y < 610) // Botão 9
                    {
                        if(but_9 == 1)
                        {
                            if(but_1 != 2 && but_2 != 2 && but_3 != 2 && but_4 != 2 && but_5 != 2 && but_6 != 2 && but_7 != 2 && but_8 != 2 && but_9 != 2 && but_10 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_9 = 2;
                                no = &but_9;
                                selecionadoEsq = 9;
                            }
                        }
                        else if(but_9 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_9 = 1;
                            selecionadoEsq = 0;
                        }
                    }
                    else if(pos_x >= 230 && pos_x <270 && pos_y > 630 && pos_y < 660) // Botão 10
                    {
                        if(but_10 == 1)
                        {
                            if(but_1 != 2 && but_2 != 2 && but_3 != 2 && but_4 != 2 && but_5 != 2 && but_6 != 2 && but_7 != 2 && but_8 != 2 && but_9 != 2 && but_10 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_10 = 2;
                                no = &but_10;
                                selecionadoEsq = 10;
                            }
                        }
                        else if(but_10 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_10 = 1;
                            selecionadoEsq = 0;
                        }
                    }
                    else if(pos_x >= 980 && pos_x <1020 && pos_y > 180 && pos_y < 210) // Botão 11
                    {
                        if(but_11 == 1)
                        {
                            if(but_11 != 2 && but_12 != 2 && but_13 != 2 && but_14 != 2 && but_15 != 2 && but_16 != 2 && but_17 != 2 && but_18 != 2 && but_19 != 2 && but_20 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_11 = 2;
                                receptor = &but_11;
                                selecionadoDir = 11;
                            }
                        }
                        else if(but_11 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_11 = 1;
                            selecionadoDir = 0;
                        }
                    }
                    else if(pos_x >= 980 && pos_x <1020 && pos_y > 230 && pos_y < 260) // Botão 12
                    {
                        if(but_12 == 1)
                        {
                            if(but_11 != 2 && but_12 != 2 && but_13 != 2 && but_14 != 2 && but_15 != 2 && but_16 != 2 && but_17 != 2 && but_18 != 2 && but_19 != 2 && but_20 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_12 = 2;
                                receptor = &but_12;
                                selecionadoDir = 12;
                            }
                        }
                        else if(but_12 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_12 = 1;
                            selecionadoDir = 0;
                        }
                    }
                    else if(pos_x >= 980 && pos_x <1020 && pos_y > 280 && pos_y < 310) // Botão 13
                    {
                        if(but_13 == 1)
                        {
                            if(but_11 != 2 && but_12 != 2 && but_13 != 2 && but_14 != 2 && but_15 != 2 && but_16 != 2 && but_17 != 2 && but_18 != 2 && but_19 != 2 && but_20 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_13 = 2;
                                receptor = &but_13;
                                selecionadoDir = 13;
                            }
                        }
                        else if(but_13 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_13 = 1;
                            selecionadoDir = 0;
                        }
                    }
                    else if(pos_x >= 980 && pos_x <1020 && pos_y > 330 && pos_y < 360) // Botão 14
                    {
                        if(but_14 == 1)
                        {
                            if(but_11 != 2 && but_12 != 2 && but_13 != 2 && but_14 != 2 && but_15 != 2 && but_16 != 2 && but_17 != 2 && but_18 != 2 && but_19 != 2 && but_20 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_14 = 2;
                                receptor = &but_14;
                                selecionadoDir = 14;
                            }
                        }
                        else if(but_14 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_14 = 1;
                            selecionadoDir = 0;
                        }
                    }
                    else if(pos_x >= 980 && pos_x <1020 && pos_y > 380 && pos_y < 410) // Botão 15
                    {
                        if(but_15 == 1)
                        {
                            if(but_11 != 2 && but_12 != 2 && but_13 != 2 && but_14 != 2 && but_15 != 2 && but_16 != 2 && but_17 != 2 && but_18 != 2 && but_19 != 2 && but_20 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_15 = 2;
                                receptor = &but_15;
                                selecionadoDir = 15;
                            }
                        }
                        else if(but_15 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_15 = 1;
                            selecionadoDir = 0;
                        }
                    }
                    else if(pos_x >= 980 && pos_x <1020 && pos_y > 430 && pos_y < 460) // Botão 16
                    {
                        if(but_16== 1)
                        {
                            if(but_11 != 2 && but_12 != 2 && but_13 != 2 && but_14 != 2 && but_15 != 2 && but_16 != 2 && but_17 != 2 && but_18 != 2 && but_19 != 2 && but_20 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_16 = 2;
                                receptor = &but_16;
                                selecionadoDir = 16;
                            }
                        }
                        else if(but_16 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_16 = 1;
                            selecionadoDir = 0;
                        }
                    }
                    else if(pos_x >= 980 && pos_x <1020 && pos_y > 480 && pos_y < 510) // Botão 17
                    {
                        if(but_17 == 1)
                        {
                            if(but_11 != 2 && but_12 != 2 && but_13 != 2 && but_14 != 2 && but_15 != 2 && but_16 != 2 && but_17 != 2 && but_18 != 2 && but_19 != 2 && but_20 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_17 = 2;
                                receptor = &but_17;
                                selecionadoDir = 17;
                            }
                        }
                        else if(but_17 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_17 = 1;
                            selecionadoDir = 0;
                        }
                    }
                    else if(pos_x >= 980 && pos_x <1020 && pos_y > 530 && pos_y < 560) // Botão 18
                    {
                        if(but_18 == 1)
                        {
                            if(but_11 != 2 && but_12 != 2 && but_13 != 2 && but_14 != 2 && but_15 != 2 && but_16 != 2 && but_17 != 2 && but_18 != 2 && but_19 != 2 && but_20 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_18 = 2;
                                receptor = &but_18;
                                selecionadoDir = 18;
                            }
                        }
                        else if(but_18 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_18 = 1;
                            selecionadoDir = 0;
                        }
                    }
                    else if(pos_x >= 980 && pos_x <1020 && pos_y > 580 && pos_y < 610) // Botão 19
                    {
                        if(but_19 == 1)
                        {
                            if(but_11 != 2 && but_12 != 2 && but_13 != 2 && but_14 != 2 && but_15 != 2 && but_16 != 2 && but_17 != 2 && but_18 != 2 && but_19 != 2 && but_20 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_19 = 2;
                                receptor = &but_19;
                                selecionadoDir = 19;
                            }
                        }
                        else if(but_19 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_19 = 1;
                            selecionadoDir = 0;
                        }
                    }
                    else if(pos_x >= 980 && pos_x <1020 && pos_y > 630 && pos_y < 680) // Botão 20
                    {
                        if(but_20 == 1)
                        {
                            if(but_11 != 2 && but_12 != 2 && but_13 != 2 && but_14 != 2 && but_15 != 2 && but_16 != 2 && but_17 != 2 && but_18 != 2 && but_19 != 2 && but_20 != 2)
                            {
                                al_stop_sample_instance(inst_button);
                                al_play_sample_instance(inst_button);
                                but_20 = 2;
                                receptor = &but_20;
                                selecionadoDir = 20;
                            }
                        }
                        else if(but_20 == 2)
                        {
                            al_stop_sample_instance(inst_button);
                            al_play_sample_instance(inst_button);
                            but_20 = 1;
                            selecionadoDir = 0;
                        }
                    }

                    if(selecionadoEsq != 0 && selecionadoDir != 0) // Atualiza a matriz
                    {
                        int esq = selecionadoEsq - 1;
                        int dir = selecionadoDir - 11;

                        if(linhas[esq][dir] == 0)
                        {
                            linhas[esq][dir] = 1;
                        }
                        else
                        {
                            linhas[esq][dir] = 0;
                        }

                        selecionadoEsq = 0;
                        selecionadoDir = 0;

                        verdarBotoes(no, receptor);
                    }
                }
            }

            if(al_is_event_queue_empty(event_queue))
            {
                al_draw_scaled_bitmap(fundo_matching,0,0,al_get_bitmap_width(fundo_matching),al_get_bitmap_height(fundo_matching),0,0,SCREEN_W,SCREEN_H,0);

                //al_draw_scaled_bitmap(linha_1,0,0,al_get_bitmap_width(linha_1),al_get_bitmap_height(linha_1),265,200,al_get_bitmap_width(linha_1), al_get_bitmap_height(linha_1),0);

                if(linhas[0][0]) // 1 -> 11
                {
                    al_draw_scaled_bitmap(linha_1,0,0,al_get_bitmap_width(linha_1),al_get_bitmap_height(linha_1),265,200,al_get_bitmap_width(linha_1), al_get_bitmap_height(linha_1),0);
                }

                if(linhas[0][1]) // 1 -> 12
                {
                    al_draw_rotated_bitmap(linha_1,0,0,265,200,0.07,0);
                }

                if(linhas[0][2]) // 1 -> 13
                {
                    al_draw_rotated_bitmap(linha_1_3,8,660,265,200,0.13,0);
                }

                if(linhas[0][3]) // 1 -> 14
                {
                    al_draw_rotated_bitmap(linha_1_3,2,660,265,200,0.19,0);
                }

                if(linhas[0][4]) // 1 -> 15
                {
                    al_draw_rotated_bitmap(linha_1_3,-2,660,265,200,0.26,0);
                }

                if(linhas[0][5]) // 1 -> 16
                {
                    al_draw_rotated_bitmap(linha_1_2,30,310,265,200,0.32,0);
                }

                if(linhas[0][6]) // 1 -> 17
                {
                    al_draw_rotated_bitmap(linha_1_2,20,310,265,200,0.38,0);
                }

                if(linhas[0][7]) // 1 -> 18
                {
                    al_draw_rotated_bitmap(linha_1_2,0,310,265,200,0.43,0);
                }

                if(linhas[0][8]) // 1 -> 19
                {
                    al_draw_rotated_bitmap(linha_1_2,0,310,265,200,0.49,0);
                }

                if(linhas[0][9]) // 1 -> 20
                {
                    al_draw_rotated_bitmap(linha_1_4,0,590,265,200,0.54,0);
                }



                if(but_1 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,180,40,40,0);
                }
                else if(but_1 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),230,180,40,40,0);
                }
                else if(but_1 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),230,180,40,40,0);
                }

                if(but_2 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,230,40,40,0);
                }
                else if(but_2 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),230,230,40,40,0);
                }
                else if(but_2 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),230,230,40,40,0);
                }

                if(but_3 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,280,40,40,0);
                }
                else if(but_3 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),230,280,40,40,0);
                }
                else if(but_3 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),230,280,40,40,0);
                }

                if(but_4 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,330,40,40,0);
                }
                else if(but_4 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),230,330,40,40,0);
                }
                else if(but_4 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),230,330,40,40,0);
                }

                if(but_5 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,380,40,40,0);
                }
                else if(but_5 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),230,380,40,40,0);
                }
                else if(but_5 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),230,380,40,40,0);
                }

                if(but_6 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,430,40,40,0);
                }
                else if(but_6 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),230,430,40,40,0);
                }
                else if(but_6 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),230,430,40,40,0);
                }

                if(but_7 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,480,40,40,0);
                }
                else if(but_7 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),230,480,40,40,0);
                }
                else if(but_7 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),230,480,40,40,0);
                }

                if(but_8 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,530,40,40,0);
                }
                else if(but_8 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),230,530,40,40,0);
                }
                else if(but_8 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),230,530,40,40,0);
                }

                if(but_9 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,580,40,40,0);
                }
                else if(but_9 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),230,580,40,40,0);
                }
                else if(but_9 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),230,580,40,40,0);
                }

                if(but_10 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),230,630,40,40,0);
                }
                else if(but_10 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),230,630,40,40,0);
                }
                else if(but_10 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),230,630,40,40,0);
                }

                if(but_11 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),980,180,40,40,0);
                }
                else if(but_11 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),980,180,40,40,0);
                }
                else if(but_11 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),980,180,40,40,0);
                }

                if(but_12 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),980,230,40,40,0);
                }
                else if(but_12 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),980,230,40,40,0);
                }
                else if(but_12 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),980,230,40,40,0);
                }

                if(but_13 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),980,280,40,40,0);
                }
                else if(but_13 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),980,280,40,40,0);
                }
                else if(but_13 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),980,280,40,40,0);
                }

                if(but_14 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),980,330,40,40,0);
                }
                else if(but_14 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),980,330,40,40,0);
                }
                else if(but_14 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),980,330,40,40,0);
                }

                if(but_15 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),980,380,40,40,0);
                }
                else if(but_15 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),980,380,40,40,0);
                }
                else if(but_15 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),980,380,40,40,0);
                }

                if(but_16 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),980,430,40,40,0);
                }
                else if(but_16 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),980,430,40,40,0);
                }
                else if(but_16 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),980,430,40,40,0);
                }

                if(but_17 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),980,480,40,40,0);
                }
                else if(but_17 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),980,480,40,40,0);
                }
                else if(but_17 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),980,480,40,40,0);
                }

                if(but_18 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),980,530,40,40,0);
                }
                else if(but_18 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),980,530,40,40,0);
                }
                else if(but_18 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),980,530,40,40,0);
                }

                if(but_19 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),980,580,40,40,0);
                }
                else if(but_19 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),980,580,40,40,0);
                }
                else if(but_19 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),980,580,40,40,0);
                }

                if(but_20 == 0)
                {
                    al_draw_scaled_bitmap(butimage_vermelho,0,0,al_get_bitmap_width(butimage_vermelho),al_get_bitmap_height(butimage_vermelho),980,630,40,40,0);
                }
                else if(but_20 == 1)
                {
                    al_draw_scaled_bitmap(butimage_verde,0,0,al_get_bitmap_width(butimage_verde),al_get_bitmap_height(butimage_verde),980,630,40,40,0);
                }
                else if(but_20 == 2)
                {
                    al_draw_scaled_bitmap(butimage_azul,0,0,al_get_bitmap_width(butimage_azul),al_get_bitmap_height(butimage_azul),980,630,40,40,0);
                }

                al_draw_scaled_bitmap(butimage_confirmar,0,0,al_get_bitmap_width(butimage_confirmar),al_get_bitmap_height(butimage_confirmar),1180,630,40,40,0);
                al_draw_scaled_bitmap(butimage_voltar,0,0,al_get_bitmap_width(butimage_voltar),al_get_bitmap_height(butimage_voltar),0,0,40,40,0);


                //desenha com as cordenada
                if(pos_x >= 1180 && pos_x <1260 && pos_y > 630 && pos_y < 665)
                {
                    al_draw_scaled_bitmap(butimage_confirmar,0,0,al_get_bitmap_width(butimage_confirmar),al_get_bitmap_height(butimage_confirmar),1160,620,60,60,0);

                }
                else if(pos_x >= 0 && pos_x <= 40 && pos_y >= 0 && pos_y <= 40)
                {
                     al_draw_scaled_bitmap(butimage_voltar,0,0,al_get_bitmap_width(butimage_voltar),al_get_bitmap_height(butimage_voltar),-4,-4,60,60,0);
                }
            }
            al_flip_display();
        }

        while(but_shortest)
        {
            ALLEGRO_EVENT ev3;
            al_wait_for_event(event_queue,&ev3);

            if(ev3.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                pos_x = ev3.mouse.x;
                pos_y = ev3.mouse.y;
            }
            else if(ev3.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                but_shortest = 0;
                break;
            }

            else if(ev3.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                //FAZ ALGO QUANDO O MOUSE FOR CLICADO
                //AI VC COLOCA AS CORDENADAS (pos_x > 100 && pos_x<200 && pos_y>50 etc){ código pra fazer algo, MAS AQ NAO MOSTRA NA TELA}
            }

            if(al_is_event_queue_empty(event_queue))
            {
                //DESENHA NA TELA OQ VC QUISER E PODE DESENHAR PELA CORDENADA DO MOUSE TBM , se o mouse tiver entre dois pontos ele desenha algo,etc
                al_draw_scaled_bitmap(fundo_matching,0,0,al_get_bitmap_width(fundo_matching),al_get_bitmap_height(fundo_matching),0,0,SCREEN_W,SCREEN_H,0);
                //Esse al draw desenha a imagem q vc criou lá em cima q eu chamei de fundo matching,e onde tem 0,0,SCREENW,SCREEN H ,
                // 0, 0 É A COORDENADA DA IMAGEM,E ONDE TA SCREEN É O TAMANHO QUE VOCE QUER QUE A IMAGEM TENHA
            }

            //ATUALIZA TELA
            al_flip_display();
        }

        if(desligar == 1)
        {
            break;
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
