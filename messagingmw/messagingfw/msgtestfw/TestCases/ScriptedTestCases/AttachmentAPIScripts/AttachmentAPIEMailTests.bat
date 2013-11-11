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

\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\01-SMTP_AddAttachmentFromPublicArea_CreateAttachment.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\02-SMTP_AddAttachmentFromPrivateArea.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\03-SMTP_AddAttachmentFromPublicArea.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\04-SMTP_AddLinkedAttachment.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\05-SMTP_AddEntryAttachment.SCRIPT

\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\06-SMTP_AddLinkAttachment_ModifyAttachmentBeforeSend.SCRIPT
REM ***Data files restoration is required after the script execution which requires deletion of the data files***
copy /Y \Epoc32\winscw\c\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\data\BackupData2.mbm \Epoc32\winscw\c\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\data\testimage1.mbm
copy /Y \Epoc32\winscw\c\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\data\BackupData.mbm \Epoc32\winscw\c\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\data\TestPicture.mbm
copy /Y \Epoc32\winscw\c\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\data\BackupData.txt \Epoc32\winscw\c\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\data\TestFile.txt

\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\07-SMTP_AddLinkAttachment_DeleteAttachmentBeforeSend.SCRIPT
REM ***Data files restoration is required after the script execution which requires deletion of the data files***
copy /Y \Epoc32\winscw\c\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\data\BackupData2.mbm \Epoc32\winscw\c\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\data\testimage1.mbm
copy /Y \Epoc32\winscw\c\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\data\BackupData.mbm \Epoc32\winscw\c\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\data\TestPicture.mbm
copy /Y \Epoc32\winscw\c\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\data\BackupData.txt \Epoc32\winscw\c\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\data\TestFile.txt

\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\09-SMTP_RemoveAttachmentUsingAttachmentId.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\10-SMTP_RemoveAttachmentUsingIndex.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\11-SMTP_RemoveAllAttachmentsUsingIndex.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\12-SMTP_RemoveAllAttachmentsUsingAttachmentId.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\13-SMTP_ListAllAttachments.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\14-SMTP_GetAttachmentInfo.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\17-SMTP_GetAttachmentFile.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\20-SMTP_AddAttachmentInvalidFilePath.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\21-SMTP_AddLinkedAttachmentInvalidFilePath.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\22-SMTP_RemoveAttachmentUsingInvalidIndex.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\23-SMTP-IMAP4_AddAttachmentFromPrivateArea_Compare.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\23-SMTP-POP3_AddAttachmentFromPrivateArea_Compare.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\24-SMTP-IMAP4_AddAttachmentFromPublicArea_Compare.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\24-SMTP-POP3_AddAttachmentFromPublicArea_Compare.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\25-SMTP-IMAP4_AddLinkedAttachment_Compare.SCRIPT
\epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\msgtest\TestFramework\ScriptedTestCases\AttachmentAPIScripts\25-SMTP-POP3_AddLinkedAttachment_Compare.SCRIPT
