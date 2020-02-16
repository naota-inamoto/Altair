#!/usr/bin/env perl

use encoding 'shiftjis';
use open IO => ':encoding(shiftjis)';

# KNP���[���t�@�C���̊���([, ], <, >)��ϊ�

for ($i = 0; $i < @ARGV; $i++) {

    open(INPUT, $ARGV[$i]);
    $ARGV[$i] =~ s/rule$/data/;

    print "make $ARGV[$i] ... \n";

    open(OUTPUT, "> $ARGV[$i]") || die;

    while (<INPUT>) {

	s/\[/\(/g;
	s/\]/\)/g;
	s/\</\(/g;
	s/\>/\)/g;

	print OUTPUT;
    }

    close INPUT;
    close OUTPUT;
}
