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

Pilha *NCVar(char nome[45]) {
	Pilha *Nova = (Pilha*)malloc(sizeof(Pilha));

	Nova->prox = NULL;
	Nova->ant = NULL;
	strcpy(Nova->conteudo.nomeVar,nome);
	Nova->conteudo.val.flag = 6; // Deixando a variável com tipo indefinido

	return Nova;
}

Valor BuscaVariavel (Token *Var, Pilha *Pilha) {
	Valor ValorVar;
	ValorVar.flag = -1;
	char flag = 0;

	while(Pilha && !flag) {
		if(!strcmp(Var->NomeToken, Pilha->conteudo.nomeVar)) {
			ValorVar = Pilha->conteudo.val;
			flag = 1;
		}
		Pilha = Pilha->prox;
	}
	return ValorVar;
}

char SeVariavel (char *NomeVar, Pilha *PilhaVar) {
	while(PilhaVar && strcmp(NomeVar,PilhaVar->conteudo.nomeVar))
		PilhaVar = PilhaVar->prox;
	if(PilhaVar)
		return 1;
	return 0;
}

void CriaVariavel (Token *Tok, Pilha **pVar) {
	Pilha *NovaVar, *aux;
	Token *T;
	Valor V;
	char expressao[100] ="", string[100], valor[100];

	NovaVar = NCVar(Tok->NomeToken);

	if(Tok->prox && !strcmp(Tok->prox->NomeToken,"="))
		T = Tok->prox->prox;
	if((T->NomeToken[0] >= '0' && T->NomeToken[0] <= '9') || (SeVariavel(T->NomeToken,*pVar))) {
		while(T) {
			if(SeVariavel(T->NomeToken,*pVar)) {
				V = BuscaVariavel(T,*pVar);
				sprintf(valor,"%f",V.variavel.fl);
				strcat(expressao,valor);
			} else
				strcat(expressao,T->NomeToken);
			T = T->prox;
		}
		NovaVar->conteudo.val.flag = 1;
		NovaVar->conteudo.val.variavel.fl = ResolveExpressao(expressao);
	} else {
		NovaVar->conteudo.val.flag = 4;
		strcpy(string,"");
		Tok = Tok->prox->prox;
		strcat(string,Tok->NomeToken);
		Tok = Tok->prox;
		while(Tok && (Tok->NomeToken[0] != 39 && Tok->NomeToken[0] != 34)) {
			strcat(string," ");
			strcat(string,Tok->NomeToken);
			Tok = Tok->prox;
		}
		strcpy(NovaVar->conteudo.val.variavel.str,string);
	}

	if(!(*pVar))
		*pVar = NovaVar;
	else {
		aux = *pVar;
		while(aux->prox)
			aux = aux->prox;
		NovaVar->ant = aux;
		aux->prox = NovaVar;
	}
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
		T = Tok->prox->prox;
	if((T->NomeToken[0] >= '0' && T->NomeToken[0] <= '9') || (SeVariavel(T->NomeToken,*pVar))) {
		while(T) {
			if(SeVariavel(T->NomeToken,*pVar)) {
				V = BuscaVariavel(T,*pVar);
				sprintf(valor,"%f",V.variavel.fl);
				strcat(expressao,valor);
			} else
				strcat(expressao,T->NomeToken);
			T = T->prox;
		}
		P->conteudo.val.flag = 1;
		P->conteudo.val.variavel.fl = ResolveExpressao(expressao);
	} else {
		P->conteudo.val.flag = 4;
		strcpy(string,"");
		Tok = Tok->prox->prox;
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
