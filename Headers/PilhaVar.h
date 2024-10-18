union vari {
	float fl; // Flag = [int = 0, float = 1, double = 2]
	char ch; // Flag = 3
	char str[100]; // Flag = 4
	Lista *pont; // Flag = 5
	// Caso a Flag seja 6 significa que a variável não possui valor atribuido.
};

struct valor {
	char flag;
	union vari variavel;
};
typedef struct valor Valor;

struct conteudo {
	char nomeVar[45];
	Valor val;
};
typedef struct conteudo Conteudo;

struct pilha {
	struct pilha *prox, *ant;
	Conteudo conteudo;
};
typedef struct pilha Pilha;

void NCVar(char nome[45], Pilha **P) {
	Pilha *aux = (Pilha*)malloc(sizeof(Pilha));

	aux->prox = *P;
	aux->ant = NULL;
	strcpy(aux->conteudo.nomeVar,nome);
	aux->conteudo.val.flag = 6; // Deixando a variável com tipo indefinido
	if(*P)
		(*P)->ant = aux;
	*P = aux;
}

Valor BuscaVariavel (Token *Var, Pilha *Pilha) {
	Valor ValorVar;
	ValorVar.flag = -1;
	char flag = 0;

	while(Pilha && !flag) {
		if(!strcmp(Var->NomeToken,Pilha->conteudo.nomeVar)) {
			ValorVar = Pilha->conteudo.val;
			flag = 1;
		}
		Pilha = Pilha->prox;
	}
	return ValorVar;
}

void CriaVariavel (Token *Tok, Pilha **pVar) {
	Pilha *P;
	Token *T;
	char expressao[100], string[100], flag = 0;

	expressao[0]='\0';
	NCVar(Tok->NomeToken,&(*pVar));

	P = *pVar;

	if(Tok->prox && !strcmp(Tok->prox->NomeToken,"="))
		if(Tok->prox->prox->NomeToken[0] >= '0' && Tok->prox->prox->NomeToken[0] <= '9') {
			T = Tok->prox->prox;
			while(T) {
				strcat(expressao,T->NomeToken);
				T = T->prox;
			}
			P->conteudo.val.flag = 1;
			P->conteudo.val.variavel.fl = resolveExpressao(expressao);
		} else {
			P->conteudo.val.flag = 4;
			strcpy(string,"");
			Tok = Tok->prox->prox->prox;
			strcat(string,Tok->NomeToken);
			Tok = Tok->prox;
			while(Tok && (Tok->NomeToken[0] != 39 && Tok->NomeToken[0] != 34)) {
				strcat(string," ");
				strcat(string,Tok->NomeToken);
				Tok = Tok->prox;
			}
			strcpy(P->conteudo.val.variavel.str,string);
		}
}

char SeVariavel (Token *T, Pilha *PilhaVar) {
	while(PilhaVar && strcmp(T->NomeToken,PilhaVar->conteudo.nomeVar))
		PilhaVar = PilhaVar->prox;
	if(PilhaVar)
		return 1;
	return 0;
}

void AtribuiValor (Token *Tok, Pilha **pVar) {
	Pilha *P;
	Token *T;
	Valor V;
	char expressao[100], string[100], valor[100];

	expressao[0]='\0';
	P = *pVar;

	while(P && strcmp(P->conteudo.nomeVar,Tok->NomeToken))
		P = P->prox;

	if(Tok->prox && !strcmp(Tok->prox->NomeToken,"="))
		if((Tok->prox->prox->NomeToken[0] >= '0' && Tok->prox->prox->NomeToken[0] <= '9') || (SeVariavel(Tok->prox->prox->NomeToken,*pVar))) {
			T = Tok->prox->prox;
			while(T) {
				if(SeVariavel(T,*pVar)) { //RESOLVER
					V = BuscaVariavel(T,*pVar);
					sprintf(valor,"%f",V.variavel.fl);
					strcat(expressao,valor);
				} else
					strcat(expressao,T->NomeToken);
				T = T->prox;
			}
			P->conteudo.val.flag = 1;
			P->conteudo.val.variavel.fl = resolveExpressao(expressao);
		} else {
			P->conteudo.val.flag = 4;
			strcpy(string,"");
			Tok = Tok->prox->prox->prox;
			strcat(string,Tok->NomeToken);
			Tok = Tok->prox;
			while(Tok && (Tok->NomeToken[0] != 39 && Tok->NomeToken[0] != 34)) {
				strcat(string," ");
				strcat(string,Tok->NomeToken);
				Tok = Tok->prox;
			}
			strcpy(P->conteudo.val.variavel.str,string);
		}
}
