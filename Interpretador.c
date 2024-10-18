#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <conio2.h>
#include <windows.h>
#include "Headers\\Mensagem.h"
#include "Headers\\Moldura.h"
#include "Headers\\ListaToken.h"
#include "Headers\\TadListaGen.h"
#include "Headers\\PilhaVar.h"
#include "Headers\\Funcoes.h"

void ExibirPrograma(Lista *L, int LinhaAtual) {
	int l=10, c=6;

	while(L) {
		if(l==(LinhaAtual+10)) {
			textbackground(14);
			textcolor(0);
		}
		LimpaLinha(l);
		gotoxy(c,l);
		while(L->prox && (!strcmp(L->pToken->NomeToken,"fimdef") || !strcmp(L->pToken->NomeToken,"fim")))
			L = L->prox;
		if(L) {
			if(strcmp(L->pToken->NomeToken,"fim")) {
				printf("%s",L->pToken->TextoToken);
				textbackground(0);
				textcolor(15);
				l++;
			}
			L = L->prox;
		}
	}
}

void ConteudoArquivo(Lista *L) {
	int l=10, c=6;

	while(L) {
		gotoxy(c,l);
		while(L->prox && (!strcmp(L->pToken->NomeToken,"fimdef") || !strcmp(L->pToken->NomeToken,"fim")))
			L = L->prox;
		if(L) {
			if(strcmp(L->pToken->NomeToken,"fim")) {
				printf("%s",L->pToken->TextoToken);
				l++;
			}
			L = L->prox;
		}
	}
}

// Algoritmo recursivo para destruir os tokens
void DestroiLinha(Token **Token) {
	if(*Token) {
		DestroiLinha(&(*Token)->prox);
		free(*Token);
		*Token = NULL;
	}
}

// Algoritmo recursivo para destruir os elementos da lista
void DestroiLista(Lista **L) {
	if(*L) {
		DestroiLinha(&(*L)->pToken);
		DestroiLista(&(*L)->prox);
		free(*L);
		*L = NULL;
	}
}

void PonteiroInicial (Lista **L, int *LinhaAtual) {
	while(*L && !strcmp((*L)->pToken->NomeToken,"def"))
		while(strcmp((*L)->pToken->NomeToken,"fimdef")) {
			if(!strcmp((*L)->pToken->NomeToken,"fim"))
				*L = (*L)->prox;
			else {
				(*L) = (*L)->prox;
				(*LinhaAtual)++;
			}
		}
	if(!strcmp((*L)->pToken->NomeToken,"fimdef"))
		*L = (*L)->prox;
}

void ExibirPilhaVar (Pilha *P) {
	int c=46, l=38, end=100;

	LimpaTelaMem();
	Moldura(45,12,118,40,9,0);
	Moldura(46,13,117,15,9,0);
	ExibirTexto((char*)"                           PILHA DE VARIAVEIS                         ",47,14,12);
	for(l=16; l<40; l++)
		ExibirTexto("            |                       |",c,l,9);
	gotoxy(48,16);
	printf("END.");
	gotoxy(61,16);
	printf("VARIAVEL");
	gotoxy(85,16);
	printf("VALOR");

	if(P) {
		while(P->prox)
			P = P->prox;
		l=39;
		c=48;
		while(P) {
			switch(P->conteudo.val.flag) {
				case 0:
					gotoxy(c,l);
					printf("%d",&(P)->conteudo.val.variavel.fl);
					gotoxy(c+13,l);
					printf("%s", P->conteudo.nomeVar);
					gotoxy(c+37,l);
					printf("%d", P->conteudo.val.variavel.fl);
					break;
				case 1:
					gotoxy(c,l);
					printf("%d",&(P)->conteudo.val.variavel.fl);
					gotoxy(c+13,l);
					printf("%s", P->conteudo.nomeVar);
					gotoxy(c+37,l);
					printf("%f", P->conteudo.val.variavel.fl);
					break;
				case 2:
					gotoxy(c,l);
					printf("%d",&(P)->conteudo.val.variavel.fl);
					gotoxy(c+13,l);
					printf("%s", P->conteudo.nomeVar);
					gotoxy(c+37,l);
					printf("%lf", P->conteudo.val.variavel.fl);
					break;
				case 3:
					gotoxy(c,l);
					printf("%p",P->conteudo.val.variavel.ch);
					gotoxy(c+13,l);
					printf("%s", P->conteudo.nomeVar);
					gotoxy(c+37,l);
					printf("%c", P->conteudo.val.variavel.ch);
					break;
				case 4:
					gotoxy(c,l);
					printf("%p",P->conteudo.val.variavel.str);
					gotoxy(c+13,l);
					printf("%s", P->conteudo.nomeVar);
					gotoxy(c+37,l);
					printf("%s", P->conteudo.val.variavel.str);
					break;
				case 5:
					gotoxy(c,l);
					printf("%p",P->conteudo.val.variavel.pont);
					gotoxy(c+13,l);
					printf("%s", P->conteudo.nomeVar);
					gotoxy(c+37,l);
					printf("%u", P->conteudo.val.variavel.pont);
					break;
				case 6:
					gotoxy(c,l);
					printf("%p",P->conteudo.val.variavel);
					gotoxy(c+13,l);
					printf("%s", P->conteudo.nomeVar);
					gotoxy(c+37,l);
					printf("UNDEFINED");
					break;
			}
			l--;
			end += 4;
			P = P->ant;
		}
	}
}

void ResolveElse (Lista **L, int *LinhaAtual, char *repeticao) {
	while(*L && strcmp((*L)->pToken->NomeToken,"fim")) {
		*L = (*L)->prox;
		(*LinhaAtual)++;
	}
	if(!strcmp((*L)->pToken->NomeToken,"fim"))
		*L = (*L)->prox;
}

void ResolveElif (Lista **L, Pilha *PilhaVar, char *condicao, int *LinhaAtual) {
	if(ComparaIf((*L)->pToken,PilhaVar)) {
		*condicao = 1;
		*L = (*L)->prox;
		(*LinhaAtual)++;
	} else {
		while(*L && strcmp((*L)->pToken->NomeToken,"fim")) {
			*L = (*L)->prox;
			(*LinhaAtual)++;
		}
		if(!strcmp((*L)->pToken->NomeToken,"fim")) {
			*L = (*L)->prox;
			if(!strcmp((*L)->pToken->NomeToken,"elif"))
				ResolveElif(&(*L),PilhaVar,&(*condicao),&(*LinhaAtual));
			else {
				if(!strcmp((*L)->pToken->NomeToken,"else")) {
					*condicao = 0;
					*L = (*L)->prox;
					(*LinhaAtual)++;
				}
			}
		}
	}
}

void ResolveIf (Lista **L, Pilha *PilhaVar, char *condicao, Lista *Linhas, int *LinhaAtual) {
	if(ComparaIf((*L)->pToken,PilhaVar)) {
		*L = (*L)->prox;
		(*LinhaAtual)++;
	} else {
		while(*L && strcmp((*L)->pToken->NomeToken,"fim")) {
			*L = (*L)->prox;
			(*LinhaAtual)++;
		}
		if(!strcmp((*L)->pToken->NomeToken,"fim")) {
			*L = (*L)->prox;
			if(!strcmp((*L)->pToken->NomeToken,"elif"))
				ResolveElif(&(*L),PilhaVar,&(*condicao),&(*LinhaAtual));
			else {
				if(!strcmp((*L)->pToken->NomeToken,"else")) {
					*condicao = 0;
					*L = (*L)->prox;
					(*LinhaAtual)++;
				}
			}
		}
	}
}

void PosicionaPont (Lista **L, int *LinhaAtual) {
	if(!strcmp((*L)->pToken->NomeToken,"elif")) {
		while((*L)->prox && strcmp((*L)->pToken->NomeToken,"fim")) {
			*L = (*L)->prox;
			(*LinhaAtual)++;
		}
		if((*L)->prox)
			*L = (*L)->prox;
		PosicionaPont(&(*L),&(*LinhaAtual));
	}
	if(!strcmp((*L)->pToken->NomeToken,"else")) {
		while((*L)->prox && strcmp((*L)->pToken->NomeToken,"fim")) {
			*L = (*L)->prox;
			(*LinhaAtual)++;
		}
		if((*L)->prox)
			*L = (*L)->prox;
	}
}


void ExecutaLinha(Lista **L, Pilha **pVar, Funcoes *Funcoes, Lista *Linhas, int Linha) {
	Pilha *P = *pVar;
	Token *T, *Tok = (*L)->pToken;
	char expressao[100], string[100], flag=0;
	
	expressao[0]='\0';
	switch(TipoFuncao(Tok,*pVar,Funcoes)) {
		case 1: // Variável já existente
			AtribuiValor(Tok,&(*pVar));
			break;
		case 2:// Funções def
			break;
		case 3: // Funções da linguagem
			if(strcmp(Tok->NomeToken,"if")==0) {
				if(ComparaIf((*L)->pToken,*pVar));
				{
					*L = (*L)->prox;
					ResolveIf(&(*L),*pVar,&flag,Linhas,&Linha);
					while(!strcpy((*L)->pToken->NomeToken,"fim"))
						*L = (*L)->prox;
				}
				getch();
			}
			/*if(strcmp(Tok->tokenName,"if")==0)
				resolveIf(Tok,pVar);
			if(strcmp(Tok->tokenName,"while")==0)
				break;
			if(strcmp(Tok->tokenName,"for")==0)
				break;
			if(strcmp(Tok->tokenName,"do")==0)
				break;*/
			break;
		case 0: // Definição de função
			break;
		default: // Criação de variável
			CriaVariavel(Tok,&(*pVar));
	}
	// Após a criação das variáveis, executa-se a função correspondente armazenando seus resultados na pilha de variáveis
}

void ExecPassos (Lista *L) {
	int LinhaAtual = 0;
	Token *Aux;
	Lista *Linhas = L;
	Pilha *PilhaVar = NULL; // Definindo a pilha de variaveis vazia
	Funcoes *Funcoes;
	char op, condicao = 0, nulo = 0;

	CriaListaFuncao(&Funcoes,L);// Listando todas as funcoes da lista
	PonteiroInicial(&L, &LinhaAtual);

	LimpaTela();
	ExibirTexto((char*)"EXECUCAO DO PROGRAMA:",6,8,14);

	do {
		LimpaTela();
		ExibirPrograma(Linhas,LinhaAtual); // Exibe o conteúdo do arquivo com efeito na linha atual .py
		LimpaMsg();
		EscrMsg((char*)"[ENTER] - PROXIMA LINHA, [F9] - CONTEUDO MEM. RAM, [F10] - EXIBIR RESULTADOS OU [ESC] PARA VOLTAR");
		EscrMsg("");
		op = getch();

		switch(op) {
			case 0:
				op = getch();
				switch(op) {
					case 67:
						LimpaMsg();
						ExibirPilhaVar(PilhaVar);
						EscrMsg((char*)"CONTEUDO DA MEMORIA RAM");
						break;
					case 68:
						LimpaMsg();
						ExibirPrint(L->pToken, PilhaVar);
				}
				getch();
				break;
			case 13: // Andando nas linhas do programa
				ExecutaLinha(&L,&PilhaVar,Funcoes,Linhas,LinhaAtual);// Executa a linha e atualiza a pilha de variáveis com resultados.
				if(L->prox) {
					L = L->prox;
					LinhaAtual++;
					// Tokens que não são Linhas de Código.
					if(!strcmp(L->pToken->NomeToken,"fimdef") || !strcmp(L->pToken->NomeToken,"fim"))
						L = L->prox;

					/*if((!strcmp(L->pToken->NomeToken,"else") || !strcmp(L->pToken->NomeToken,"elif")) && condicao) {
						condicao = 0;
						PosicionaPont(&L,&LinhaAtual);
						if(!L->prox && L->pToken->TextoToken[0] == ' ')
							op = 27;
						else
							if(!L && L->pToken->TextoToken[0] != ' ')
								op = 27;
					}
					if(!strcmp(L->pToken->NomeToken,"if") && !condicao) {
						condicao = 1;
						ResolveIf(&L,PilhaVar,&condicao,&LinhaAtual);
					}*/
				} else
					op=27;
		}
	} while(op!=27);
}

void AbrirArquivo (Lista **L) {
	FILE *arq;
	char op, endereco[100], arquivo[100];
	int i=0;

	strcpy(endereco,"Teste//");

	/*LigaCursor();
	Moldura(40,10,90,16,9,0);
	Moldura(41,11,89,13,9,0);
	ExibirTexto((char*)"DIGITE O NOME DO ARQUIVO",53,12,14);
	ExibirTexto((char*)"~ ",42,14,12);
	textcolor(15);
	gets(arquivo);
	RetiraCursor();*/
	strcpy(arquivo,"condicao.py");
	strcat(endereco,arquivo);

	//arq = fopen(endereco,"a");
	//fputc('\n',arq);
	//	fclose(arq);

	arq = fopen(endereco,"r");

	LimpaMsg();
	if(!arq) {
		EscrMsg((char*)"ARQUIVO NAO ENCONTRADO");
		getch();
	} else {
		EscrMsg("");
		printf("ARQUIVO %s ABERTO COM SUCESSO, PRESSIONE QUALQUER TECLA", arquivo);
		getch();

		CriaListaTokens(arq, &(*L)); // Cria a lista de linhas do arquivo .py.
		fclose(arq); //Fecha o arquivo e a partir daqui trabalha apenas com ponteiros.

		if(!L)
			EscrMsg((char*)"ERRO NAO ENCONTRADO!");
		else {
			LimpaTela();
			gotoxy(6,6);
			textcolor(12); 
			printf("%s",arquivo);
			textcolor(15);
			ExibirTexto((char*)"CONTEUDO DO ARQUIVO:",6,8,14);

			ConteudoArquivo(*L);

			LimpaMsg();
			EscrMsg((char*)"PRESSIONE [F8] PARA EXECUTAR PASSO A PASSO OU [ESC] PARA VOLTAR");

			do {
				op = getch();
				if(op == 0) {
					op = getch();
					if(op == 66) {
						ExecPassos(*L); // Executa passo a passo do programa
						LimpaMsg();
						EscrMsg((char*)"EXECUCAO FINALIZADA PRESSIONE [F8] PARA EXECUTAR NOVAMENTE OU [ESC] PARA SAIR");
					}
				} else if (op == 'x') {
					clrscr();
					exibe(*L);
					getch();
				}
			} while(op != 27);
		}
	}
	if(*L)
		DestroiLista(&(*L));
}

void Menu (void) {
	textcolor(14);
	ExibirTexto((char*)"TRABALHO BIMESTRAL - ESTRUTURAS DE DADOS II",40,3,14);
	LimpaMsg();
	EscrMsg((char*)"PRESSIONE [F7] PARA ABRIR O ARQUIVO .PY OU [ESC] PARA SAIR");
}

int main(void) {
	Lista *L = NULL;
	char op;

	SetConsoleTitle("Interpretador Python");
	Dimensao();
	MolduraCompleta();
	RetiraCursor();

	do {
		LimpaTelaInteira();
		Menu();
		op = getch();
		if(op == 0) {
			op = getch();
			if(op == 65)
				AbrirArquivo(&L);
		}
		LimpaTelaInteira();
	} while(op!=27);

	MoldFim();
	MsgFim();
	getch();

	return 0;
}
