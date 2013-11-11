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

package DataQueryCSVFile;
use strict;
use FileHandle;
use Configuration;
#===============================================================================
# Module: DataQueryCSVFile
#
# Description: Provides an implementation for the interface defined by module
#	DataQuery. Provides access to data held in CSV files. Some of the files
#	are held in a directory structure of form <base_dir>\<config>\<build>
#	where <config> and <build> parameters are required as part of the query.
#===============================================================================

#-------------------------------------------------------------------------------
# Function: new - creates a new instance of this implementation class
#-------------------------------------------------------------------------------
sub new
{
	return bless {}, "DataQueryCSVFile";
}

#-------------------------------------------------------------------------------
# Function: _getDataFilePath
#
# Arguments:
#	$entity - the entity for which the path name is requested.
#
#	$config, $build - when requesting the path to a results file, the config
#	and build are required in order to construct the full path name.
#	
#-------------------------------------------------------------------------------
sub _getDataFilePath
{
	my $entity = shift;
	my $path;

	if ($entity eq 'Result')
	{
		my ($config, $build) = @_;
		$path = $config . "\\" . $build . "\\" . Configuration::ResultsFileName;
	}
	elsif ($entity eq 'MeasurementDef')
	{
		$path = Configuration::MeasurementDefFileName;
	}
	elsif ($entity eq 'EventCodes')
	{
		$path = Configuration::EventCodeFileName;
	}
	elsif ($entity eq 'Thresholds')
	{
		$path = Configuration::ThresholdFileName;
	}
	elsif ($entity eq 'Configs')
	{
		$path = Configuration::ConfigFileName;
	}
	else
	{
		die "Error: No path name for entity: $entity.";
	}

	return $path;
}

#-------------------------------------------------------------------------------
# Function: _mapLineToHash
#
# Arguments:
#	$line - a line of text that is split into tokens for mapping to a hash
#
#	@mapping - the mapping to use for the hash key names:- '$mapping[N]' supplies
#	the key name to use to store the value <tokenN>.
#
#-------------------------------------------------------------------------------
sub _mapLineToHash
{
	my ($line, @mapping) = @_;

	# split the csv line into an array
	my @row_array = split (/,/, $line);

	# convert the array into a hash using the mapping
	my %row_hash;

	my $column;
	foreach $column (@mapping)
	{
		$row_hash{$column} = shift @row_array;
		#print "Got $column=", $row_hash{$column}, "\n";
	}
	#print "\n\n";
	return %row_hash;
}

#=========================================================================================
# The following code handles queries for measurement ids. The mapping is held in a csv file
# which (upon the first query) is read into an internal table in order to serve queries
# for mapping event codes from the raw log data into the defined measurement ids.
#=========================================================================================

my @MEASUREMENT_TRANSLATION_ROW_MAPPING =
(
	"raw_data_code",
	"measurement_id",
	"measurement_description"
);

my $MEASUREMENT_TRANSLATION_TABLE = undef;

#-----------------------------------------------------------------------------------------
# Function: selectMeasurementTranslation
#
# Argument(s): %query (hash)
#
#	If the hash is empty the entire translation table is returned (cf an SQL query of
#	the form 'select * from MeasurementTranslation').
#
#	If the hash is of the form:- %query = ( raw_data_code => value ) then measurement ids
#	corresponding to the supplied raw data code are returned (cf an SQL query of the form
#	'select * from MeasurementTranslation where raw_data_code = value').
#
#	Note that query formats which are not recognised will cause the script to 'die'.
#
# Returns: A list of zero or more hash references with keys :-
#		( raw_data_code, measurement_id, measurement_description)
#
#-----------------------------------------------------------------------------------------
sub selectMeasurementTranslation
{
	my $self = shift;
	my %query = @_;

	if (not $MEASUREMENT_TRANSLATION_TABLE)
	{
		$MEASUREMENT_TRANSLATION_TABLE = { };

		my $filename = _getDataFilePath('MeasurementDef');
		my $fh = FileHandle->new;
		$fh->open("< $filename") or die "Error: $filename: $!.";

		my @results_array;

		while (my $line = <$fh>)
		{
			chomp($line);
			my @row = split (/,/, $line);

			$MEASUREMENT_TRANSLATION_TABLE->{$row[0]} =
				{
				measurement_id => $row[1],
				measurement_description => $row[2]
				}
		}

		$fh->close;
	}

	my @results;

	if (keys %query == 0)
	{
		foreach my $code (keys %{$MEASUREMENT_TRANSLATION_TABLE})
		{
			push @results,
			{
			raw_data_code => $code,
			measurement_id => $MEASUREMENT_TRANSLATION_TABLE->{$code}->{measurement_id},
			measurement_description => $MEASUREMENT_TRANSLATION_TABLE->{$code}->{measurement_description}
			}
		}
	}
	elsif (exists $query{raw_data_code})
	{
		if (exists $MEASUREMENT_TRANSLATION_TABLE->{$query{raw_data_code}})
		{
			my $code = $query{raw_data_code};

			push @results,
			{
			raw_data_code => $code,
			measurement_id => $MEASUREMENT_TRANSLATION_TABLE->{$code}->{measurement_id},
			measurement_description => $MEASUREMENT_TRANSLATION_TABLE->{$code}->{measurement_description}
			}
		}
	}
	else
	{
		die "_selectMeasurementTranslation: Query not recognised.";
	}

	return @results;
}

#=========================================================================================
# The following code handles queries for results data. In the CSV file implementation
# results for a specific <config> and <build> are held in a file under a correspondingly
# named directory (i.e. <base_dir>\<config>\<build>). In an SQL implementation <config> and
# <build> information would usually be stored in separate database tables which would be
# related with a results table via appropriately chosen foreign key columns.
#=========================================================================================

my @RESULT_ROW_MAPPING =
(
	"measurement_id",
	"minimum_threshold",
	"maximum_threshold",
	"mean",
	"sdev",
	"sdev_to_mean",
	"minimum",
	"maximum",
	"count",
	"sample_80pc_confidence",
	"sample_90pc_confidence"
);

#-----------------------------------------------------------------------------------------
# Function: selectResult
#
# Argument(s): %query (hash)
#
#	If the hash is of the form:- %query = { config_name => value1, build => value2 }
#	then all results for the specified config/build are returned.
#
#	If the hash is of the form:- 
#		%query = { config_name => value1, build => value2, measurement_id => [ id1, id2, ..., idN ] }
#	then the results are additionally filtered according to the supplied set of measurement ids
#
# Returns: A list of zero or more hash references with keys :-
#	(config_name, build, measurement_id, minimum_threshold, maximum_threshold, mean, sdev, sdev_to_mean, 
#	minimum, maximum, count, sample_80pc_confidence, sample_90pc_confidence)
#
#-----------------------------------------------------------------------------------------
sub selectResult
{
	#print "_selectResult\n";
	my $self = shift;
	my %query = @_;
	die "'config_name' is a mandatory parameter." if not exists $query{config_name};
	die "'build' is a mandatory parameter." if not exists $query{build};

	my %measurement_id_filter;
	
	if (exists $query{measurement_id})
	{
		%measurement_id_filter = map { $_ => '' } @{ $query{measurement_id} };
	}

	my $config_name = $query{config_name};
	my $build = $query{build};

	my $filename = _getDataFilePath('Result', $query{config_name}, $query{build});
	my $fh = FileHandle->new;
	$fh->open("< $filename") or die "Error: $filename: $!.";

	my @results_array;

	while (my $line = <$fh>)
	{
		# Skip line if it is starting with '#'
		next if ($line =~ /^\#/);
				
		# convert the CSV line into a hash using the mapping
		my %row_hash = _mapLineToHash($line, @RESULT_ROW_MAPPING);

		# now filter result row by measurement id (if provided)

		if (keys %measurement_id_filter == 0 or exists $measurement_id_filter{$row_hash{measurement_id}})
		{
			$row_hash{config_name} = $config_name;
			my @build_cpts = split /\s/, $build;
			$row_hash{build_no} = $build_cpts[1];
			$row_hash{week_no} = $build_cpts[2];

			push @results_array, \%row_hash;
		}
	}

	$fh->close;

	return @results_array;
}

#=========================================================================================
# The following code handles queries about ranges of available build numbers. In the CSV file
# implementation per-build data is arranged in the <base_dir>\<config>\<build> directory structure
# Where the <build> directories correspond to specific builds with the build number and date
# embedded within each directory name.
#
# Thus to handle queries about builds for a specific config requires us to parse and process
# the <build> directory names under a given <config> directory according to the following
# naming convention :- <build> the build directory name consisting of the string
# 'Build <build_no> <week_no>' where <build_no> is of the form 'nnnnnssss' with 'n' a digit
# and 's' alphabetic (e.g. 03474 or 3450b) and <week_no> is of the form 'nnnn'wk'nn' (e.g.
# 2005wk06)
#=========================================================================================
sub compareBuildNo
{
	my ($buildstring, $test) = @_;
	my $result = 0;

	if ($test =~ /(\d\d\d\d)wk(\d\d)/) # e.g. "2005wk08"
	{
		my $testweek = $1 . $2;
		$buildstring =~ /(\d\d\d\d)wk(\d\d)/;
		my $buildweek = $1 . $2;
		$result = ($buildweek <=> $testweek);
	}
	elsif ($test =~ /(\d+)([a-zA-Z]*)/) # e.g. "03497" or "03497b"
	{
		my ($testno, $teststr) = ($1, (defined $2 ? $2 : ""));
		$buildstring =~ /(\d+)([a-zA-Z]*)/;
		my ($buildno, $buildstr) = ($1, (defined $2 ? $2 : ""));
		$result = ($buildno <=> $testno);
		$result = ($buildstr cmp $teststr) if $result == 0;
	}
	else
	{
		die "Error: Illegal build number format: '$test'.";
	}

	#print "compareBuildNo: <$buildstring>, <$test> => <$result>\n";
	return $result;
}

sub buildWithinRange
{
	my ($build, $range) = @_;
	my ($low, $high) = ($range->{build_low}, $range->{build_high});

	# Note: $build is a full build number comprising the numerical build and week no.
	# whereas $low/$high may include either (or both) this information

	my $ok = ((compareBuildNo($build, $low) >= 0) and (compareBuildNo($build, $high) <= 0));
	return $ok;
}

#-----------------------------------------------------------------------------------------
# Function: selectBuildRange
#
# Argument(s): %query (hash)
#
#	The query parameters are :- %query = (
#		config_name = value1, build_range => { build_low => value2, build_high => value3 }
#		)
#
#	config_name matches the name of a config directory in <base_dir>\<config>\<build>
#
#	build_low, build_high can be of the form <build_no> or <week_no>
#			e.g. for <build_no> '03474' or '03450b' etc.
#			e.g. for <week_no> '2005wk06'
#	week/build numbers can be mixed if required when specifying the range
#
# Returns: A list of zero or more hash references of the form :-
#		{ config_name => <config>, build => <build> } satisfying the criterion
#	that each <build> is within the range 'build_low'/'build_high'.
#-----------------------------------------------------------------------------------------
sub selectBuildRange
{
	my $self = shift;
	my %query = @_;
	die "'config_name' is a mandatory parameter." if not exists $query{config_name};
	die "'build_range' is a mandatory parameter." if not exists $query{build_range};
	die "'build_low' is a mandatory parameter." if not exists $query{build_range}->{build_low};
	die "'build_high' is a mandatory parameter." if not exists $query{build_range}->{build_high};

	my $config = $query{config_name};
	my @build_directories_list = <$config\\*>;

	#print "_selectBuildRange: [", (join ",", @build_directories_list), "]\n";
	my @build_range = ();

	foreach my $build_dir (@build_directories_list)
	{
		if (-d $build_dir)
		{
			my ($build) = ($build_dir =~ /$config\\(.*)$/);
			#print "_selectBuildRange: <$config>, <$build_dir>, <$build>\n";
			if (buildWithinRange($build, $query{build_range}))
			{
				#print "OK for <$build>\n";
				push @build_range, { config_name => $config, build => $build };
			}
		}
	}

	return @build_range;
}

#-----------------------------------------------------------------------------------------
# Function: selectConfig
#
# Argument(s): None
#
# Returns: A list of zero or more hash references of the form :-
#		{ config_name => <config> }
#-----------------------------------------------------------------------------------------

sub selectConfig
{
	my $self = shift;
	my %query = @_;

	my @results = ();

	if (keys %query == 0)
	{
		# enable reading whole file as one chunk
		my $SAVE_RS = $/;
		undef $/;

		# read the file as a single chunk
		my $filename = _getDataFilePath('Configs');
		my $fh = FileHandle->new;
		$fh->open("< $filename") or die "Error: $filename: $!";
		my $config_text = <$fh>;

		# restore previous record separator
		$/ = $SAVE_RS;	

		my @config_list = split /\s+/, $config_text;
		@results = map { { config_name => $_ } } @config_list;
	}

	return @results;
}

#=========================================================================================
# The following code handles event code lookup (event_def.csv file). The test output log
# file contains timing log messages. Each log message is identified by an event code which
# states that the log message marks a time interval start point or end point. The function(s)
# below perform event code lookup so that we can process the log message time stamps into
# a collection of sample time intervals (the 'raw_data').
#=========================================================================================

my $EVENT_CODE_TABLE = undef;

#-----------------------------------------------------------------------------------------
# Function: selectEventCode
#
# Argument(s): %query (hash)
#
#	If the hash is of the form :- %query = ( start_code => value ) : finds the list of
#		end codes corresponding to the specified start_code value.
#
#	If the hash is empty : returns the entire start_code => end_codes mapping table
#
# Returns: A hash comprising one key for each start_code queried whose value is a list
#	reference to a list of corresponding end_codes :-
#
#	{ start_code1 => [ end_code1a, end_code1b, ... ], start_code2 => [ ... ], ... }
#
#-----------------------------------------------------------------------------------------
sub selectEventCode
{
	my $self = shift;
	my %query = @_;

	if (not defined $EVENT_CODE_TABLE)
	{
		# first time we get called load up the event codes file into the mapping table

		$EVENT_CODE_TABLE = { };														# Reference to new anonymous hash

		my $filename = _getDataFilePath('EventCodes');
		my $fh = FileHandle->new;
		$fh->open("< $filename") or die "Error: $filename: $!.";

		while (my $line = <$fh>)
		{
			my ($start_code, $end_code) = split /,/, $line;								# split /PATTERN/,EXPR
			$EVENT_CODE_TABLE->{$start_code} = [ ] if not exists $EVENT_CODE_TABLE->{$start_code}; #reference to new Anonymous array.(= [ ])
			push @{ $EVENT_CODE_TABLE->{$start_code} }, $end_code;
		}
	}

	my %results;

	if (exists $query{start_code})
	{
		# find and return the end_code(s) corresponding to the specified start_code

		my $start_code = $query{start_code};
		$results{$start_code} = [ ];
		push @{ $results{$start_code} }, @{ $EVENT_CODE_TABLE->{$start_code} }
				if exists $EVENT_CODE_TABLE->{$start_code};
	}
	elsif (keys %query == 0)
	{
		# return the whole start => end codes mapping
		# make a deep copy in case the caller fiddles with the returned data.

		foreach my $start_code (sort keys %{ $EVENT_CODE_TABLE })
		{
			my @end_code_list = @{ $EVENT_CODE_TABLE->{$start_code} };
			$results{$start_code} = \@end_code_list;
		}
	}
	else
	{
		die "Error: _selectEventCode: query not recognised";
	}

	return %results;
}

#-----------------------------------------------------------------------------------------
# Function: selectEventName
#
# Argument(s): %query (hash)
#
# This function returns returns a list of Event Names matching the Event IDs contained
# in %query
#
# The hash can be of the following forms:
#   - { start_code => value }
#       Returns all Events matching this start code
#   - { end_code => value }
#       Returns all Events matching this end code
#   - { start_code => value, end_code => value }
#       Returns event matching both this start and end code (only one)
#   - Empty hash
#       Returns all Events
#
# Returns: A hash containing all events matching the specified Event IDs and which keys
#    are made of the start and end codes separated by a hyphenate
#
#	{ 'start_code-end_code' => eventname, ...}
#	  e.g.:
#   { '27-28' => , 'Contacts - open DB' }
#
#-----------------------------------------------------------------------------------------

my $EVENT_NAME_TABLE = undef;

sub selectEventName
{
	my $self = shift;
	my %query = @_;

	if (not defined $EVENT_NAME_TABLE)
	{
		# first time we get called load up the event codes file into the mapping table

		$EVENT_NAME_TABLE = { };

		my $filename = _getDataFilePath('EventCodes');
		my $fh = FileHandle->new;
		$fh->open("< $filename") or die "Error: $filename: $!.";

		while (<$fh>)
		{
			chomp;
			my ($start_code, $end_code, $event_name) = split /,/;
			$EVENT_NAME_TABLE->{"$start_code-$end_code"} = $event_name if not exists $EVENT_NAME_TABLE->{"$start_code-$end_code"};
		}
	}

	my %results = ();

	if (exists $query{start_code} and exists $query{end_code})
	{
		# Both START and END IDs are specified
		my $start_code = $query{start_code};
		my $end_code = $query{end_code};
		$results{"$start_code-$end_code"} = $$EVENT_NAME_TABLE{"$start_code-$end_code"};
	}
	elsif (exists $query{start_code} and not exists $query{end_code})
	{
		# Only START ID is specified
		foreach my $code (sort keys %{ $EVENT_NAME_TABLE })
		{
			$results{$code} = $$EVENT_NAME_TABLE{$code} if ($code =~ /^$query{start_code}\-/);
		}
	}
	elsif (exists $query{end_code} and not exists $query{start_code})
	{
		# Only END ID is specified
		foreach my $code (sort keys %{ $EVENT_NAME_TABLE })
		{
			$results{$code} = $$EVENT_NAME_TABLE{$code} if ($code =~ /\-$query{end_code}$/);
		}
	}
	elsif (keys %query == 0)
	{
		# Make a deep copy of $EVENT_NAME_TABLE
		foreach my $code (sort keys %{ $EVENT_NAME_TABLE })
		{
			$results{$code} = $$EVENT_NAME_TABLE{$code};
		}
	}
	else
	{
		die "Error: _selectEventName: query not recognised";
	}

	return %results;
}

#-----------------------------------------------------------------------------------------

my $THRESHOLD_LOOKUP = undef;

sub __loadThresholdData
{
	$THRESHOLD_LOOKUP = { };

	my $filename = _getDataFilePath('Thresholds');
	my $fh = FileHandle->new;
	$fh->open("< $filename") or die "Error: File: $filename: $!";

	while (my $line = <$fh>)
	{
		chomp($line);
		my ($id, @thresholds) = split(/,/, $line);
		$THRESHOLD_LOOKUP->{$id} = \@thresholds;
	}
}

sub selectThreshold
{
	my $self = shift;
	my %query = @_;

	die "Error: Must supply 'measurement_id' value." if not exists $query{measurement_id};

	__loadThresholdData() if not defined $THRESHOLD_LOOKUP;

	my $id = $query{measurement_id};
	my @thresholds = (0, 0);

	@thresholds = @{$THRESHOLD_LOOKUP->{$id}} if exists $THRESHOLD_LOOKUP->{$id};
	my %result = ( measurement_id => $id, threshold_min => $thresholds[0], threshold_max => $thresholds[1] );

	return %result;
}

1;
