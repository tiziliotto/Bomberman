#include<stdio.h>
#include<windows.h>
#include<time.h>
//#include<conio2.h>
#include<conio.h>
#include<stdlib.h>
#define COL 61
#define LIN 25

typedef struct //coordenada xy
{
    int x;
    int y;

} posi;
typedef struct  // jogador
{
    posi pos;
    int bomba;
    int vidas;
    int pontos;
    int chaves;
    int vivo;
} player;
typedef struct  // bombas
{
    posi pos;
    time_t start;
} bomb;
/*PROTÓTIPO DAS FUÇÕES*/
posi percorre_matriz_jogador(char matriz[LIN][COL]);
posi imprime_mapa(char matriz[LIN][COL]);
player abre_mapa_carregado(char matriz[LIN][COL]);
player andar_baixo(player jog,char matriz[LIN][COL]);
player andar_esquerda(player jog,char matriz[LIN][COL]);
player andar_direita(player jog,char matriz[LIN][COL]);
player andar_cima(player jog,char matriz[LIN][COL]);
player atualiza(int x,int y, player atualiza);
player planta_bomba(player jog,int x,char matriz[LIN][COL],bomb *b1);
player coleta_chave(player jog,char matriz[LIN][COL],int x);
player colisao_jogador(player jog, char matriz[LIN][COL]);
player dano_bomba(bomb b1,player p1, char matriz[LIN][COL]);
player explode_bomba(bomb b1, clock_t end, char matriz[LIN][COL],int *entrou,player p1);
char menu_meio(char matriz[LIN][COL],player jog);
void imprime_infos(player jog);
void abre_mapa(char matriz[LIN][COL], char nome[]);
void novo_jogo(char matriz[LIN][COL]);
void imprime_mapa_carregado(char matriz[LIN][COL]);
void novo_jogo_carregado(char matriz[LIN][COL]);
void opcao_menu_inicio(char matriz[LIN][COL]);
void menu_inicio(char matriz[LIN][COL]);
void opcao_menu_meio(char matriz[LIN][COL],player jog);
void colisao_comum(int i,int j,char matriz[LIN][COL],bomb b1);
void colisao_caixa_chave(int i,int j,char matriz[LIN][COL],bomb b1);
void jogo_principal(player p1, char matriz[LIN][COL]);
void HideCursor();
void salvar_jogo(char matriz[LIN][COL],player jog);
void proximo_mapa(char matriz[LIN][COL], player p1);

/*DEFINIÇÃO DAS FUNÇÕES*/
player atualiza(int x,int y, player atualiza)// atualiza a coordenada do jogador
{
    atualiza.pos.x=x;
    atualiza.pos.y=y;
    return atualiza;
}
void imprime_infos(player jog)// imprime informações
{
    int i;
    gotoxy(1,26); //final do jogo
    for(i=1; i<=COL/2; i++)
    {
//        textcolor(YELLOW);
        printf("=");
//        textcolor(LIGHTCYAN);
        printf("=");
    }
//    textcolor(LIGHTCYAN);
    printf("\n::::Vidas((%d))::::Bombas((%d))::::Pontos((%d))::::Chaves((%d))\n",jog.vidas,jog.bomba,jog.pontos,jog.chaves);
}
void abre_mapa(char matriz[LIN][COL], char nome[]) // abre o arquivo do mapa
{
    FILE *arq;
    char c;
    int i,j;

    arq = fopen(nome, "r");
    if(arq == NULL) //testa se abriu o mapa
    {
        printf("Erro ao abrir arquivo\n");
    }
    else // se abriu faz a leitura do arquivo para dentro da matriz
    {
        for(i=0; i<LIN; i++)
        {
            for(j=0; j<COL; j++)
            {
                matriz[i][j] = getc(arq);
            }
        }
        fclose(arq);
    }
}
posi imprime_mapa(char matriz[LIN][COL]) // imprime o mapa
{
    int i,j;
    posi pos;
    gotoxy(0,0);//começa no ponto 0,0 da tela
    for(i=0; i<LIN; i++)
    {
        for(j=0; j<COL; j++)
        {
            if(matriz[i][j]=='J')
            {
//                textcolor(LIGHTMAGENTA);
                printf("J");
                pos.x=j;// armazena as coordenadas do jogador
                pos.y=i;
            }
            else if(matriz[i][j]=='W')//parede indestrutível
            {
//                textcolor(LIGHTGRAY);
                printf("%c",219);

            }
            else if(matriz[i][j]=='D')//parede destritível
            {
//                textcolor(DARKGRAY);
                printf("%c",219);
            }
            else if (matriz[i][j]=='E')//inimigo
            {
//                textcolor(YELLOW);
                printf("&");
            }
            else if (matriz[i][j]=='B' || matriz[i][j]=='K')//caixas
            {
                if(matriz[i][j]=='K')
                {
//                    textcolor(LIGHTCYAN);
                    printf("#");
                }
                else
                {
//                    textcolor(LIGHTCYAN);
                    printf("#");
                }
            }
            else if (matriz[i][j]==' ')// espaços em brancos
            {
                printf(" ",matriz[i][j]);
            }
            else if (matriz[i][j]=='\n')//pula linhas
            {
                printf("\n");
            }
            else if (matriz[i][j]=='@') //pula linhas
            {
                matriz[i][j]=' ';
                printf(" ");
            }
        }
    }
    return pos; //retorna a posição do jogador
}
void novo_jogo(char matriz[LIN][COL])// começa um novo jogo
{
    posi pos;
    system("cls"); //apaga tudo que estava na tela
    abre_mapa(matriz,"mapa1.txt");
    pos=imprime_mapa(matriz); //imprime a matriz e encontra jogador
    player p1= {pos.x+1,pos.y+1,3,5,0,0,1}; //inicializa o jogador(usa+1 por causa da diferença matriz/tela);
    imprime_infos(p1);
    jogo_principal(p1,matriz);
}
void salvar_jogo(char matriz[LIN][COL],player jog)
{
    FILE *arq;
    char c;
    int i,j;
    char nome_jogo[50];

    system("cls");
    printf("\nINSIRA O NOME DO JOGO:   ");
    gets(nome_jogo);
    arq = fopen(nome_jogo, "w");
    if(arq == NULL) //testa se abriu o arquivo
    {
        printf("Erro ao abrir arquivo");
    }
    else // se abriu faz a impressao da matriz no arquivo
    {
        fprintf(arq,"%d\n%d\n%d\n%d\n%d\n%d\n",jog.pos.x,jog.pos.y,jog.bomba,jog.vidas,jog.pontos,jog.chaves);//escreve as informações do jogador no arquibo
        for(i=0; i<LIN; i++)
        {
            for(j=0; j<COL; j++)
            {
                putc(matriz[i][j],arq);
            }
        }
        printf("%s SALVO COM SUCESSO",nome_jogo);
        fclose(arq);
    }
}
player abre_mapa_carregado(char matriz[LIN][COL]) // abre o arquivo do mapa
{
    FILE *arq;
    char c;
    int i,j;
    char nome_arq[50];
    player jog;

    system("cls");
    printf("INSIRA O NOME DO JOGO:  ");
    gets(nome_arq);

    arq = fopen(nome_arq, "r");

    if(arq == NULL) //testa se abriu o mapa
    {
        printf("Erro ao abrir arquivo %s\n");
    }
    else // se abriu faz a leitura do arquivo para dentro da matriz
    {
        fscanf(arq,"%d%d%d%d%d%d\n",&jog.pos.x,&jog.pos.y,&jog.bomba,&jog.vidas,&jog.pontos,&jog.chaves);//le as informações do jogador
        jog.bomba=3;
        for(i=0; i<LIN; i++)
        {
            for(j=0; j<COL; j++)
            {
                matriz[i][j] = getc(arq); // le o mapa
            }
        }
        fclose(arq);
    }
    return jog;
}
void imprime_mapa_carregado(char matriz[LIN][COL]) // imprime o mapa
{
    int i,j;
    posi pos;
    gotoxy(0,0);//começa no pornto 0,0 da tela
    for(i=0; i<LIN; i++)
    {
        for(j=0; j<COL; j++)
        {
            if(matriz[i][j]=='J')
            {
//                textcolor(LIGHTMAGENTA);
                printf("J");

            }
            else if(matriz[i][j]=='W')//parede indestrutível
            {
//                textcolor(LIGHTGRAY);
                printf("%c",219);

            }
            else if(matriz[i][j]=='D')//parede destritível
            {
//                textcolor(DARKGRAY);
                printf("%c",219);
            }
            else if (matriz[i][j]=='E')//inimigo
            {
//                textcolor(YELLOW);
                printf("&");
            }
            else if (matriz[i][j]=='B'  || matriz[i][j]=='K')//caixas
            {
//                textcolor(LIGHTCYAN);
                printf("#");
            }
            else if(matriz[i][j]=='A')
            {
//                textcolor(LIGHTGREEN);
                printf("$");
            }
            else if (matriz[i][j]==' ') // espaços em brancos
            {
                printf(" ",matriz[i][j]);
            }
            else if (matriz[i][j]=='\n')//pula linhas
            {
                printf("\n",matriz[i][j]);
            }
            else if (matriz[i][j]=='@')
            {
                matriz[i][j]=' ';
                printf(" ");

            }

        }
    }
}
void novo_jogo_carregado(char matriz[LIN][COL])// começa um novo jogo
{
    player p1;
    p1.vivo=1;
    p1=abre_mapa_carregado(matriz);
    system("cls");
    imprime_mapa_carregado(matriz);
    imprime_infos(p1);
    jogo_principal(p1,matriz);
}
void opcao_menu_inicio(char matriz[LIN][COL])// le a opção de inicio
{
    char c;
    do
    {
        c = getch(); //espera uma tecla
    }
    while(c!='n'&&c!='c'&&c!='q'); //enquanto n for válida
    switch(c)
    {
    case'n':
        novo_jogo(matriz);
    case'c':
            novo_jogo_carregado(matriz);
    case'q':
        system("cls");
        printf("\n\n\n\n\n\n\n                     =====FIM DE JOGO=====\n\n\n\n\n\n                 Desenvolvido por Lara Almeida\n\n\n\n\n\n\n");
        break;
    }
}
void menu_inicio(char matriz[LIN][COL])//imprime o menu
{
//    textcolor(LIGHTCYAN);
    printf("\n\n\n\n                             BOMBERMAN\n\n\n\n");
    printf("\n\n                     SELECIONE A OPCAO DE JOGO\n\n\n");
    printf("\n\n\n\n                           N- NOVO JOGO\n\n                           C- CARREGAR JOGO\n\n                           Q-SAIR DO JOGO");
    opcao_menu_inicio(matriz);
}
void opcao_menu_meio(char matriz[LIN][COL],player jog)// le a opção
{
    char c;
    do
    {
        c = getch(); //espera uma tecla
    }
    while(c!='n'&&c!='c'&&c!='q'&&c!='s'&&c!='v'); //enquanto n for válida
    switch(c)
    {
    case'n':
        novo_jogo(matriz);
        break;
    case'c':
            novo_jogo_carregado(matriz);
        break;
    case 's':
        salvar_jogo(matriz,jog);
        break;
    case'q':
        system("cls");
        printf("\n\n\n\n\n\n\n                     =====FIM DE JOGO=====\n\n\n\n\n\n\n\n                 Desenvolvido por Lara Almeida\n\n\n\n\n\n\n");
        break;
    case 'v':
        system("cls");
        imprime_mapa(matriz);
        jogo_principal(jog,matriz);
        break;
    }
}
char menu_meio(char matriz[LIN][COL],player jog)//imprime o menu
{
//    textcolor(LIGHTCYAN);
    printf("\n\n\n\n                             BOMBERMAN\n\n\n\n");
    printf("\n\n                     SELECIONE A OPCAO DE JOGO\n\n\n");
    printf("\n\n\n\n                           V- VOLTAR AO JOGO\n\n                           N- NOVO JOGO\n\n                           C- CARREGAR JOGO\n\n                           Q-SAIR DO JOGO\n\n                           S-SALVAR JOGO");
    opcao_menu_meio(matriz,jog);
    return 'c';
}
player coleta_chave(player jog,char matriz[LIN][COL],int x)
{
    jog.chaves++; //incrementa chaves
    Beep(5000,50);
    jog.pontos=jog.pontos+200;
    if(jog.chaves!=5)
    {
        switch(x)
        {
        // cada caso é para uma posição da bomba em relação ao objeto
        case 1:
            gotoxy(jog.pos.x,jog.pos.y);
            printf(" "); //apaga onde estava
            matriz[jog.pos.y-1][jog.pos.x-1]=' ';
            gotoxy(jog.pos.x,jog.pos.y-1);
            printf("J");//imprime no novo lugar
            matriz[jog.pos.y-2][jog.pos.x-1]='J';
            jog=atualiza(jog.pos.x,jog.pos.y-1,jog); //atualiza a posição do jogador
            break;
        case 2:
            gotoxy(jog.pos.x,jog.pos.y);
            printf(" ");
            matriz[jog.pos.y-1][jog.pos.x-1]=' ';
            gotoxy(jog.pos.x,jog.pos.y+1);
            printf("J");
            matriz[jog.pos.y][jog.pos.x-1]='J';
            jog=atualiza(jog.pos.x,jog.pos.y+1,jog);
            break;
        case 3:
            gotoxy(jog.pos.x,jog.pos.y);
            printf(" ");
            matriz[jog.pos.y-1][jog.pos.x-1]=' ';
            gotoxy(jog.pos.x-1,jog.pos.y);
            printf("J");
            matriz[jog.pos.y-1][jog.pos.x-2]='J';
            jog=atualiza(jog.pos.x-1,jog.pos.y,jog);

            break;
        case 4:
            gotoxy(jog.pos.x,jog.pos.y);
            printf(" ");
            matriz[jog.pos.y-1][jog.pos.x-1]=' ';
            gotoxy(jog.pos.x+1,jog.pos.y);
            printf("J");
            matriz[jog.pos.y-1][jog.pos.x]='J';
            jog = atualiza(jog.pos.x+1,jog.pos.y,jog);
            break;
        }
        return jog;
    }
    else
    {
        system("cls"); //apaga tudo que estava na tela
        printf("\n\n\n\n\n\n\n\n\n\n            ===VOCE COLETOU TODAS AS CHAVES E PASSOU DE NIVEL===");
        Sleep(3000);
        system("cls");
        printf("\n\n\n\n\n\n\n\n\n\n\n\n                ##### NOVA FASE SERA INICIADA EM 5 #####\n");
        Sleep(1000);
        printf("                ##### NOVO FASE SERA INICIADA EM 4 #####\n");
        Sleep(1000);
        printf("                ##### NOVA FASE SERA INICIADA EM 3 #####\n");
        Sleep(1000);
        printf("                ##### NOVA FASE SERA INICIADA EM 2 #####\n");
        Sleep(1000);
        printf("                ##### NOVA FASE SERA INICIADA EM 1 #####\n");
        Sleep(1000);
        printf("                ##### NOVA FASE SERA INICIADA EM 0 #####\n");
        Sleep(1000);
        system("cls");
        proximo_mapa(matriz,jog);
    }

}
void proximo_mapa(char matriz[LIN][COL], player p1)
{
    {
        posi pos;
        abre_mapa(matriz,"mapa2.txt");
        pos=imprime_mapa(matriz); //imprime a matriz e encontra jogador
        player p1= {pos.x+1,pos.y+1,3,5,0,0,1}; //inicializa o jogador(usa+1 por causa da diferença matriz\tela);
        imprime_infos(p1);
        jogo_principal(p1,matriz);
    }
}
player andar_cima(player jog,char matriz[LIN][COL]) //andar para cima
{
    if(matriz[jog.pos.y-2][jog.pos.x-1]==' ')//verifica se pode ir pra cima
    {
        gotoxy(jog.pos.x,jog.pos.y);
        printf(" "); //apaga onde estava
        matriz[jog.pos.y-1][jog.pos.x-1]=' ';
        gotoxy(jog.pos.x,jog.pos.y-1);
        printf("J");//imprime no novo lugar
        matriz[jog.pos.y-2][jog.pos.x-1]='J';
        jog=atualiza(jog.pos.x,jog.pos.y-1,jog); //atualiza a posição do jogador

    }
    else if(matriz[jog.pos.y-2][jog.pos.x-1]=='A')//verifica se nao encontrou uma chave
    {
        jog = coleta_chave(jog,matriz,1);
    }
    else if(matriz[jog.pos.y-2][jog.pos.x-1]=='E')//verifica se nao colidiu com o inimigo
    {
        jog.vivo=0;

    }
    else
    {
        gotoxy(jog.pos.x,jog.pos.y);
        printf("J");
    }
    return jog; //retorna o jogador com a posição atualizada
}
player andar_baixo(player jog,char matriz[LIN][COL])//andar para baixo
{
    if(matriz[jog.pos.y][jog.pos.x-1]==' ')
    {
        gotoxy(jog.pos.x,jog.pos.y);
        printf(" ");
        matriz[jog.pos.y-1][jog.pos.x-1]=' ';
        gotoxy(jog.pos.x,jog.pos.y+1);
        printf("J");
        matriz[jog.pos.y][jog.pos.x-1]='J';
        jog=atualiza(jog.pos.x,jog.pos.y+1,jog);

    }
    else if(matriz[jog.pos.y][jog.pos.x-1]=='A')
    {
        jog=coleta_chave(jog,matriz,2);
    }
    else if(matriz[jog.pos.y][jog.pos.x-1]=='E')
    {
        jog.vivo=0;
    }
    else
    {
        gotoxy(jog.pos.x,jog.pos.y);
        printf("J");
    }
    return jog;
}
player andar_esquerda(player jog, char matriz[LIN][COL])//andar para esquerda
{
    if(matriz[jog.pos.y-1][jog.pos.x-2]==' ')
    {
        gotoxy(jog.pos.x,jog.pos.y);
        printf(" ");
        matriz[jog.pos.y-1][jog.pos.x-1]=' ';
        gotoxy(jog.pos.x-1,jog.pos.y);
        printf("J");
        matriz[jog.pos.y-1][jog.pos.x-2]='J';
        jog=atualiza(jog.pos.x-1,jog.pos.y,jog);

    }
    else if(matriz[jog.pos.y-1][jog.pos.x-2]=='A')
    {
        jog=coleta_chave(jog,matriz,3);
    }
    else if(matriz[jog.pos.y-1][jog.pos.x-2]=='E')
    {
        jog.vivo=0;
    }
    else
    {
        gotoxy(jog.pos.x,jog.pos.y);
        printf("J");
    }
    return jog;
}
player andar_direita(player jog, char matriz[LIN][COL])//andar para direita
{
    if(matriz[jog.pos.y-1][jog.pos.x]==' ')
    {
        gotoxy(jog.pos.x,jog.pos.y);
        printf(" ");
        matriz[jog.pos.y-1][jog.pos.x-1]=' ';
        gotoxy(jog.pos.x+1,jog.pos.y);
        printf("J");
        matriz[jog.pos.y-1][jog.pos.x]='J';
        jog = atualiza(jog.pos.x+1,jog.pos.y,jog);

    }
    else if(matriz[jog.pos.y-1][jog.pos.x]=='A')
    {
        jog=coleta_chave(jog,matriz,4);

    }
    else if(matriz[jog.pos.y-1][jog.pos.x]=='E')
    {
        jog.vivo=0;
    }
    else
    {
        gotoxy(jog.pos.x,jog.pos.y);
        printf("J");
    }
    return jog;
}
player dano_bomba(bomb b1,player p1, char matriz[LIN][COL])//raio de explosao da bomba 5x5
{
    int inicio_linha=b1.pos.y-2; // variaveis para percorrero raio da bomba
    int fim_linha=b1.pos.y+2;
    int inicio_coluna=b1.pos.x-2;
    int fim_coluna=b1.pos.x+2;
    int i,j;

    for(i=inicio_linha; i<=fim_linha; i++) // percorre o raio verificando as colisoes
    {
        for(j=inicio_coluna; j<=fim_coluna; j++)
        {
            if(matriz[i][j]=='D')
            {
                colisao_comum(i,j,matriz,b1);
                p1.pontos=p1.pontos+10; //pontuação
            }
            if(matriz[i][j] == 'K')
            {
                colisao_caixa_chave(i,j,matriz,b1);
                p1.pontos=p1.pontos+30;
            }
            if(matriz[i][j]=='B')
            {
                colisao_comum(i,j,matriz,b1);
                p1.pontos=p1.pontos+10;

            }
            if(matriz[i][j]=='E')
            {
                colisao_comum(i,j,matriz,b1);
                p1.pontos=p1.pontos+100;
            }
            if(matriz[i][j] == 'J')
            {
                p1.vivo=0;
            }
        }
    }
    return p1;
}
void colisao_comum(int i,int j,char matriz[LIN][COL],bomb b1)//bomba destroi passagem
{
    int x,y;
    //cada caso corresponde a uma posição da bomba em relação ao objeto
    if(i==b1.pos.y && j>b1.pos.x)
    {
        matriz[b1.pos.y][j] =' ';
        gotoxy(j+1,i+1);
        printf(" ");
    }
    else if(i==b1.pos.y && j<b1.pos.x)
    {
        matriz[b1.pos.y][j] =' ';
        gotoxy(j+1,i+1);
        printf(" ");
    }
    else  if(i<b1.pos.y && j==b1.pos.x)
    {
        matriz[i][b1.pos.x] =' ';
        gotoxy(j+1,i+1);
        printf(" ");
    }
    else if(i>b1.pos.y && j==b1.pos.x)
    {
        matriz[i][b1.pos.x] =' ';
        gotoxy(j+1,i+1);
        printf(" ");
    }
    else  if(i<b1.pos.y && j<b1.pos.x)
    {
        y=b1.pos.y-(b1.pos.y-i);
        x=b1.pos.x-(b1.pos.x-j);
        matriz[y][x] =' ';
        gotoxy(j+1,i+1);
        printf(" ");
    }
    else  if(i>b1.pos.y && j>b1.pos.x)
    {
        y=b1.pos.y+(i-b1.pos.y);
        x=b1.pos.x+(j-b1.pos.x);
        matriz[y][x] =' ';
        gotoxy(j+1,i+1);
        printf(" ");
    }
    else if(i>b1.pos.y && j<b1.pos.x)
    {
        y=b1.pos.y+(i-b1.pos.y);
        x=b1.pos.x-(b1.pos.x-j);
        matriz[y][x] =' ';
        gotoxy(j+1,i+1);
        printf(" ");
    }
    else if(i<b1.pos.y && j>b1.pos.x)
    {
        y=b1.pos.y-(b1.pos.y-i);
        x=b1.pos.x+(j-b1.pos.x);
        matriz[y][x] =' ';
        gotoxy(j+1,i+1);
        printf(" ");
    }
}
void colisao_caixa_chave(int i,int j,char matriz[LIN][COL],bomb b1)
{
    int x,y;
    //cada caso corresponde a uma posição da bomba em relação ao objeto
//    textcolor(LIGHTGREEN);
    if(i==b1.pos.y && j>b1.pos.x)
    {
        matriz[b1.pos.y][j] ='A';//muda a matriz pra chave
        gotoxy(j+1,i+1);
        printf("$");
    }
    else if(i==b1.pos.y && j<b1.pos.x)
    {
        matriz[b1.pos.y][j] ='A';
        gotoxy(j+1,i+1);
        printf("$");
    }
    else  if(i<b1.pos.y && j==b1.pos.x)
    {
        matriz[i][b1.pos.x] ='A';
        gotoxy(j+1,i+1);
        printf("$");
    }
    else if(i>b1.pos.y && j==b1.pos.x)
    {
        matriz[i][b1.pos.x] ='A';
        gotoxy(j+1,i+1);
        printf("$");
    }
    else  if(i<b1.pos.y && j<b1.pos.x)
    {
        y=b1.pos.y-(b1.pos.y-i);
        x=b1.pos.x-(b1.pos.x-j);
        matriz[y][x] ='A';
        gotoxy(j+1,i+1);
        printf("$");
    }
    else  if(i>b1.pos.y && j>b1.pos.x)
    {
        y=b1.pos.y+(i-b1.pos.y);
        x=b1.pos.x+(j-b1.pos.x);
        matriz[y][x] ='A';
        gotoxy(j+1,i+1);
        printf("$");
    }
    else if(i>b1.pos.y && j<b1.pos.x)
    {
        y=b1.pos.y+(i-b1.pos.y);
        x=b1.pos.x-(b1.pos.x-j);
        matriz[y][x] ='A';
        gotoxy(j+1,i+1);
        printf("$");
    }
    else if(i<b1.pos.y && j>b1.pos.x)
    {
        y=b1.pos.y-(b1.pos.y-i);
        x=b1.pos.x+(j-b1.pos.x);
        matriz[y][x] ='A';
        gotoxy(j+1,i+1);
        printf("$");
    }
}
posi percorre_matriz_jogador(char matriz[LIN][COL])//encontra jogador na matriz
{
    posi pos;
    int i,j;
    for(i=0; i<LIN; i++)
    {
        for(j=0; j<COL; j++)
        {
            if(matriz[i][j]=='J')
            {
                pos.x=j;
                pos.y=i;
            }
        }
    }
    return pos;
}
player colisao_jogador(player jog, char matriz[LIN][COL])//bomba ou inimigo mata jogador
{
    posi posicao_jogador;
    if(jog.vivo==0)
    {


        if(jog.vidas>1)
        {
            Beep(1000,100);
            Beep(550,100);
            Beep(500,100);
            jog.vidas--;
            jog.vivo=1;
            posicao_jogador=percorre_matriz_jogador(matriz);
            matriz[posicao_jogador.y][posicao_jogador.x]=' ';
            gotoxy(posicao_jogador.x+1,posicao_jogador.y+1);
            printf(" ");
            gotoxy(3,3);
//            textcolor(LIGHTMAGENTA);
            printf("J");
            jog.pos.x = 3;
            jog.pos.y = 3;
            matriz[2][2]='J';
            if(jog.pontos>=100)
            {
                jog.pontos=jog.pontos-100;
            }
            else
            {
                jog.pontos=0;
            }
            return jog;
        }
        else
        {
//            textcolor(LIGHTCYAN);
            jog.vidas--;
            system("cls");
            Beep(5000,1000);
            printf("\n\n\n\n\n\n\n\n\n\n\n\n             ##### VOCE PERDEU TODAS SUAS VIDAS #####");
            Sleep(3000);
            system("cls");
            printf("\n\n\n\n\n\n\n\n\n\n\n\n                ##### NOVO JOGO SERA INICIADO EM 5 #####\n");
            Sleep(1000);
            printf("                ##### NOVO JOGO SERA INICIADO EM 4 #####\n");
            Sleep(1000);
            printf("                ##### NOVO JOGO SERA INICIADO EM 3 #####\n");
            Sleep(1000);
            printf("                ##### NOVO JOGO SERA INICIADO EM 2 #####\n");
            Sleep(1000);
            printf("                ##### NOVO JOGO SERA INICIADO EM 1 #####\n");
            Sleep(1000);
            printf("                ##### NOVO JOGO SERA INICIADO EM 0 #####\n");
            Sleep(1000);
            system("cls");
            novo_jogo(matriz);

        }
    }
}
player planta_bomba(player jog,int x,char matriz[LIN][COL],bomb *b1) //plantar bomba
{
    switch (x) // seleciona a flag da bomba
    {
    case 1://para baixo
        if(matriz[jog.pos.y-2][jog.pos.x-1]==' ')
        {
            Beep(600,250);
            gotoxy(jog.pos.x,jog.pos.y-1);
//            textcolor(LIGHTRED);
            printf("@");
            matriz[jog.pos.y-2][jog.pos.x-1]='@';
            b1->pos.x=jog.pos.x-1;
            b1->pos.y=jog.pos.y-2;
            jog.bomba--;
        }
        return jog;
    case 2://para cima
        if(matriz[jog.pos.y][jog.pos.x-1]==' ')
        {
            Beep(600,250);
            gotoxy(jog.pos.x,jog.pos.y+1);
//            textcolor(LIGHTRED);
            printf("@");
            matriz[jog.pos.y][jog.pos.x-1]='@';
            b1->pos.x=jog.pos.x-1;
            b1->pos.y=jog.pos.y;
            jog.bomba--;
        }
        return jog;
    case 3://para esquerda
        if(matriz[jog.pos.y-1][jog.pos.x-2]==' ')
        {
            Beep(600,250);
            gotoxy(jog.pos.x-1,jog.pos.y);
//            textcolor(LIGHTRED);
            printf("@");
            matriz[jog.pos.y-1][jog.pos.x-2]='@';
            b1->pos.x=jog.pos.x-2;
            b1->pos.y=jog.pos.y-1;
            jog.bomba--;
        }
        return jog;
    case 4://para direita
        if(matriz[jog.pos.y-1][jog.pos.x]==' ')
        {
            Beep(300,250);
            gotoxy(jog.pos.x+1,jog.pos.y);
//            textcolor(LIGHTRED);
            printf("@");
            matriz[jog.pos.y-1][jog.pos.x]='@';
            b1->pos.x=jog.pos.x;
            b1->pos.y=jog.pos.y-1;
            jog.bomba--;
        }
        return jog;
    }
}
player explode_bomba(bomb b1, clock_t end, char matriz[LIN][COL],int *entrou,player p1)
{
    if((double)(end-(b1.start))/(double)(CLOCKS_PER_SEC)>3 && *entrou==1)//verifica se passaram os 3 SEG
    {
        gotoxy(b1.pos.x+1,b1.pos.y+1);
        printf(" ");
        Beep(400,250);
        matriz[b1.pos.y][b1.pos.x]=' ';
        p1.bomba++;
        p1=dano_bomba(b1,p1,matriz);
        *entrou = 0;
    }
    return p1;
}
void jogo_principal(player p1, char matriz[LIN][COL]) //jogo principal
{
    int i;
    int x=0; // flag para saber a direção do jogador
    bomb b1[3]; //vetor de bombas
    clock_t end; //variavel tempo
    int entrou[3]; //flag para saber se a bomba esta no mapa ou no estoque
    char c;

    do
    {
        end = clock(); //atualiza o tempo
        if(kbhit())
        {
//            textcolor(LIGHTMAGENTA);
            c = getch( );// aguarda comandos do teclado ***SETAS E ESC***
            if((int)c == -32)  //se a primeira parte é -32 então pode ser uma seta
            {
                end = clock();
                c = getch(); //pega segunda parte
                switch (c)
                {

                case 72:
                    p1 = andar_cima(p1,matriz);
                    x=1;
                    break;
                case 80:
                    p1 = andar_baixo(p1,matriz);
                    x=2;
                    break;
                case 75:
                    p1 = andar_esquerda(p1,matriz);
                    x=3;
                    break;
                case 77:
                    p1 = andar_direita(p1,matriz);
                    x=4;
                    break;
                }
            }
            else if((int)c != -32) // se n for seta
            {
                end = clock();
                switch (c)
                {
                case 'b':
                    if(p1.bomba!=0)// verifica se o estoque é != 0
                    {

                        if(p1.bomba==3)
                        {
                            b1[0].start = clock(); // starta o tempo da bomba
                            p1=planta_bomba(p1,x,matriz,&b1[0]); //planta a bomba
                            entrou[0]=1;//informa que a bomba esta no mapa
                        }
                        else if (p1.bomba==2)
                        {
                            b1[1].start = clock();
                            p1=planta_bomba(p1,x,matriz,&b1[1]);
                            entrou[1]=1;
                        }
                        else if (p1.bomba==1)
                        {
                            b1[2].start = clock();
                            p1=planta_bomba(p1,x,matriz,&b1[2]);
                            entrou[2]=1;
                        }

                    }
                    break;
                case 27:

                    system("cls");//limpa a tela e chama menu
                    p1.bomba=3;
                    c=menu_meio(matriz,p1);
                    for(i=0; i<3; i++)
                    {
                        entrou[i]=0;
                    }
                }
            }
            else
            {
                end = clock();
            }
        }
        //verifica se alguma bomba deve ser explodida
        p1=explode_bomba(b1[0],end,matriz,&entrou[0],p1);
        p1=explode_bomba(b1[1],end,matriz,&entrou[1],p1);
        p1=explode_bomba(b1[2],end,matriz,&entrou[2],p1);
        p1=colisao_jogador(p1,matriz);
        //imprime as informações atualizadas
        gotoxy(1,25);
        imprime_infos(p1);

    }
    while(c!=27);   //enquanto nao for ESC
}
void HideCursor()
{
    CONSOLE_CURSOR_INFO cursor = {1, FALSE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}
int main() //começa o jogo
{
    char matriz[LIN][COL]= {'0'};//inicializa uma matriz
    HideCursor();
    menu_inicio(matriz);//chama menu de inicio
}
