#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"
#include "getvsize.h"
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

MODULE = Proc::Vsize		PACKAGE = Proc::Vsize

size_t
vsize(...)
	PROTOTYPE: ;$
	CODE:
		pid_t pid;
		size_t vsize;
		if (items == 0) {
			pid = getpid();
		}
		else if (items == 1) {
			pid = (pid_t) SvIV( ST(0) );
		}
		else {
			croak("Usage: vsize([pid])");
		}
		if (getvsize(pid, &vsize, NULL)) {
			croak("getvsize(%d): %s",pid,strerror(errno));
		}
		RETVAL = vsize;
	OUTPUT:
		RETVAL

size_t
rss(...)
	PROTOTYPE: ;$
	CODE:
		pid_t pid;
		size_t rss;
		if (items == 0) {
			pid = getpid();
		}
		else if (items == 1) {
			pid = (pid_t) SvIV( ST(0) );
		}
		else {
			croak("Usage: vsize([pid])");
		}
		if (getvsize(pid, NULL, &rss)) {
			croak("getvsize(%d): %s",pid,strerror(errno));
		}
		RETVAL = rss;
	OUTPUT:
		RETVAL
