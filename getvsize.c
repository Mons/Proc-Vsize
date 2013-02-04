#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h> // For O_RDONLY
#include <errno.h>

#include "getvsize.h"

#if __FreeBSD__

#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/user.h>
#include <kvm.h>
#include <limits.h> // for _POSIX2_LINE_MAX

int PAGESIZE = getpagesize();

/*  Could someone help me with freebsd < 5 and rss size ? */

#if __FreeBSD__ >= 5
#	define KVM_PID(kp)   kp->ki_pid
#	define KVM_VSIZE(kp) kp->ki_size
#	define KVM_RSSSIZE(kp) (((size_t)kp->ki_rssize) * PAGESIZE)
#else
#	define KVM_PID(kp)   kp->kp_proc.p_pid
#	define KVM_VSIZE(kp) kp->kp_eproc.e_vm.vm_map.size
#	define KVM_RSSSIZE(kp) ((size_t)0)
#endif

int getvsize ( pid_t pid, size_t *vsize, size_t *rss ) {
	kvm_t *kd;
	char errbuf[_POSIX2_LINE_MAX];
	int nr;
	struct kinfo_proc *kip;
	size_t vsize;
	
	kd = kvm_openfiles("/dev/null", "/dev/null", NULL, O_RDONLY, errbuf);
	if (!kd) {
		//printf("kvm_openfiles failed: %s\n",errbuf);
		return -1;
	}
	kip = kvm_getprocs(kd, KERN_PROC_PID, pid, &nr);
	if (!kip) {
		//printf ("kvm error: %s\n", kvm_geterr(kd));
		return -1;
	}
	//printf("ok: %d | %d | size=%d, dsize=%d, ssize=%d\n",pid,kip->ki_pid, kip->ki_size, kip->ki_dsize, kip->ki_ssize);
	//printf("ok: %d | %d | size=%d\n",pid,kip->kp_proc.p_pid, KVM_VSIZE(kip));
	if (vsize) {
		*vsize = KVM_VSIZE(kip);
	}
	if (rss) {
		*rss = KVM_RSSSIZE(kip);
	}
	kvm_close(kd);
	return 0;
}

#else
#if __linux__

#include <unistd.h> // open,read
#include <stdio.h> //sscanf

int getvsize ( pid_t pid, size_t *vsize, size_t *rss ) {
	int num;
	static char filename[80];
	static char buffer[1024];
	char *buf;
	int fd, nread, len;
	size_t nul;
	
	if ( snprintf(filename, 80, "/proc/%u/stat", pid) < 0 ) {
		errno = ESRCH;
		return -1;
	}
	//printf( "read from: %s\n", filename);
	
	fd = open(filename, O_RDONLY, 0);
	if( fd == -1 ) {
		switch(errno) {
			case ENOENT:
				errno = ESRCH;
				break;
		}
		return -1;
	}
	len = 1023;
	buf = buffer;
	while (( nread = read(fd,buf,len)) > 0) {
		len -= nread;
		buf += nread;
	}
	close(fd);
	if( len == 1023 ) {
		errno = ENOBUFS;
		return -1;
	}
	*buf = '\0';
	while ( buf > buffer + 2 && *( buf - 2 ) != ')' ) buf--;
	
	//warn("read %s\n",  buf);
	num = sscanf(buf,
		"%*c "
		"%*s %*s %*s %*s %*s "
		"%*s %*s %*s %*s %*s "
		"%*s %*s %*s %*s "  /* utime stime cutime cstime */
		"%*s %*s "
		"%*s "
		"%*s "
		"%*s "  /* start_time */
		"%zu "  /* vsize */
		"%zu "  /* rss */
		,
		vsize ? vsize : &nul,
		rss ? rss : &nul
	);
	if (num < 2) {
		return -1;
	}
	//printf("got %d from sscanf: vsize=%zu + %zu\n", num, vsize ? *vsize : 0, rss ? *rss : 0);
	return 0;
}

#else

#include <stdint.h>
#ifndef uint
typedef unsigned int uint;
#endif

#error "Platform " PLATFORM_NAME " not supported"

int getvsize ( pid_t pid, size_t *vsize, size_t *rss  ) {
	errno = ENOSYS;
	return -1;
}

#endif // __linux__
#endif
