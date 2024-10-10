struct funcoes {
	char nome[45];
	Lista *inicio;
	struct funcoes *prox;
};
typedef struct funcoes Funcoes;

int whatsIt(Token *T,Pilha *P,Funcoes *F) {
	if(strcmp(T->NomeToken,"def")==0)
		return 0;

	while(P && strcmp(P->conteudo.nomeVar,T->NomeToken))
		P = P->prox;
	if(P)
		return 1; // Busca uma variavel com o mesmo nome

	while(F && strcmp(F->nome,T->NomeToken))
		F = F->prox;
	if(F)
		return 2; // Busca uma função com o mesmo nome

	if(!strcmp(T->NomeToken,"if") ||
	        !strcmp(T->NomeToken,"while") ||
	        !strcmp(T->NomeToken,"for") ||
	        !strcmp(T->NomeToken,"elif") ||
	        !strcmp(T->NomeToken,"else") ||
	        !strcmp(T->NomeToken,"do") ||
	        !strcmp(T->NomeToken,"switch") ||
	        !strcmp(T->NomeToken,"print"))
		return 3;// Se for função do sistema
	return 10;// Se for criação de variavel
}

void CriaListaFuncao (Funcoes **F, Lista *L) {
	Funcoes * aux, *ultimo;
	Token *tAux;

	*F = NULL;
	while(L) {
		if(!strcmp(L->pToken->NomeToken,"def")) {
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
		L = L->prox;
	}
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

char ComparaIf(Token *T, Pilha *P) {
	Valor Valor1, Valor2;
	Token *prim, *seg, *terc;
	char flag, op[3], valor[100];

	T = T->prox;
	prim = T;

	if(prim->NomeToken[0] >= '0' && prim->NomeToken[0] <= '9') {
		Valor1.variavel.fl = atof(prim->NomeToken);
		Valor1.flag = 1;
	} else
		Valor1 = BuscaVariavel(T,P);

	T = T->prox;
	seg = T;
	strcpy(op,T->NomeToken);
	if(T->prox->NomeToken[0] == '=') {
		T = T->prox;
		strcat(op,T->NomeToken);
	}

	T = T->prox;
	terc = T;

	if(terc->NomeToken[0] >= '0' && terc->NomeToken[0] <= '9') {
		Valor2.variavel.fl = atof(terc->NomeToken);
		Valor2.flag = 1;
	} else
		Valor2 = BuscaVariavel(T,P);

	if(Valor1.flag != -1 && Valor2.flag != -1) {
		if(!strcmp(op,">")) {
			flag = Valor1.flag;
			if(flag==0)
				return Valor1.variavel.in > Valor2.variavel.in;
			if(flag==1)
				return Valor1.variavel.fl > Valor2.variavel.fl;
			if(flag==2)
				return Valor1.variavel.db > Valor2.variavel.db;
			if(flag==3)
				return Valor1.variavel.ch > Valor2.variavel.ch;
			if(flag==4)
				return strcmp(Valor1.variavel.str, Valor2.variavel.str) > 0;
		} else if(!strcmp(op,"<")) {
			flag = Valor1.flag;
			if(flag==0)
				return Valor1.variavel.in < Valor2.variavel.in;
			if(flag==1)
				return Valor1.variavel.fl < Valor2.variavel.fl;
			if(flag==2)
				return Valor1.variavel.db < Valor2.variavel.db;
			if(flag==3)
				return Valor1.variavel.ch < Valor2.variavel.ch;
			if(flag==4)
				return strcmp(Valor1.variavel.str, Valor2.variavel.str) < 0;
		} else if(!strcmp(op,">=")) {
			flag = Valor1.flag;
			if(flag==0)
				return Valor1.variavel.in >= Valor2.variavel.in;
			if(flag==1)
				return Valor1.variavel.fl >= Valor2.variavel.fl;
			if(flag==2)
				return Valor1.variavel.db >= Valor2.variavel.db;
			if(flag==3)
				return Valor1.variavel.ch >= Valor2.variavel.ch;
			if(flag==4)
				return strcmp(Valor1.variavel.str, Valor2.variavel.str) >= 0;
		} else if(!strcmp(op,"<=")) {
			flag = Valor1.flag;
			if(flag==0)
				return Valor1.variavel.in <= Valor2.variavel.in;
			if(flag==1)
				return Valor1.variavel.fl <= Valor2.variavel.fl;
			if(flag==2)
				return Valor1.variavel.db <= Valor2.variavel.db;
			if(flag==3)
				return Valor1.variavel.ch <= Valor2.variavel.ch;
			if(flag==4)
				return strcmp(Valor1.variavel.str, Valor2.variavel.str) <= 0;
		} else if(!strcmp(op,"==")) {
			flag = Valor1.flag;
			if(flag==0)
				return Valor1.variavel.in == Valor2.variavel.in;
			if(flag==1)
				return Valor1.variavel.fl == Valor2.variavel.fl;
			if(flag==2)
				return Valor1.variavel.db == Valor2.variavel.db;
			if(flag==3)
				return Valor1.variavel.ch == Valor2.variavel.ch;
			if(flag==4)
				return strcmp(Valor1.variavel.str, Valor2.variavel.str) == 0;
		}
	} else {
		if(Valor1.flag == -1) {
			if(Valor2.flag == -1) {
				if(!strcmp(op,"==")) {
					return strcmp(prim->NomeToken,terc->NomeToken) == 0;
				} else if(!strcmp(op,">")) {
					return strcmp(prim->NomeToken,terc->NomeToken) > 0;
				} else if(!strcmp(op,"<")) {
					return strcmp(prim->NomeToken,terc->NomeToken) < 0;
				} else if(!strcmp(op,">=")) {
					return strcmp(prim->NomeToken,terc->NomeToken) >= 0;
				} else if(!strcmp(op,"<=")) {
					return strcmp(prim->NomeToken,terc->NomeToken) <= 0;
				}
			} else {
				flag = Valor2.flag;
				if(flag == 4) {
					if(!strcmp(op,"==")) {
						return strcmp(Valor2.variavel.str,prim->NomeToken) == 0;
					} else if(!strcmp(op,">")) {
						return strcmp(Valor2.variavel.str,prim->NomeToken) > 0;
					} else if(!strcmp(op,"<")) {
						return strcmp(Valor2.variavel.str,prim->NomeToken) < 0;
					} else if(!strcmp(op,">=")) {
						return strcmp(Valor2.variavel.str,prim->NomeToken) >= 0;
					} else if(!strcmp(op,"<=")) {
						return strcmp(Valor2.variavel.str,prim->NomeToken) <= 0;
					}
				} else {
					if(flag == 0) {
						sprintf(valor,"%d",Valor2.variavel.in);
						if(!strcmp(op,"==")) {
							return strcmp(valor,prim->NomeToken) == 0;
						} else if(!strcmp(op,">")) {
							return strcmp(valor,prim->NomeToken) > 0;
						} else if(!strcmp(op,"<")) {
							return strcmp(valor,prim->NomeToken) < 0;
						} else if(!strcmp(op,">=")) {
							return strcmp(valor,prim->NomeToken) >= 0;
						} else if(!strcmp(op,"<=")) {
							return strcmp(valor,prim->NomeToken) <= 0;
						}
					} else if(flag == 1) {
						sprintf(valor,"%f",Valor2.variavel.fl);
						if(!strcmp(op,"==")) {
							return strcmp(valor,prim->NomeToken) == 0;
						} else if(!strcmp(op,">")) {
							return strcmp(valor,prim->NomeToken) > 0;
						} else if(!strcmp(op,"<")) {
							return strcmp(valor,prim->NomeToken) < 0;
						} else if(!strcmp(op,">=")) {
							return strcmp(valor,prim->NomeToken) >= 0;
						} else if(!strcmp(op,"<=")) {
							return strcmp(valor,prim->NomeToken) <= 0;
						}
					} else if(flag == 2) {
						sprintf(valor,"%lf",Valor2.variavel.db);
						if(!strcmp(op,"==")) {
							return strcmp(valor,prim->NomeToken) == 0;
						} else if(!strcmp(op,">")) {
							return strcmp(valor,prim->NomeToken) > 0;
						} else if(!strcmp(op,"<")) {
							return strcmp(valor,prim->NomeToken) < 0;
						} else if(!strcmp(op,">=")) {
							return strcmp(valor,prim->NomeToken) >= 0;
						} else if(!strcmp(op,"<=")) {
							return strcmp(valor,prim->NomeToken) <= 0;
						}
					} else if(flag == 3) {
						sprintf(valor,"%c",Valor2.variavel.ch);
						if(!strcmp(op,"==") ) {
							return strcmp(valor,prim->NomeToken) == 0;
						} else if(!strcmp(op,">")) {
							return strcmp(valor,prim->NomeToken) > 0;
						} else if(!strcmp(op,"<")) {
							return strcmp(valor,prim->NomeToken) < 0;
						} else if(!strcmp(op,">=")) {
							return strcmp(valor,prim->NomeToken) >= 0;
						} else if(!strcmp(op,"<=")) {
							return strcmp(valor,prim->NomeToken) <= 0;
						}
					}
				}
			}
		} else {
			flag = Valor1.flag;
			if(flag == 4) {
				if(!strcmp(op,"==")) {
					return strcmp(Valor1.variavel.str,terc->NomeToken) == 0;
				} else if(!strcmp(op,">")) {
					return strcmp(Valor1.variavel.str,terc->NomeToken) > 0;
				} else if(!strcmp(op,"<")) {
					return strcmp(Valor1.variavel.str,terc->NomeToken) < 0;
				} else if(!strcmp(op,">=")) {
					return strcmp(Valor1.variavel.str,terc->NomeToken) >= 0;
				} else if(!strcmp(op,"<=")) {
					return strcmp(Valor1.variavel.str,terc->NomeToken) <= 0;
				}
			} else {
				if(flag == 0) {
					sprintf(valor,"%d",Valor1.variavel.in);
					if(!strcmp(op,"==")) {
						return strcmp(valor,terc->NomeToken) == 0;
					} else if(!strcmp(op,">")) {
						return strcmp(valor,terc->NomeToken) > 0;
					} else if(!strcmp(op,"<")) {
						return strcmp(valor,terc->NomeToken) < 0;
					} else if(!strcmp(op,">=")) {
						return strcmp(valor,terc->NomeToken) >= 0;
					} else if(!strcmp(op,"<=")) {
						return strcmp(valor,terc->NomeToken) <= 0;
					}
				} else if(flag == 1) {
					sprintf(valor,"%f",Valor1.variavel.fl);
					if(!strcmp(op,"==")) {
						return strcmp(valor,terc->NomeToken) == 0;
					} else if(!strcmp(op,">")) {
						return strcmp(valor,terc->NomeToken) > 0;
					} else if(!strcmp(op,"<")) {
						return strcmp(valor,terc->NomeToken) < 0;
					} else if(!strcmp(op,">=")) {
						return strcmp(valor,terc->NomeToken) >= 0;
					} else if(!strcmp(op,"<=")) {
						return strcmp(valor,terc->NomeToken) <= 0;
					}
				} else if(flag == 2) {
					sprintf(valor,"%lf",Valor1.variavel.db);
					if(!strcmp(op,"==")) {
						return strcmp(valor,terc->NomeToken) == 0;
					} else if(!strcmp(op,">")) {
						return strcmp(valor,terc->NomeToken) > 0;
					} else if(!strcmp(op,"<")) {
						return strcmp(valor,terc->NomeToken) < 0;
					} else if(!strcmp(op,">=")) {
						return strcmp(valor,terc->NomeToken) >= 0;
					} else if(!strcmp(op,"<=")) {
						return strcmp(valor,terc->NomeToken) <= 0;
					}
				} else if(flag == 3) {
					sprintf(valor,"%c",Valor1.variavel.ch);
					if(!strcmp(op,"==")) {
						return strcmp(valor,terc->NomeToken) == 0;
					} else if(!strcmp(op,">")) {
						return strcmp(valor,terc->NomeToken) > 0;
					} else if(!strcmp(op,"<")) {
						return strcmp(valor,terc->NomeToken) < 0;
					} else if(!strcmp(op,">=")) {
						return strcmp(valor,terc->NomeToken) >= 0;
					} else if(!strcmp(op,"<=")) {
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
