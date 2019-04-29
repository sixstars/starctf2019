use strict;
use IPC::Open3;

my $pid = open3(\*CHLD_IN, \*CHLD_OUT, \*CHLD_ERR, '/readflag') or die "open3() failed $!";

my $r;

$r = <CHLD_OUT>;
print "$r";
$r = <CHLD_OUT>;
print "$r";
$r=eval "$r";
print "$r\n";
print CHLD_IN "$r\n";
$r = <CHLD_OUT>;
print "$r";
$r = <CHLD_OUT>;
print "$r";
