use 5.012000;
use ExtUtils::MakeMaker;
# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
	NAME              => 'Proc::Vsize',
	VERSION_FROM      => 'lib/Proc/Vsize.pm', # finds $VERSION
	ABSTRACT_FROM     => 'lib/Proc/Vsize.pm', # retrieve abstract from module
	AUTHOR            => 'Mons Anderson <mons@cpan.org>',
	PREREQ_PM         => {}, # e.g., Module::Name => 1.1
	LIBS              => ['-lkvm'], # e.g., '-lm'
	DEFINE            => '', # e.g., '-DHAVE_SOMETHING'
	INC               => '-I.', # e.g., '-I. -I/usr/include/other'
	# Un-comment this if you add C files to link with later:
	OBJECT            => 'Vsize.o vsize.o', # link all the C files too
);
