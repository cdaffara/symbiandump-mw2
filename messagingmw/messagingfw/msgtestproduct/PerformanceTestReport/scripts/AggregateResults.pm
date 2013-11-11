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

package AggregateResults;
use strict;
use FileHandle;
use Configuration;
use DataQuery;
1;

my $ERRFUNC_80 = 1.383;
my $ERRFUNC_90 = 1.895;

sub min
{
	return ($_[0] < $_[1] ? $_[0] : $_[1]);
}

sub max
{
	return ($_[0] > $_[1] ? $_[0] : $_[1]);
}

sub stddev
{
	my ($sumX, $sumX2, $count) = @_;
	my $sdev = ($count * $sumX2 - $sumX * $sumX);
	$sdev = ($count > 1 ? $sdev / ($count * ($count - 1)) : $sdev);
	$sdev = sqrt($sdev);
	return $sdev;	
}

sub sampleSizeForConfidence
{
	my ($erfValue, $stdDev, $halfWidth) = @_;
	my $size = $erfValue * $stdDev / $halfWidth;
	return int(0.5 + $size * $size);
}

sub WriteAggregatedResult
{
	my ($current, $fields, $outputFH) = @_;
	my $name = $current->{name};
	my $count = $current->{count};
	my $sumX = $current->{linearsum};
	my $sumX2 = $current->{squaredsum};

	my %thresholds = DataQuery::selectThreshold( measurement_id => $name );
	my $minThresh = $thresholds{threshold_min};
	my $maxThresh = $thresholds{threshold_max};

	my $mean = $sumX / $count;
	my $sdev = stddev($sumX, $sumX2, $count);
	my $sdev2mean = $sdev / $mean;
	my $min = $current->{minimum};
	my $max = $current->{maximum};

	my $samples80 = sampleSizeForConfidence($ERRFUNC_80, $sdev, $mean * 0.05);
	my $samples90 = sampleSizeForConfidence($ERRFUNC_90, $sdev, $mean * 0.035);

	unless ($name eq '')
	{
		print $outputFH "$name, $minThresh, $maxThresh, $mean, $sdev, $sdev2mean, $min, $max, $count, $samples80, $samples90\n";
	}
}

#--------------------------------------------------------------------------------
# Function: results
#
# Arguments:
#	$inputFH - File handle to read for input raw data.
#
#	$outputFH - File handle to write aggregated output data.
#
# Description: The input raw data CSV file is already sorted by the name of the
#	sample time interval. This routine reads each raw data record and accumulates
#	data for measurements with the same name. At the end of each group of
#	identically named samples various measures are calculated and output to
#	the output file handle in CSV format.
#
#--------------------------------------------------------------------------------
sub results
{
	my ($inputFH, $outputFH) = @_;
	my $group = undef;
	my @fields;
	
	# Print table headers
	print $outputFH "#Measurement name,Minimum threshold,Maximum threshold,Mean,SD,SD/mean,Min,Max,Count,80pc confidence sample size,90pc confidence sample size\n";
	
	while (my $line = <$inputFH>)
	{
		# Skip line if it is starting with a '#'
		next if ($line =~ /^\#/);
		
		@fields = split /,/, $line;
 		my $measurementName = $fields[1];
		my $measurementValue = $fields[2];

		if (! defined $group or $group->{name} ne $measurementName)
		{
			# this is either the very first record of the file or the
			# very first record in the next group of records
			if (defined $group)
			{
				# starting next group - process accumulated data
				WriteAggregatedResult($group, \@fields, $outputFH);
			}

			$group =
				{
				name		=> $measurementName,
				count		=> 1,
				minimum		=> $measurementValue,
				maximum		=> $measurementValue,
				linearsum	=> $measurementValue,
				squaredsum	=> $measurementValue * $measurementValue
				};
		}
		elsif ($group->{name} eq $measurementName)
		{
			# the current record is in the current group - accumulate the data
			$group->{count}		+= 1;
			$group->{minimum}	=  min($group->{minimum}, $measurementValue);
			$group->{maximum}	=  max($group->{maximum}, $measurementValue);
			$group->{linearsum}	+= $measurementValue;
			$group->{squaredsum}	+= $measurementValue * $measurementValue;
		}
	}

	# process accumulated data for the final group of records
	WriteAggregatedResult($group, \@fields, $outputFH);
}

1;
