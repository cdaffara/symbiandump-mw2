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
REM Run this batch file to run all the Capabilities test scripts
REM using the messaging test framework.
REM
REM Add any new test scripts for the Capabilities test suite to this list

echo Running capabilities test scripts...

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvGetEntry.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvGetChildren.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvGetRemainingChildren.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvChangeEntry.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvCreateEntry.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvDeleteEntries.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvMoveEntries.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvCopyEntries.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvNotifySession.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvCancelSessionEventNotification.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvReadStore.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvLockStore.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvReleaseStore.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvOperationData.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvCommandData.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvCancelOperation.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvOperationProgress.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvOperationCompletion.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvOperationMtm.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvMtmCommand.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvFillRegisteredMtmDllArray.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvDeInstall_InstallMtmGroup.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvUseMtmGroup.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvReleaseMtmGroup.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvGetMtmGroupData.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvCloseServer.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvStopService.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvServiceActive.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvServiceProgress.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvRemoveEntry.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvSlotAvailable.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvSetSessionAsObserver.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvSetFailure.script 

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvChangeAttributes.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvGetChildIds.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvChangeDrive.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvOutstandingOperations.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvGetNotifySequence.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvSetReceiveEntryEvents.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvDecStoreReaderCount.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvGetMessageDrive.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvGetMtmRequiredCapabilities.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvCreateAttachmentForWrite.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvOpenAttachment.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvOpenAttachmentForWrite.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvDeleteAttachment.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvOpenFileStoreForRead.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvOpenTempStoreFile.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvReplaceFileStore.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvDeleteFileStore.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvFileStoreExists.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvGetAndClearIndexCorruptFlag.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvSlotAvailable.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvSetSessionAsObserver.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvCopyStore.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvDeleteStore.script

testexecute.exe  c:\msgtest\TestFramework\ScriptedTestCases\Capabilities\Cap_EMsvDriveContainsStore.script

