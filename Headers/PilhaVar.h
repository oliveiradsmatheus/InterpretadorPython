union vari {
	int in; // Flag = 0
	float fl; // Flag = 1
	double db; // Flag = 2
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

void CriaVariavel(char nome[45], Pilha **P) {
	Pilha *aux = (Pilha*)malloc(sizeof(Pilha));
	
	aux->prox = *P;
	aux->ant = NULL;
	strcpy(aux->conteudo.nomeVar,nome);
	aux->conteudo.val.flag = 6; // Deixando a variável com tipo indefinido
	if(*P)
		(*P)->ant = aux;
	*P = aux;
}
