#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main () {
	pid_t pid = getpid();
	void *mem1;
	void *mem2;
	size_t st = (size_t)(-1);
	mem1 = malloc( 1024*1024*1024 );
	mem2 = malloc( 1024*1024*1024 );
	printf("ok: %d (%p, %p) (max=%zu)\n",pid, mem1,mem2,st);
	usleep(1000*1000000);
	return 0;
}
