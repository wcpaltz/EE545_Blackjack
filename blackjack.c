#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include <string.h>
#define CARDS 52

int balance;
int pScore;
int dScore;
int pot;
int cardTypes[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10};

int bet();
void play();
void turn(int, int);
int hit();
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
  printf("Beginning game...\n500 credits added to your account\n");
  balance = 500;
  int card1 = 0;
  int card2 = 0;
  pot = 0;
  while(balance > 0){
    bet();
    card1 = getCard();
    card2 = getCard();
    pScore = card1 + card2;
    turn(card1, card2);
    balance = 0;
  }
}

void turn(int mCard1, int mCard2){
  char decision;
  printf("Inside turn()...\n");
  printf("Two cards: %i - %i\n", mCard1, mCard2);
  printf("Current Score: %i\n", mCard1 + mCard2);
  dealer();
  decision = hit_or_stand();
  if(decision == 'h'){
    int newCard;
    while(decision == 'h'){
      newCard = getCard();
      pScore += newCard;
      if(pScore > 21){
        printf("BUST!\n");
        decision = 'b';
        break;
      }
      printf("New score: %i\n", pScore);
      decision = hit_or_stand();
    }
  }
  
  if(decision == 's'){
    printf("STAND\n");
    if(pScore > dScore){
      printf("dealer score: %i\n", dScore);
      printf("Player WINS!\n");
    }
    else if(pScore == dScore){
      printf("TIE!\n");
    }
    else{
      printf("You LOSE\n");
    }
  }
}

void stand(){
  // DO NOTHING  
}

int getCard(){
//  printf("Inside getCard()...\n");
  int card = (rand() % (12));
//  printf("Card dealt: %i\n", cardTypes[card]);
  return cardTypes[card];
}

void dealer(){
//  printf("Inside dealer()...\n");
  int deal1 = getCard();
  int deal2 = getCard();
  dScore = deal1 + deal2;
  printf("Dealer cards: %i - %i\n", deal1, deal2);
  printf("Dealer Score: %i\n", deal1 + deal2);
  // TODO
}

int bet(){
//  printf("Inside bet()...\n");
  printf("Current balance: %i\n", balance);
  printf("Enter the amount you would like to bet.\n");
  int myBet = 0;
  scanf("%i", &myBet);
  while(myBet >= balance){
    printf("Your bet must be less than your current token balance.\n");
    scanf("%i\n", &myBet);
  }
  balance -= myBet;
  printf("You made a bet of %i. Good Luck!\n", myBet);
  pot = myBet;
  return 0;
}

int hit(){
  printf("Inside hit()...\n");
  return 0;
  // TODO
}

int ace(){
  printf("Inside ace()...\n");
  return 0;
}

char hit_or_stand(){
  char hs;
  printf("Would you like to hit or stand (h/s)?\n");
  scanf("%s", &hs);
  while(hs != 'h' && hs != 's'){
    printf("Would you like to hit or stand (h/s)?\n");
    scanf("%s", &hs);
  }
  return hs;
}