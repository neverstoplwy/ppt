#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//unistd.h 是 C 和 C++ 程序设计语言中提供对 POSIX 操作系统 API 的访问功能的头文件的名称。
//该头文件由 POSIX.1 标准（单一UNIX规范的基础）提出，故所有遵循该标准的操作系统和编译器均应提供该头文件（如 Unix 的所有官方版本，包括 Mac OS X、Linux 等）
#include "unistd.h"
int main (int argc, char * argv[])
{
  int seconds = 2;
  char line[128];
  char message[64];
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

	//unistd.h
  	sleep(seconds);
	printf("(%d)%s\n",seconds,message);
      }
    }

}

