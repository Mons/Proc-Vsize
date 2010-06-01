use 5.012;
use ExtUtils::testlib;
use Proc::Vsize;
eval { Proc::Vsize::vsize(); };
say int(Proc::Vsize::vsize($$)/1024);
say Proc::Vsize::vsize_kb($$);
say `ps uww -p $$`;