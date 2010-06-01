package Proc::Vsize;

use 5.008008;
use strict;
use warnings;

require Exporter;

our @ISA = qw(Exporter);

# Items to export into callers namespace by default. Note: do not export
# names by default without a very good reason. Use EXPORT_OK instead.
# Do not simply export all your public functions/methods/constants.

# This allows declaration	use Proc::Vsize ':all';
# If you do not need this, moving things directly into @EXPORT or @EXPORT_OK
# will save memory.
our %EXPORT_TAGS = ( 'all' => [ qw(
	vsize vsize_kb
) ] );

our @EXPORT_OK = ( @{ $EXPORT_TAGS{'all'} } );

our @EXPORT = qw(
	vsize
);

our $VERSION = '0.01';

require XSLoader;
XSLoader::load('Proc::Vsize', $VERSION);

1;
__END__
=head1 NAME

Proc::Vsize - Get process vsize, X-platform

=head1 SYNOPSIS

    use Proc::Vsize;
    say "My vsize is ".vsize($$);

=head1 DESCRIPTION

If you need to determine vsize of process in a fast and lightweight way.

=head2 EXPORT

=head3 vsize($pid)

Exported by default. Return vsize of process in bytes

=head3 vsize_kb($pid)

Not exported by default. Return vsize of process in kbytes

=head1 AUTHOR

Mons Anderson, E<lt>mons@cpan.orgE<gt>

=head1 COPYRIGHT AND LICENSE

Copyright (C) 2010 by Mons Anderson

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself, either Perl version 5.12.0 or,
at your option, any later version of Perl 5 you may have available.

=cut
