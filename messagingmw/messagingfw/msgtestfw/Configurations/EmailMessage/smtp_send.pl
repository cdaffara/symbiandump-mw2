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


use strict;
use IO::Socket;


if(scalar @ARGV !=3) {
  die "syntax <smtp host> <username> <rfc file>";
}

my $host=@ARGV[0];
my $username=@ARGV[1];

open RFC,"@ARGV[2]" or die "Couldn't open @ARGV[2]: $@";
my @rfcfile = <RFC>;
close RFC;

my $socket = IO::Socket::INET->new(PeerAddr => $host, PeerPort => 25,  Proto => "tcp", Type => SOCK_STREAM) or die "Couldn't connect to $host : $@\n";


my $welcome = <$socket>;
print "<< $welcome";

sendWait($socket,"HELO [127.0.0.1]");
sendWait($socket,"RSET");
sendWait($socket,"MAIL FROM:<$username>");
sendWait($socket,"RCPT TO:<$username>");
sendWait($socket,"DATA");


foreach my $line (@rfcfile) {
  chomp $line;
  sendNoWait($socket,$line);
}

sendWait($socket,".");

print $socket "QUIT\r\n";
print ">> QUIT\n";
close($socket);

exit;

sub sendWait($$)
  {
    my $socket;
    my $line;
    ($socket,$line) = @_;

    print $socket "$line\r\n";
    print ">> \"$line\"\n";
    my $result;
    chomp ($result = <$socket>);
    print "<< $result\n";
  }
    

sub sendNoWait($$)
  {
    my $socket;
    my $line;
    ($socket,$line) = @_;

    print $socket "$line\r\n";
    print ">> \"$line\"\n";
  }
    
