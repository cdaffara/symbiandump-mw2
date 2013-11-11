# Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#!perl

$count = @ARGV;
die "ERROR: Bad arguments\n\nUsage: \n\nlogsplit logfilename\n\n" if ($count!=1);

$state = 1;
while (<>) {
	if ($state==1) {
		# regular expression here searching for ---->START
		if (/=====> START /) {
			@fields = split /\[/;
			shift @fields;
			@fields = split /\]/;
			$_ = shift @fields;
			@fields = split / /;
			$filename = $fields[2];
			@fields = split /\\/, $filename;
			$filename = $fields[@fields-1];
			# open file here
			print "$filename \n";
			open LOGHANDLE, ">$filename"
				or die "FAILED: opening file $filename.\n($!)\n";
			$state=2;
		}
	}
	elsif ($state==2) {
		# regular expression here searching for ---->STOP
		if (/=====> STOP /) {
			# check that start and stop matches ?
			# close file here
			close LOGHANDLE;
			$state=1;
		}
		elsif (/=====> START /) {
			# Start found during a file...?
			close LOGHANDLE;
			die "FAILED: source file corrupt!\n\n";
		}
		else {
			print LOGHANDLE $_;
			# write line to file
		}
	}
}
