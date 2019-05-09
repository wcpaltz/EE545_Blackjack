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

int wins;
int loses;
int balance;
int pScore;
int dScore;
int pot;
int cardTypes[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 11};

void turn(int, int);
void hit_dealer();
void dealer();
int getCard();
int ace();
void stand();
char hit_or_stand();

int main() {
	void *virtual_base;
	int fd;
	void *h2p_lw_led_addr;

	// map the address space for the LED registers into user space so we can interact with them.
	// we'll actually map in the entire CSR span of the HPS since we want to access various registers within that span
	
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
	

	// toggle the LEDs a bit

	balance = 0x005;
	printf("Beginning game...\n");
    int card1 = 0;
    int card2 = 0;
	*(uint32_t *)h2p_lw_led_addr = ~balance;
	usleep(1000*1000);
	while(balance > 0x000) {
		printf("----- NEW ROUND -----\n");
        printf("WINS: %i - LOSES: %i\n", wins, loses);
		card1 = getCard();
        card2 = getCard();
        pScore = card1 + card2;
        turn(card1, card2);
		// control led
		*(uint32_t *)h2p_lw_led_addr = ~balance;
		// wait 100ms
		usleep(1000*1000);
	}
	

	// clean up our memory mapping and exit
	
	if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
		printf( "ERROR: munmap() failed...\n" );
		close( fd );
		return( 1 );
	}

	close( fd );

	return( 0 );
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
//		balance -= 0x001;
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
//	  balance += 0x001;
      wins++;
    }
    else if(pScore > dScore || dScore > 21 || pScore == 21){
      printf("Player score: %i\n", pScore);
      printf("Dealer score: %i\n", dScore);
      printf("Player WINS!\n");
//	  balance += 0x001;
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
//      balance -= 0x001;
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