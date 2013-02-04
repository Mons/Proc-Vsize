#!/usr/bin/env perl

use strict;
use warnings;

use Test::More tests => 5;
use Errno;

BEGIN { use_ok('Proc::Vsize') };

ok !eval { Proc::Vsize::vsize(-1); 1 }, 'vsize of non existent dies';
is 0+$!, Errno::ESRCH, 'errno ok';
ok Proc::Vsize::vsize($$), 'vsize of self';
is Proc::Vsize::vsize(), Proc::Vsize::vsize($$), 'vsize of self eq noargs';
