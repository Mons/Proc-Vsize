#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h> // For O_RDONLY

#include "vsize.h"


#if __FreeBSD__

#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/user.h>
#include <kvm.h>
#include <limits.h> // for _POSIX2_LINE_MAX
#if __FreeBSD__ >= 5
#	define KVM_PID(kp)   kp->ki_pid
#	define KVM_VSIZE(kp) kp->ki_size
#else
#	define KVM_PID(kp)   kp->kp_proc.p_pid
#	define KVM_VSIZE(kp) kp->kp_eproc.e_vm.vm_map.size
#endif

size_t getvsize ( unsigned int pid ) {
//static size_t getvsize ( pid_t pid ) {
	kvm_t *kd;
	char errbuf[_POSIX2_LINE_MAX];
	int nr;
	struct kinfo_proc *kip;
	size_t vsize;
	
	kd = kvm_openfiles("/dev/null", "/dev/null", NULL, O_RDONLY, errbuf);
	if (!kd) {
		//printf("kvm_openfiles failed: %s\n",errbuf);
		return 0;
	}
	kip = kvm_getprocs(kd, KERN_PROC_PID, pid, &nr);
	if (!kip) {
		//printf ("kvm error: %s\n", kvm_geterr(kd));
		return 0;
	}
	//printf("ok: %d | %d | size=%d, dsize=%d, ssize=%d\n",pid,kip->ki_pid, kip->ki_size, kip->ki_dsize, kip->ki_ssize);
	//printf("ok: %d | %d | size=%d\n",pid,kip->kp_proc.p_pid, KVM_VSIZE(kip));
	vsize = KVM_VSIZE(kip);
	kvm_close(kd);
	return vsize;
}

#else 
#if __linux__

#include <unistd.h> // open,read
#include <stdio.h> //sscanf

size_t getvsize ( int pid ) {
	unsigned num;
	static char filename[80];
	static char buffer[1024];
	char *buf;
	int fd, nread, len;
	//long unsigned int vsize;
	size_t vsize;
	size_t  rss;
	
	if ( snprintf(filename, 80, "/proc/%u/stat", pid) < 0 ) {
		return 0;
	}
	//printf( "read from: %s\n", filename);
	
	fd = open(filename, O_RDONLY, 0);
	if( fd == -1 ) return 0;
	len = 1023;
	buf = buffer;
	while (( nread = read(fd,buf,len)) > 0) {
		len -= nread;
		buf += nread;
	}
	close(fd);
	if( len == 1023 ) return 0;
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
		&vsize, &rss
	);
	printf("got %d from sscanf: vsize=%lu + %u\n", num, vsize, rss);
	return vsize;
}

#else

size_t getvsize ( int pid ) { return 0 }

#endif // __linux__
#endif
