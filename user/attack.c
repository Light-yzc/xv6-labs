#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

#define DATASIZE (8*4096)


int
main(int argc, char *argv[])
{
  // Your code here.
  char *marker = "This may help.";
  int size = strlen(marker);
  int i = 0;
  char *data = sbrk(DATASIZE * 2);
  while (i < 2 * DATASIZE)
  {
    if (memcmp(&data[i], marker, size) == 0){
      // printf("find！！");
      printf("%s\n", &data[i + size + 2]);
      exit(0);
    }
    i++;
  }
  
  exit(1);
}
