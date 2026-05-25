#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char buf[1];
// char buf_string[];

int
is_digit(char c)
{
  return c >= '0' && c <= '9';
}

void sixfive(int fd) {
    int n = 0;
    int valid = 1;
    int num = 0;
    int has_num = 0;
    while((n = read(fd, buf, sizeof(buf))) > 0)
    {
        char c = buf[0];
        if (c != ' ' && c != '-' && c != '\r' && c != '\t' &&
         c != '\n' && c != '.' && c != '/' && c != ',') {
            if (is_digit(c) == 0) {
                valid = 0;
                has_num = 0;

            }
            else {
                num = num * 10 + (c - '0');
                valid = 1;
                has_num = 1;
            }
        }
        else {
            if (valid && has_num) {
                if (num % 5  == 0 || num % 6 == 0) {
                    printf("%d\n", num);
                }
            }
            valid = 1;
            num = 0;
            has_num = 0;
        }
    }
    if (valid && has_num) {
        if (num % 5  == 0 || num % 6 == 0) {
            printf("%d\n", num);
            }
        }
    
}
int main(int argc, char *argv[]) {
    int fd;
    if (argc < 2) {
        fprintf(2, "arg error\n");
        exit(0);
    }
    
    for (int i = 1; i < argc; i++) {
        if((fd = open(argv[i], O_RDONLY)) < 0){
        fprintf(2, "cannot open %s\n", argv[1]);
            exit(1);
        }
        sixfive(fd);
        close(fd);
    }



    exit(0);
}
