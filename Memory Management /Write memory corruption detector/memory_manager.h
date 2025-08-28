#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>  // size_t

#ifdef __cplusplus
extern "C" {
#endif

void  setup_memory_pool(void);
void* my_malloc(int bytes_needed);
void  my_free(void* user_pointer);
int   check_memory_corruption(void);
void  show_memory_stats(void);

#ifdef __cplusplus
}
#endif

#endif // MEMORY_MANAGER_H
