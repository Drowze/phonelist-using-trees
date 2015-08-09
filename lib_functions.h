#ifndef LIB_FUNCTIONS_H_INCLUDED
#define LIB_FUNCTIONS_H_INCLUDED

#define size_nome 32
#include <sys/time.h>

struct s_registro{
    char nome[size_nome];
    double CPF;
    double numeros[5];
    int qtd_numeros;
};
typedef struct s_registro s_registro;

struct no_registro{
    struct s_registro cadastro;
    struct no_registro *esq;
    struct no_registro *dir;
};
typedef struct no_registro no_registro;

struct linked_list{
	struct no_registro *registro;
	struct linked_list *prox;
};
typedef struct linked_list linked_list;

typedef enum {
    false = 0,
    true = 1,
} bool;

no_registro *g; /* variavel global para a remoção */

int escreve_no_arquivo(no_registro *Lista);
void escreve_aux(no_registro *tree, FILE *arquivo);
no_registro *le_arquivo();
no_registro *struct_para_no(s_registro registro); //CHECK

//Prioridade:
no_registro *cria_no(); //CHECK
void adicionar_registro(no_registro **Lista, no_registro *novo_no, bool *pertence); //CHECK
//
no_registro *busca_registro_cpf(no_registro *tree, double cpf, int *profundidade); //CHECK
int busca_registro_nome(no_registro *tree, no_registro *aux, char nome[], struct timeval before); //CHECK
int busca_registro_numero(no_registro *tree, no_registro *aux, double numero, struct timeval before); //CHECK
void remove_registro (no_registro *no_removido, no_registro **Lista); //CHECK
void remove2_registro (no_registro **aux); //CHECK
//
void altera_registro(no_registro *no_alterado, no_registro **Lista); //CHECK
//
void exibe_in_ordem(no_registro *Lista, bool oganiza_cpf); //CHECK
void exibe_registro(s_registro registro); //CHECK
//
void exibe_tree(no_registro *Lista);
//

//Lista ligada:
int linked_adiciona(no_registro *novo_no, linked_list **lista); //CHECK
int exibe_linked(linked_list *lista); //CHECK
int linked_wipe(linked_list **lista); //CHECK
void tree_to_linked(no_registro *tree, linked_list **lista); //CHECK

//auxiliares
//void imprime_string_sem_n(char string[]);
bool cpf_valido(double cpf); //CHECK
void capitalizing(char string[]); //CHECK
double time_diff(struct timeval x , struct timeval y); //CHECK
int altura(no_registro *tree, no_registro *no_procurado, int depth); //CHECK
void debugger_exibe_arvore(no_registro *tree, no_registro *tree_original);

#endif