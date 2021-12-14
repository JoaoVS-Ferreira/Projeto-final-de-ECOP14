/João Vitor Souza Ferreira 
#include <pic18f4520.h>
#include "bits.h"
#include "config.h"
#include "lcd.h"
#include "keypad.h"
#include "ssd.h"
#include "timer.h"
#include "rgb.h"
#include "atraso.h"
//defines para facilitar entendimento codigo e controlar lcd
#define L_L1  0x80  //linha 1
#define L_OFF 0x08 //desliga
#define L_CLR 0x01 //limpa
#define L_L2  0xC0 //linha 2
#define L_ON  0x0F //liga

unsigned char verifica = 0; // variavel flag
unsigned char op = 0;
unsigned int tecla; // valor novo dchave
unsigned int tempo;
char resposta;
char gabarito;
char pos1;
short int x[] = {4, 2, 6, 8, 6, 9}; //numeros para valor x
short int y[] = {2, 1, 3, 8, 1, 5}; //numeros para valor y 


void luzes() {
    if (resposta == gabarito) { //caso acertou
        turnOn(2); //liga led rgb verde
        turnOff(1);
        turnOff(4);
        atraso_ms(3000);
        
    }else if(resposta != gabarito){ //caso errou
        turnOn(1); //liga led rgb vermelho
        turnOff(2);
        turnOff(4);
        atraso_ms(3000);
    }
}

void leTeclado(void) {
    if ((kpRead() != tecla) && (kpReadKey!=0)) { //checa se apertou tecla
        tecla = kpRead(); //recebe antigo endereço
        switch (kpReadKey()) { //recebe valor da tecla
            case 'U': // somar
                ssdUpdate();
                resposta = x[pos1] + y[pos1];
                lcdCommand(L_L2);
                lcdString("  +" );
                break;
            case 'L': // subtrair
                ssdUpdate();
                resposta = x[pos1] - y[pos1];
                lcdCommand(L_L2);
                lcdString("  -" );
                break;
            case 'D': // multiplicar
                ssdUpdate();
                resposta = x[pos1] * y[pos1];
                lcdCommand(L_L2);
                lcdString("  *" );
                break;
            case 'R': // dividir
                ssdUpdate();
                resposta = x[pos1] / y[pos1]; //calcula resposta do usuario
                lcdCommand(L_L2); //escolhe segunda linha
                lcdString("  /" ); //imprime na tela
                break;
            case 's': //prox
                luzes(); //chama rgb
                pos1++; //troca a posição no vetor de numeros
                geraconta(); //chama função para gerar novo desafio
                tempo = 0; //reseta timer
                break;
            default:
                break;
        }//end switch 
    }
}//end le teclado



int geraconta() {
    switch (op) { //gera a equação após apertar prox
        case 0:
            gabarito = 8; //gabarito da equação
            lcdCommand(L_CLR); //limpa display
            lcdString("4 ? 2 = 8"); //imprime equação na força bruta para economizar tempo e recursos
            op = 1; //proxima equação
            break;
        case 1:
            gabarito = 3;
            lcdCommand(L_CLR);
            lcdString("2 ? 1 = 3");
            op = 2;
            break;
        case 2:
            gabarito = 2;
            lcdCommand(L_CLR);
            lcdString("6 ? 3 = 2");
            op = 3;
            break;
        case 3:
            gabarito = 1;
            lcdCommand(L_CLR);
            lcdString("8 ? 8 = 1");
            op = 4;
            break;
        case 4:
            gabarito = 5;
            lcdCommand(L_CLR);
            lcdString("6 ? 1 = 5");
            op = 5;
            break;
        case 5:
            gabarito = 45;
            lcdCommand(L_CLR);
            lcdString("9 ? 5 = 45");
            op = 0;
            break;
        default:
            break;
    }//end switch
    
}//end equacao

void cronometro(void) {
    ssdUpdate();
    int v0, v1, v2, v3;
    v0 = ((tempo / 10000) % 10); //calcula valores de tempo para cada ssd
    v1 = ((tempo / 1000) % 10);
    v2 = ((tempo / 100) % 10);
    v3 = ((tempo / 10)%10);
    ssdDigit(v0, 0); //mostra valores em cada ssd
    ssdDigit(v1, 1);
    ssdDigit(v2, 2);
    ssdDigit(v3, 3);
}//end mostra display

void main(void) {
    char slot;
    lcdInit(); //inicia perifericos
    kpInit();
    ssdInit();
    timerInit();
    geraconta(); //gera desafio inicial
    for (;;) {
        kpDebounce(); //realiza debounce no teclado
        ssdUpdate(); //atualiza display ssd
        atraso_ms(10);
        tempo+=1; //adcciona + 1 ao tempo
        switch (slot) {
            case 0:
                leTeclado();
                slot = 1;
                break;
            case 1:
                ssdUpdate(); //atualiza display ssd
                slot = 2;
                break;
            case 2:
                cronometro(); //recalcula valores para ssd
                slot = 0;
                break;
            default:
                slot = 0;
                break;
        }//end switch
        ssdUpdate(); //atualiza display ssd
        atraso_ms(10); 
    }//end for
}//end main
