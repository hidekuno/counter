#!/usr/bin/perl
#
open(FD, "<test_file") || die "$!";

while (<FD>) {
	$line = $_;
}
print $line;

while (1) {
	sleep(300);
}
