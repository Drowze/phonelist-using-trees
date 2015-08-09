//Pra diferença de tempo em micros: gettimeofday(); difftime();
//Para salvar: usar pré-ordem que dá um baguio zig zag loko

#include <stdio_ext.h> //consigo usar __fpurge(stdin) sem warnings (warning acusado usando gcc -wall)
#include <stdlib.h>
#include <string.h>
#include "lib_functions.h"
#include <ctype.h>
#include <sys/time.h>

int escreve_no_arquivo(no_registro *tree){
    if(tree == NULL)
        return 1;
    
    FILE *arquivo = NULL;
    arquivo = fopen("arvore_registros.bin", "wb");
    if(arquivo == NULL)
        return 1;
    
    escreve_aux(tree, arquivo);

    fclose(arquivo);
    return 0;    
}

void escreve_aux(no_registro *tree, FILE *arquivo){
    if(tree != NULL){
        fwrite(&(tree->cadastro), sizeof(s_registro), 1, arquivo);
        escreve_aux(tree->esq, arquivo);
        escreve_aux(tree->dir, arquivo);
    }
}

no_registro *le_arquivo(){
    s_registro registro;
    no_registro *tree = NULL;
    no_registro *novo_no;
    bool pertence = false;

    FILE *arquivo = fopen("arvore_registros.bin", "rb");
    if(arquivo == NULL)
        return NULL;
    while(fread(&registro, sizeof(s_registro), 1, arquivo) == 1){
        novo_no = struct_para_no(registro);
        adicionar_registro(&tree, novo_no, &pertence);
    }

    fclose(arquivo);
    return tree;
}

no_registro *cria_no(){
    s_registro novo_registro;
    int i;
    
    printf("Digite o nome: ");
    __fpurge(stdin); fgets(novo_registro.nome, size_nome, stdin);
    capitalizing(novo_registro.nome);
    printf("Digite o CPF: ");
    do{
        scanf("%lf", &novo_registro.CPF);
        if(!cpf_valido(novo_registro.CPF))
            printf("CPF invalido, digite novamente");
    }while(!cpf_valido(novo_registro.CPF));
    do{
        printf("Quantidade de numeros para contato: ");
        scanf("%d", &novo_registro.qtd_numeros);
        if(novo_registro.qtd_numeros < 1 || novo_registro.qtd_numeros > 5)
            printf("Quantidade invalida");
    }while(novo_registro.qtd_numeros < 1 || novo_registro.qtd_numeros > 5);
    for(i = 0; i < novo_registro.qtd_numeros; i++) {
        printf("%dº telefone para contato: ", i+1);
        scanf("%lf", &novo_registro.numeros[i]);
    }

    no_registro *novo_no;
    novo_no = struct_para_no(novo_registro);

    return novo_no;
}

//Utilidade principal: converter as structs de um arquivo para nós de uma lista
no_registro *struct_para_no(s_registro cadastro){
    no_registro *novo_no = (no_registro *)malloc(sizeof(no_registro));
    novo_no->esq = NULL;
    novo_no->dir = NULL;
    novo_no->cadastro = cadastro;

    return novo_no;
}

void adicionar_registro(no_registro **Lista, no_registro *novo_no, bool *pertence){
    if (*Lista == NULL) {
        *Lista = novo_no;
        *pertence = false;
    } else
        if (novo_no->cadastro.CPF == (*Lista)->cadastro.CPF){
            *pertence = true;
        } else 
            if (novo_no->cadastro.CPF < (*Lista)->cadastro.CPF){
                adicionar_registro(&((*Lista)->esq), novo_no, pertence);
            } else
                adicionar_registro(&((*Lista)->dir), novo_no, pertence);
}

void remove_registro (no_registro *no_removido, no_registro **Lista) {
    if (*Lista == NULL) 
        printf("Nao ha chave k\n");
    else
        if (no_removido->cadastro.CPF < (*Lista)->cadastro.CPF) {
            remove_registro (no_removido, &(*Lista)->esq);
        } else
            if (no_removido->cadastro.CPF > (*Lista)->cadastro.CPF){
                remove_registro (no_removido, &(*Lista)->dir);
            } else {
                g = *Lista;
                if (g->dir == NULL) 
                    *Lista = g->esq;
                else 
                    if (g->esq == NULL) 
                        *Lista = g->dir;
                    else
                        remove2_registro (&g->esq);
                    free (g);
            }
}

void remove2_registro (no_registro **r) {
    if ((*r)->dir != NULL) {
        remove2_registro (&(*r)->dir) ;
    } else {
        g->cadastro = (*r)->cadastro ;
        g = *r ;
        *r = (*r)->esq ;
    }
}

void altera_registro (no_registro *no_alterado, no_registro **Lista){
    //1) copiar no_alterado->cadastro para copia
    //2) alterações no copia
    //3) remover no_alterado da lista
    //4) criar no_copia e colocar na lista

    int op, i, aux, aux1;
    s_registro copia = no_alterado->cadastro;
    no_registro *no_copia;
    bool auxiliar_boolean = false;

    do{
        printf("\n Qual campo deseja alterar?\n");
        printf("1- Nome \t| atual: %s", copia.nome);
        printf("2- CPF \t\t| atual: %.0lf\n", copia.CPF);
        printf("3- Numeros de telefone\n");
        printf("\n 0: Salvar e sair");
        printf("\n-1: Sair sem salvar\n");
        printf("Sua opcao: ");
        scanf("%d", &op);
        switch(op) {
            case 1:
                printf("Digite o novo nome: ");
                __fpurge(stdin); fgets(copia.nome, size_nome, stdin);
                break;
            case 2:
                printf("Digite o novo CPF: ");
                do{
                    scanf("%lf", &copia.CPF);
                    if(!cpf_valido(copia.CPF))
                        printf("CPF invalido, digite novamente\n");
                    else if(busca_registro_cpf(*Lista, copia.CPF, &aux) != NULL);
                        printf("CPF ja registrado. Escolha outro\n");
                }while(!cpf_valido(copia.CPF) || busca_registro_cpf(*Lista, copia.CPF, &aux) != NULL);
                break;
            case 3:
                printf("\n  Operacoes:\n");
                printf("1- Adicionar\n");
                printf("2- Alterar existente\n");
                printf("3- Remover existente\n");
                printf("\n0- Cancelar\n");
                printf("Sua opcao: ");
                scanf("%d", &aux1);
                do{
                    if(aux1 < 0 && aux1 > 3)
                        printf("Erro: opcao invalida. Tente novamente: ");
                }while(aux1 < 0 && aux1 > 3);
                if(aux1 == 1) {
                    if(copia.qtd_numeros < 5) {
                        scanf("%lf", &copia.numeros[copia.qtd_numeros]);
                        copia.qtd_numeros++;
                    } else
                        printf("Erro: voce nao pode adicionar mais que 5 numeros");
                } else
                        if(op != 0) {
                            printf("\n Numeros de telefone: \n");
                            for(i = 0; i < copia.qtd_numeros; i++){
                                printf("%d: %.0lf\n", i+1, copia.numeros[i]);
                            }
                            printf("\nNumero a ser alterado (escolha a opcao correspondente): ");
                            scanf("%d", &aux);
                            if(aux-1 < 0 || aux-1 > copia.qtd_numeros){
                                printf("Numero invalido\n");
                            } else {
                                if(aux1 == 2) {
                                    printf("Digite o novo numero: ");
                                    scanf("%lf", &(copia.numeros[aux-1]));
                                } else
                                    if(aux1 == 3) {
                                        if(copia.qtd_numeros > 1) {
                                            for(; aux < 4; aux++)
                                                copia.numeros[i] = copia.numeros[i+1];
                                            copia.qtd_numeros--;
                                        } else
                                            printf("Erro: voce nao pode apagar o unico contato do cliente\n");
                                    }
                            }
                        }
                break;
            case 0:
                no_copia = struct_para_no(copia);
                remove_registro(no_alterado, Lista);
                adicionar_registro(Lista, no_copia, &auxiliar_boolean);
                break;
            case -1:
                break;
        }
    }while(op != 0 && op != -1);
}

//Se organiza_cpf = true, exibe in-ordem por CPF, se não, exibe por nome (vai dar treta isso)
void exibe_in_ordem(no_registro *p, bool organiza_cpf) {
    if(organiza_cpf) {
        if(p != NULL) {
            exibe_in_ordem(p->esq, organiza_cpf);
            exibe_registro(p->cadastro);
            printf("----\n");
            exibe_in_ordem(p->dir, organiza_cpf);
        }
    } else {
        linked_list *lista = NULL;
        tree_to_linked(p, &lista);
        exibe_linked(lista);
        linked_wipe(&lista);
    }
}

//Função simples para exibir o conteúdo da struct registro
void exibe_registro(s_registro registro) {
    int i;
    printf("Nome: %s", registro.nome);
    printf("CPF: %011.0lf\n", registro.CPF);
    for(i = 0; i < registro.qtd_numeros; i++)
        printf("%dº numero para contato: %.0lf\n", i+1, registro.numeros[i]);
}

//Busca registro com CPF "cpf" na árvore "tree"
no_registro *busca_registro_cpf(no_registro *tree, double cpf, int *profundidade) {
    if(tree == NULL) {
        return NULL; // registro não encontrado
    } else 
        if(cpf < tree->cadastro.CPF) {
            (*profundidade)++;
            return busca_registro_cpf(tree->esq, cpf,  profundidade);
        } else
            if(cpf > tree->cadastro.CPF) {
                (*profundidade)++;
                return busca_registro_cpf(tree->dir, cpf,  profundidade);
            } else {
                return tree; // registro encontrado
            }
}

//Busca registro com parte da string "nome" no campo cadastro.nome na árvore tree (aux é uma cópia de tree)
int busca_registro_nome(no_registro *tree, no_registro *aux, char nome[], struct timeval before) {
    int ocorrencias = 0;
    if(aux != NULL) {
        if(strstr(aux->cadastro.nome, nome) != NULL){
            struct timeval after;
            gettimeofday(&after, NULL);

            exibe_registro(aux->cadastro);
            printf("Tempo gasto durante a busca (microssegundos): %lf\n", time_diff(before, after));
            printf("Profundidade: %d", altura(tree, aux, 1));
            printf("\n----\n");

            ocorrencias++;
        }
        ocorrencias += busca_registro_nome(tree, aux->esq, nome, before);
        ocorrencias += busca_registro_nome(tree, aux->dir, nome, before);
        return ocorrencias;
    }
    return 0;
}

//Busca registro com numero "numero" na árvore tree (aux é cópia de tree)
int busca_registro_numero(no_registro *tree, no_registro *aux, double numero, struct timeval before){
    int i, ocorrencias = 0;
    if(aux != NULL) {
        for(i = 0; i < 5; i++){
            if(i == aux->cadastro.qtd_numeros)
                break;
            if(aux->cadastro.numeros[i] == numero){
                struct timeval after;
                gettimeofday(&after, NULL);

                exibe_registro(aux->cadastro);
                printf("Tempo gasto durante a busca (microssegundos): %lf\n", time_diff(before, after));
                printf("Profundidade: %d", altura(tree, aux, 1));
                printf("\n----\n");

                ocorrencias++;
            }
        }
        ocorrencias += busca_registro_numero(tree, aux->esq, numero, before);
        ocorrencias += busca_registro_numero(tree, aux->dir, numero, before);
        return ocorrencias;
    }
    return 0;
}

/* FUNÇÕES PARA TRABALHAR A LISTA LIGADA */
/* Exibindo em ordem alfabetica: tree_to_linked, exibe_linked, linked_wipe */

//Joga os elementos da árvore binária de busca tree na lista ligada ordenada lista
void tree_to_linked(no_registro *tree, linked_list **lista) {
    if (tree != NULL) {
        tree_to_linked(tree->esq, lista);
        linked_adiciona(tree, lista);
        tree_to_linked(tree->dir, lista);
    }
}

//Adiciona novo_no na lista ligada ordenada lista
int linked_adiciona(no_registro *novo_no, linked_list **lista) {
    linked_list *aux = *lista;
    linked_list *anterior = NULL;
    
    linked_list *new_node = (linked_list *)malloc(sizeof(linked_list));
    if(new_node == NULL)
        return -1;
    new_node->registro = novo_no;
    new_node->prox = NULL;

    if(*lista == NULL) {
        *lista = new_node;
        (*lista)->prox = NULL;
        return 0;
    } else {
        while((aux != NULL) && strcmp(aux->registro->cadastro.nome, new_node->registro->cadastro.nome) < 0){
            anterior = aux;
            aux = aux->prox;
        }
        if(aux == NULL) {
            anterior->prox = new_node;
            new_node->prox = NULL;
        } else {
            if(anterior == NULL) {
                *lista = new_node;
                new_node->prox = aux;
            }
            else{
                anterior->prox = new_node;
                new_node->prox = aux;
            }
        }
        return 0;
    }
}

//Apaga a lista ligada (para liberar memória)
int linked_wipe(linked_list **lista) {
    if(*lista == NULL)
        return -1;

    linked_list *aux = *lista;
    linked_list *proximo;

    while(aux != NULL){
        proximo = aux->prox;
        free(aux);
        aux = proximo;
    }

    return 0;

    *lista = NULL;
}

//Exibe a lista ligada
int exibe_linked(linked_list *lista) {
    if(lista == NULL)
        return -1;
    else{
        while(lista != NULL){
            exibe_registro(lista->registro->cadastro);
            if(lista->prox != NULL) 
                printf("\t --- \n");
            lista = lista->prox;
        }
        return 0;
    }
}

/* Funções auxiliares */
//Verificador de CPF
bool cpf_valido(double cpf) {
    //return true; //remover no release final:
    int sm=0, i, r, num;
    char dig10, dig11, cpfchar[12];
    snprintf(cpfchar, 12, "%011lf", cpf);

    if(cpf - (long long)cpf != 0) //Apenas números são aceitos; número decimal é inválido
        return false;

    
    /* fonte: http://www.pb.utfpr.edu.br/omero/C/Exercicios/B/B8.Htm */
    // calcula o 1º. digito verificador do CPF
    for (i=0; i<9; i++) {
    num = cpfchar[i] - 48;  // transforma o caracter '0' no inteiro 0
                        // (48 eh a posição de '0' na tabela ASCII)
    sm = sm + (num * (10 - i));  
    }
    r = 11 - (sm % 11);
    if ((r == 10) || (r == 11))
        dig10 = '0';
    else
        dig10 = r + 48;

    // calcula o 2º. digito verificador do CPF
    sm = 0;
    for (i=0; i<10; i++) {
        num = cpfchar[i] - 48;
        sm = sm + (num * (11 - i));
    }
    r = 11 - (sm % 11);  
    if ((r == 10) || (r == 11))
        dig11 = '0';
    else
        dig11 = r + 48;

    if ((dig10 == cpfchar[9]) && (dig11 == cpfchar[10]))
        return true;
    else
        return false;
}

//Tornar as primeiras letras maiúsculas
void capitalizing(char string[]) {
    int i;
    int x = strlen(string);
    if(isalpha(string[0]))
        string[0] = toupper(string[0]);
    for (i=1;i<x;i++)
         if (isalpha(string[i]) && string[i-1] == ' ')
             string[i]= toupper(string[i]);
}

//Calcula a diferença de tempo com precisão de microsegundos
double time_diff(struct timeval x , struct timeval y) {
    double x_ms , y_ms , diff;
     
    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;
     
    diff = (double)y_ms - (double)x_ms;
     
    return diff;
}

//Retorna a altura do no_procurado na árvore binária de busca tree
int altura(no_registro *tree, no_registro *no_procurado, int depth){
    if(tree == NULL)
        return 0;
    else if(tree == no_procurado)
        return depth;
    else{
        if(no_procurado->cadastro.CPF > tree->cadastro.CPF)
            return altura(tree->dir, no_procurado, depth) + 1;
        else
            return altura(tree->esq, no_procurado, depth) + 1;
    }
}

//Sem uso para o usuário final; útil para debuggar o programa
void debugger_exibe_arvore(no_registro *tree, no_registro *tree_original){
    if(tree != NULL){
        printf("Altura: %d\n", altura(tree_original, tree, 1));
        printf("Endereco: %p\n", (void *) tree);
        printf("Endereco (struct): %p\n", (void *) &(tree->cadastro));
        printf("Nome: %s", tree->cadastro.nome);
        printf("CPF: %lf\n", tree->cadastro.CPF);
        printf("Endereco->esq: %p\n", (void *) (tree->esq));
        printf("Endereco->prox: %p\n\n", (void *) (tree->dir));
        debugger_exibe_arvore(tree->esq, tree_original);
        debugger_exibe_arvore(tree->dir, tree_original);
    }
}