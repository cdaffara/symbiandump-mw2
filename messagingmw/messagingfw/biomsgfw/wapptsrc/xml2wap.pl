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

#!/usr/bin/perl

# xml2wap.pl
# This script takes an XML file and creates a WAP Binary encoded file from it.
# The constants derfined here are for the WAP Provisioning messages - to handle a different DTD
# you should just be able to change the constants (but I haven't tested that).
# Ian McDowall December 2001

use strict;

$Main::DebugLevel = 2; # Set higher for more debug or lower for less.

# Global variables used to set standard header values - modify these here
$Main::XMLVersion = 0x01 ; # 0x01 = v1.1
$Main::PublicId = 0x01 ; # Magic number taken from OTA doc
$Main::Charset = 106 ; # Magic number from OTA

$Main::EndToken = 1 ; #Magic number from OTA
$Main::InLineString = 3 ; # Magic number from OTA

# Hash containing defined tag tokens
%Main::TagTokens = (
	"CHARACTERISTIC-LIST", 0x05,
	"CHARACTERISTIC", 0x06,
	"PARM", 0x07);

# Hash containing defined attribute name-value pairs.
# Ones which do not appear here are handled as inline strings
%Main::AttributePairs = (
	"TYPE=ADDRESS", 0x06,
	"TYPE=URL", 0x07,
	"TYPE=NAME", 0x08,
	"TYPE=MMSURL", 0x7c,
	"NAME=BEARER", 0x12,
	"NAME=PROXY", 0x13,
	"NAME=PORT", 0x14,
	"NAME=NAME", 0x15,
	"NAME=PROXY_TYPE", 0x16,
	"NAME=URL", 0x17,
	"NAME=PROXY_AUTHNAME", 0x18,
	"NAME=PROXY_AUTHSECRET", 0x19,
	"NAME=SMS_SMSC_ADDRESS", 0x1A,
	"NAME=USSD_SERVICE_CODE", 0x1B,
	"NAME=GPRS_ACCESSPOINTNAME", 0x1C,
	"NAME=PPP_LOGINTYPE", 0x1D,
	"NAME=PROXY_LOGINTYPE", 0x1E,
	"NAME=CSD_DIALSTRING", 0x21,
	"NAME=CSD_CALLTYPE", 0x28,
	"NAME=CSD_CALLSPEED", 0x29,
	"NAME=PPP_AUTHTYPE", 0x22,
	"NAME=PPP_AUTHNAME", 0x23,
	"NAME=PPP_AUTHSECRET", 0x24,
	"VALUE=GSM/CSD", 0x45,
	"VALUE=GSM/SMS", 0x46,
	"VALUE=GSM/USSD", 0x47,
	"VALUE=IS-136/CSD", 0x48,
	"VALUE=GPRS", 0x49,
	"VALUE=9200", 0x60,
	"VALUE=9201", 0x61,
	"VALUE=9202", 0x62,
	"VALUE=9203", 0x63,
	"VALUE=AUTOMATIC", 0x64,
	"VALUE=MANUAL", 0x65,
	"VALUE=AUTO", 0x6a,
	"VALUE=9600", 0x6b,
	"VALUE=14400", 0x6c,
	"VALUE=19200", 0x6d,
	"VALUE=28800", 0x6e,
	"VALUE=38400", 0x6f,
	"VALUE=PAP", 0x70,
	"VALUE=CHAP", 0x71,
	"VALUE=ANALOGUE", 0x72,
	"VALUE=ISDN", 0x73,
	"VALUE=43200", 0x74,
	"VALUE=57600", 0x75,
	"VALUE=MSISDN_NO", 0x76,
	"VALUE=IPV4", 0x77,
	"VALUE=MS_CHAP", 0x78,
	"TYPE=MMSURL", 0x7c,
	"TYPE=ID", 0x7d,
	"NAME=ISP_NAME", 0x7e,
	"TYPE=BOOKMARK", 0x7f
);

# Hash containing defined attribute names which get linked to inline strings
%Main::AttributeSingles = (
	"NAME", 0x10,
	"VALUE", 0x11);

# Global Variables used for data storage :-(
$Main::ReadBuffer = ''; # Buffer holds a line at a time as read in
$Main::InFileComplete = 0; # Boolean - have we finished reading the input file
$Main::ReadAllTokens = 0; # Boolean - have we read all tokens yet

# Global variables used for pending writes.
# The octet used for a tag has the top two bits set depending on whether or
# it includes content (i.e. other elements) and / or attributes.
# We don't know whether or not it includes these until we hit another tag or an
# end tag.  Therefore, we build up a string of pending attribute data and have a 
# pending tag identifier.  We never need more than one.
$Main::PendingTag = 0;
$Main::PendingAttribs = 0;
$Main::TagHasContent = 0;
$Main::TagHasAttribs = 0;

###############################################################################
# Main entry point

if(!$ARGV[0] || !$ARGV[1])
	{
	&OutputHelp();
	}
else
	{
	my($InFileName, $OutFileName);# Input and output file names
	$InFileName = $ARGV[0];
	$OutFileName = $ARGV[1];

	# Open the XML file
	open( INFILE, $InFileName ) or die("Unable to open $InFileName for reading");
	print "Reading $InFileName\n";

	# Open the output file and create the binary encoded version of the XML
	open( OUTFILE, ">$OutFileName" ) or die("Unable to open $OutFileName for writing");
	print "Creating $OutFileName\n";

	&ParseXMLFile();

	close( INFILE );
	close( OUTFILE );
	print "Processing complete\n";
	}


###############################################################################
# Describe required arguments
sub OutputHelp
	{
	print "This script requires two arguments.  The first is the name of an XML file\n";
	print "to be taken as input.  The second is the name of a WAP encoded binary XML\n";
	print "to be created.\n";
	}

###############################################################################
# Routine to read and parse an input XML file
sub ParseXMLFile
	{
	# Output the version, publicid and charset
	&OutputStandardHeader();

	my $Token = &ReadToken();
	while(!$Main::ReadAllTokens)
		{
		# Behaviour depends on the token type
		if($Token eq '<?xml')
			{# Ignore version and skip
			&DebugPrint(2,"Skipping XML version\n");
			&SkipTill('?>');
			}
		elsif($Token eq '<!DOCTYPE')
			{# Ignore DTD and skip
			&DebugPrint(2,"Skipping DTD\n");
			&SkipTill(']>');
			}
		elsif(substr($Token,0,1) eq '<')
			{# Element - process it
			&ParseElement($Token);
			}
		$Token = &ReadToken();
		}
	}

###############################################################################
# Routine to parse an element
sub ParseElement
	{
	my $ElementName = shift(@_);
	$ElementName = substr($ElementName,1); #trim leading '<'
	if(substr($ElementName,0,1) eq '/')
		{# closing tag
		chop($ElementName);
		&DebugPrint(2,"Parsing end of element $ElementName\n");

		# Flush any pending tag
		if($Main::PendingTag != 0)
			{
			&OutputPendingTag();
			}

		# Output an end token, regardless of what closing tag we have
		&OutputOctet($Main::EndToken);
		}
	else
		{
		my $HasAttribs = 1;
		if(substr($ElementName,-1) eq '>')
			{
			$HasAttribs = 0;
			chop($ElementName);
			}
		my $HasContent = 1;
		if(substr($ElementName,-1) eq '/')
			{
			$HasContent = 0;
			chop($ElementName);
			}
		&DebugPrint(2,"Parsing element $ElementName\n");

		# Flush any pending tag
		if($Main::PendingTag != 0)
			{
			&OutputPendingTag();
			}

		# We have a new tag - make it pending
		$Main::PendingTag = $Main::TagTokens{$ElementName};
		if(!$Main::PendingTag)
			{
			print "Unrecognised tag $ElementName\n";
			}
		splice(@Main::PendingAttribs,0); #empty pending attribs array

		if($HasAttribs)
			{
			my $AttToken = &ReadToken();
			while(substr($AttToken,-1) ne '>') # sloppy I know
				{
				my $EqToken = &ReadToken(); # we could check this
				my $ValToken = &ReadToken();
				&PushAttribute($AttToken, $ValToken);
				&DebugPrint(2,"Attribute $AttToken = $ValToken\n");
				$AttToken = &ReadToken();
				}
			if(substr($AttToken,-2) eq '/>')
				{
				$HasContent = 0;
				}
			}

		# If we had any attributes then flag this
		if(@Main::PendingAttribs > 0)
			{
			$Main::TagHasAttributes = 1;
			}
		$Main::TagHasContent = $HasContent;

		# Flush any pending tag
		if($Main::PendingTag != 0)
			{
			&OutputPendingTag();
			}

		}
	}

###############################################################################
# Routine to skip until it matches a token (eating the matched token)
sub SkipTill
	{
	my $SkipToken = shift(@_);
	my $Token = &ReadToken();
	while(!$Main::ReadAllTokens && ($Token ne $SkipToken))
		{
			$Token = &ReadToken();
		}
	&DebugPrint(3,"Skipped till $SkipToken\n");
	}

###############################################################################
# Routine to read the next token from the input file
# A token is delineated by whitespace (so it won't work too well with strings with
# embedded newlines) or quotes
sub ReadToken
	{
	my($FoundToken);
	$FoundToken = '';

	if(!$Main::ReadAllTokens)
		{
		&RefreshInputBuffer;
		if(@Main::ReadTokens > 0)
			{
			$FoundToken = shift(@Main::ReadTokens);
			}
		while(length($FoundToken) <= 0)
			{
			if((@Main::ReadTokens <= 0) && $Main::InFileComplete)
				{
				$Main::ReadAllTokens = 1;
				last;
				}
			&RefreshInputBuffer;
			if(@Main::ReadTokens > 0)
				{
				$FoundToken = shift(@Main::ReadTokens);
				}
			}
		}

	&DebugPrint(4,"Token :$FoundToken:\n");
	return $FoundToken;
	}

###############################################################################
# Routine to refresh the input buffer
sub RefreshInputBuffer
	{
	while((@Main::ReadTokens <= 0) && !$Main::InFileComplete)
		{
		$Main::ReadBuffer = <INFILE>;
		&DebugPrint(5, $Main::ReadBuffer);

		if(length($Main::ReadBuffer) <= 0)
			{
			$Main::InFileComplete = 1;
			&DebugPrint(4,"Input file exhausted\n");
			}
		else
			{
			# Remove leading and trailing whitespace
			$Main::ReadBuffer =~ s/^\s+//;
			$Main::ReadBuffer =~ s/\s+$//;
			# Split into an array of tokens on whitespace and quotes and equals - lose the quotes on the way
			my ($OneToken, $OneChar);
			while(length($Main::ReadBuffer) > 0)
				{
				$OneChar = substr($Main::ReadBuffer,0,1);
				$Main::ReadBuffer = substr($Main::ReadBuffer,1);
				if(($OneChar eq ' ') || ($OneChar eq "\t"))
					{ # whitespace is separator outside quotes
					&DebugPrint(6,"Whitespace\n");
					if(length($OneToken) > 0)
						{
						push(@Main::ReadTokens, $OneToken);
						$OneToken = '';
						}
					}
				elsif($OneChar eq '"')
					{ # copy to next quote, including whitespace
					&DebugPrint(6,"Start of quotes\n");
					if(length($OneToken) > 0)
						{
						push(@Main::ReadTokens, $OneToken);
						$OneToken = '';
						}
					$OneChar = '';
					until(($OneChar eq '"') || (length($Main::ReadBuffer) <= 0))
						{
						$OneToken = $OneToken.$OneChar;
						$OneChar = substr($Main::ReadBuffer,0,1);
						$Main::ReadBuffer = substr($Main::ReadBuffer,1);
						}
					&DebugPrint(6,"quoted string '$OneToken'\n");				
					push(@Main::ReadTokens, $OneToken);
					$OneToken = '';
					}
				elsif($OneChar eq '=')
					{ # = is separate token
					&DebugPrint(6,"char =\n");
					if(length($OneToken) > 0)
						{
						push(@Main::ReadTokens, $OneToken);
						$OneToken = '';
						}
					push(@Main::ReadTokens,'=');
					}
				else
					{ # routine char - append to building token
					&DebugPrint(6,"Char $OneChar\n");
					$OneToken = $OneToken.$OneChar;
					}
				}#endwhile
			#last token on the line
			if(length($OneToken) > 0)
				{
				push(@Main::ReadTokens, $OneToken);
				$OneToken = '';
				}

			&DebugPrint(5,join( ':', @Main::ReadTokens)."\n");
			}
		}
	}

###############################################################################
# Routine to output a standard set of header fields
# These are all set by globals at the head of the script (for ease of modification)
sub OutputStandardHeader()
	{
	# Output the XML version
	&OutputOctet($Main::XMLVersion);
	
	# Output a standard public Id
	&Output_mb_u_int32($Main::PublicId);

	# Output charset
	&Output_mb_u_int32($Main::Charset);

	# Output a zero-length string table
	&OutputOctet(0);

	}

###############################################################################
# Routine to push an attribute name, value pair in one of a number of ways
sub PushAttribute()
	{
	my $AttName = shift(@_);
	my $AttVal = shift(@_);

	my $PairToken = $Main::AttributePairs{$AttName.'='.$AttVal};
	if($PairToken)
		{
		push(@Main::PendingAttribs,$PairToken);
		}
	else
		{
		my $AttribToken = $Main::AttributeSingles{$AttName};
		if($AttribToken)
			{
			push(@Main::PendingAttribs,$AttribToken);
			&PushInLineString($AttVal);
			print "Attribute $AttName has string (rather than token) value $AttVal\n";
			}
		else
			{
			print "Unrecognised attribute $AttName\n";
			}
		}
	}

###############################################################################
# Routine to flush a pending tag and any attributes
#
sub OutputPendingTag()
	{
	&DebugPrint(3,"Output pending tag $Main::PendingTag\n");
	my $TagOctet = $Main::PendingTag;
	if( $Main::TagHasContent)
		{
		$TagOctet = $TagOctet | 0x40 ; # Set next-to-top bit for has content
		}
	if( $Main::TagHasAttributes)
		{
		$TagOctet = $TagOctet | 0x80 ; # Set top bit for has attributes
		}
	&OutputOctet($TagOctet);

	if($Main::TagHasAttributes)
		{
		&OutputOctetArray(@Main::PendingAttribs);
		&OutputOctet($Main::EndToken); # END after attributes
		}
	else
		{
		&DebugPrint(3,"Tag has no attributes\n");
		}

	# Clean out pending
	$Main::PendingTag = 0;
	splice(@Main::PendingAttribs,0); #empty pending attribs array
	}

###############################################################################
# Routine to output a mb_u_int32 value - up to 32 bits but divided into 7-bit
# chunks and the top bit is set for continuation
sub Output_mb_u_int32()
	{
	my $InNum = shift(@_);

	my @OutBytes;
	my $ByteCount = 0;
	while( $ByteCount < 5 ) # magic number - maximum number of octets output
		{
		my $OctetValue = $InNum & 0x7f;
		if($OctetValue > 0)
			{
			push(@OutBytes, $OctetValue);
			$InNum = $InNum >> 7;
			$ByteCount ++;
			}
		else
			{
			last;
			}
		}

	if($ByteCount > 0)
		{
		while($ByteCount > 0)
			{
			if($ByteCount == 1)
				{ # Output last byte without continuation bit
				&OutputOctet($OutBytes[$ByteCount-1]);
				}
			else
				{ # Output byte with continuation bit
				&OutputOctet($OutBytes[$ByteCount-1]|0x80);
				}
			$ByteCount --;
			}
		}
	else # zero - just output a zero byte
		{
		&OutputOctet(0);
		}
	}

###############################################################################
# Output a single octet
sub OutputOctet
	{
	my $OctetValue = shift(@_);	
	print OUTFILE chr($OctetValue);
	&DebugPrint(3, sprintf(":%lx:", $OctetValue));
	}

###############################################################################
# Add an inline string to the pending attributes buffer
sub PushInLineString
	{
	my $InString = shift(@_);
	push(@Main::PendingAttribs, $Main::InLineString);
	my $Index;
	for( $Index = 0 ; $Index < length($InString) ; $Index ++)
		{
		push(@Main::PendingAttribs, ord(substr($InString,$Index,1)));
		}
	push(@Main::PendingAttribs, 0); # terminating null
	}

###############################################################################
# Output an array of octet values
sub OutputOctetArray
	{
	while(@_ > 0)
		{
		my $OctetValue = shift(@_);
		&OutputOctet($OctetValue);
		}
	}

###############################################################################
# Debug print routine - takes a level of detail and a string and conditionally
# prints the string
sub DebugPrint()
	{
	my $DebugLevel = shift(@_);
	my $DebugString = shift(@_);
	if( $DebugLevel <= $Main::DebugLevel )
		{
		print $DebugString;
		}
	}

###### End of File ######

