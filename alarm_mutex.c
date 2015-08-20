/*该程序用一个线程来实现闹钟 */
/*主线程将“闹钟”数据结构放入到工作链表中，工作线程读取链表,主线程和工作线程通过静态的互斥量控制对链表的访问 */
/* main返回时，线程『蒸发』 */
/* 存在的问题 */
#include "pthread.h"
#include "errors.h"
#include "time.h"

typedef struct alarm_tag{
  struct alarm_tag * link;
  int seconds;
  char message[64];
  time_t time;
} alarm_t;
pthread_mutex_t alarm_mutex = PTHREAD_MUTEX_INITIALIZER;
alarm_t *alarm_list = NULL;
/* the alarm thread start routine */
void * alarm_thread (void *arg)
{
  alarm_t * alarm;
  int sleep_time;
  time_t now;
  int status;

  while(1) {
    
    status = pthread_mutex_lock(&alarm_mutex);
    if (status !=0)
      err_abort(status,"Lock mutex");
    
    alarm = alarm_list;
    if (alarm == NULL)
      sleep_time = 1;
    else {
      alarm_list = alarm->link;
      now = time (NULL);
      if (alarm->time <= now)
        sleep_time = 0;
      else
        /* 在睡眠的时候收不到主线程新来的请求，如果睡了10s，而主线程立即增加了个2s后的闹钟，则不能响应 */
        sleep_time = alarm->time - now;

      /* 在编译的时候使用 gcc -DDEBUG alarm_mutex.c可以输出调试信息 */
#ifdef DEBUG
      printf("[waiting: %d(%d)\"%s\"]\n", alarm->time, sleep_time, alarm->message);
#endif
    }

    status = pthread_mutex_unlock (&alarm_mutex);
    if (status !=0)
      err_abort (status, "Unlock mutex");

    if (sleep_time >0)
      sleep (sleep_time);
    else
      /* 这个函数可以使用另一个级别等于或高于当前线程的线程先运行。
         如果没有符合条件的线程，那么这个函数将会立刻返回然后继续执行当前线程的程序。 */
      sched_yield ();

    if (alarm != NULL) {
      printf ("(%d) %s\n", alarm->seconds, alarm->message);
      free (alarm);
    }
  }
}
int main(int argc, char * argv[])
{
  int status;
  char line[128];
  alarm_t *alarm, **last, *next; 
  pthread_t thread;

  status = pthread_create(&thread,NULL,alarm_thread,NULL);
  if (status != 0)
    err_abort (status,"Create alarm thread");

  while (1)
    {
      printf("alarm>");
      
      if (fgets(line, sizeof(line), stdin) == NULL) exit(0); /* stdlib.h 正常退出进程，释放资源 */
      if (strlen(line)<=1) continue;

      /* 每输入一次就分配了一次的alarm,为了测试就不用在堆上分配内存 */
      
      alarm = (alarm_t*)malloc (sizeof (alarm_t)); 
            
      if(sscanf(line,"%d %64[^\n]",&alarm->seconds,alarm->message)<2){
        fprintf(stderr,"Bad Command!\n");
        free (alarm);
      }
      else {
        status = pthread_mutex_lock(&alarm_mutex);
        if (status != 0)
          err_abort(status,"Lock mutex");
        
        /* 返回1970到当前的秒数 */
        alarm->time = time (NULL) + alarm->seconds;

        last = &alarm_list;     /* 将链表的地址赋值给last,last代表了列表 */
        next = *last;           /* 将next初始化，用链表的地址 */

        /* 这里的while break和if结构，是典型的判断方式：第一次如何如何，以后分两种情况应当如何如何处理，和英语里的典型句型差不多.... */
        while (next != NULL) {
          if (alarm->time < next->time) { /* 新加入的节点时间和当前节点进行比较 */
            alarm->link = next;
            *last = alarm;      /* 重新将整个链表的首地址赋值给last; last = &alarm是错误的 */
            break;              /* 如果新加入的节点时间小于链表首节点的话 */
          }
          /* 如果新加入的节点时间大于链表首节点的话,两个指针向后移动，直到出现小于等于的情况 */
          last = &next->link;
          next = next->link;
        }
        
        if (next == NULL) {
          /* 第一次链表为空的时候，用alarm来初始化last */
          *last = alarm;        /* last = &alarm 是错误的 */
          alarm->link = NULL;
        }
#ifdef DEBUG
        printf("[list: ");
        for (next = alarm_list; next != NULL;next = next->link)
          printf ("%d(%d)[\"%s\"]",next->time,next->time - time(NULL),next->message);
        printf ("]\n");
#endif
        status = pthread_mutex_unlock(&alarm_mutex);
        if (status != 0)
          err_abort (status, "Unlock mutex");
      
        /* 输出链表的值，用来测试使用 */
        /* for (alarm_t *p = alarm_list; p!= NULL; p = p->link) { */
        /*   printf("时间:%s   信息:%s",ctime(&p->time),p->message); */
        /*   printf("\n\n"); */
      }
    }
  return 0;
}
