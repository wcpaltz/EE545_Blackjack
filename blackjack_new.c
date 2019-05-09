#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include <string.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
//#include "hps_0.h"

#define OUTPUT_SEVEN_SEGMENT_COMPONENT_TYPE   seven_segment
#define OUTPUT_SEVEN_SEGMENT_COMPONENT_NAME   output_seven_segment
#define OUTPUT_SEVEN_SEGMENT_BASE             0x50000
#define OUTPUT_SEVEN_SEGMENT_SPAN             1024
//#define OUTPUT_SEVEN_SEGMENT_END              0x403ff

void*	virtual_base;
int 	fd;
void*	addr_seven_seg;

int main(){
  while(1){
      if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
          printf("ERROR: could not open \"/dev/mem\"...\n");

          return 1;
      }

      virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, HW_REGS_BASE);

      if (virtual_base == MAP_FAILED) {
          printf("ERROR: mmap() failed...\n");
          close(fd);

          return 1;
      }

      addr_seven_seg = virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + OUTPUT_SEVEN_SEGMENT_BASE) & (unsigned long)(HW_REGS_MASK));

      alt_write_word(uint32_t addr, uint32_t data);
      alt_write_word(addr_seven_seg, 0x12F4A6);
      alt_write_word(addr_seven_seg+4, 0x150);  // Set brightness to 58%
      sleep(1);
      printf("Probably still not working");
  }
}
