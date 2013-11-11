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

#!/usr/bin/perl -w
#-----------------------------------------------------------------------------------------
# Script: results.pl
#
# Description: This script reads a number of html test logs extracting logged performance
#	timestamps. Named time interval samples are calculated from timestamps with codes
#	which match interval start/end codes defined in the event_defs file. These time
#	interval samples are logged to an intermediate 'raw data' file (raw_data.csv).
#	The raw data is processed to aggregate sample data for identically named intervals
#	into a number of statistical measures (e.g. average, standard deviation, ...).
#	The aggregated data for each named time interval is then output to STDOUT or a named
#	file.
#
# Arguments:
#
#	<file1>...<fileN> : List of input files to process
#
#	-o <out_file>: Final output file name (default is STDOUT)
#
#	-- : Read standard input for list of log file names
#
#	-e <event_codes_file> : The 'event_def' file containing interval start/end event
#		codes. Each logged timestamp in the html log is identified by an event code.
#		The event_def file defines the event codes used to match up timestamps
#		corresponding to start and end times for a time interval to be sampled as
#		raw data.
#
#	-m <measurement_def_file> : File containing start/end event code to measurement id
#		mapping. The interval names constructed from the html logs using event_def
#		definitions comprise teh concatenated test case name, start code and end code.
#		This file defines a mapping of these low level names to a more readable/
#		convenient format. The name mapping is performed when outputting the raw data
#		file and both the original name and the mapped name are present in the output.
#		
#
#	-t <threshold_file> : file containing the threshold times per measurement interval.
#		As the raw data is aggregated a minimum and maximum thresold time for the
#		interval is looked up in this file and output in the aggregated data for
#		the interval.
#
#-----------------------------------------------------------------------------------------
use strict;
use FindBin;

use lib $FindBin::Bin;

use FileHandle;
use ParseLogs;
use Configuration;
use AggregateResults;

#-----------------------------------------------------------------------------------------
# 
#-----------------------------------------------------------------------------------------
my $THRESHOLD_DEFAULT = "threshold.csv";
my $EVENTCODE_DEFAULT = "event_def.csv";
my $MEASUREMENT_DEF_DEFAULT = "measurement_def.csv";

my $RAWFILE_DEFAULT = "raw_data.csv";

sub usage
{
	print <<USAGE;
usage:
	results.pl [<file> [ <file> ... ]] [-logfiletype <speedtest/ramusagetest>] [ -o <out-file> ] 
	[ -- ] [ -e <event-defs> ] [ -m <measurement-defs> ] [ -t <thresholds> ] [-raw_data <FileName>]

	<File>: Log file to process (may appear zero or more times), wild card character is also allowed.

	--: Read standard input for list of log file names to process (may use this option 
		standalone or in conjunction with above "<file> ..." arguments).

	-o <out_file>: Final output file name (default is STDOUT).

	-e <event-defs>: Alternative 'event_def' file 
		(default is .\\event_def.csv, then <config_dir>\\event_def.csv).

	-m <measurement_defs>: Alternative 'measurement_def' file 
		(default is .\\measurement_def.csv, then <config_dir>\\measurement_def.csv).

	-t <thresholds>: Alternative thresholds file 
		(default is .\\threshold.csv, then then <config_dir>\\threshold.csv).

	-logfiletype <speedtest/ramusagetest>: Specifies the type of information 
		logged in the .htm log file (default is "speedtest").

	-raw_data <File Name>: File for capturing raw data. Default value is "raw_data.csv".
	
USAGE
}

sub Main
{
	my @argv = @_;

	if (@argv == 0)
	{
		usage;
		exit;
	}

	my $threshold_file = $THRESHOLD_DEFAULT;
	my $eventcode_file = $EVENTCODE_DEFAULT;
	my $measurement_def_file = $MEASUREMENT_DEF_DEFAULT;
	my $rawdata_file = $RAWFILE_DEFAULT;
	my $output_file;
	
	my @errors;
	my @file_list;
	
	# look for configuration files at the default locations
	# Order is:
	# 1. Current working directory
	# 2. ..\config directory
	
	if ((not -r $threshold_file) and (-r $FindBin::Bin."\\..\\config\\$threshold_file"))
	{
		$threshold_file = $FindBin::Bin."\\..\\config\\$threshold_file";
	}
	if ((not -r $eventcode_file) and (-r $FindBin::Bin."\\..\\config\\$eventcode_file"))
	{
		$eventcode_file = $FindBin::Bin."\\..\\config\\$eventcode_file";
	}
	if ((not -r $measurement_def_file) and (-r $FindBin::Bin."\\..\\config\\$measurement_def_file"))
	{
		$measurement_def_file = $FindBin::Bin."\\..\\config\\$measurement_def_file";
	}
		
	# process any options etc. in the argument list.

	while (@argv > 0)
	{
		my $arg = shift @argv;
		
		if ($arg eq "-logfiletype")
		{
			# Variable logfiletype has been defined in Configuration package.
			$Configuration::logfiletype = shift @argv;
			if (not $Configuration::logfiletype)
			{
				push @errors, "must specify speedtest or ramusagetest with '-logfiletype'";
			}
			elsif ($Configuration::logfiletype ne "speedtest" and $Configuration::logfiletype ne "ramusagetest")
			{
				push @errors, "must specify speedtest or ramusagetest with '-logfiletype'";
			}

		}
		elsif($arg eq "-raw_data")
		{
			#The alternative file name for raw data. This would provide an option for the user to keep raw data file safe inspite of several run.
			$rawdata_file = shift @argv;
			if (not $rawdata_file)
			{
				push @errors, "must specify rawdata file name with '-t'";
			}
		}
		elsif ($arg eq "-t")
		{
			# the alternative location for the threshold file
			$threshold_file = shift @argv;
			if (not $threshold_file)
			{
				push @errors, "must specify threshold file with '-t'";
			}
			elsif (not -r $threshold_file)
			{
				push @errors, "no such threshold file: $threshold_file";
			}
		}
		elsif ($arg eq "-e")
		{
			# the alternative location for the event codes file
			$eventcode_file = shift @argv;
			if (not $eventcode_file)
			{
				push @errors, "must specify event codes file with '-e'";
			}
			elsif (not -r $eventcode_file)
			{
				push @errors, "no such event codes file: $eventcode_file";
			}
	
		}
		elsif ($arg eq "-m")
		{
			# the alternative location for the measurement def file
			$measurement_def_file = shift @argv;
			if (not $measurement_def_file)
			{
				push @errors, "must specify event codes file with '-m'";
			}
			elsif (not -r $measurement_def_file)
			{
				push @errors, "no such event codes file: $measurement_def_file";
			}
	
		}
		elsif ($arg eq "-o")
		{
			# write the output to specified file
			$output_file = shift @argv;
			if (not $output_file)
			{
				push @errors, "must specify output file name with '-o'";
			}
		}
		elsif ($arg eq "--")
		{
			# list of files (e.g. output from dir /b) to be read from stdin
			while (my $file = <STDIN>)
			{
				chomp($file);
				push @file_list, $file;
			}
		}
		else
		{
			# process files directly passed as arguments (including wildcards '*')
			foreach (glob($arg))
			{
				if (not -r $_)
				{
					push @errors, "no such file: $_";
				}
				else
				{
				push @file_list, $_;
				}
			}
		}
	}

	if (@errors == 0 and @file_list > 0)
	{
		# set locations for various files
		Configuration::ThresholdFileName($threshold_file);
		Configuration::EventCodeFileName($eventcode_file);
		Configuration::MeasurementDefFileName($measurement_def_file);
		
		print STDERR "Threshold File :         $threshold_file\n";
		print STDERR "Event Codes File :       $eventcode_file\n";
		print STDERR "Measurement Names File : $measurement_def_file\n";

		my $rawFH = FileHandle->new;
		$rawFH->open("> $rawdata_file") or die "Error: $rawdata_file: $!";
		
		ParseLogs::parse($rawFH, @file_list);
		$rawFH->close;
		$rawFH->open("< $rawdata_file") or die "Error: $rawdata_file: $!";

		my $resultsFH = \*STDOUT; # results to STDOUT by default
		
		if (defined $output_file)
		{
			$resultsFH = FileHandle->new;
			$resultsFH->open("> $output_file") or die "Error: File $output_file: $!";
		}

		#AggregateResults::results($rawFH, $resultsFH);

		$rawFH->close;
		$resultsFH->close;
	}
	elsif (@errors > 0)
	{
		print STDERR "Errors: ", (join "\n", @errors), "\n";
	}
	else
	{
		print STDERR "No files specified\n";
	}
}

Main(@ARGV);