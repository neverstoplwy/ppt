#include "pthread.h"
#include "errors.h"

typedef struct alarm_tag {
  int seconds;
  char message[64];
} alarm_t;

void * alarm_thread(void * arg)
{
  alarm_t * alarm = (alarm_t *) arg;
  int status;

  /* 分离线程的作用仅仅是告诉操作系统可以在线程结束时释放该线程的资源，包括线程的堆栈等 */
  status = pthread_detach(pthread_self());
  if (status != 0) err_abort(status, "Detach thread");
  sleep(alarm->seconds);
  printf("now the seconds is %d:", alarm->seconds);
  printf("(%d) %s \n", alarm->seconds, alarm->message);
  free (alarm);
  return NULL;
}


int main (int argc, char * argv[])
{

  int status;
  char line[128];
  alarm_t *alarm;
  pthread_t thread;

  /* err_abort 和 errno_abort 的参数个数不一样，主要原因是有的函数在出错的时候会把错误值写到全局变量的errno中，而有的是返回错误值 */
  /* do while(0) 这种语法是为了保持一个增加了abort也是一个语句，主要是为了宏定义考虑的 */
  while(1)
    {
      printf("Alarm >");

      if (fgets(line, sizeof(line), stdin) == NULL) exit(0); /* stdlib.h 正常退出进程，释放资源 */
      if (strlen(line)<=1) continue;
      alarm = (alarm_t*)malloc(sizeof(alarm_t)); /* 每创建一个线程就分配了一次的alarm */
      
      if (alarm == NULL)
        errno_abort ("Allocate alarm");

      if(sscanf(line,"%d %64[^\n]",&alarm->seconds,alarm->message)<2)
        {
          fprintf(stderr,"Bad Command!\n");
          free(alarm);
        }
      else
        {
          status = pthread_create(&thread,NULL,alarm_thread,alarm);
          if (status != 0) err_abort (status,"Create alarm thread");
        }
    }
}
