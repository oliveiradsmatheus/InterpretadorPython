struct funcoes {
	char nome[45];
	Lista *inicio;
	struct funcoes *prox;
};
typedef struct funcoes Funcoes;

int whatsIt(Token *T,Pilha *P,Funcoes *F) {
	if(strcmp(T->NomeToken,"def")==0)
		return 0;

	while(P!=NULL && strcmp(P->conteudo.nomeVar,T->NomeToken)!=0)
		P=P->prox;
	if(P!=NULL)
		return 1; // Busco uma variavel com o mesmo nome

	while(F!=NULL && strcmp(F->nome,T->NomeToken)!=0)
		F=F->prox;
	if(F!=NULL)
		return 2; // Busco uma função com o mesmo nome

	if(strcmp(T->NomeToken,"if")==0 ||
	        strcmp(T->NomeToken,"while")==0 ||
	        strcmp(T->NomeToken,"for")==0 ||
	        strcmp(T->NomeToken,"elif")==0 ||
	        strcmp(T->NomeToken,"else")==0 ||
	        strcmp(T->NomeToken,"do")==0 ||
	        strcmp(T->NomeToken,"switch")==0 ||
	        strcmp(T->NomeToken,"print")==0)
		return 3;// Se for função do sistema
	return 10;// Se for criação de variavel
}

void CriaListaFuncao (Funcoes **F, Lista *L) {
	Funcoes * aux, *ultimo;
	Token *tAux;

	*F = NULL;
	while(L!=NULL) {
		if(strcmp(L->pToken->NomeToken,"def")==0) {
			aux = (Funcoes*)malloc(sizeof(Funcoes));
			tAux = L->pToken->prox;
			strcpy(aux->nome,tAux->NomeToken);
			aux->inicio = L;
			aux->prox = NULL;
			if(!(*F)) {
				*F = aux;
				ultimo = *F;
			} else {
				ultimo->prox = aux;
				ultimo = aux;
			}
		}
		L=L->prox;
	}
}

int resolveIf(Token *T, Pilha *P) {
	Pilha *aux = P, *aux2=P;
	int flag;
	Token *prim,*seg,*terc;
	T = T->prox;
	prim=T;
	char op[3],valor[100];

	aux = (Pilha*)malloc(sizeof(Pilha));
	aux2 = (Pilha*)malloc(sizeof(Pilha));


	if(prim->NomeToken[0] >= '0' && prim->NomeToken[0] >= '9') {
		aux->conteudo.val.variavel.fl = atof(prim->NomeToken);
		aux->conteudo.val.flag = 1;
	} else
		while(aux!=NULL && strcmp(T->NomeToken,aux->conteudo.nomeVar)!=0)
			aux=aux->prox;
			
	T=T->prox;
	seg=T;
	strcpy(op,T->NomeToken);
	if(T->prox->NomeToken[0] == '=') {
		T = T->prox;
		strcat(op,T->NomeToken);
	}

	if(seg->NomeToken[0] >= '0' && seg->NomeToken[0] >= '9') {
		aux2->conteudo.val.variavel.fl = atof(seg->NomeToken);
		aux2->conteudo.val.flag = 1;
	} else
		while(aux2 && strcmp(T->NomeToken,aux2->conteudo.nomeVar)!=0)
			aux2=aux2->prox;
	
	T=T->prox;
	terc=T;

	if(aux->conteudo.val.flag == 0 || aux->conteudo.val.flag == 2)
		aux->conteudo.val.flag = 1;

	if(aux!=NULL && aux2!=NULL) {
		if(strcmp(op,">")==0) {
			flag = aux->conteudo.val.flag;
			if(flag==0)
				return aux->conteudo.val.variavel.in > aux2->conteudo.val.variavel.in;
			if(flag==1)
				return aux->conteudo.val.variavel.fl > aux2->conteudo.val.variavel.fl;
			if(flag==2)
				return aux->conteudo.val.variavel.db > aux2->conteudo.val.variavel.db;
			if(flag==3)
				return aux->conteudo.val.variavel.ch > aux2->conteudo.val.variavel.ch;
			if(flag==4)
				return strcmp(aux->conteudo.val.variavel.str, aux2->conteudo.val.variavel.str) > 0;
			else if(strcmp(op,"<")==0) {
				flag = aux->conteudo.val.flag;
				if(flag==0)
					return aux->conteudo.val.variavel.in < aux2->conteudo.val.variavel.in;
				if(flag==1)
					return aux->conteudo.val.variavel.fl < aux2->conteudo.val.variavel.fl;
				if(flag==2)
					return aux->conteudo.val.variavel.db < aux2->conteudo.val.variavel.db;
				if(flag==3)
					return aux->conteudo.val.variavel.ch < aux2->conteudo.val.variavel.ch;
				if(flag==4)
					return strcmp(aux->conteudo.val.variavel.str, aux2->conteudo.val.variavel.str) < 0;
			} else if(strcmp(op,">=")==0) {
				flag = aux->conteudo.val.flag;
				if(flag==0)
					return aux->conteudo.val.variavel.in >= aux2->conteudo.val.variavel.in;
				if(flag==1)
					return aux->conteudo.val.variavel.fl >= aux2->conteudo.val.variavel.fl;
				if(flag==2)
					return aux->conteudo.val.variavel.db >= aux2->conteudo.val.variavel.db;
				if(flag==3)
					return aux->conteudo.val.variavel.ch >= aux2->conteudo.val.variavel.ch;
				if(flag==4)
					return strcmp(aux->conteudo.val.variavel.str, aux2->conteudo.val.variavel.str) >= 0;
			} else if(strcmp(op,"<=")==0) {
				flag = aux->conteudo.val.flag;
				if(flag==0)
					return aux->conteudo.val.variavel.in <= aux2->conteudo.val.variavel.in;
				if(flag==1)
					return aux->conteudo.val.variavel.fl <= aux2->conteudo.val.variavel.fl;
				if(flag==2)
					return aux->conteudo.val.variavel.db <= aux2->conteudo.val.variavel.db;
				if(flag==3)
					return aux->conteudo.val.variavel.ch <= aux2->conteudo.val.variavel.ch;
				if(flag==4)
					return strcmp(aux->conteudo.val.variavel.str, aux2->conteudo.val.variavel.str) <= 0;
			} else if(strcmp(op,"==")==0) {
				flag = aux->conteudo.val.flag;
				if(flag==0)
					return aux->conteudo.val.variavel.in == aux2->conteudo.val.variavel.in;
				if(flag==1)
					return aux->conteudo.val.variavel.fl == aux2->conteudo.val.variavel.fl;
				if(flag==2)
					return aux->conteudo.val.variavel.db == aux2->conteudo.val.variavel.db;
				if(flag==3)
					return aux->conteudo.val.variavel.ch == aux2->conteudo.val.variavel.ch;
				if(flag==4)
					return strcmp(aux->conteudo.val.variavel.str, aux2->conteudo.val.variavel.str) == 0;
			}
		}
	} else {
		if(aux==NULL) {
			if(aux2==NULL) {
				if(strcmp(op,"==") == 0) {
					return strcmp(prim->NomeToken,terc->NomeToken) == 0;
				} else if(strcmp(op,">") == 0) {
					return strcmp(prim->NomeToken,terc->NomeToken) > 0;
				} else if(strcmp(op,"<") == 0) {
					return strcmp(prim->NomeToken,terc->NomeToken) < 0;
				} else if(strcmp(op,">=") == 0) {
					return strcmp(prim->NomeToken,terc->NomeToken) >= 0;
				} else if(strcmp(op,"<=") == 0) {
					return strcmp(prim->NomeToken,terc->NomeToken) <= 0;
				}

			} else {
				flag = aux2->conteudo.val.flag;
				if(flag == 4) {
					if(strcmp(op,"==") == 0) {
						return strcmp(aux2->conteudo.val.variavel.str,prim->NomeToken) == 0;
					} else if(strcmp(op,">") == 0) {
						return strcmp(aux2->conteudo.val.variavel.str,prim->NomeToken) > 0;
					} else if(strcmp(op,"<") == 0) {
						return strcmp(aux2->conteudo.val.variavel.str,prim->NomeToken) < 0;
					} else if(strcmp(op,">=") == 0) {
						return strcmp(aux2->conteudo.val.variavel.str,prim->NomeToken) >= 0;
					} else if(strcmp(op,"<=") == 0) {
						return strcmp(aux2->conteudo.val.variavel.str,prim->NomeToken) <= 0;
					}
				} else {
					if(flag==0) {
						sprintf(valor,"%d",aux2->conteudo.val.variavel.in);
						if(strcmp(op,"==") == 0) {
							return strcmp(valor,prim->NomeToken) == 0;
						} else if(strcmp(op,">") == 0) {
							return strcmp(valor,prim->NomeToken) > 0;
						} else if(strcmp(op,"<") == 0) {
							return strcmp(valor,prim->NomeToken) < 0;
						} else if(strcmp(op,">=") == 0) {
							return strcmp(valor,prim->NomeToken) >= 0;
						} else if(strcmp(op,"<=") == 0) {
							return strcmp(valor,prim->NomeToken) <= 0;
						}
					} else if(flag ==1) {
						sprintf(valor,"%f",aux2->conteudo.val.variavel.fl);
						if(strcmp(op,"==") == 0) {
							return strcmp(valor,prim->NomeToken) == 0;
						} else if(strcmp(op,">") == 0) {
							return strcmp(valor,prim->NomeToken) > 0;
						} else if(strcmp(op,"<") == 0) {
							return strcmp(valor,prim->NomeToken) < 0;
						} else if(strcmp(op,">=") == 0) {
							return strcmp(valor,prim->NomeToken) >= 0;
						} else if(strcmp(op,"<=") == 0) {
							return strcmp(valor,prim->NomeToken) <= 0;
						}
					} else if(flag==2) {
						sprintf(valor,"%lf",aux2->conteudo.val.variavel.db);
						if(strcmp(op,"==") == 0) {
							return strcmp(valor,prim->NomeToken) == 0;
						} else if(strcmp(op,">") == 0) {
							return strcmp(valor,prim->NomeToken) > 0;
						} else if(strcmp(op,"<") == 0) {
							return strcmp(valor,prim->NomeToken) < 0;
						} else if(strcmp(op,">=") == 0) {
							return strcmp(valor,prim->NomeToken) >= 0;
						} else if(strcmp(op,"<=") == 0) {
							return strcmp(valor,prim->NomeToken) <= 0;
						}
					} else if(flag==3) {
						sprintf(valor,"%c",aux2->conteudo.val.variavel.ch);
						if(strcmp(op,"==") == 0) {
							return strcmp(valor,prim->NomeToken) == 0;
						} else if(strcmp(op,">") == 0) {
							return strcmp(valor,prim->NomeToken) > 0;
						} else if(strcmp(op,"<") == 0) {
							return strcmp(valor,prim->NomeToken) < 0;
						} else if(strcmp(op,">=") == 0) {
							return strcmp(valor,prim->NomeToken) >= 0;
						} else if(strcmp(op,"<=") == 0) {
							return strcmp(valor,prim->NomeToken) <= 0;
						}
					}

				}

			}
		} else {
			flag = aux->conteudo.val.flag;
			if(flag == 4) {
				if(strcmp(op,"==") == 0) {
					return strcmp(aux->conteudo.val.variavel.str,terc->NomeToken) == 0;
				} else if(strcmp(op,">") == 0) {
					return strcmp(aux->conteudo.val.variavel.str,terc->NomeToken) > 0;
				} else if(strcmp(op,"<") == 0) {
					return strcmp(aux->conteudo.val.variavel.str,terc->NomeToken) < 0;
				} else if(strcmp(op,">=") == 0) {
					return strcmp(aux->conteudo.val.variavel.str,terc->NomeToken) >= 0;
				} else if(strcmp(op,"<=") == 0) {
					return strcmp(aux->conteudo.val.variavel.str,terc->NomeToken) <= 0;
				}
			} else {
				if(flag==0) {
					sprintf(valor,"%d",aux->conteudo.val.variavel.in);
					if(strcmp(op,"==") == 0) {
						return strcmp(valor,terc->NomeToken) == 0;
					} else if(strcmp(op,">") == 0) {
						return strcmp(valor,terc->NomeToken) > 0;
					} else if(strcmp(op,"<") == 0) {
						return strcmp(valor,terc->NomeToken) < 0;
					} else if(strcmp(op,">=") == 0) {
						return strcmp(valor,terc->NomeToken) >= 0;
					} else if(strcmp(op,"<=") == 0) {
						return strcmp(valor,terc->NomeToken) <= 0;
					}
				} else if(flag ==1) {
					sprintf(valor,"%f",aux->conteudo.val.variavel.fl);
					if(strcmp(op,"==") == 0) {
						return strcmp(valor,terc->NomeToken) == 0;
					} else if(strcmp(op,">") == 0) {
						return strcmp(valor,terc->NomeToken) > 0;
					} else if(strcmp(op,"<") == 0) {
						return strcmp(valor,terc->NomeToken) < 0;
					} else if(strcmp(op,">=") == 0) {
						return strcmp(valor,terc->NomeToken) >= 0;
					} else if(strcmp(op,"<=") == 0) {
						return strcmp(valor,terc->NomeToken) <= 0;
					}
				} else if(flag==2) {
					sprintf(valor,"%lf",aux->conteudo.val.variavel.db);
					if(strcmp(op,"==") == 0) {
						return strcmp(valor,terc->NomeToken) == 0;
					} else if(strcmp(op,">") == 0) {
						return strcmp(valor,terc->NomeToken) > 0;
					} else if(strcmp(op,"<") == 0) {
						return strcmp(valor,terc->NomeToken) < 0;
					} else if(strcmp(op,">=") == 0) {
						return strcmp(valor,terc->NomeToken) >= 0;
					} else if(strcmp(op,"<=") == 0) {
						return strcmp(valor,terc->NomeToken) <= 0;
					}
				} else if(flag==3) {
					sprintf(valor,"%c",aux->conteudo.val.variavel.ch);
					if(strcmp(op,"==") == 0) {
						return strcmp(valor,terc->NomeToken) == 0;
					} else if(strcmp(op,">") == 0) {
						return strcmp(valor,terc->NomeToken) > 0;
					} else if(strcmp(op,"<") == 0) {
						return strcmp(valor,terc->NomeToken) < 0;
					} else if(strcmp(op,">=") == 0) {
						return strcmp(valor,terc->NomeToken) >= 0;
					} else if(strcmp(op,"<=") == 0) {
						return strcmp(valor,terc->NomeToken) <= 0;
					}
				}
			}
		}
	}
}

// Funcão de teste de exibição de funções
void exibirFuncoes (Funcoes *F) {
	int c = 42, l = 12;
	while(F) {
		gotoxy(c,l);
		printf("%s",F->nome);
		l++;
		F = F->prox;
	}
}
