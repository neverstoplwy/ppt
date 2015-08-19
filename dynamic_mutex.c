/* 通常不静态初始化一个pthread_mutex_t 类型，而使用maclloc动态的分配一个含有互斥量的数据结构 */
/* 用malloc分配时通常使用pthread_mutex_init函数来初始化对应的phread_mutex_t 变量 */

#include "pthread.h"
#include "errors.h"

typedef struct my_struct_tag{
  pthread_mutex_t mutex;
  int value;
} my_struct_t;



int main()
{
  my_struct_t * data;
  int status;

  data  = (my_struct_t *)malloc(sizeof(my_struct_t));
  if(data == NULL)
    errno_abort("Allocate structure");

  /* 第二个参数const pthread_mutexattr_t *mutexattr,其中mutexattr用于指定互斥锁属性（见下），如果为NULL则使用缺省属性。 */
  status = pthread_mutex_init(&data->mutex,NULL);
  if(status != 0)
    err_abort(status,"Init mutex");

  status = pthread_mutex_destroy(&data->mutex);
  if(status != 0)
    err_abort(status,"Destroy mutex");

  free(data);

  return 0;

}
