#include <stdio.h>

int main(void)
{
   char cmdbuf[256];
   char pathbuf[1024];
   
   mkdir("testdir", 0755);
   strcpy(cmdbuf, "ls -l | grep ");
   strcat(cmdbuf,"testdir");
   printf("%s\n", cmdbuf);
   system(cmdbuf);
   
   chdir("testdir");
   if(getcwd(pathbuf,1023)==NULL) {
      perror("getwd");
      exit(1);
   }
   printf("Current Directory : %s\n", pathbuf);

   chdir("..");        // 상위 디렉토리로 이동
   rmdir("testdir");
   printf("%s\n", cmdbuf);
   system(cmdbuf);
   
   return 0;
}
