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
#-------------------------------------------------------------------------------
# Script: compare.pl
#
# Description: This script combines results.csv files for specified config\build
#	combinations (and optionally a specified measurement subset). Given one
#	or more config/build specifications this script combines the output of the
#	results.pl scripts under each <base>\<config>\<build> directory into a single
#	CSV file for use by e.g. excel for displaying graphical comparisons of results
#	across different config and/or build combinations.
#
# Arguments:
#
#	config@build : a config/build to process (may be repeated)
#
#	'build' may specify a single build (e.g. in either format 03510 or 2005wk05)
#	or a range of builds (e.g. 2005wk06..2005wk10 or 03501..03520 or mixed).
#
#	'config' specifies a specific hardware/software combination for which
#	to process the specified build results. e.g. 9.1-NAND-H2.
#
#	if 'config' has the value 'ALL' then all available configs will be
#	processed for the specified builds.
#
#	-i <in_file>: input file name to use under specified build directories (default is results.csv)
#
#	-o <out_file>: output file name (default is STDOUT)
#
#	-t <measurement_id> : specifies a measurement id to include (default is all)
#
#	Note: multiple config@build arguments and multiple '-t' options are allowed.
#
#-------------------------------------------------------------------------------

use FileHandle;
use CompareResults;
use FindBin;

my $OUTPUT_FILE = undef;
my $RESULTS_FILE = "results.csv";
my $CONFIG_FILE = "$FindBin::Bin\\config.csv";

sub usage
{
	print <<USAGE;
usage:
	compare.pl <config>\@<build> [ <config>\@<build> .... ] [ -i <in-file> ] [ -o  <out-file> ] [ -t <msmnt-id> [ -t ... ] ]

where:
	<config>\@<build> : a config/build to process

	<config> specifies a specific hardware/software combination for which
	to process the specified build results. e.g. 9.1-NAND-H2.

	if <config> has the value 'ALL' then all available configs will be
	processed for the specified builds.

	<build> may specify a single build (e.g. in either format 03510 or 2005wk05)
	or a range of builds (e.g. 2005wk06..2005wk10 or 03501..03520 or mixed).

	-i <in-file>: input file name to use under specified build directories (default is results.csv)

	-o <out-file>: output file name (default is STDOUT)

	-t <msmnt-id> : specifies a measurement id to include (default is all)

	Note: multiple config\@build arguments and multiple '-t' options are allowed.
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

	@errors = ();
	@compare  = ();
	@testcases = ();

	# process the command line arguments

	while (@argv > 0)
	{
		my $arg = shift @argv;
		#print "Got: <$arg>\n";

		if ($arg =~ /^(.*)\@(.*)$/)
		{
			my $config = $1;
			my $build = $2;
			#print "Got: (config, build) = (<$config>, <$build>)\n";

			push @compare, { config_name => $config, build => $build };
		}
		elsif ($arg =~ /-t/)
		{
			if (@argv > 0)
			{
				push @testcases, shift @argv;
			}
			else
			{
				push @errors, "Error: Option -t requires testcase name argument.";
			}
		}
		elsif ($arg =~ /-o/)
		{
			if (@argv > 0)
			{
				$OUTPUT_FILE = shift @argv;
			}
			else
			{
				push @errors, "Error: Option -o requires ouput file argument.";
			}
		}
		elsif ($arg =~ /-i/)
		{
			if (@argv > 0)
			{
				$RESULTS_FILE = shift @argv;
			}
			else
			{
				push @errors, "Error: Option -i requires results input file argument.";
			}
		}
	}

	if (@errors == 0)
	{
		# set the location of the list of available configs
		Configuration::ConfigFileName($CONFIG_FILE);
		Configuration::ResultsFileName($RESULTS_FILE);

		# set the choice of implementation for the underlying data storage
		# For the time being we use a hard-coded choice of CSV files
		DataQuery::initialise('DataQueryCSVFile');

		my $outputFH = \*STDOUT;

		if (defined $OUTPUT_FILE)
		{
			$outputFH = FileHandle->new;
			$outputFH->open("> $OUTPUT_FILE") or die "Error: File $OUTPUT_FILE: $!";
		}

		CompareResults::compare(\@compare, \@testcases, $outputFH);

		$outputFH->close;
	}
	else
	{
		print STDERR "Errors: ", (join "\n", @errors), "\n";
	}

}

Main(@ARGV);