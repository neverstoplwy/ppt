/* pthread_mutex_t类型可以声明为静态的，静态的需要用宏来初始化 */
/* 通常情况下会把数据和mutex放到一起 */
#include "pthread.h"
#include "errors.h"

typedef struct my_struct_tag{
  pthread_mutex_t mutex;
  int value;
} my_struct_t;

my_struct_t data = {PTHREAD_MUTEX_INITIALIZER, 0};

int main()
{
  return 0;
}
