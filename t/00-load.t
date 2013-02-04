# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl Proc-Vsize.t'

#########################

# change 'tests => 1' to 'tests => last_test_to_print';

use strict;
use warnings;

use Test::More tests => 3;
BEGIN { use_ok('Proc::Vsize') };

#########################

# Insert your test code below, the Test::More module is use()ed here so read
# its man page ( perldoc Test::More ) for help writing this test script.

is Proc::Vsize::vsize(-1),0;
ok Proc::Vsize::vsize($$);
