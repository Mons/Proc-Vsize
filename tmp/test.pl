use 5.010;
use ExtUtils::testlib;
use Proc::Vsize;

my $pid = 15314;

#eval { Proc::Vsize::vsize(); };

say `ps uww -p $pid`;
say int(Proc::Vsize::vsize($pid)/1024);

__END__
say "\n\nAlloc 1";
keys( my %h1 ) = 2**26;
say `ps uww -p $$`;
say int(Proc::Vsize::vsize($$)/1024);

say "\n\nAlloc 2";
keys( my %h1 ) = 2**26;
say `ps uww -p $$`;
say int(Proc::Vsize::vsize($$)/1024);


__END__
keys( my %h2 ) = 2**27;
say `ps uww -p $$`;
keys( my %h3 ) = 2**27;

say `ps uww -p $$`;

__END__
say int(Proc::Vsize::vsize($$)/1024);
say Proc::Vsize::vsize_kb($$);

undef %h1;
undef %h2;
