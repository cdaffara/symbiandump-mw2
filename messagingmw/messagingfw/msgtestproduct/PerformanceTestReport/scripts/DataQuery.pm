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

package DataQuery;
use strict;
use FileHandle;
#===============================================================================
# Module: Data Query
#
# Description: Provides an interface to an underlying data storage mechanism. The
# 	interface does not implement general purpose data manipulation functions
#	but implements certain application specific functions which an underlying
#	implementation may perform using a specific mechanism (i.e. SQL database
#	or CSV files).
#
#	The implementation to use is specified and created by the initialise()
#	subroutine. Calls to other methods before the implemenmtation has been
#	created will cause the script to exit with a call to die().
#
#===============================================================================

my $IMPLEMENTATION_NAME = "";
my $IMPLEMENTATION = undef;

sub initialise
{
	$IMPLEMENTATION_NAME = shift if @_ > 0;

	# dynamically load the perl module for the required implementation
	# and if successful create an instance via its 'new' method.

	eval ("require $IMPLEMENTATION_NAME;"); 

	if ($@)
	{
		die "Error loading implementation module: $IMPLEMENTATION_NAME";
	}
	else
	{
		$IMPLEMENTATION = $IMPLEMENTATION_NAME->new(@_);
	}
}

sub selectMeasurementTranslation
{
	die "Error: Not initialised." if not defined $IMPLEMENTATION;

	return $IMPLEMENTATION->selectMeasurementTranslation(@_);
}

sub selectResult
{
	die "Error: Not initialised." if not defined $IMPLEMENTATION;

	return $IMPLEMENTATION->selectResult(@_);
}

sub selectBuildRange
{
	die "Error: Not initialised." if not defined $IMPLEMENTATION;

	return $IMPLEMENTATION->selectBuildRange(@_);
}

sub selectConfig
{
	die "Error: Not initialised." if not defined $IMPLEMENTATION;

	return $IMPLEMENTATION->selectConfig(@_);
}

sub selectEventCode
{
	die "Error: Not initialised." if not defined $IMPLEMENTATION;

	return $IMPLEMENTATION->selectEventCode(@_);
}

sub selectEventName
{
	die "Error: Not initialised." if not defined $IMPLEMENTATION;

	return $IMPLEMENTATION->selectEventName(@_);
}

sub selectThreshold
{
	die "Error: Not initialised." if not defined $IMPLEMENTATION;

	return $IMPLEMENTATION->selectThreshold(@_);
}

1;