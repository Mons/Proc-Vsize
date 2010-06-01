#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

MODULE = Proc::Vsize		PACKAGE = Proc::Vsize		

void
hello()
	CODE:
		printf("Hello\n");

#include "vsize.h"

int
vsize(pid)
		int pid
	CODE:
		RETVAL = getvsize(pid);
	OUTPUT:
		RETVAL

int
vsize_kb(pid)
		int pid
	CODE:
		RETVAL = getvsize(pid) / 1024;
	OUTPUT:
		RETVAL

