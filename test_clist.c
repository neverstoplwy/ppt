/* 该代码是用来学习测试C语言的列表的 */
/* 这个例子中是在增加链表元素时顺便排序了 */

#include "errors.h"
#include "time.h"

typedef struct alarm_tag{
  struct alarm_tag * link;
  int seconds;
  char message[64];
  time_t time;
} alarm_t;
alarm_t *alarm_list = NULL;

int main(int argc, char * argv[])
{
  char line[128];
  /* 注意这里用一个指向指针的指针来表示链表，不使用的话alarm_list的值得不到更新 */
  /* 对链表进行操作：alarm_list表示一个不变的链表，而申请last代表的是一个『链表变量』，尤其是在排序插入时，所以要用指向链表的指针 */
  alarm_t *alarm;               /* 代表要加入到链表的新建立的节点 */
  alarm_t **last;               /* 代表什么 指当前的链表？为什么用指针的指针*/
  alarm_t *next;                /* 代表什么 */

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
      }
      
      /* 输出链表的值，用来测试使用 */
      for (alarm_t *p = alarm_list; p!= NULL; p = p->link) {
        printf("时间:%s   信息:%s",ctime(&p->time),p->message);
        printf("\n\n");
      }
    }
  return 0;
}
