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

use strict;
use Spreadsheet::ParseExcel;
use Spreadsheet::WriteExcel;
use Data::Dumper;
use Switch;

# cobbled together from examples for the Spreadsheet::ParseExcel and
# Spreadsheet::WriteExcel modules

my $destname;
my $extension = ".xls";
my $matchword="ManufacturerSoftwareBuild";
my @contents;
my $final_destname;

my $diffTime;

my @create_array;
my @send_array;
my @header_download_array;
my @body_download_array;
my @copy_array;
my @move_array;
my @delete_array;

my @temp_array;

my $startTime;
my $endTime;
my $entryValue = 0;
my $sizeValue = 0;
my $row_index=0;
my $column_index=0;
my $sheet_count = 0;
my $count;

my $platform = shift @ARGV or die "invocation: $0 <platform> winscw or armv5";

parsePerformanceLogs();

open(FILE,"/epoc32/data/buildinfo.txt");
my @data = <FILE>;
close(FILE);
foreach(@data)
{
	#print "$_";
	if($_ =~ /$matchword/ig)
	{
			print "match perfect. $_ \n";
			$destname = substr($_,length($matchword)+2);
			my $position = index($destname,"_",0);
			#@contents = split(/ /,$_);
			print "$destname \n";
			$destname = substr($destname,0,$position);
			$destname = $destname."_".$platform;
			$final_destname = $destname.$extension;
			print "$final_destname \n";
			my $fileCount = 0;
			while(-e $final_destname)
			{
				print "File name $final_destname already exists\n";
				$fileCount = $fileCount + 1;
				$final_destname = $destname."_".$fileCount.$extension;
			}
	}
	else
	{
		print "mismatch \n";
	}
}

# my $sourcename = shift @ARGV;
# my $destname = shift @ARGV or die "invocation: $0 <destination file>";

my $storage_book;
my $dest_sheet;
my @SheetNames = ("Create", "Send", "Header-Download", "Body-Download", "Copy", "Move", "Delete");

my $dest_book  = Spreadsheet::WriteExcel->new("$final_destname")
 or die "Could not create a new Excel file in $final_destname: $!";

print "Size of the create array is $#{$create_array[0]}\n";
print "Size of the create array is $#create_array\n";
my $row_array = 0;
my $column_array = 0;

for ($count=0; $count < @SheetNames; $count++)
{
	$dest_sheet = $dest_book->addworksheet($SheetNames[$count]);
	print "Sheet : $SheetNames[$count] added \n";
	addTemplate();
}

print "Perl recognized the following data (sheet/row/column order):\n";
# print Dumper $storage_book;

$dest_book->close();

print "done!\n";

sub addTemplate
{
	my $entry_index = 2;
	$dest_sheet->write(5, $entry_index, "EMail Size Range (Kb)");
	$entry_index++;
	$dest_sheet->write(5, $entry_index, "Number of Entries");
	$dest_sheet->write(6, $entry_index++, "1");
	$dest_sheet->write(6, $entry_index++, "10");
	$dest_sheet->write(6, $entry_index++, "50");
	$dest_sheet->write(6, $entry_index++, "100");
	$dest_sheet->write(6, $entry_index++, "200");
	$dest_sheet->write(6, $entry_index++, "500");
	$dest_sheet->write(6, $entry_index++, "1000");
	$dest_sheet->write(6, $entry_index++, "2000");
	$dest_sheet->write(6, $entry_index++, "5000");
	$dest_sheet->write(6, $entry_index++, "15000");

	$dest_sheet->write(7, 2, "1Kb");
	$dest_sheet->write(8, 2, "5Kb");
	$dest_sheet->write(9, 2, "10Kb");
	$dest_sheet->write(10, 2, "100Kb");
	$dest_sheet->write(11, 2, "200Kb");
	$dest_sheet->write(12, 2, "500Kb");
	$dest_sheet->write(13, 2, "1000Kb");

	my @temp_array;
	switch($count)
	{
		case 0
		{
			@temp_array = @create_array;
		}
		case 1
		{
			@temp_array = @send_array;
		}
		case 2
		{
			@temp_array = @header_download_array;
		}
		case 3
		{
			@temp_array = @body_download_array;
		}
		case 4
		{
			@temp_array = @copy_array;
		}
		case 5
		{
			@temp_array = @move_array;
		}
		case 6
		{
			@temp_array = @delete_array;
		}
	}
	my $row_counter = $#temp_array;
#	my $column_counter = $#{$temp_array[0]};
	print "Row counter = $row_counter\n";

	for ($row_array = 0; $row_array <= $row_counter; $row_array++)
	{
		my $column_counter = $#{$temp_array[$row_array]};
		print "Column counter = $column_counter\n";
		for($column_array=0; $column_array <= $column_counter; $column_array++)
		{
			my $row_value = $row_array+7;
			my $column_value = $column_array+3;
			$dest_sheet->write($row_value,$column_value,$temp_array[$row_array][$column_array]);
			print "Value is $temp_array[$row_array][$column_array]\n";
		}
	}
}

sub parsePerformanceLogs
{
	my @files = </performance-logs/$platform/*.htm>;
	my $tempCount = 0;;
	foreach my $file (@files) 
	{
		print "\n" . $file . "\n";
		open(LOGFILE,"$file");
		@data = <LOGFILE>;
		close(LOGFILE);

		if($file =~ /performance(\d+)_(\d+)/)
		{
			$entryValue = $1;
			$sizeValue = $2;
		}
		switch($entryValue)
		{
			case 1
			{
				$column_index=0;
			}
			case 10
			{
				$column_index=1;
			}
			case 50
			{
				$column_index=2;
			}
			case 100
			{
				$column_index=3;
			}
			case 200
			{
				$column_index=4;
			}
			case 500
			{
				$column_index=5;
			}
			case 1000
			{
				$column_index=6;
			}
			case 2000
			{
				$column_index=7;
			}
			case 5000
			{
				$column_index=8;
			}
			case 15000
			{
				$column_index=9;
			}

		}
		switch($sizeValue)
		{
			case 1
			{
				$row_index=0;
			}
			case 5
			{
				$row_index=1;
			}
			case 10
			{
				$row_index=2;
			}
			case 100
			{
				$row_index=3;
			}
			case 200
			{
				$row_index=4;
			}
			case 500
			{
				$row_index=5;
			}
			case 1000
			{
				$row_index=6;
			}
		}
		print $entryValue."\n".$sizeValue;
		$sheet_count = 0;
		foreach(@data)
		{
			if($_ =~ /EVENT_LOG_INFORMATION/ig)
			{
				@contents = split(",",$_);
				#print $contents[4]."\n";
				if($tempCount)
				{
					$tempCount = 0;
					$endTime = $contents[4];
					print $endTime."\n";
					calculateDiffTime();
					updateArray();
					$sheet_count = $sheet_count + 1;
				}
				else
				{
					$tempCount = 1;
					$startTime = $contents[4];
					print $startTime."\n";
				}
			}
		}
	}
}

sub calculateDiffTime
{
	my $srcTime;
	my $dstTime;
	my @sourceTime = split(":",$startTime);
	my @destTime = split(":",$endTime);
#	print "Inside calculateDiffTime\n";
#	print $startTime."\n";
#	print $endTime."\n";
	my $startHours = $sourceTime[0];
	my $startMinutes = $sourceTime[1];
	my $startSeconds = $sourceTime[2];
	my $startMilliseconds = $sourceTime[3];
	my $endHours = $destTime[0];
	my $endMinutes = $destTime[1];
	my $endSeconds = $destTime[2];
	my $endMilliseconds = $destTime[3];
	if($endHours < $startHours)
	{
		$endHours = $endHours + 24;
	}
	$srcTime = $startMilliseconds + ($startSeconds*1000) + ($startMinutes*60*1000) + $startHours*60*60*1000;
	$dstTime = $endMilliseconds + ($endSeconds*1000) + ($endMinutes*60*1000) + $endHours*60*60*1000;
	$diffTime = ($dstTime - $srcTime)/1000;
#	print "Time Difference is $diffTime milliseconds\n";
}

sub updateArray
{
	switch($sheet_count)
	{
		case 0
		{
			#push(@create_array,'-1');
			#push(@create_array[$row_index],'-1');
			$create_array[$row_index][$column_index] = $diffTime;
			print " Create array updated at $row_index and $column_index with value $diffTime\n";
		}
		case 1
		{
			#push(@send_array,'-1');
			#push(@send_array[$row_index],'-1');
			$send_array[$row_index][$column_index] = $diffTime;
			print " Send array updated at $row_index and $column_index with value $diffTime\n";
		}
		case 2
		{
			#push(@header-download_array,'-1');
			#push(@header-download_array[$row_index],'-1');
			$header_download_array[$row_index][$column_index] = $diffTime;
			print " Header download array updated at $row_index and $column_index with value $diffTime\n";
		}
		case 3
		{
			#push(@body-download_array,'-1');
			#push(@body-download_array[$row_index],'-1');
			$body_download_array[$row_index][$column_index] = $diffTime;
			print " Body download array updated at $row_index and $column_index with value $diffTime\n";
		}
		case 4
		{
			#push(@copy_array,'-1');
			#push(@copy_array[$row_index],'-1');
			$copy_array[$row_index][$column_index] = $diffTime;
			print " Copy array updated at $row_index and $column_index with value $diffTime\n";
		}
		case 5
		{
			#push(@move_array,'-1');
			#push(@move_array[$row_index],'-1');
			$move_array[$row_index][$column_index] = $diffTime;
			print " Move array updated at $row_index and $column_index with value $diffTime\n";
		}
		case 6
		{
			#push(@delete_array,'-1');
			#push(@delete_array[$row_index],'-1');
			$delete_array[$row_index][$column_index] = $diffTime;
			print " Delete array updated at $row_index and $column_index with value $diffTime\n";
		}
	}
}

exit;
