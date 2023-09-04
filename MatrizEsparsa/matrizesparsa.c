#include <stdio.h>
#include <stdlib.h>
#include "matrizesparsa.h" 

struct elemento{
    struct elemento *ant;
    struct elemento *prox;
    struct elemento *top;
    struct elemento *baixo;
    int info;
    int l;
    int c;
};

typedef struct elemento Elem;

Matriz* inicializa_Matriz(){ //ponteiro que ira olhar para a primeira posicao da matriz esparsa
    Matriz* mat = (Matriz*) malloc(sizeof(Matriz));
    if(mat != NULL)
        *mat = NULL;
    return mat;
}

int cria_Matriz(Matriz* mat, int linha, int coluna) {
    if (mat == NULL)
        return 0;

    //criacao da cabeça -1 -1
    Elem* inicio = (Elem*) malloc(sizeof(Elem));
    if (inicio == NULL)
        return 0;

    *mat = inicio;
    inicio->c = -1;
    inicio->l = -1;

    // Criação das colunas
    Elem* coluna_atual = inicio;
    for (int c = 0; c < coluna; c++) {
        Elem* no = (Elem*) malloc(sizeof(Elem));
        if (no == NULL)
            return 0;

        coluna_atual->prox = no;
        no->ant = coluna_atual;
        no->prox = NULL;
        no->top = NULL;
        no->baixo = NULL;
        no->l = -1;
        no->c = c;

        coluna_atual = coluna_atual->prox;
    }

    // Criação das linhas
    inicio = *mat;
    for (int l = 0; l < linha; l++) {
        Elem* no = (Elem*) malloc(sizeof(Elem));
        if (no == NULL)
            return 0;

        inicio->baixo = no;
        no->top = inicio;
        no->baixo = NULL;
        no->ant = NULL;
        no->prox = NULL;
        no->l = l;
        no->c = -1;

        inicio = inicio->baixo;
    }

    return 1;
}


void libera_Matriz(Matriz* mat){
    if(mat == NULL)
        return;

    Elem* linha_atual = *mat;
    Elem* linha_proxima;

    while (linha_atual != NULL) {
        Elem* coluna_atual = linha_atual->prox;
        Elem* coluna_proxima;

        while (coluna_atual != NULL) {
            coluna_proxima = coluna_atual->prox;
            free(coluna_atual);
            coluna_atual = coluna_proxima;
        }

        linha_proxima = linha_atual->baixo;
        free(linha_atual);
        linha_atual = linha_proxima;
    }
    free(mat);
}


void imprime_Matriz(Matriz* mat) {
    Elem* linha_atual = (*mat)->baixo;

    while (linha_atual != NULL) {
        Elem* coluna_atual = linha_atual->prox;

        while (coluna_atual != NULL) {
            printf("%d.%d: %d \n",coluna_atual->l, coluna_atual->c, coluna_atual->info); //printa valor armazenado
            coluna_atual = coluna_atual->prox;
        }
        linha_atual = linha_atual->baixo;
    }
}



void menu(){
    printf("1 - Inserir/Mudar valor\n");
    printf("2 - Remover valor\n");
    printf("3 - Buscar valor de posicao\n");
    printf("4 - Buscar posicao de um valor\n");
    printf("5 - Monstrar vizinhos\n");
    printf("6 - Monstrar Matriz\n");
    printf("7 - Sair\n");
    printf("Oque deseja fazer: ");
}

int insere_matriz(Matriz* mat, int linha, int coluna) {
    if (mat == NULL)
        return 0;

    int c, l, valor;

    printf("Informe a linha: ");
    l = ler_inteiro();
    printf("Informe a coluna: ");
    c = ler_inteiro();

    if (l >= linha || c >= coluna || l < 0 || c < 0) { // Verifica se as coordenadas estão dentro da matriz
        printf("Posicao Invalida!\n");
        return 0;
    }

    printf("Informe o valor: ");
    valor = ler_inteiro();


    if(procura_noExistente(mat,l,c,valor))
        return 1;

    Elem* no = (Elem*) malloc(sizeof(struct elemento));
    if (no == NULL)
        return 0;
    no->info = valor;
    no->c = c;
    no->l = l;
    no->prox = NULL;
    no->top = NULL;
    no->baixo = NULL;
    no->ant = NULL;

    Elem* no_coluna = *mat;
    while(no_coluna->c != c){ // procura a posicao informada
        no_coluna = no_coluna->prox;
    }

    if (no_coluna->baixo == NULL) { // insere primeiro
        no_coluna->baixo = no;
    } else {
        while (no_coluna->baixo != NULL && no_coluna->baixo->l < no->l) {
            no_coluna = no_coluna->baixo;
        }
        if (no_coluna->baixo == NULL) { //insere final
            no_coluna->baixo = no;
            no->top = no_coluna;
        } else { //insere meio e inicio
            no_coluna->baixo->top = no;
            no->baixo = no_coluna->baixo;
            no_coluna->baixo = no;
            if(no_coluna->l != -1){ //verifica se meu top pertence a matriz
                no->top = no_coluna;
            }
        }
    }

    Elem* no_linha = *mat;
    while(no_linha->l != l){ // procura a posicao informada
        no_linha = no_linha->baixo;
    }

    if (no_linha->prox == NULL) { // insere primeiro
        no_linha->prox = no;
        no->ant = NULL;
    } else {
        while (no_linha->prox != NULL && no_linha->prox->c < no->c) {
            no_linha = no_linha->prox;
        }
        if (no_linha->prox == NULL) { //insere final
            no_linha->prox = no;
            no->ant = no_linha;
        } else { //insere meio e inicio
            no_linha->prox->ant = no;
            no->prox = no_linha->prox;
            no_linha->prox = no;
            if(no_linha->c != -1){ //verifica se meu ant pertence a matriz
               no->ant = no_linha; 
            }
        }
    }
    return 1;
}


int remove_matriz(Matriz* mat, int linha, int coluna) {
    if (mat == NULL)
        return 0;

    int c, l;

    printf("Informe a linha: ");
    l = ler_inteiro();
    printf("Informe a coluna: ");
    c = ler_inteiro();

    if (l >= linha || c >= coluna || l < 0 || c < 0) { // Verifica se as coordenadas estão dentro da matriz
        printf("Posicao Invalida!\n");
        return 0;
    }

    Elem* coluna_superior = *mat;
    while(coluna_superior->c != c){
        coluna_superior = coluna_superior->prox;
    }
    Elem* linha_atual = (*mat)->baixo;

    while (linha_atual != NULL) {
        Elem* coluna_atual = linha_atual->prox;

        while (coluna_atual != NULL) {
            if (coluna_atual->l == l && coluna_atual->c == c) {
                // Remove o nó da linha
                if (coluna_atual->ant == NULL) {
                    linha_atual->prox = coluna_atual->prox;
                } else {
                    coluna_atual->ant->prox = coluna_atual->prox;
                }
                if (coluna_atual->prox != NULL) {
                    coluna_atual->prox->ant = coluna_atual->ant;
                }

                // Remove o nó da coluna
                if (coluna_atual->top == NULL) {
                    coluna_superior->baixo = coluna_atual->baixo;
                } else {
                    coluna_atual->top->baixo = coluna_atual->baixo;
                }
                if (coluna_atual->baixo != NULL) {
                    coluna_atual->baixo->top = coluna_atual->top;
                }

                free(coluna_atual);
                return 1;
            }
            coluna_atual = coluna_atual->prox;
        }
        linha_atual = linha_atual->baixo;
    }

    printf("Desculpe, essa posicao ja esta nula!\n");
    return 1;
}


int busca_vp(Matriz* mat,int linha,int coluna){
    if(mat == NULL)
        return 0;

    int c,l;

    printf("Informe a linha: ");
    l = ler_inteiro();
    printf("Informe a coluna: ");
    c = ler_inteiro();

    if(l >= linha || c >= coluna || l < 0 || c < 0){ // Verifica se as coordenadas estão dentro da matriz
        printf("Posicao Invalida!\n");
        return 0;
    }

    Elem* linha_atual = (*mat)->baixo;

    while (linha_atual != NULL) {
        Elem* coluna_atual = linha_atual->prox;

        while (coluna_atual != NULL) {
            if(coluna_atual->c == c && coluna_atual->l == l){
                printf("O valor da posicao %d.%d eh: %d\n",l,c,coluna_atual->info); //printa o valor armazenado
                return 1;
            }
            coluna_atual = coluna_atual->prox;
        }
        linha_atual = linha_atual->baixo;
    }

    printf("O valor da posicao eh: null"); //printa se null
    return 1;
}

int busca_pv(Matriz* mat){
    if(mat == NULL)
        return 0;

    int c, l, valor, valor_encontrado = 1;

    printf("Informe o valor: ");
    if (scanf("%d", &valor) != 1){
        return 0;
    }


    printf("A posicao do valor %d eh: ", valor);
    Elem* linha_atual = (*mat)->baixo;

    while (linha_atual != NULL) {
        Elem* coluna_atual = linha_atual->prox;

        while (coluna_atual != NULL) {
            if(coluna_atual->info == valor){
                printf("%d.%d ",coluna_atual->l, coluna_atual->c);
                valor_encontrado = 0; //variavel para verificar se algum valor foi encontrado pelo menos uma vez
            }
            coluna_atual = coluna_atual->prox;
        }
        linha_atual = linha_atual->baixo;
    }

    if(valor_encontrado){ //caso nao tenha encontrado nada
        printf("Nenhuma");
    }
    printf("\n");
    return 1;
}

int vizinhos(Matriz* mat,int linha,int coluna){
    if(mat == NULL)
        return 0;

    int c,l;

    printf("Informe a linha: ");
    l = ler_inteiro();
    printf("Informe a coluna: ");
    c = ler_inteiro();

    if(l >= linha || c >= coluna || l < 0 || c < 0){ // Verifica se as coordenadas estão dentro da matriz
        printf("Posicao Invalida!\n");
        return 0;
    }

    Elem* linha_atual = (*mat)->baixo;
    while (linha_atual != NULL) {
        Elem* coluna_atual = linha_atual->prox;

        while (coluna_atual != NULL) {
            if(coluna_atual->c == c && coluna_atual->l == l){
                //conjuto de verificacao para garantir que o vizinho é mesmo o vizinho ou esta null
                if(coluna_atual->top != NULL && coluna_atual->top->l == l-1){
                    printf("O vizinho do top eh: %d\n", coluna_atual->top->info);
                }else{
                    printf("O vizinho do top eh: null\n");
                }
                if(coluna_atual->baixo != NULL && coluna_atual->top->l == l+1){
                    printf("O vizinho de baixo eh: %d\n", coluna_atual->baixo->info);
                }else{
                    printf("O vizinho de baixo eh: null\n");
                }
                if(coluna_atual->prox != NULL && coluna_atual->prox->c == c+1){
                    printf("O vizinho da frente eh: %d\n", coluna_atual->prox->info);
                }else{
                    printf("O vizinho da frente eh: null\n");
                }
                if(coluna_atual->ant != NULL && coluna_atual->prox->c == c-1){
                    printf("O vizinho anterior eh: %d\n", coluna_atual->ant->info);
                }else{
                    printf("O vizinho anterior eh: null\n");
                }
                return 1;
            }
            coluna_atual = coluna_atual->prox;
        }
        linha_atual = linha_atual->baixo;
    }
    printf("A posicao inserida ainda nao existe!\n");
    return 1;
}

int ler_inteiro(){
    int ope;
    if (scanf("%d", &ope) != 1) {
        // Limpando o buffer do teclado para evitar loop infinito
        while (getchar() != '\n');
        return -1;
    }else{
        while (getchar() != '\n'); //Esse while garante que, se o usuário digite pelo menos o primeiro valor inteiro, ele irá desconsiderar as letras posteriores e continuará a execução do programa
        return ope;   
    }
}

int procura_noExistente(Matriz* mat, int l, int c, int valor){
    Elem* linha_atual = (*mat)->baixo;

    while (linha_atual != NULL) {
        Elem* coluna_atual = linha_atual->prox;

        while (coluna_atual != NULL) { //procura se o no com as coordenadas passadas existe
            if(coluna_atual->c == c && coluna_atual->l == l){
                coluna_atual->info = valor; //se sim muda o valor
                return 1;
            }
            coluna_atual = coluna_atual->prox;
        }
        linha_atual = linha_atual->baixo;
    }
    return 0;
}