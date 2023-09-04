#include <stdio.h>
#include <stdlib.h>
#include "matrizesparsa.h"

int main(){ 
    int linha, coluna, ope, loop = 1;

    printf("Informe a quantidade de linhas: ");
    linha = ler_inteiro();
    printf("Informe a quantidade de colunas: ");
    coluna = ler_inteiro();     

    if(linha <= 0 || coluna <= 0){ //nao aceita tamanhos negativos ou nullos
        printf("Valores Invalido! os tamanhos nao podem ser negativos, nullos ou letras...");
        system("pause");
        exit(0);
    }

    Matriz* mat = inicializa_Matriz();
    cria_Matriz(mat,linha,coluna);


    while(loop){
        system("cls");
        menu();
        ope = ler_inteiro();

        switch(ope){
        case 1:
            insere_matriz(mat,linha,coluna);
            break;
        case 2:
            remove_matriz(mat,linha,coluna);
            break;
        case 3:
            busca_vp(mat,linha,coluna);
            break;
        case 4:
            busca_pv(mat);
            break;
        case 5:
            vizinhos(mat,linha,coluna);
            break;  
        case 7:
            loop = !loop;
            break;
        case 6:
            imprime_Matriz(mat); 
            break;                                         
        default:
            printf("Operacao Invalida! tente novamente...");
        }
        system("pause");
    }
    libera_Matriz(mat);
    return 0;
}

