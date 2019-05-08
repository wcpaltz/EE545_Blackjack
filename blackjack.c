#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include <string.h>
#define CARDS 52

// FPGA VARIABLES
volatile int * LED_ptr = (int *) 0xFF200000; // red LED address
volatile int * HEX7_HEX0_ptr = (int *) 0xFF200020; // HEX3_HEX0 address
volatile int * SW_switch_ptr = (int *) 0xFF200040; // SW slider switch address
volatile int * KEY_ptr = (int *) 0xFF200050; // pushbutton KEY address

int HEX_bits = 0x0000000F; // initial pattern for HEX displays


int wins;
int loses;
int balance;
int pScore;
int dScore;
int pot;
int cardTypes[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10};

void set_hex_display();
int check_key_1_press();

void play();
void turn(int, int);
void hit_dealer();
void dealer();
int getCard();
int ace();
void stand();
char hit_or_stand();

int main(){
  printf("Welcome to the FPGA BlackJack Simulator!\n");
  srand(time(0));
  play();
  return 0;
}

void play(){
//  printf("Beginning game...\n500 credits added to your account\n");
  printf("Beginning game...\n");
  balance = 500;
  int card1 = 0;
  int card2 = 0;
  pot = 0;
  while(balance > 0){
    printf("----- NEW ROUND -----\n");
    printf("WINS: %i - LOSES: %i\n", wins, loses);
    card1 = getCard();
    card2 = getCard();
    pScore = card1 + card2;
    turn(card1, card2);
    char ys;
    while(ys != 'y' && ys != 'n'){
      printf("Would you like to be done? (y/n)\n");
      scanf("%s", &ys);
    }
    if(ys == 'y'){
      balance = 0;
    }
  }
}

void turn(int mCard1, int mCard2){
  char decision;
//  printf("Two cards: %i - %i\n", mCard1, mCard2);
  printf("Your Current Score: %i\n", mCard1 + mCard2);
  decision = hit_or_stand();
  if(decision == 'h'){
    int newCard;
    while(decision == 'h'){
      newCard = getCard();
      pScore += newCard;
      if(pScore > 21){
        printf("Player score: %i\n", pScore);
        printf("BUST!\n");
        decision = 'b';
        loses++;
        break;
      }
      printf("New score: %i\n", pScore);
      decision = hit_or_stand();
    }
  }
  
  if(decision == 's'){
    dealer();
    if(pScore == 21){
      printf("Player score: %i\n", pScore);
      printf("BLACKJACK! Player WINS!\n");
      wins++;
    }
    else if(pScore > dScore || dScore > 21 || pScore == 21){
      printf("Player score: %i\n", pScore);
      printf("Dealer score: %i\n", dScore);
      printf("Player WINS!\n");
      wins++;
    }
    else if(pScore == dScore){
      printf("Player score: %i\n", pScore);
      printf("Dealer score: %i\n", dScore);
      printf("TIE!\n");
    }
    else{
      printf("Player score: %i\n", pScore);
      printf("Dealer score: %i\n", dScore);
      printf("You LOSE\n");
      loses++;
    }
  }
}

void stand(){
  // DO NOTHING  
}

int getCard(){
  int card = (rand() % (12));
  return cardTypes[card];
}

void dealer(){
  int deal1 = getCard();
  int deal2 = getCard();
  dScore = deal1 + deal2;
//  printf("Dealers Turn...");
  while(dScore < 15){
    hit_dealer();
  }
}

// DON'T USE ANYMORE
//int bet(){
////  printf("Inside bet()...\n");
//  printf("Current balance: %i\n", balance);
//  printf("Enter the amount you would like to bet.\n");
//  int myBet = 0;
//  scanf("%i", &myBet);
//  while(myBet >= balance){
//    printf("Your bet must be less than your current token balance.\n");
//    scanf("%i\n", &myBet);
//  }
//  balance -= myBet;
//  printf("You made a bet of %i. Good Luck!\n", myBet);
//  pot = myBet;
//  return 0;
//}

int ace(){
  printf("Inside ace()...\n");
  return 0;
  // TODO
}

void hit_dealer(){
  int nCard = getCard();
  dScore += nCard;
}

char hit_or_stand(){
  char hs;
  while(hs != 'h' && hs != 's'){
    printf("Would you like to hit or stand (h/s)?\n");
    scanf("%s", &hs);
  }
  return hs;
}

int check_key_1_press(){
  // return 1 if key pressed else return 0
  if (*KEY_ptr != 0){ // KEY PRESSED
    return 1;
  }
  else{
    return 0;
  }
}

void set_hex_display(){
  int HEX_bits = 0x00000000;
  *(HEX7_HEX0_ptr) = HEX_bits;
}