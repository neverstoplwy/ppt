/* 线程生命周期 */
/* 该例子演示了thread_join函数的作用 */
#include "pthread.h"
#include "errors.h"

void * thread_routine (void * arg)
{
  sleep(10);
  return arg;
}

int main ()
{
  pthread_t thread_id;
  void * thread_result;
  int status;

  status = pthread_create(&thread_id, NULL, thread_routine,NULL);
  if (status != 0)
    err_abort (status,"Create alarm thread");
  /* pthread_join 阻塞，等待所有线程都执行完成再返回，防止进程比线程退出的早 */
  /* 当pthread_join 返回时，这个线程已经呗分离了*/
  status = pthread_join(thread_id,&thread_result);

  if (status != 0)
    err_abort (status,"join thread");

  /* thread_result 应当和 return arg 相同 */
  if (thread_result == NULL)
    return 0;
  else
    return 1;
  
}

