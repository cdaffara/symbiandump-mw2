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
# -----------------------------------------------------------------

sub load_default_bat {
	unless (open DEFAULT_BAT, $default_bat_file) {
		die "ERROR: Cannot open default batch file: $!";
	}
	@default_bat = <DEFAULT_BAT>;
	close DEFAULT_BAT;
}

# -----------------------------------------------------------------

sub create_default_cleanup {
	if (@default_clean == 0) {
		unless (open DEFAULT_CLEAN, "DEFAULTCLEAN.TXT") {
			die "ERROR: Cannot open default cleanup batch file: $!";
		}
		@default_clean = <DEFAULT_CLEAN>;
		close DEFAULT_CLEAN;
	}
	$file_name = "\\epoc32\\data\\z\\system\\msgtest\\$_[0]\\T_$_[1]_CLEANUP_FROM_TEST.BAT";
	unless (open NEW_CLEAN, ">$file_name") {
		die "ERROR: Could not create cleanup batch file: $!";
	}
	print NEW_CLEAN @default_clean;
	close NEW_CLEAN;
}

# -----------------------------------------------------------------

sub auto_create_bat {
	# check for existance of bat file, if not present, create
	$file_name = "\\epoc32\\data\\z\\system\\msgtest\\$_[0]\\$_[1].BAT";
	unless (open NEW_BAT, $file_name) {
		unless (open NEW_BAT, ">$file_name") {
			die "ERROR: Could not create batch file: $!";
		}
		@auto_bat = @default_bat;
		foreach (@auto_bat) {
			s/\@HARNESSNAME@/$_[1]/;
		}
		print NEW_BAT @auto_bat;
		print "$file_name created.\n";
	}
	close NEW_BAT;
}

# -----------------------------------------------------------------

sub add_to_ibybat {
	$file_name = "T_$_[0]_$_[1]_TEST.BAT";
	print HARNESSIBY "data=\\epoc32\\data\\z\\system\\msgtest\\BUILD_DIR\\$file_name System\\MsgTest\\$file_name\n";
	print AUTOEXECUDEB "$file_name\n";
	print AUTOEXECUDEB $change_dir;
	print AUTOEXECUREL "$file_name\n";
	print AUTOEXECUDEB $change_dir;
}

# -----------------------------------------------------------------

die "ERROR: Bad arguments\n\n" if (@ARGV!=2);

$default_bat_file = $ARGV[1];
pop @ARGV;

&load_default_bat();

unless (open HARNESSIBY, ">\\epoc32\\rom\\include\\MsgTestHarnesses.iby") {
	die "ERROR: Cannot create MSGTESTHARNESSES.IBY file: $!";
}
print HARNESSIBY "REM\nREM Copyright © 2002 Symbian Ltd. All rights reserved.\nREM\n\n";
print HARNESSIBY "data=\\epoc32\\data\\z\\system\\msgtest\\BUILD_DIR\\AUTOEXEC.BAT AUTOEXEC.BAT\n\n";


$change_dir = "z:\ncd \\system\\msgtest\n";
unless (open AUTOEXECUDEB, ">\\epoc32\\data\\z\\system\\msgtest\\udeb\\AUTOEXEC.BAT") {
	die "ERROR: Cannot create file: $!";
}
print AUTOEXECUDEB $change_dir;

unless (open AUTOEXECUREL, ">\\epoc32\\data\\z\\system\\msgtest\\urel\\AUTOEXEC.BAT") {
	die "ERROR: Cannot create file: $!";
}
print AUTOEXECUREL $change_dir;

while (<>) {
	chomp;
	# remove comments
	@fields = split /#/, $_;
	$_ = $fields[0];
	next if ("");

	# check for a COMPONENT line
	if (/COMPONENT/i) {
		# add cleanup stage for previous component AUTOEXEC file(s)...
		&add_to_ibybat($component,"CLEANUP_FROM") if ($start_located);

		# expect line of this form: COMPONENT <COMPONENT_NAME>
		@fields = split;
		$component = $fields[1];
		print HARNESSIBY "\nREM $component\n\n#include <T_$component.IBY>\n";
		print "COMPONENT: $component.\n";

		&create_default_cleanup("UREL", $component);
		&create_default_cleanup("UDEB", $component);

		# add preparation stage to AUTOEXEC file(s).
		&add_to_ibybat($component,"PREPARE_FOR");		
		$start_located = 1;
		next;
	}

	# check for a test harness line
	if ((/UREL|UDEB/i) && defined($start_located)) {
		@fields = split;
		$test_harness = $fields[0];
		$test_harness_iby = $test_harness . ".IBY";
		$test_harness_bat = $test_harness . ".BAT";

		# add to iby
		print HARNESSIBY "data=\\epoc32\\data\\z\\system\\msgtest\\BUILD_DIR\\$test_harness_bat System\\MsgTest\\$test_harness_bat\n";

		&auto_create_bat("urel",$test_harness);
		&auto_create_bat("udeb",$test_harness);

		if (/UREL/si) {
			print AUTOEXECUREL "$test_harness_bat\n";
			print AUTOEXECUREL $change_dir;
			print AUTOEXECUREL "T_SERIALLOG c:\\MsgLogs\\$test_harness.THUMB.REL.LOG\n";
		}

		if (/UDEB/si) {
			print AUTOEXECUDEB "$test_harness_bat\n";
			print AUTOEXECUDEB $change_dir;
####			print AUTOEXECUDEB "T_SERIALLOG d:\\MsgLogs\\$test_harness.THUMB.DEB.LOG\n";
		}
#		print $_ . "\n";
	}

}

if (defined($component)) {
	&add_to_ibybat($component,"CLEANUP_FROM");
}

close HARNESSIBY;

print AUTOEXECUREL $change_dir;
print AUTOEXECUREL "T_SERIALLOG TERMINATE\n";
print AUTOEXECUREL "T_SERIALLOG TERMINATE\n";
close AUTOEXECUREL;

print AUTOEXECUDEB $change_dir;
####print AUTOEXECUDEB "T_SERIALLOG TERMINATE\n";
####print AUTOEXECUDEB "T_SERIALLOG TERMINATE\n";
close AUTOEXECUDEB;

print "Automatic preparation completed.\n";
