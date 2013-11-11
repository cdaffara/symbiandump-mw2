@rem
@rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description:
@rem

\EPOC32\RELEASE\WINSCW\UDEB\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\EmailOverSms\EOS_1_1_SendSingleSMS.script
REM Establishes that the TSY can send a straight SMS. 
REM Ensures that the current SMS code has not been broken, with the addition of the EMAIL over SMS code.

\EPOC32\RELEASE\WINSCW\UDEB\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\EmailOverSms\EOS_1_2_ReceiveSingleSMS.script
REM Establishes that the TSY can receive a straight SMS.
REM Ensures that the current SMS code has not been broken, with the addition of the EMAIL over SMS code.


\EPOC32\RELEASE\WINSCW\UDEB\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\EmailOverSms\EOS_2_1_SendMOTestAddressForm1.script
REM Send an email of the form "user@domain1.domain2"

\EPOC32\RELEASE\WINSCW\UDEB\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\EmailOverSms\EOS_2_2_SendMOTestAddressForm2.script
REM Send an email of the form "User name<user@domain1.domain2>"


\EPOC32\RELEASE\WINSCW\UDEB\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\EmailOverSms\EOS_3_1_ReceiveMTTestAddressForm1.script
REM Receive an email of the form "user@domain1.domain2"
REM Checks that the code breaks down correctly the header into  the email addess and messages.

\EPOC32\RELEASE\WINSCW\UDEB\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\EmailOverSms\EOS_3_2_ReceiveMTTestAddressForm2.script
REM Receive an email of the form "User name<user@domain1.domain2>"
REM Checks that the code breaks down correctly the header into  the email addess and messages.


\EPOC32\RELEASE\WINSCW\UDEB\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\EmailOverSms\EOS_4_1_SubjectForm1.script
REM "User Name<user@domain1.domain2> (Subject)message"

\EPOC32\RELEASE\WINSCW\UDEB\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\EmailOverSms\EOS_4_2_SubjectForm2.script
REM Check  that the subject is broken out correcty from 
REM "user@domain1.domain2 ##Subject#message"
