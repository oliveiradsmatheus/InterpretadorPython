void Moldura (int CI,int LI,int CF,int LF,int CorT,int CorF) {
	int i;

	textcolor(CorT);
	textbackground(CorF);

	gotoxy(CI,LI);
	printf("%c",201);
	gotoxy(CF,LI);
	printf("%c",187);
	gotoxy(CI,LF);
	printf("%c",200);
	gotoxy(CF,LF);
	printf("%c",188);

	for(i=CI+1; i<CF; i++) {
		gotoxy(i,LI);
		printf("%c",205);
		gotoxy(i,LF);
		printf("%c",205);
	}
	for(i=LI+1; i<LF; i++) {
		gotoxy(CI,i);
		printf("%c",186);
		gotoxy(CF,i);
		printf("%c",186);
	}

	textbackground(0);
	textcolor(15);
}

void MolduraFina (int CI,int LI,int CF,int LF,int CorT,int CorF) {
	int i;

	textcolor(CorT);
	textbackground(CorF);

	gotoxy(CI,LI);
	printf("%c",218);
	gotoxy(CF,LI);
	printf("%c",191);
	gotoxy(CI,LF);
	printf("%c",192);
	gotoxy(CF,LF);
	printf("%c",217);

	for(i=CI+1; i<CF; i++) {
		gotoxy(i,LI);
		printf("%c",196);
		gotoxy(i,LF);
		printf("%c",196);
	}
	for(i=LI+1; i<LF; i++) {
		gotoxy(CI,i);
		printf("%c",179);
		gotoxy(CF,i);
		printf("%c",179);
	}

	textbackground(0);
	textcolor(15);
}

void RetiraCursor (void) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void LigaCursor (void) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = TRUE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void Dimensao (void) {
	system("mode con cols=120 lines=145");
}

void Fundo (int Cor) {
	int L=2;

	textbackground(Cor);
	while(L<45) {
		gotoxy(2,L);
		printf("                                                                                                                      ");
		L++;
	}
}

void LimpaLinha(int Num) {
	gotoxy(3,Num);
	printf("                                                                                                                    ");
}

void LimpaTela(void) {
	int i;

	for(i=10; i<41; i++)
		LimpaLinha(i);
}

void LimpaTelaMem (void) {
	int i;
	
	for(i=15;i<41;i++) {
		gotoxy(46,i);
		printf("                                                                        ");
	}
}

void LimpaPrint (void) {
	int i;
	
	for(i=10;i<16;i++) {
		gotoxy(20,i);
		printf("                                                                                          ");
	}
}

void LimpaTelaInteira(void) {
	int i;

	for(i=6; i<41; i++)
		LimpaLinha(i);
}

void MolduraCompleta (void) {
	Moldura(1,1,120,45,9,0); // Moldura Externa
	Moldura(2,2,119,4,9,0); // Moldura do Titulo
	Moldura(2,5,119,41,9,0); // Moldura Central
	Moldura(2,42,119,44,9,0); // Moldura da Mensagem Inferior
}

void MoldFim() {
	Fundo(0);
	Moldura(1,1,120,45,9,0);
	Moldura(2,2,119,4,9,0);
	Moldura(2,42,119,44,9,0);
}
