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


mkdir \performance-logs\
mkdir \performance-logs\winscw\

\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance1_1kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance1_1kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance1_5kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance1_5kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance1_10kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance1_10kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance1_100kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance1_100kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance1_200kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance1_200kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance1_500kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance1_500kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance10_1kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance10_1kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance10_5kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance10_5kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance10_10kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance10_10kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance10_100kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance10_100kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance10_200kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance10_200kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance10_500kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance10_500kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance50_1kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance50_1kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance50_5kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance50_5kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance50_10kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance50_10kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance50_100kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance50_100kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance50_200kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance50_200kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance50_500kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance50_500kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance100_1kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance100_1kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance100_5kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance100_5kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance100_10kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance100_10kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance100_100kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance100_100kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance100_200kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance100_200kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance100_500kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance100_500kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance200_1kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance200_1kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance200_5kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance200_5kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance200_10kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance200_10kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance200_100kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance200_100kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance200_200kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance200_200kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance200_500kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance200_500kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance500_1kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance500_1kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance500_5kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance500_5kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance500_10kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance500_10kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance500_100kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance500_100kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance500_200kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance500_200kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance500_500kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance500_500kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance1000_1kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance1000_1kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance1000_5kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance1000_5kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance1000_10kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance1000_10kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance1000_100kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance1000_100kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance1000_200kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance1000_200kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance1000_500kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance1000_500kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance1_1000kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance1_1000kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance10_1000kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance10_1000kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance50_1000kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance50_1000kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance100_1000kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance100_1000kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance200_1000kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance200_1000kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance500_1000kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance500_1000kb.htm \performance-logs\winscw /R /Y
\epoc32\release\winscw\urel\testexecute c:\msgtest\email\imap\performance\msg-imap-performance1000_1000kb.script
call xcopy \epoc32\winscw\c\logs\testexecute\msg-imap-performance1000_1000kb.htm \performance-logs\winscw /R /Y
perl write-to-excell.pl winscw
