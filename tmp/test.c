#include <stdio.h>

#include <fcntl.h> // For O_RDONLY

# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)

#if __linux__
#include <unistd.h> // open,read

static size_t getvsize ( pid_t pid ) {
	unsigned num;
	static char filename[80];
	static char buffer[1024];
	char *buf;
	int fd, nread, len;
	long unsigned int vsize;
	//size_t vsize;
	unsigned rss;
	
	if ( unlikely( snprintf(filename, 80, "/proc/%u/stat", pid) < 0 ) ) {
		return -1;
	}
	//printf( "read from: %s\n", filename);
	
	fd = open(filename, O_RDONLY, 0);
	if( fd == -1 ) return -1;
	len = 1023;
	buf = buffer;
	while (( nread = read(fd,buf,len)) > 0) {
		len -= nread;
		buf += nread;
	}
	close(fd);
	if( len == 1023 ) return -1;
	*buf = '\0';
	while ( buf > buffer + 2 && *( buf - 2 ) != ')' ) buf--;
	
	//printf("read %s\n",  buf);
	num = sscanf(buf,
		"%*c "
		"%*d %*d %*d %*d %*d "
		"%*u %*u %*u %*u %*u "
		"%*u %*u %*u %*u "  /* utime stime cutime cstime */
		"%*d %*d "
		"%*d "
		"%*d "
		"%*u "  /* start_time */
		"%lu "  /* vsize */
		"%d "   /* rss */
		,
		&vsize, &rss
	);
	//printf("got %d from sscanf: vsize=%lu + %u\n", num, vsize, rss);
	return (size_t) vsize;
}

#else
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

static size_t getvsize ( pid_t pid ) {
	kvm_t *kd;
	char errbuf[_POSIX2_LINE_MAX];
	int nr;
	struct kinfo_proc *kip;
	size_t vsize;
	
	kd = kvm_openfiles("/dev/null", "/dev/null", NULL, O_RDONLY, errbuf);
	if (!kd) {
		printf("kvm_openfiles failed: %s\n",errbuf);
		return -1;
	}
	kip = kvm_getprocs(kd, KERN_PROC_PID, pid, &nr);
	if (!kip) {
		printf ("kvm error: %s\n", kvm_geterr(kd));
		return -1;
	}
	//printf("ok: %d | %d | size=%d, dsize=%d, ssize=%d\n",pid,kip->ki_pid, kip->ki_size, kip->ki_dsize, kip->ki_ssize);
	//printf("ok: %d | %d | size=%d\n",pid,kip->kp_proc.p_pid, KVM_VSIZE(kip));
	vsize = KVM_VSIZE(kip);
	kvm_close(kd);
	return vsize;
}

#endif

#include <sys/types.h> //
#include <unistd.h> // getpid()

int main () {
	pid_t pid = getpid();
	printf("ok: %d | size=%d\n",pid,getvsize(pid));
	//usleep(10*1000000);
	return 0;
}


/* TODO
 * unlikely ??
 */
