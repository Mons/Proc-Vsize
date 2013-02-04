use 5.008008;
use ExtUtils::MakeMaker;
$ENV{AUTHOR} and require CPAN::Meta;

WriteMakefile(
	NAME              => 'Proc::Vsize',
	VERSION_FROM      => 'lib/Proc/Vsize.pm',
	ABSTRACT_FROM     => 'lib/Proc/Vsize.pm',
	AUTHOR            => 'Mons Anderson <mons@cpan.org>',
	PREREQ_PM         => {},
	LIBS              => [ $^O eq 'freebsd' ? ( '-lkvm' ) : () ],
	DEFINE            => '-std=c99 -Wall',
	INC               => '-I.',
	OBJECT            => 'Vsize.o getvsize.o',
);
