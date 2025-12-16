#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
   struct stat fstatbuf;
   if(argc!=2) {
       fprintf(stderr, "Usage : ex01 filename\n");
       exit(1);
   }
   if(stat(argv[1],&fstatbuf)==-1) {
       perror(argv[1]);
       exit(1);
   }
   if(S_ISDIR(fstatbuf.st_mode))
       printf("%s is Directory\n", argv[1]);
   else if(S_ISREG(fstatbuf.st_mode))
       printf("%s is Regular File\n", argv[1]);
   else if(S_ISCHR(fstatbuf.st_mode))
       printf("%s is Character Device File\n", argv[1]);
   else if(S_ISBLK(fstatbuf.st_mode))
       printf("%s is Block Device File\n", argv[1]);
   printf("Permission :");
   if(fstatbuf.st_mode & S_IRUSR)
       putchar('r');
   if(fstatbuf.st_mode & S_IWUSR)
       putchar('w');
   if(fstatbuf.st_mode & S_IXUSR)
       putchar('x');
   putchar('\n');
   printf("inode = %ld, uid = %d, size = %ld\n",\
       fstatbuf.st_ino, fstatbuf.st_uid, fstatbuf.st_size);
   return 0;
}