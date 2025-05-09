#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUMERO_DE_PEOES 4
#define NUMERO_DE_JOGADORES 4

//Definição dos structs.
typedef struct {
    int posição;
} Peao;

typedef struct {
    int jogador_atual[NUMERO_DE_JOGADORES];
    Peao peoes[NUMERO_DE_PEOES];
} Player;

//Função que rola o dado.
int dado() {
    return rand() % 6 + 1;
}

//Função que não permite que o usuário escolha algo que seja diferente de 2 jogadores para jogar.
int validar_numero_jogadores(int numero_de_jogadores) {
    if (numero_de_jogadores == 2) {
        return 1;
    } else {
        printf("Digite um número válido de jogadores, (2)\n");
        exit(0);
    }
}

//Função que inicializa todos os peões na posição 0.
void inicializador(Player jogadores[], int num_de_jogadores) {
    int i, j;
    for (i = 0; i < num_de_jogadores; i++) {
        for (j = 0; j < NUMERO_DE_PEOES; j++) {
            jogadores[i].peoes[j].posição = 0;
        }
    }
}

//Função que age como tabuleiro, mostrando os jogadores com os seus respectivos peões e posições.
void tabuleiro(Player jogadores[], int num_de_jogadores, char cor_jogadores[][10]) {
    int i, j;
    printf("\n~~~~~~~PLACAR DO JOGO~~~~~~~");
    for (i = 0; i < num_de_jogadores; i++) {
        printf("\n\nJOGADOR %s\n", cor_jogadores[i]);
        for (j = 0; j < NUMERO_DE_PEOES; j++) {
            printf("\nPeão %d: Posição %d", j+1, jogadores[i].peoes[j].posição);
        }
    }
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
}

//Função que pergunta ao usuário qual peão deseja retirar da posição inicial, a função é chamada apenas quando o jogador não tem nenhum peão em jogo.
int primeira_jogada(int *retirar_peao) {
    printf("Digite o número do peão (1 a 4): ");
    scanf("%d", retirar_peao);
    return 1;
}

//Função que faz as jogadas dos possíveis 2º e 3º dado. 
int jogadas_dos_dados(int *num_do_dado, int *seis_consecutivos) {
    num_do_dado[1] = dado();
    num_do_dado[2] = dado();

    if (num_do_dado[1] == 6) {
        (*seis_consecutivos)++;
    }
    if (num_do_dado[2] == 6) {
        (*seis_consecutivos)++;
    }

    return 1;
}

//Função que permite o usuário decidir qual peão decide mover nos demais casos.
int escolhas_dos_peoes(int *movimentos_peao) {
    printf("Escolha um peão para mover: ");
    scanf("%d", movimentos_peao);
    return 1;
}

//Função que verifica se há mais de um peão sendo de jogadores distintos na mesma posição, e caso verdade, 'come o peão inimigo'.
void mesma_posição(Player jogadores[], int movimentos_peao, int *peoes_em_jogo) {
    
    for (int i = 0; i < NUMERO_DE_PEOES; i++) {
        if (jogadores[0].peoes[movimentos_peao - 1].posição == jogadores[1].peoes[i].posição && jogadores[1].peoes[i].posição !=0) {
            jogadores[1].peoes[i].posição = 0;
            peoes_em_jogo[1]--;
            printf("Peão do jogador 2 voltou à posição inicial.\n");
        } 
        else if (jogadores[1].peoes[movimentos_peao - 1].posição == jogadores[0].peoes[i].posição && jogadores[0].peoes[i].posição !=0) {
            jogadores[0].peoes[i].posição = 0;
            peoes_em_jogo[0]--;
            printf("Peão %d do jogador 1 voltou à posição inicial.\n");
        }
    }
}

//Início do main.
int main() {
    char cor_jogadores[4][10] = {"VERDE", "VERMELHO", "AZUL", "AMARELO"};
    int num_de_jogadores, valor_dado = 0;
    Player jogadores[NUMERO_DE_JOGADORES];
    int retirar_peao = 0;
    int movimentos_peao = 0;
    int seis_consecutivos = 0;
    int peoes_em_jogo[2] = {0, 0};
    int num_do_dado[3];
    int casas_seguras[8] = {1, 9, 14, 22, 27, 35, 40, 48};
    int peao_no_fim[2] = {0,0};

    printf("Quantos jogadores vão jogar? Disponível apenas 2. ");
    scanf("%d", &num_de_jogadores);

    validar_numero_jogadores(num_de_jogadores);

    srand(time(NULL));

    inicializador(jogadores, num_de_jogadores);
    bool jogo = true;

//Enquanto verdade o jogo roda.
    while (jogo) {
        int i, j;

        for (i = 0; i < num_de_jogadores; i++) {
            tabuleiro(jogadores, num_de_jogadores, cor_jogadores);
            printf("Vez do jogador %s:\n\n", cor_jogadores[i]);
            valor_dado = dado();
            printf("O 1º dado saiu %d.\n",valor_dado);
//Enquanto o jogador não tiver nenhum peão já em jogo e o dado for diferente de 6, passa o jogador e o dado é jogado novamente.
            if (peoes_em_jogo[i] == 0) {
                if (valor_dado != 6) {
                    continue;
                } else {
                    do {
                        primeira_jogada(&retirar_peao);
                    }while (retirar_peao < 1 || retirar_peao > 4);
                    seis_consecutivos++;
                    peoes_em_jogo[i]++;
//Quando o primeiro dado saiu 6 no primeiro movimento do jogador, o peão escolhido automaticamente tem como posição 1.
                    jogadores[i].peoes[retirar_peao - 1].posição = 1; 
                    tabuleiro(jogadores, num_de_jogadores, cor_jogadores);
                    jogadas_dos_dados(num_do_dado, &seis_consecutivos);
                    if (num_do_dado[1] != 6) {
                        printf("O 2º dado saiu %d\n\n", num_do_dado[1]);
//Caso logo após o jogador retirar o seu primeiro peão, ele consiga um outro dado != 6 o valor é somado automaticamente ao único peão em jogo.
                        jogadores[i].peoes[retirar_peao - 1].posição += num_do_dado[1];
                        mesma_posição(jogadores, movimentos_peao, peoes_em_jogo);
//Caso o jogador tenha 3 jogadas para fazer, ele decide se quer mover ou tirar algúm peão da posição inicial.
                    } else if (seis_consecutivos < 3) {
                        tabuleiro(jogadores, num_de_jogadores, cor_jogadores);
                        printf("O 2º dado saiu %d\n", num_do_dado[1]);
                        do {
                            printf("Faça um movimento pro valor %d: \n",num_do_dado[1]);
                            primeira_jogada(&retirar_peao);
                        } while (retirar_peao < 1 || retirar_peao > 4);
//2º jogada. Se o jogador escolher um peão que está em uma posição != de 0, o valor do dado é somado ao valor do peão.
                        if (jogadores[i].peoes[retirar_peao - 1].posição > 0) {
                            jogadores[i].peoes[retirar_peao - 1].posição += num_do_dado[1];
//2º jogada. Se o jogador escolher um peão que está em uma posição == 0, o peão escolhido tem como valor = 1.
                        } else {
                            peoes_em_jogo[i]++;
                            jogadores[i].peoes[retirar_peao - 1].posição = 1;
                        }
                        tabuleiro(jogadores, num_de_jogadores, cor_jogadores);
                        printf("O 3º dado saiu %d\n", num_do_dado[2]);
                        do {
                            printf("Faça um movimento pro valor %d:\n",num_do_dado[2]);
                            escolhas_dos_peoes(&movimentos_peao);
                        } while (movimentos_peao < 1 || movimentos_peao > 4 && jogadores[i].peoes[movimentos_peao - 1].posição == 0);
//3º jogada, o valor do dado será somado ao peão escolhido, desde que o mesmo esteja fora da posição inicial.
                        jogadores[i].peoes[movimentos_peao - 1].posição += num_do_dado[2];
                    } 
//Caso os 3 dados sejam == 6, o jogador não faz nenhum movimento com os dados e a jogada é passada para o próximo jogador.
                    else {
                        printf("Tirou 3 números 6 consecutivos.\nPerdeu a vez.\n");
                    }
                }
            }
//Caso o jogador tenha apenas uma jogada disponível. O valor do dado é somado a um peão que já esteja em jogo.
            else if (peoes_em_jogo[i] > 0 && peoes_em_jogo[i] < 5) {
                seis_consecutivos = 0;
                num_do_dado[0] = valor_dado;
                jogadas_dos_dados(num_do_dado, &seis_consecutivos);
                if (num_do_dado[0] != 6) { // Se o dado for != de 6 o jogador só tem uma jogada
                    do {
                        escolhas_dos_peoes(&movimentos_peao);
                    } while ((movimentos_peao < 1 || movimentos_peao > 4) || jogadores[i].peoes[movimentos_peao - 1].posição ==0);
                    jogadores[i].peoes[movimentos_peao - 1].posição += num_do_dado[0];
                    mesma_posição(jogadores, movimentos_peao, peoes_em_jogo);
                } 
//Caso o jogador tenha duas jogadas disponíveis. Se o dado 1 for 6 e o segundo for != de 6, o jogador pode escolher mover ou retirar um peão da posição inicial com o primeiro peão, já com o segundo só é possível mover com um peão já em jogo.
                else if (num_do_dado[0] == 6 && num_do_dado[1] != 6) {
                    seis_consecutivos++;
                    printf("O 2º dado saiu %d.\n\n",num_do_dado[1]);
                    do {
                        printf("Faça um movimento pro valor %d:\n ",num_do_dado[0]);
                        escolhas_dos_peoes(&movimentos_peao);
                    } while (movimentos_peao < 1 || movimentos_peao > 4);
//Se o jogador escolher um peão que está em uma posição != de 0, o valor do dado é somado ao valor do peão.
                    if (jogadores[i].peoes[movimentos_peao - 1].posição != 0) {
                        jogadores[i].peoes[movimentos_peao - 1].posição += num_do_dado[0];
                        mesma_posição(jogadores, movimentos_peao, peoes_em_jogo);
                    } 
//Se o jogador escolher um peão que está em uma posição == 0, o peão escolhido tem como valor = 1.
                    else {
                        peoes_em_jogo[i]++;
                        jogadores[i].peoes[movimentos_peao - 1].posição = 1;
                        mesma_posição(jogadores, movimentos_peao, peoes_em_jogo);
                    }
                    tabuleiro(jogadores, num_de_jogadores, cor_jogadores);
//O segundo valor do dado só é somada a um peão que já está em uma posição != 0.
                    do {
                        printf("Faça um movimento pro valor %d: \n",num_do_dado[1]);
                        escolhas_dos_peoes(&movimentos_peao);
                    } while (movimentos_peao < 1 || movimentos_peao > 4 || jogadores[i].peoes[movimentos_peao - 1].posição == 0);
                    jogadores[i].peoes[movimentos_peao - 1].posição += num_do_dado[1];
                    mesma_posição(jogadores, movimentos_peao, peoes_em_jogo);
                }
//Caso o jogador tenha 3 jogadas para fazer. Permite nas jogadas de valor 6 decidir se irá tirar um peão ou mover um já em jogo. E na outra jogada só é possível mover o peão que esteja em jogo.
                else if (num_do_dado[0] == 6 && num_do_dado[1] == 6 && num_do_dado[2] != 6) {
                    printf("O 2º dado saiu %d\n", num_do_dado[1]);
                    printf("O 3º dado saiu %d\n", num_do_dado[2]);
                    do {
                    printf("Faça um movimento pro valor %d\n",num_do_dado[0]);
                    escolhas_dos_peoes(&movimentos_peao);
                } while (movimentos_peao < 1 || movimentos_peao > 4);
//1º jogada. Se o jogador escolher um peão que está em uma posição != de 0, o valor do dado é somado ao valor do peão.
                if (jogadores[i].peoes[movimentos_peao - 1].posição != 0) {
                    jogadores[i].peoes[movimentos_peao - 1].posição += num_do_dado[0];
                    mesma_posição(jogadores, movimentos_peao, peoes_em_jogo);
                } 
//1º jogada. Se o jogador escolher um peão que está em uma posição == 0, o peão escolhido tem como valor = 1.
                else {
                    peoes_em_jogo[i]++;
                    jogadores[i].peoes[movimentos_peao - 1].posição = 1;
                    mesma_posição(jogadores, movimentos_peao, peoes_em_jogo);
                }
                tabuleiro(jogadores, num_de_jogadores, cor_jogadores);
                do {
                    printf("Faça um movimento pro valor %d: \n",num_do_dado[1]);
                    escolhas_dos_peoes(&movimentos_peao);
                } while (movimentos_peao < 1 || movimentos_peao > 4);
//2º jogada. Se o jogador escolher um peão que está em uma posição != de 0, o valor do dado é somado ao valor do peão.
                if (jogadores[i].peoes[movimentos_peao - 1].posição > 0) {
                    jogadores[i].peoes[movimentos_peao - 1].posição += num_do_dado[1];
                    mesma_posição(jogadores, movimentos_peao, peoes_em_jogo);
                } 
//2º jogada. Se o jogador escolher um peão que está em uma posição == 0, o peão escolhido tem como valor = 1.
                else {
                    peoes_em_jogo[i]++;
                    jogadores[i].peoes[movimentos_peao - 1].posição = 1;
                    mesma_posição(jogadores, movimentos_peao, peoes_em_jogo);
                }
                tabuleiro(jogadores, num_de_jogadores, cor_jogadores);
                do {
                    printf("Faça um movimento pro valor %d: \n",num_do_dado[2]);
                    escolhas_dos_peoes(&movimentos_peao);
                } while (movimentos_peao < 1 || movimentos_peao > 4 || jogadores[i].peoes[movimentos_peao - 1].posição == 0);
//3º jogada, o valor do dado será somado ao peão escolhido, desde que o mesmo esteja fora da posição inicial.
                jogadores[i].peoes[movimentos_peao - 1].posição += num_do_dado[2];
                mesma_posição(jogadores, movimentos_peao, peoes_em_jogo);
                }    
//Caso os 3 dados sejam == 6, o jogador não faz nenhum movimento com os dados e a jogada é passada para o próximo jogador.
                else{
                    printf("O 1º dado saiu %d\n", num_do_dado[0]);
                    printf("O 2º dado saiu %d\n", num_do_dado[1]);
                    printf("O 3º dado saiu %d\n", num_do_dado[2]);
                    printf("3 números 6 consecutivos.\nPerdeu a vez.");
                }        
            }
//Verifica de houve um vencedor, considerando que para um jogador ganhar, todos os seus peões precisam estar em posições iguais ou maiores que 57.
            if(jogadores[0].peoes[0].posição >=57 && jogadores[0].peoes[1].posição >=57 && jogadores[0].peoes[2].posição >=57 && jogadores[0].peoes[3].posição >=57){
                    printf("FIM DE JOGO!");
                    printf("JOGADOR %s CAMPEÃO!",cor_jogadores[0]);
                    jogo = false;
            } 
            else if(jogadores[1].peoes[0].posição >=57 && jogadores[1].peoes[1].posição >=57 && jogadores[1].peoes[2].posição >=57 && jogadores[1].peoes[3].posição >=57){
                    printf("FIM DE JOGO!\n");
                    printf("JOGADOR %s CAMPEÃO!",cor_jogadores[1]);
                    jogo = false;
            }
        }
    }

    return 0;
}