typedef struct elemento* Matriz;

Matriz* inicializa_Matriz();
int cria_Matriz(Matriz* mat, int l, int c);
void libera_Matriz(Matriz* mat);
void imprime_Matriz(Matriz* mat);
void menu();
int insere_matriz(Matriz* mat, int linha, int coluna);
int remove_matriz(Matriz* mat,int linha,int coluna);
int busca_vp(Matriz* mat,int linha,int coluna);
int busca_pv(Matriz* mat);
int vizinhos(Matriz* mat,int linha,int coluna);
int ler_inteiro();
int procura_noExistente(Matriz* mat, int l, int c, int valor);