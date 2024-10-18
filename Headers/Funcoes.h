struct funcoes {
	char nome[45];
	Lista *inicio;
	struct funcoes *prox;
};
typedef struct funcoes Funcoes;

int TipoFuncao(Token *T, Pilha *P,Funcoes *F) {
	if(!strcmp(T->NomeToken,"def"))
		return 0;

	while(P && strcmp(P->conteudo.nomeVar,T->NomeToken))
		P = P->prox;
	if(P && !strcmp(P->conteudo.nomeVar,T->NomeToken))
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
			if(flag == 0 || flag == 1 || flag == 2)
				return Valor1.variavel.fl > Valor2.variavel.fl;
			if(flag == 3)
				return Valor1.variavel.ch > Valor2.variavel.ch;
			if(flag == 4)
				return strcmp(Valor1.variavel.str, Valor2.variavel.str) > 0;
		} else if(!strcmp(op,"<")) {
			flag = Valor1.flag;
			if(flag == 0 || flag == 1 || flag == 2)
				return Valor1.variavel.fl < Valor2.variavel.fl;
			if(flag == 3)
				return Valor1.variavel.ch < Valor2.variavel.ch;
			if(flag == 4)
				return strcmp(Valor1.variavel.str, Valor2.variavel.str) < 0;
		} else if(!strcmp(op,">=")) {
			flag = Valor1.flag;
			if(flag == 0 || flag == 1 || flag == 2)
				return Valor1.variavel.fl >= Valor2.variavel.fl;
			if(flag == 3)
				return Valor1.variavel.ch >= Valor2.variavel.ch;
			if(flag == 4)
				return strcmp(Valor1.variavel.str, Valor2.variavel.str) >= 0;
		} else if(!strcmp(op,"<=")) {
			flag = Valor1.flag;
			if(flag == 0 || flag == 1 || flag == 2)
				return Valor1.variavel.fl <= Valor2.variavel.fl;
			if(flag == 3)
				return Valor1.variavel.ch <= Valor2.variavel.ch;
			if(flag == 4)
				return strcmp(Valor1.variavel.str, Valor2.variavel.str) <= 0;
		} else if(!strcmp(op,"==")) {
			flag = Valor1.flag;
			if(flag == 0 || flag == 1 || flag == 2)
				return Valor1.variavel.fl == Valor2.variavel.fl;
			if(flag == 3)
				return Valor1.variavel.ch == Valor2.variavel.ch;
			if(flag == 4)
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
					if(flag == 0 || flag == 1 || flag == 2) {
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
				if(flag == 0 || flag == 1 || flag == 2) {
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

void ExibirPrint(Token *token, Pilha *pilhaVar) {
	Valor val;
	Token *var;
	char string[100], aux[100], valor[10], mascara=0;

	val.flag = -1;
	strcpy(string,"");

	if(strcmp(token->NomeToken,"print"))
		EscrMsg("A LINHA NAO CONTEM FUNCAO PRINT");
	else {
		token = token->prox;
		if(token->NomeToken[0] == '(') {
			token = token->prox;
			if(token->NomeToken[0] == 34 || token->NomeToken[0] == 39) {
				token = token->prox;
				while(token && (token->NomeToken[0]!=34 && token->NomeToken[0]!=39)) {
					if(strcmp(token->NomeToken,"%d") && strcmp(token->NomeToken,"%f") && strcmp(token->NomeToken,"%c") && strcmp(token->NomeToken,"%s")) {
						// Se não for uma máscara ele deve concatenar com a string a ser apresentada
						strcpy(aux," ");
						strcat(aux,token->NomeToken);
						strcat(string,aux);
					} else {
						// Caso encontre uma máscara ele deve procurar a variável e verificar sua existência.
						mascara = 1;
						if(val.flag == -1) {
							var = token;
							while(var && strcmp(var->NomeToken,"%"))
								var = var->prox;
							if(var->prox->NomeToken[0] == '(')
								var = var->prox;
							var = var->prox;
							val = BuscaVariavel(var,pilhaVar);
						} else {
							var = var->prox;
							val = BuscaVariavel(var,pilhaVar);
						}
						if(val.flag != -1) {
							switch(token->NomeToken[1]) {
								case 'd': // Int
									sprintf(valor," %.0f",val.variavel.fl);
									break;
								case 'f': // Float
									sprintf(valor," %.6f",val.variavel.fl);
									break;
								case 2: // Double
									sprintf(valor," %.6lf",val.variavel.fl);
									break;
								case 'c': // Char
									sprintf(valor," %d",val.variavel.fl);
									break;
								case 's': // String
									strcpy(valor," ");
									strcat(valor,val.variavel.str);
							}
							strcat(string,valor);
						}
					}
					token = token->prox;
				}
				if(!mascara) {
					var = token;
					while(var && (var->NomeToken[0] != 34 && var->NomeToken[0] != 39))
						var = var->prox;
					while(var && var->NomeToken[0] != ')') {
						if(var->NomeToken[0] != ',' || var->NomeToken[0] != '+') {
							val = BuscaVariavel(var,pilhaVar);
							if(val.flag != -1) {
								if(val.flag == 1)
									sprintf(valor," %.0f",val.variavel.fl);
								else {
									strcpy(valor," ");
									strcat(valor,val.variavel.str);
								}
								strcat(string,valor);
							}
						}
						var = var->prox;
					}
				}
			} else {
				var = token;
				while(var && var->NomeToken[0] != ')') {
					if(var->NomeToken[0] != ',' || var->NomeToken[0] != '+') {
						val = BuscaVariavel(var,pilhaVar);
						if(val.flag != -1) {
							if(val.flag == 1)
								sprintf(valor," %.0f",val.variavel.fl);
							else {
								strcpy(valor," ");
								strcat(valor,val.variavel.str);
							}
							strcat(string,valor);
						}
					}
					var = var->prox;
				}
			}
			EscrMsg("");
			LimpaPrint();
			Moldura(20,10,110,16,9,0);
			Moldura(21,11,109,13,9,0);
			ExibirTexto((char*)"RESULTADO DO PRINT",53,12,14);
			gotoxy(22,14);
			printf("%s",string);
			textcolor(15);
		}
	}
}
