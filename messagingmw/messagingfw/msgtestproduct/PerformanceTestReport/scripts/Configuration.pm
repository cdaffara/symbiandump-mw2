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

package Configuration;
use strict;
1;

my $ThresholdFileName = "threshold.csv";
my $MeasurementDefFileName = "measurement_def.csv";
my $EventCodeFileName = "event_def.csv";
my $RawDataFileName = "raw_data.csv";
my $ResultsFileName = "results.csv";
my $ConfigFileName = "config.csv";
my $logfiletype = "speedtest";

sub ThresholdFileName
{
	$ThresholdFileName = shift if @_ > 0;
	return $ThresholdFileName;
}

sub MeasurementDefFileName
{
	$MeasurementDefFileName = shift if @_ > 0;
	return $MeasurementDefFileName;
}

sub EventCodeFileName
{
	$EventCodeFileName = shift if @_ > 0;
	return $EventCodeFileName;
}

sub RawDataFileName
{
	$RawDataFileName = shift if @_ > 0;
	return $RawDataFileName;
}

sub ResultsFileName
{
	$ResultsFileName= shift if @_ > 0;
	return $ResultsFileName;
}

sub ConfigFileName
{
	$ConfigFileName= shift if @_ > 0;
	return $ConfigFileName;
}

