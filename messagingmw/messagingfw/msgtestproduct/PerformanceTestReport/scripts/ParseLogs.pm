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

package ParseLogs;
use strict;
use Configuration;
use DataQuery;
use Cwd;
use Getopt::Long;
use File::Copy;
use File::Basename;

my $Test_Case_Start_Tag = "START_TESTCASE";
my $Test_Case_End_Tag = "END_TESTCASE";
my $Event_print = "EVENT_LOG_INFORMATION";
my $BootEventCode = 1;

sub millisec
{ 
	my ($mytime) = @_ ;
	$mytime =~ /([0-9]*):([0-9]*):([0-9]*):([0-9]*)$/;
	my $msecs = $4 + ($3 *1000) + ($2 * 60000) + ($1 * 3600000);
	return $msecs;
	
}

my %RAW_DATA = ();

sub write_raw_data_file
{
	my $outFH = shift;

	# Print table headers
	if($Configuration::logfiletype eq "ramusagetest")
	{
		print $outFH "#Test case-Event name,Measurement name,Start Event ID,End Event ID,Initial RAM Usage(in KB),Maximum RAM usage(in KB),RAM memory consumed(in KB)\n";
	}
	else
	{
		print $outFH "#Test case-Event name,Measurement name,Start Event ID,End Event ID,Time started,Time ended,Duration (ms)\n";
	}
	
	foreach my $key (sort keys %RAW_DATA)
	{
		my $id = "*ERROR*"; # use this for the id if we don't find the real one

		my @result = DataQuery::selectMeasurementTranslation(raw_data_code => $key);
		$id = $result[0]->{measurement_id} if @result == 1;

		foreach my $data (@{ $RAW_DATA{$key} })
		{
			print $outFH "$key,$id,$data \n";
		}
	}
}


sub append_record
{
	my ($key, @data) = @_;
	$RAW_DATA{$key} = [] if not exists $RAW_DATA{$key};
	push @{ $RAW_DATA{$key} }, (join "," , @data);
}

sub process_log_file
{
	my ($data_file, $output_FH) = @_;
		
	# enable reading whole file as one chunk
	my $SAVE_RS = $/;
	undef $/;

	# open and read the log file in a single line
	open(DAT, $data_file) || die("Could not open file $data_file!");
	my $raw_data=<DAT>;
#This change is made to give the fix for the defect DEF076227 : Event names are not correctly captured from log files	
	my $pos = index($raw_data,"TestExecute Started");
	$pos=$pos+length("TestExecute Started  ");
	seek(DAT,$pos,0);
	$raw_data=<DAT>;
#End of chagnges for the defect DEF076227 : Event names are not correctly captured from log files
	close(DAT);
	$raw_data=~ s/\n/ /g;

	# restore previous record separator
	$/ = $SAVE_RS;	

	
	while ($raw_data && $raw_data =~ /$Test_Case_Start_Tag\s+(\S+)/)
	{
		# step through the file in chunks delimited by the start/end test case tags.
		# get the current chunk in '$test_case_data' and set '$raw_data' to the start of
		# the next chunk. Then process '$test_case_data' for event logging info.

		my $test_case_name = $1;
		my ($test_case_data) = ($raw_data =~ /$Test_Case_Start_Tag(.*?)$Test_Case_End_Tag/);

		if (not $test_case_data)
		{
			print STDERR "Error: Test case: $test_case_name: Suspected missing END_TESTCASE tag.\n";
		}

		($raw_data) = ($raw_data =~ /$Test_Case_End_Tag(.*)/);
		($raw_data) = ($raw_data =~ /($Test_Case_Start_Tag.*)/) if $raw_data;

		while ($test_case_data and ($test_case_data =~ /$Event_print(.*)/) )
		{
			# get the next log message in the current test case log text and search it for corresponding 'end' log messages

			my $start_log_msg = $1;
			#Check if the logging information has recorded time
			if($start_log_msg !~ /,[0-9]+,[0-9]+,[0-9]+,[0-9]+:[0-9]+:[0-9]+:[0-9]+\s(.*)/)
			{
				print "Incorrect log format. Check if the the log file logs Time information in $data_file\n";
				exit;
			}
		   
			$start_log_msg =~ /,[0-9]+,[0-9]+,([0-9]+),(.+?)\s(.*)/;
			my $start_code = $1;
			my $start_time = $2;
			$test_case_data = $3;
			
			my %end_time_list; # this hash will contain one or more "end_code => end_time" mappings

			# using the start event code - make a query to get the associated end codes

			my %event_codes = DataQuery::selectEventCode(start_code => $start_code);
			my @end_code_list = @{ $event_codes{$start_code} };

			if (@end_code_list > 0)
				{
				# now search the text for any log messages corresponding to one of the end codes
				#print "List : @end_code_list\n";
				foreach my $end_code (@end_code_list)
					{
					if ($start_code == $end_code)
						{
						# the bootup case is handled specially since we cannot log a start event
						# the system clock is set to 12:00 at the start of bootup and an event is logged at the end

						$end_time_list{$start_code} = $start_time;
						$start_time = "12:00:00:000";
						#print "Inside if\n";
						}
					elsif ($test_case_data  =~ /($Event_print,[0-9]+,[0-9]+,$end_code,(.+?))\s/)
						{
							#This while implementation is to support more one end time logging.

							while($test_case_data  =~ /($Event_print,[0-9]+,[0-9]+,$end_code,(.+?))\s/)
							{
							$end_time_list{$end_code} = $2;
							#print "end_code:$end_time_list{$end_code} \n";
							# NOTE: this routine, as currently implemented will search
							# for mulitple end_codes corresponding to a single start_code
							# if we also require the capability that an end_code may also
							# be a start_code (i.e. that as well as A->B and A->C we can
							# also capture B->C) then we should comment out the following
							# line so that this message is a candidate for treating as
							# a log message with start_code in a later iteration of the loop
							$test_case_data =~ s/$1//;
							}
						
						}
					}
				}
			else
				{
				print STDERR "Warning: Test case: $test_case_name: no end codes for start code '$start_code'.\n";
				}
			
			if (keys %end_time_list > 0)
					{

				
				foreach my $end_code (keys %end_time_list)
				{
				
					my $end_time = $end_time_list{$end_code};
					my $diff = millisec($end_time) - millisec($start_time);
					
					# Look up for an event name matching the event ID pair
					my %event_names_hash = DataQuery::selectEventName(start_code => $start_code, end_code => $end_code);
					my $event_name;
					if (exists $event_names_hash{"$start_code-$end_code"})
					{
						$event_name = $event_names_hash{"$start_code-$end_code"};
					}
					else
					{
						$event_name = "UnknownEvent($start_code-$end_code)";
						print STDERR "WARNING: Unknown event pair $start_code-$end_code.\n";
					}
					
					append_record("$test_case_name-$event_name" , $start_code, $end_code, $start_time, $end_time, $diff);	
				}
			}
			else
			{
				print STDERR "Warning: Test case: $test_case_name: no end log messages corresponding to start code '$start_code'.\n";
			}
		}
	}
}
sub process_log_file_RamUsage
{
	my ($data_file, $output_FH) = @_;
		
	# enable reading whole file as one chunk
	my $SAVE_RS = $/;
	undef $/;

	# open and read the log file in a single line
	open(DAT, $data_file) || die("Could not open file $data_file!");
	my $raw_data=<DAT>;
#This change is made to give the fix for the defect DEF076227 : Event names are not correctly captured from log files	
	my $pos = index($raw_data,"TestExecute Started");
	$pos=$pos+length("TestExecute Started  ");
	seek(DAT,$pos,0);
	$raw_data=<DAT>;
#End of chagnges for the defect DEF076227 : Event names are not correctly captured from log files
	close(DAT);
	$raw_data=~ s/\n/ /g;

	# restore previous record separator
	$/ = $SAVE_RS;	

	
	while ($raw_data && $raw_data =~ /$Test_Case_Start_Tag\s+(\S+)/)
	{
		# step through the file in chunks delimited by the start/end test case tags.
		# get the current chunk in '$test_case_data' and set '$raw_data' to the start of
		# the next chunk. Then process '$test_case_data' for event logging info.

		my $test_case_name = $1;

		my ($test_case_data) = ($raw_data =~ /$Test_Case_Start_Tag(.*?)$Test_Case_End_Tag/);

		if (not $test_case_data)
		{
			print STDERR "Error: Test case: $test_case_name: Suspected missing END_TESTCASE tag.\n";
		}

		($raw_data) = ($raw_data =~ /$Test_Case_End_Tag(.*)/);
		($raw_data) = ($raw_data =~ /($Test_Case_Start_Tag.*)/) if $raw_data;

		while ($test_case_data and ($test_case_data =~ /$Event_print(.*)/) )
		{
			# get the next log message in the current test case log text and search it for corresponding 'end' log messages

			my $start_log_msg = $1;
		   
			if($start_log_msg !~ /,[0-9]+,[0-9]+,([0-9]+),([0-9]+)\s.*/)
			{
				print "Incorrect log format. Check if the the log file logs RAM usage information in $data_file\n";
				exit;
			}
			$start_log_msg =~ /,[0-9]+,[0-9]+,([0-9]+),(.+?)\s(.*)/;

			my $start_code = $1;
			my $start_RAMSize = $2;
			$test_case_data = $3;
			
			my %end_RAMSize_list; # this hash will contain one or more "end_code => end_RAMSize" mappings

			# using the start event code - make a query to get the associated end codes

			my %event_codes = DataQuery::selectEventCode(start_code => $start_code);
			my @end_code_list = @{ $event_codes{$start_code} };

			if (@end_code_list > 0)
				{
				# now search the text for any log messages corresponding to one of the end codes
				#print "List : @end_code_list\n";
				foreach my $end_code (@end_code_list)
					{

					if ($start_code == $end_code)
						{
						# the bootup case is handled specially since we cannot log a start event
						# the system clock is set to 12:00 at the start of bootup and an event is logged at the end

						$end_RAMSize_list{$start_code} = $start_RAMSize;
						$start_RAMSize = "0";
						#print "Inside if\n";
						} 
					elsif ($test_case_data  =~ /($Event_print,[0-9]+,[0-9]+,$end_code,([0-9]+))\s/)
						{
							#This while implementation is to support more one end time logging.

							while($test_case_data  =~ /($Event_print,[0-9]+,[0-9]+,$end_code,([0-9]+))\s/)
							{
							$end_RAMSize_list{$end_code} = $2;
							#print "end_code:$end_RAMSize_list{$end_code} \n";
							# NOTE: this routine, as currently implemented will search
							# for mulitple end_codes corresponding to a single start_code
							# if we also require the capability that an end_code may also
							# be a start_code (i.e. that as well as A->B and A->C we can
							# also capture B->C) then we should comment out the following
							# line so that this message is a candidate for treating as
							# a log message with start_code in a later iteration of the loop
							$test_case_data =~ s/$1//;
							}
						
						}
					}
				}
			else
				{
				print STDERR "Warning: Test case: $test_case_name: no end codes for start code '$start_code'.\n";
				}
			
			if (keys %end_RAMSize_list > 0)
					{

				
				foreach my $end_code (keys %end_RAMSize_list)
				{
				
					my $end_RAMSize = $end_RAMSize_list{$end_code};
					my $diff = $end_RAMSize - $start_RAMSize;
					
					# Look up for an event name matching the event ID pair
					my %event_names_hash = DataQuery::selectEventName(start_code => $start_code, end_code => $end_code);
					my $event_name;
					if (exists $event_names_hash{"$start_code-$end_code"})
					{
						$event_name = $event_names_hash{"$start_code-$end_code"};
					}
					else
					{
						$event_name = "UnknownEvent($start_code-$end_code)";
						print STDERR "WARNING: Unknown event pair $start_code-$end_code.\n";
					}
					
					append_record("$test_case_name-$event_name" , $start_code, $end_code, $start_RAMSize, $end_RAMSize, $diff);	
				}
			}
			else
			{
				print STDERR "Warning: Test case: $test_case_name: no end log messages corresponding to start code '$start_code'.\n";
			}
		}
	}
}

sub parse
{
	my ($outFH, @file_list) = @_;
	DataQuery::initialise('DataQueryCSVFile'); # choice of CSV hard-coded FTB

	# process the input file list
	if($Configuration::logfiletype eq "ramusagetest")
	{
		foreach my $filename (@file_list)
		{
			print STDERR "Processing File: $filename\n";
			ParseLogs::process_log_file_RamUsage($filename, $outFH);
		}

	}
	else
	{
		foreach my $filename (@file_list)
		{
			print STDERR "Processing File: $filename\n";
			ParseLogs::process_log_file($filename, $outFH);
		}

	}
	# write out the raw data (grouped by test case/event)

	write_raw_data_file($outFH);
}

1;
