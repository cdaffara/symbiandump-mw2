# Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:
#

## System include
#!/usr/bin/perl -w;
use Cwd;
use Getopt::Long;

my $file = "test.htm";
my $startid = 0;
my $endid = 0;
my $testcasename = ""; 

sub GetParameters()
	{
	my ($ret) = GetOptions(
	'file=s'	=> \$file,
        'startid=s' 	=> \$startid,
        'endid=s' 	=> \$endid,
	'testcasename=s'=> \$testcasename);
	if(!$ret)
		{
		exit(1);
		}
	}
sub millisec
{ 
	my ($mytime) = @_ ;
	$mytime =~ /([0-9]*):([0-9]*):([0-9]*):([0-9]*)$/;
	my $msecs = $4 + ($3 *1000) + ($2 * 60000) + ($1 * 3600000);
	return $msecs;
	
}

sub execute()
	{
	print "This is the Root mean square program\n";
	open FH, "$file" or die "file not opened\n";
	my $loop = 0;
	my $testcase = 0;
	my @vals;
	my $i = 0;
	while(<FH>)
		{
		$value = $_;
		if(m/START_TESTCASE $testcasename/i)
			{
			$testcase = 1;
			}
		if(m/END_TESTCASE $testcasename/i)
			{
			$testcase = 0;
			}
		if(m/EVENT_LOG_INFORMATION/i and $testcase == 1)
			{
			$time = substr($value,rindex($value,",")+1,12);
			$eventid = substr($value,0,rindex($value,","));
			$eventid = substr($eventid,rindex($eventid,",")+1);
			if($eventid == $startid or $eventid == $endid)
				{
				$loop = 1;
				}
			else
				{
				$loop = 0;
				}
			if($loop == 1)
				{
				$vals[$i] = $time;
				$i = $i +1;
				}
			
			}
		}
	close(FH);
	my $start_time=$vals[0];
	my @value;
	for($j=1;$j<$i;$j=$j+1)
		{
		my $diff = millisec($vals[$j])- millisec($start_time) ;
		push @value, $diff;
		print " events---,$start_time,$vals[$j], $diff \n ";
		$start_time=$vals[$j];
		}
	
	print "The count is $j\n";
	print "The difference is $difftime\n";
	my $square=0;
	foreach my $rms ( @value )
		{
		$square = $square + $rms ** 2;
		}
	print " square is.......................$square \n ";
	my $mean_square = $square / $j-1;
	my $root_mean_square = $mean_square ** (1/2);
	print "........root mean square is ........$root_mean_square \n";
	}

sub main()
	{
	GetParameters();
	execute();
	}

main();