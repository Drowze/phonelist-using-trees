/*
Integrante 1 - Nome: Leonardo Saito		RA: 
Integrante 2 - Nome: Rafael Gibim		RA: 14081673
Integrante 3 - Nome: Rodrigo Groot		RA: 
Integrante 4 - Nome: Vinicius Nunes		RA: 
Resultados obtidos:
Projeto básico: _____ % concluído - Obs: ___________________________________
( ) Opcional 1 - Obs: ____________________________________________________
( ) Opcional 2 - Obs: ____________________________________________________
( ) Opcional 3 - Obs: ____________________________________________________
( ) Opcional 4 - Obs: ____________________________________________________
( ) Opcional 5 - Obs: ____________________________________________________
( ) Opcional 6 - Obs: ____________________________________________________ 
*/


#include <stdio_ext.h> //consigo usar __fpurge(stdin) sem warnings (warning acusado usando gcc -wall)
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "lib_functions.h"

#define MSG_CPFNAOENCONTRADO "Erro: CPF nao encontrado\n\n"
#define MSG_CPFINVALIDO "Erro: CPF invalido\n\n"


int main(){
	char op;

	no_registro *tree = le_arquivo();
	no_registro *no = NULL;
	linked_list *lista;

	bool pertence;

	double cpf;
	int profundidade = 0;

    struct timeval before;
    struct timeval after;
    double time_elapsed;

    char nome_parcial[size_nome];
    double numero_contato;

	do{
		puts("\nDigite a opcao:");
		puts("1- Inserir cadastro");
		puts("2- Remover cadastro");
		puts("3- Alterar cadastro");
		puts("4- Buscar cadastro"); 
		puts("5- Exibir cadastros"); 
		puts("6- Exibir arvore de cadastros (NAO IMPLEMENTADO)");
		puts("\n0- Sair");
		scanf("%c", &op);

		switch(op){
			case '1':
				adicionar_registro(&tree, cria_no(), &pertence);
				break;
			case '2':
				printf("\nDigite o CPF a ser removido: ");
				scanf("%lf",&cpf);
				if(cpf_valido(cpf)){
					no = busca_registro_cpf(tree, cpf, &profundidade);
					if(no != NULL){
						remove_registro(busca_registro_cpf(tree, cpf, &profundidade), &tree);
					} else
						printf(MSG_CPFNAOENCONTRADO);
				} else
					printf(MSG_CPFINVALIDO);
				break;
			case '3':
				printf("\nDigite o CPF do cadastro a ser alterado: ");
				scanf("%lf",&cpf);
				if(cpf_valido(cpf)){
					no = busca_registro_cpf(tree, cpf, &profundidade);
					if(no != NULL){
						altera_registro (no, &tree);
					} else
						printf(MSG_CPFNAOENCONTRADO);
				}
				break;
			case '4':
                printf("\n1- Buscar por CPF");
                printf("\n2- Buscar por parte do nome");
                printf("\n3- Buscar por numero");
				printf("\nOpcao: ");
                __fpurge(stdin); scanf("%c", &op);
                switch(op){
                    case '1':
                        printf("Digite o CPF a ser buscado: ");
        				scanf("%lf", &cpf);
        				if(cpf_valido(cpf)) {
        					profundidade = 1;
        					no = busca_registro_cpf(tree, cpf, &profundidade);
        					if(no != NULL) {
                                gettimeofday(&before, NULL);
                                printf("\n\nResultado: \n");
        						exibe_registro(no->cadastro);
                                gettimeofday(&after, NULL);
                                time_elapsed = time_diff(before, after);
        						printf("Profundidade na árvore: %d\n", profundidade);
                                printf("Tempo gasto durante a busca (microssegundos): %lf\n", time_elapsed);
        					} else
        						printf("Nao foram encontrados registros\n\n");
        				} else
        					printf(MSG_CPFINVALIDO);
                        break;
                    case '2':
                        printf("Digite uma parte do nome: ");
                        __fpurge(stdin); scanf("%s", nome_parcial);
                        gettimeofday(&before, NULL);
                        printf("\n\nResultados: \n");
                        printf("\n%d ocorrencias:\n\n", busca_registro_nome(tree, tree, nome_parcial, before));


                        break;
                    case '3':
                        printf("Digite o numero de contato: ");
                        scanf("%lf", &numero_contato);
                        gettimeofday(&before, NULL);
                        printf("\n\nResultados: \n");
                        printf("\n%d ocorrencias:\n\n", busca_registro_numero(tree, tree, numero_contato, before));


                        break;
                    }
				break;
			case '5':
				printf("\n1- Ordem de CPF");
				printf("\n2- Ordem alfabetica");
				__fpurge(stdin); printf("\nOpcao: ");
				scanf("%c", &op);
				if(op == '1'){
                    printf("\n\nResultados: \n");
					exibe_in_ordem(tree, true);
				} else 
                    if (op == '2'){
                        printf("\n\nResultados: \n");
				        exibe_in_ordem(tree, false);
				    } else {
				        printf("Opcao invalida");
				    }


				
				break;
			case '9':
				debugger_exibe_arvore(tree, tree);
				break;
            case '0':
                break;
            default:
                printf("Opcao invalida\n");
                break;
		}
        __fpurge(stdin);

	}while(op != '0');
	escreve_no_arquivo(tree);
}