#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"
#include "vsize.h"

MODULE = Proc::Vsize		PACKAGE = Proc::Vsize

size_t
vsize(pid)
		int pid
	CODE:
		RETVAL = getvsize(pid);
	OUTPUT:
		RETVAL

size_t
vsize_kb(pid)
		int pid
	CODE:
		RETVAL = getvsize(pid) / 1024;
	OUTPUT:
		RETVAL

