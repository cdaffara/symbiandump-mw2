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
REM Run this batch file to run all the Attachment API unit test scripts
REM using the messaging test framework.
REM
REM Add any new test scripts for the attachment api test suite to this list

\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddAttachmentByHandle01.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddAttachmentByHandle02.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddAttachmentByHandleLargeAsync.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddAttachmentByHandleLargeSync.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddAttachmentByPath01.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddAttachmentByPath02.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddAttachmentByPathLargeAsync.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddAttachmentByPathLargeSync.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddAttachmentInvalidPathAsync.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddAttachmentInvalidPathSync.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddAttachmentWithDestroy.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddAttachmentWithRevert.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddEntryAttachment01.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddLinkedAttachment01.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddMixedAttachments01.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddMulitpleAttachmentByPath01.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddMultipleAttachmentByHandle01.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\CreateAttachment01.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\CreateAttachment02.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\RemoveEntryAttachmentById.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\RemoveFileAttachmentById.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\RemoveFileAttachmentByIndex.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\RemoveFileAttachmentWithDestroy.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\RemoveFileAttachmentWithRevert.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\RemoveLinkedAttachmentById.script
\epoc32\release\winscw\udeb\testexecute.exe c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\AddTestMimeHeaders01.script
