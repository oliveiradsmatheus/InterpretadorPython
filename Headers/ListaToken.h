//Linhas de execução
struct token {
	char NomeToken[45], TextoToken[100];
	struct token *prox;
};
typedef struct token Token;

struct lista {
	Token *pToken;
	struct lista *prox, *ant;
};
typedef struct lista Lista;

void CriaTokens(Token **pTokens, char string[100]) {
	int i = 0, j = 0;
	char aux[100];
	Token *fim;
	*pTokens = NULL;

	while (i < strlen(string) - 1) {
		j = 0;
		while (string[i] != '+' && string[i]!='>' && string[i]!='<' && string[i] != '-' && string[i] != '*' && string[i] != '/' && string[i] != ' ' &&
		        string[i] != '=' && string[i] != ':' && string[i] != '(' && string[i] != ')' && string[i] != ',' &&
		        i < strlen(string) && string[i] != 39 && string[i] != '"') {
		    aux[j++] = string[i++];
		}
		aux[j] = '\0';
		if(aux[strlen(aux)-1] == '\n')
			aux[strlen(aux)-1] = '\0';
		if (j > 0)
			if (*pTokens == NULL) {
				*pTokens = (Token *) malloc(sizeof(Token));
				(*pTokens)->prox = NULL;
				if(strcmp(string,"fim") || strcmp(string,"fimdef"))
					strcpy((*pTokens)->TextoToken,string);
				strcpy((*pTokens)->NomeToken, aux);
				fim = *pTokens;
			} else {
				fim->prox = (Token *) malloc(sizeof(Token));
				fim->prox->prox = NULL;
				strcpy(fim->prox->NomeToken, aux);
				fim = fim->prox;
			}
		if (string[i] != ' ' && string[i] != ',' && string[i] != ':' && string[i] != '\0' && i < strlen(string)) {
			aux[0] = string[i];
			aux[1] = '\0';
			fim->prox = (Token*)malloc(sizeof(Token));
			fim->prox->prox = NULL;
			if(strcmp(string,"fim") || strcmp(string,"fimdef"))
				strcpy((*pTokens)->TextoToken,string);
			strcpy(fim->prox->NomeToken, aux);
			fim = fim->prox;
		}
		i++;
	}
}

void CriaListaTokens(FILE *arq, Lista **L) {
	Lista *Aux, *Nova;
	int idenAnt=0, idenAtu=0, i;
	char string[100], def=0, func=0, fim;

	fgets(string,100,arq);
	while(!feof(arq)) {
		if(strlen(string)>1) {
			// Descobrindo se a linha é um def
			if(strlen(string)>4 && (string[0] == 'd' && string[1] == 'e' && string[2] == 'f' && string[3] == ' '))
				def++;
			i = 0;
			idenAnt = idenAtu;
			idenAtu = 0;
			while (string[i] == ' ') {
				idenAtu++;
				i++;
			}
			if (idenAnt > idenAtu) {
				if(idenAtu || (!idenAtu && !def))
					func = (idenAnt-idenAtu)/4;
				else if(!idenAtu && idenAnt>4)
					func = (idenAnt-idenAtu)/4 - 1; // Menos um porque o último é o fimdef
					
				while(func) {
					Nova = (Lista*)malloc(sizeof(Lista));
					Nova->prox = NULL;
					Nova->ant = NULL;
					CriaTokens(&Nova->pToken,"fim");
					if(!*L)
						*L = Nova;
					else {
						Aux = *L;
						while(Aux->prox)
							Aux = Aux->prox;
						Nova->ant = Aux;
						Aux->prox = Nova;
					}
					func--;
				}
				if(def && !idenAtu) {
					Nova = (Lista*)malloc(sizeof(Lista));
					Nova->prox = NULL;
					Nova->ant = NULL;
					CriaTokens(&Nova->pToken,"fimdef");
					if(!L)
						*L = Nova;
					else {
						Aux = *L;
						while(Aux->prox)
							Aux = Aux->prox;
						Nova->ant = Aux;
						Aux->prox = Nova;
					}
					def--;
				}
			}
			Nova = (Lista*)malloc(sizeof(Lista));
			Nova->prox = NULL;
			Nova->ant = NULL;
			CriaTokens(&Nova->pToken,string);
			if(!*L)
				*L = Nova;
			else {
				Aux = *L;
				while(Aux->prox)
					Aux = Aux->prox;
				Nova->ant = Aux;
				Aux->prox = Nova;
			}
		}
		fgets(string,100,arq);
	}
	while(idenAtu > 0) {
		Nova = (Lista*)malloc(sizeof(Lista));
		Nova->prox = NULL;
		Nova->ant = NULL;
		CriaTokens(&Nova->pToken,"fim");
		if(!*L)
			*L = Nova;
		else {
			Aux = *L;
			while(Aux->prox)
				Aux = Aux->prox;
			Nova->ant = Aux;
			Aux->prox = Nova;
		}
		idenAtu -= 4;
	}
}

// Função de Exibição dos Tokens para testes
void exibe(Lista *l) {
	int i = 0;
	int j = 0;
	Token *p;

	if(!l) {
		EscrMsg("LISTA VAZIA");
		getch();
	}
	while (l != NULL) {
		printf("%d ", i);
		p = l->pToken;
		j = 1;
		while (p != NULL) {
			printf("\n %d.%d %s ", i, j++, p->NomeToken);
			p = p->prox;
		}
		i++;
		printf("\n\n");
		l = l->prox;
	}
}
