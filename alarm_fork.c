#include "errors.h"
int main (int argc, char * argv[])
{
  int seconds = 2;
  char line[128];
  char message[64];

  int status;
  pid_t pid;

  
  while(1)
    {
      printf("Alarm >");

      //char *fgets(char *buf, int bufsize, FILE *stream);
      //从文件结构体指针stream中读取数据，每次读取一行。
      //读取的数据保存在buf指向的字符数组中，每次最多读取bufsize-1个字符（第bufsize个字符赋'\0'），如果文件中的该行不足bufsize个字符，则读完该行就结束。
      //如若该行（包括最后一个换行符）的字符数超过bufsize-1，则fgets只返回一个不完整的行，但是，缓冲区总是以NULL字符结尾，对fgets的下一次调用会继续读该行
      
      if (fgets(line, sizeof(line), stdin) == NULL) exit(0); /* stdlib.h 正常退出进程，释放资源 */
      if (strlen(line)<=1) continue;
      
      //sscanf与scanf类似，都是用于输入的，只是后者以键盘(stdin)为输入源，前者以固定字符串为输入源.
      //输入任意字符，最多64个，遇到回车结束
      if(sscanf(line,"%d %64[^\n]",&seconds,message)<2) {
      	fprintf(stderr,"Bad Command!\n");
      } else {
	/* fork函数总是“调用一次，返回两次”，在父进程中调用一次，在父进程和子进程中各返回一次。
	   fork在子进程中的返回值是0，而在父进程中的返回值则是子进程的id。搜索
	   子进程在创建的时候会复制父进程的当前状态（PCB信息相同，用户态代码和数据也相同）。 */
	pid = fork();
	if (pid == (pid_t)-1) errno_abort("Fork");
	if (pid == (pid_t)0) {
	  /* in the child */
	  sleep(seconds);
	  printf("(%d)%s\n",seconds,message);
	}else {
	  /* in the parent */
	  do {
	    /* -1表示等待任何子进程，NULL表示子进程结束状态值，WNOHANG表示在等待进程没有结束也不等待,waitpid()函数返回0 */
	    /* 子进程负责sleep和打印信息就退出了，父进程一直负责着输入和控制 */
	    pid = waitpid((pid_t)-1,NULL,WNOHANG);
	    if (pid == (pid_t)-1) errno_abort("Wait for child");
	    printf("the parent progress wait is :%d",pid);
	    sleep(1);
	  }while (pid != (pid_t)0);
	}
      }
    }

}


