#include <stdio.h>

#include <kvm.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/user.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>

#if __FreeBSD__ >= 5
#	define KVM_PID(kp)   &(kp)->ki_pid
#	define KVM_VSIZE(kp) &(kp)->ki_size
#else
#	define KVM_PID(kp)   &(kp)->kp_proc.p_pid
#	define KVM_VSIZE(kp) &(kp)->kp_eproc.e_vm.vm_map.size
#endif

int main () {
	kvm_t *kd;
	char errbuf[_POSIX2_LINE_MAX];
	int nr;
	pid_t pid = getpid();
	struct kinfo_proc *kip;

	kd = kvm_openfiles("/dev/null", "/dev/null", NULL, O_RDONLY, errbuf);
	if (!kd) {
		printf("kvm_openfiles failed: %s\n",errbuf);
		return 1;
	}
	kip = kvm_getprocs(kd, KERN_PROC_PID, pid, &nr);
	if (!kip) {
		printf ("kvm error: %s\n", kvm_geterr(kd));
		return 1;
	}
	//printf("ok: %d | %d | size=%d, dsize=%d, ssize=%d\n",pid,kip->ki_pid, kip->ki_size, kip->ki_dsize, kip->ki_ssize);
	//printf("ok: %d | %d | size=%d\n",pid,kip->kp_proc.p_pid, KVM_VSIZE(kip));
	printf("ok: %d | %d | size=%d\n",pid,KVM_PID(kip), KVM_VSIZE(kip));
	kvm_close(kd);
	return 0;
}
