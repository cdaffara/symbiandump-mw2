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
rem NOTE! Do not use tabs here!
rem 
rem

md c:\System\Libs
copy z:\System\MsgTest\System\Libs\clientmtm.dll c:\System\Libs\
copy z:\System\MsgTest\System\Libs\findmtm.dll c:\System\Libs\
copy z:\System\MsgTest\System\Libs\brokenfindmtm.dll c:\System\Libs\
copy z:\System\MsgTest\System\Libs\mtm1.dll c:\System\Libs\
copy z:\System\MsgTest\System\Libs\mtm2.dll c:\System\Libs\
copy z:\System\MsgTest\System\Libs\regmtm1.dll c:\System\Libs\
copy z:\System\MsgTest\System\Libs\regmtm2.dll c:\System\Libs\
copy z:\System\MsgTest\System\Libs\servermtm.dll c:\System\Libs\
copy z:\System\MsgTest\System\Libs\testservermtm.dll c:\System\Libs\
attrib c:\system\libs\*.* -r

md c:\System\Programs
copy z:\System\MsgTest\System\Programs\mailinit.exe c:\System\Programs\
copy z:\System\MsgTest\System\Programs\obsexe1.exe c:\System\Programs\
copy z:\System\MsgTest\System\Programs\obsexe2.exe c:\System\Programs\
attrib c:\system\programs\*.* -r 

md c:\MsgTest
md c:\MsgTest\msgs
copy z:\System\MsgTest\MsgTest\msgs\mtm.rsc c:\MsgTest\msgs\
copy z:\System\MsgTest\MsgTest\msgs\t_msgs_registry.rsc c:\MsgTest\msgs\
attrib c:\msgtest\msgs\*.* -r
