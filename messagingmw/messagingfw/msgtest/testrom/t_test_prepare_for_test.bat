rem Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
rem All rights reserved.
rem This component and the accompanying materials are made available
rem under the terms of "Eclipse Public License v1.0"
rem which accompanies this distribution, and is available
rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
rem
rem Initial Contributors:
rem Nokia Corporation - initial contribution.
rem
rem Contributors:
rem
rem Description:
rem

REM NOTE! Do not use tabs here!
md c:\MsgTest
md c:\MsgTest\emailsmoke
md c:\MsgTest\sms


copy z:\System\MsgTest\msgtest\smoke\email\script\script.txt c:\msgtest\emailsmoke\script.txt		
copy z:\System\MsgTest\msgtest\smoke\email\script\rfc822test.txt c:\msgtest\emailsmoke\rfc822test.txt	
attrib c:\msgtest\emailsmoke\*.* -r
                                                            
copy z:\System\MsgTest\msgtest\smoke\sms\script\smoke_enum.script c:\msgtest\sms\smoke_enum.script		
copy z:\System\MsgTest\msgtest\smoke\sms\script\smoke_recv.script c:\msgtest\sms\smoke_recv.script		
copy z:\System\MsgTest\msgtest\smoke\sms\docs\t_sms.html c:\msgtest\sms\t_sms.html				
attrib c:\msgtest\sms\*.* -r




