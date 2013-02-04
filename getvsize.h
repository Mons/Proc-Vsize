#ifndef _PROC_VSIZE_H_
#define _PROC_VSIZE_H_

#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
              
extern int getvsize ( pid_t pid, size_t *vsize, size_t *rss );

#endif // _PROC_VSIZE_H_
