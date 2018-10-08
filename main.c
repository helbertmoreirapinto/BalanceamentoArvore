#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RET_OK              0
#define RET_ERRO_MEMORIA    1
#define RET_ERRO_DUPLICADO  2
#define RET_ERRO            3

#define DIR_ELE_ESQ         0
#define ESQ_ELE_DIR         1
#define ELE_ESQ_DIR         2
#define DIR_ESQ_ELE         3
#define ESQ_DIR_ELE         4
#define ELE_DIR_ESQ         5

#define LADO_ESQUERDA       0
#define LADO_DIREITA        1

#define MAX 5
typedef int Dado;

struct Elemento{
    struct Elemento* pai;
    struct Elemento* esq;
    struct Elemento* dir;
    Dado dado;
};

typedef struct Elemento Elemento;
typedef Elemento*  Elemento_PTR;
typedef Elemento** Elemento_PTR_PTR;

int inserir_arvore(Elemento_PTR_PTR, Dado);
int imprimir_arvore(Elemento_PTR, int);
void modos_imprimir(Elemento_PTR);
int balanc_AVL(Elemento_PTR_PTR);

int main(){
    int retorno;
    int num;
    Elemento_PTR raiz = NULL;
    srand((unsigned)time(NULL));
    int i = 0;
    do{
        num = (rand() % MAX);
        retorno = inserir_arvore(&raiz, num);
        switch(retorno){
        case RET_OK:
            printf("Numero %2d OK\n", num);
            retorno = balanc_AVL(&raiz);
            printf("--------------\n");
            break;
        case RET_ERRO_DUPLICADO:
//            printf("Numero %2d Duplicado\n", num);
            continue;
        case RET_ERRO:
            printf("Numero %2d Erro.\n", num);
            break;
        case RET_ERRO_MEMORIA:
            printf("Erro ao alocar memoria\n");
            return 0;
        }
        i++;
    }while(i < MAX);

    modos_imprimir(raiz);
    return 0;
}

int contaNV(Elemento_PTR elem, int nivel){
    if(!elem)
        return 0;
    int e = contaNV(elem->esq, nivel);
    int d = contaNV(elem->dir, nivel);
    nivel++;

    if(e >= d)
        nivel += e;
    else if(d > e)
        nivel += d;
    return nivel;
}

void rotacionar(Elemento_PTR_PTR elemA, Elemento_PTR_PTR elemB, int lado){
    if(lado == LADO_DIREITA){
        printf("horario\n");
        if((*elemA)->pai){
            (*elemA)->pai->dir = (*elemB);
            (*elemA)->pai->esq = NULL;

        }
        (*elemA)->esq = (*elemB)->esq;
        (*elemB)->dir = (*elemA);
    }else{
        printf("anti-horario\n");
        if((*elemA)->pai){
            (*elemA)->pai->esq = (*elemB);
            (*elemA)->pai->dir = NULL;
        }
        (*elemA)->dir = (*elemB)->dir;
        (*elemB)->esq = (*elemA);
    }
    (*elemB)->pai = (*elemA)->pai;
    (*elemA)->pai = (*elemB);
}

int balanc_AVL(Elemento_PTR_PTR elem){
    if(!(*elem))
        return RET_ERRO;
    int nvlE, nvlF;

    balanc_AVL(&(*elem)->esq);
    balanc_AVL(&(*elem)->dir);
    nvlE = contaNV((*elem)->esq, 0) - contaNV((*elem)->dir, 0);
    printf("elem(%d) -> %d\n",(*elem)->dado, nvlE);
    if(nvlE < -1){
        printf("BALANCEAMENTO AQUI\n");
        nvlF = contaNV((*elem)->dir->esq, 0) - contaNV((*elem)->dir->dir, 0);
        if(nvlF < 0){
            printf("RSE\n");
            rotacionar(elem, &(*elem)->dir, LADO_ESQUERDA);
        }else{
            printf("RDE -> RSD RSE\n");
            rotacionar(&(*elem)->dir, &(*elem)->dir->esq, LADO_DIREITA);
            rotacionar(elem, &(*elem)->dir, LADO_ESQUERDA);
        }
    }else if(nvlE > 1){
        printf("BALANCEAMENTO AQUI\n");
        nvlF = contaNV((*elem)->esq->esq, 0) - contaNV((*elem)->esq->dir, 0);
        if(nvlF < 0){
            printf("RDD -> RSE RSD\n");
            rotacionar(&(*elem)->esq, &(*elem)->esq->dir, LADO_ESQUERDA);
            rotacionar(elem, &(*elem)->esq, LADO_DIREITA);
        }else{
            printf("RSD\n");
            rotacionar(elem, &(*elem)->esq, LADO_DIREITA);
        }
    }

    return RET_ERRO;
}

int inserir_arvore(Elemento_PTR_PTR raiz, Dado dado){
    Elemento_PTR no = (Elemento_PTR) calloc(1, sizeof(Elemento));
    if(!no) return RET_ERRO_MEMORIA;

    no->dado = dado;

    if(!(*raiz)){
        *raiz = no;
        return RET_OK;
    }

    Elemento_PTR aux = *raiz;
    do{
        if(no->dado < aux->dado){
            if(!aux->esq){
                no->pai = aux;
                aux->esq = no;
                return RET_OK;
            }else
                aux = aux->esq;
        }else if(no->dado > aux->dado){
            if(!aux->dir){
                no->pai = aux;
                aux->dir = no;
                return RET_OK;
            }else
                aux = aux->dir;
        }else if(no->dado == aux->dado)
            return RET_ERRO_DUPLICADO;

    }while(aux);

    return RET_ERRO;
}

void modos_imprimir(Elemento_PTR raiz){
    printf("ESQ_ELE_DIR ");
    imprimir_arvore(raiz, ESQ_ELE_DIR);
    printf("\n");

    printf("ESQ_DIR_ELE ");
    imprimir_arvore(raiz, ESQ_DIR_ELE);
    printf("\n");

    printf("ELE_ESQ_DIR ");
    imprimir_arvore(raiz, ELE_ESQ_DIR);
    printf("\n");

    printf("ELE_DIR_ESQ ");
    imprimir_arvore(raiz, ELE_DIR_ESQ);
    printf("\n");

    printf("DIR_ESQ_ELE ");
    imprimir_arvore(raiz, DIR_ESQ_ELE);
    printf("\n");

    printf("DIR_ELE_ESQ ");
    imprimir_arvore(raiz, DIR_ELE_ESQ);
}

int imprimir_arvore(Elemento_PTR elem, int tipo){
    if(!elem)
        return 0;

    switch(tipo){
    case ESQ_DIR_ELE:
        imprimir_arvore(elem->esq, ESQ_DIR_ELE);
        imprimir_arvore(elem->dir, ESQ_DIR_ELE);
        printf("%2d ", elem->dado);
        break;
    case ESQ_ELE_DIR:
        imprimir_arvore(elem->esq, ESQ_ELE_DIR);
        printf("%2d ", elem->dado);
        imprimir_arvore(elem->dir, ESQ_ELE_DIR);
        break;
    case DIR_ELE_ESQ:
        imprimir_arvore(elem->dir, DIR_ELE_ESQ);
        printf("%2d ", elem->dado);
        imprimir_arvore(elem->esq, DIR_ELE_ESQ);
        break;
    case DIR_ESQ_ELE:
        imprimir_arvore(elem->dir, DIR_ESQ_ELE);
        imprimir_arvore(elem->esq, DIR_ESQ_ELE);
        printf("%2d ", elem->dado);
        break;
    case ELE_DIR_ESQ:
        printf("%2d ", elem->dado);
        imprimir_arvore(elem->dir, ELE_DIR_ESQ);
        imprimir_arvore(elem->esq, ELE_DIR_ESQ);
        break;
    case ELE_ESQ_DIR:
        printf("%2d ", elem->dado);
        imprimir_arvore(elem->esq, ELE_ESQ_DIR);
        imprimir_arvore(elem->dir, ELE_ESQ_DIR);
        break;
    }
    return 1;
}

