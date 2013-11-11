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

$_ = "\U$ARGV[0]";
die "ERROR: Bad arguments\n\n" if ((@ARGV!=1) || !(/UREL|UDEB/));

$release = $_;
$header_file_name = "\\epoc32\\rom\\include\\header.iby";
$ARGV[0] = $header_file_name;

while (<>) {
	s/(urel|udeb)/$release/gi if (/define BUILD_DIR/);
	push @header_file, $_;
}

open OVERWRITE, ">$header_file_name";
print OVERWRITE @header_file;
close OVERWRITE;
