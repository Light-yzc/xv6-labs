#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

void run_exec(char *cmd_argv[], char *path) {
  char *args[MAXARG];
  int i;

    for(i = 0; cmd_argv[i] != 0 && i < MAXARG - 2; i++)
        args[i] = cmd_argv[i];

    args[i] = path;
    i++;
    args[i] = 0;

    if(fork() == 0) {
        exec(args[0], args);
        fprintf(2, "exec %s failed\n", args[0]);
        exit(1);
    }
    wait(0);

}

char*
basename(char *path)
{
  char *p;

  for(p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  return p + 1;
}


void find(char *path, char *target, char **cmd) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;


    
    if((fd = open(path, O_RDONLY)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type == T_FILE) {
         char* file_name = basename(path);
         if (strcmp(file_name, target) == 0) {
            if(cmd == 0) {
                printf("%s\n", path);
            }
            else
            {
                run_exec(cmd, path);
            }
            
         }
    }
    else if (st.type == T_DIR)
    {
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("ls: cannot stat %s\n", buf);
            continue;
        }
        // printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, (int) st.size);
        find(buf, target, cmd);
        }
    }
    close(fd);
    
}


int main(int argc, char *argv[]) {

    if(argc < 3){
    fprintf(2, "Usage: find path name\n");
    exit(1);
    }
    if(argc == 3){
    find(argv[1], argv[2], 0);
    } else if(argc > 4 && strcmp(argv[3], "-exec") == 0){
    find(argv[1], argv[2], &argv[4]);
    } else {
    fprintf(2, "Usage: find path name [-exec cmd args...]\n");
    exit(1);
    }
}