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

package CompareResults;
use strict;
use FileHandle;
use DataQuery;
1;

#-------------------------------------------------------------------------------
# This array defines the order in which retrieved columns will appear in the output
# CSV text and is also the title of each column appearing at the head of the output.
#-------------------------------------------------------------------------------
my @COLUMN_ORDER =
(
	"config_name",
	"build_no",
	"week_no",
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

#-------------------------------------------------------------------------------
# Function: compare
#
# Arguments:
#	$comparisons - reference to a list of comparisons. Each comparison is of
#	the form 'config@build' where 'config' specifies a test configuration and
#	'build' specifies a build (or range) under that configuration. A single
#	build may be specified in terms of its build number (e.g. 03474 or 03450b)
#	or its week number (e.g. 2005wk06). Alternatively build may specify a range
#	of builds in the form 'low..high' where low and high are each in the build
#	or week number fromat already discussed. If the value 'ALL' is given for
#	config then all builds matching the given build specification under every
#	build configuration are selected for comparison.
#
#	$testcases - reference to a list of measurement_ids (each such id corresponds
#	to a named time interval measurement by the performance tests). If the list
#	is empty then all the config/build results selected by '$comparisons' are
#	included in the output. If non-empty then only those results whose id matches
#	one of those specified in '$testcases' are included in teh output.
#
#	$outputFH - a filehandle to a file opened for output (may be STDOUT) - the
#	results of teh selected config/build/testcases are written to this file.
#
#-------------------------------------------------------------------------------
sub compare
{
	my ($comparisons, $testcases, $outputFH) = @_;

	my @all_results;
	#
	# process the build numbers so that any ranges of build numbers of the form
	# "<min>..<max>" are expanded to the full list form.
	#
	my @expanded_comparisons = ();

	while (@{$comparisons} > 0)
	{
		my $test = shift @{$comparisons};

		my $buildstring = $test->{build};
		my $config = $test->{config_name};

		if ($config eq "ALL")
		{
			my @allconfigs = DataQuery::selectConfig;

			while (@allconfigs > 0)
			{
				my $cfgitem = pop @allconfigs;
				$cfgitem->{build} = $buildstring;
				unshift @{$comparisons}, $cfgitem;
			}

			$test = shift @{$comparisons};
			$buildstring = $test->{build};
			$config = $test->{config_name};
		}

		my %range;

		if ($buildstring =~ /^(.*?)\.\.(.*?)$/) # e.g. like '03401..03420'
		{
			%range = ( build_low => $1, build_high => $2 );
		}
		else
		{
			%range = ( build_low => $buildstring, build_high => $buildstring );
		}

		my @results = DataQuery::selectBuildRange(config_name => $config, build_range => \%range);
		push @expanded_comparisons, @results;
	}
	


	# get the results for each (config, build) comparison. This retrieves the
	# results for all test cases unless a subset is also specified.
	#
	while (@expanded_comparisons > 0)
	{
		my %query = %{ shift @expanded_comparisons };

		$query{measurement_id} = $testcases if (@{$testcases} > 0);

		my @results = DataQuery::selectResult(%query);
		push @all_results, @results;
	}

	#
	# process and output the complete result set (with the column titles at the head)
	#
	print $outputFH (join ",", @COLUMN_ORDER);

	while (@all_results > 0)
	{
		my $output = "";
		my $row = shift @all_results;

		foreach my $column (@COLUMN_ORDER)
		{
			my $value = $row->{$column};
			$output .= $value . ",";
		}

		$output =~ s/,$/\n/;
		print $outputFH $output;
	}
}

