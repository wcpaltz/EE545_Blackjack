#include <stdio.h>
#include <stdlib.h>

int starting_value = 0x005;

// function to convert decimal to hexadecimal 
void decToHexa()
{    
	starting_value += 0x002;
	while(starting_value < 0x006){
		starting_value += 0x002;
		printf("Value %x\n", starting_value);
//		sleep(1);
	}
}

// Driver program to test above function 
int main() 
{
  printf("Value %x\n", starting_value);
  decToHexa();
  printf("Value %x\n", starting_value);
} 