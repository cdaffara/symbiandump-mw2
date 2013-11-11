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



my @data,@data1;
my ($entry_value, $size_value)=0;

my @entry_array = (1, 10, 50 , 100, 200, 500, 1000, 2000, 5000, 15000);
my @size_array = (1, 5, 10, 100, 200, 500, 1000);

#my @entry_array = (1, 10, 50);
#my @size_array = (1, 5);

# ##### script copying
my $filename = "msg-imap-performance";
open(FILE,"msg-imap-performance0_0kb.script");
@data = <FILE>;
close(FILE);


for($entry_value=0; $entry_value < @entry_array; $entry_value++)
{
	for($size_value=0; $size_value < @size_array; $size_value++)
	{
	
		print $entry_array[$entry_value]."_".$size_array[$size_value]."\n";
		$filename = "M:\\performance-scripts\\msg-imap-performance";
		@data1 = @data;
 		foreach(@data1)
 		{
 			$_ =~ s/performance1_1/performance$entry_array[$entry_value]_$size_array[$size_value]/isg;
 		}
 		$filename = $filename.$entry_array[$entry_value]."_".$size_array[$size_value]."kb.script";
 		open(FILE,">>$filename");
 		print FILE @data1;
 		close(FILE);
 		
 	}
 }

    
exit;

