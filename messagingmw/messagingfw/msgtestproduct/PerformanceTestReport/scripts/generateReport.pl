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

use strict;
use FindBin;
use FileHandle;

# Take two .csv file as input. Selects two lines, each from different files. 
# Checks if first four fields matches then concats  the lines keeping the common fields only once.
# write the new line two a new file.
sub mergetwofiles
{
	my ($reportype,$file1,$file2) = @_;
	my $outputhand = FileHandle->new;
	
	if($reportype eq "speed")
	{
		$outputhand->open("> SpeedtestReport.csv")or die "Error: output.csv $!";
		print $outputhand "#Test case-Event name,Measurement name,Start Event ID,End Event ID,Time started for Old API,Time ended for Old API,Duration for Old API (ms),Time started for New API,Time ended for New API,Duration for New API (ms),Performance Improvement\n";
	}
	else
	{
		$outputhand->open("> RamUsageReport.csv")or die "Error: output.csv $!";
		print $outputhand "#Test case-Event name,Measurement name,Start Event ID,End Event ID,RAM size at start for Old API,RAM size at stop for Old API,RAM memory used for Old API (ms),RAM size at start for New API,RAM size at stop for New API,RAM memory used for New API (ms),Performance Improvement\n";
	}

	while(my $line1=<$file1>)
	{	
		next if ($line1 =~ /^\#/);
		$line1=~ s/\n//;
		my @splitline1= split(/,/,$line1);
		while(my $line2=<$file2>)
		{
			next if ($line2 =~ /^\#/);
			$line2=~ s/\n//;
			my @splitline2= split(/,/,$line2);
			if($splitline1[0] eq $splitline2[0] and $splitline1[1] eq $splitline2[1] and $splitline1[2] eq $splitline2[2] and $splitline1[3] eq $splitline2[3])
			{
				my $difference=$splitline1[6]-$splitline2[6];
				print $outputhand "$line1,$splitline2[4],$splitline2[5],$splitline2[6],$difference\n";
			}
		}
		seek($file2,0,0);
	}
		$outputhand->close;
}

sub usage
{
	print <<USAGE;
Description:
	Generates a report to show the performance test data for existing and new Messaging API.
	
Usage:
	generateReport.pl -reporttype <ramusage/speed> <file1> <file2>

	-reporttype: ramusage - Indicates that file1 and file2 hold RAM usage information.
				 speed - Indicates that file1 and file2 hold time usage(speed) information.

	<file1>         : Raw data file for old API
	<file2>         : Raw data file for new API

NOTE: All the command line options are mandatory and user is supposed to provide correct data.
	
USAGE
}

# Generates a common report for RAM usage or time inforamtio as specified in the command line.
sub GenerateReport
{
	my @argv = @_;
	if (@argv != 4 )
	{
		usage;
		exit;
	}
	my $reportype;
	my $oldrawdatafile;
	my $newrawdatafile;
	while (@argv > 0)
	{
		my $arg = shift @argv;
		
		if ($arg eq "-reporttype")
		{
			# Variable logfiletype has been defined in Configuration package.
			$reportype = shift @argv;
			if ($reportype ne "ramusage" && $reportype ne "speed")
			{
				print "Error : Illegal command option";
				usage;
				exit;
			}
		}
		$oldrawdatafile = shift @argv;
		if(not -e $oldrawdatafile)
		{
			print "$oldrawdatafile : Does not exist";
			exit;
		}
		$newrawdatafile = shift @argv;
		if(not -e $newrawdatafile)
		{
			print "$newrawdatafile : Does not exist";
			exit;
		}
	}

	my $Handle1 = FileHandle->new;
	$Handle1->open("< $oldrawdatafile");
	my $Handle2 = FileHandle->new;
	$Handle2->open("< $newrawdatafile");
	mergetwofiles($reportype,$Handle1,$Handle2);
	$Handle2->close;
	$Handle1->close;
}
GenerateReport(@ARGV);