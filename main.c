#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"
#include "hps_0.h"

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )

int balance;
int pScore;
int dScore;
int pot;
int cardTypes[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 11};

void change_led();
void play();
void turn(int, int);
void hit_dealer();
void dealer();
int getCard();
char hit_or_stand();

void *virtual_base;
int fd;
void *h2p_lw_led_addr;

// map the address space for the LED registers into user space so we can interact with them.
// we'll actually map in the entire CSR span of the HPS since we want to access various registers within that span

int main(){
  printf("Welcome to the FPGA BlackJack Simulator!\n");
  if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
    printf( "ERROR: could not open \"/dev/mem\"...\n" );
    return( 1 );
  }

  virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );

  if( virtual_base == MAP_FAILED ) {
    printf( "ERROR: mmap() failed...\n" );
    close( fd );
    return( 1 );
  }

  h2p_lw_led_addr=virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + PIO_0_BASE ) & ( unsigned long)( HW_REGS_MASK ) );

  balance = 0x005;
  srand(time(0));
  *(uint32_t *)h2p_lw_led_addr = ~balance;
  play();
  	// clean up our memory mapping and exit

  if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
    printf( "ERROR: munmap() failed...\n" );
    close( fd );
    return( 1 );
  }
  close( fd );
  return 0;
}

void play(){
//  printf("Beginning game...\n500 credits added to your account\n");
  printf("Beginning game...\n");
  int card1 = 0;
  int card2 = 0;
  pot = 0;
  while(balance > 0x000){
    printf("----- NEW ROUND -----\n");
	printf("Balance: %x\n", balance);
    card1 = getCard();
    card2 = getCard();
    pScore = card1 + card2;
//    balance += 0x001;
    usleep(1000*1000);
    turn(card1, card2);
    change_led();
  }
}

void turn(int mCard1, int mCard2){
  int decision;
//  printf("Two cards: %i - %i\n", mCard1, mCard2);
  printf("Your Current Score: %i\n", mCard1 + mCard2);
  decision = hit_or_stand();
  if(decision == 0){
    int newCard;
    while(decision == 0){
      newCard = getCard();
      pScore += newCard;
      if(pScore > 21){
        balance -= 0x001;
        break;
      }
      decision = hit_or_stand();
    }
  }

  if(decision == 1){
    dealer();
    if(pScore > dScore){
      balance += 0x001;
    }
    else{
      balance -= 0x001;
    }
    if(pScore == 21){
      balance += 0x001;
    }
    else if(pScore > dScore || dScore > 21){
      balance += 0x001;
    }
    else if(pScore == dScore){
      // DO NOTHING
    }
    else{
      balance -= 0x001;
    }
  }
}


int getCard(){
  int card = (rand() % (12));
  return cardTypes[card];
}

void dealer(){
  int deal1 = getCard();
  int deal2 = getCard();
  dScore = deal1 + deal2;
  while(dScore < 15){
    hit_dealer();
  }
}

void hit_dealer(){
  int nCard = getCard();
  dScore += nCard;
}

char hit_or_stand(){
  int hs;
  while(hs != 1 && hs != 0){
    printf("Would you like to hit or stand h(0)/s(1)?\n");
    scanf("%i", &hs);
  }
  return hs;
}

void change_led(){
  *(uint32_t *)h2p_lw_led_addr = ~balance;
}