/*
* Copyright (c) 20 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: Unit test class for CVmbxPbkStore
*
*/

#ifndef UT_VMBXENGINE_H
#define UT_VMBXENGINE_H

#include <QObject>

class CVoiceMailbox;
class CVoiceMailboxEntry;

class Ut_VmbxEngine : public QObject
{
    Q_OBJECT

public:
    Ut_VmbxEngine();
    virtual ~Ut_VmbxEngine();

private slots:

    // Test New and Delete Api
    void testVmbxApi();
    void testEntryApi();
    
    // CVoiceMailbox test code
    void testGetStoredEntry();
    void testSaveEntry();
    void testQueryNewEntry();
    void testQueryChangeEntry();
    void testQueryVmbxType();
    //void testNotifyVmbxNumberChangeL();
    //void NotifyVmbxNumberChangeCancel();
    void testCheckConfiguration();
    void testGetServiceIds();
    void testSaveProvisionedEntry();
    
    // CVoiceMailboxEntry test code
    void testType();
    void testName();
    void testServiceId();
    void testLineType();
    void testNumber();
    void testBrandId();
    void testUsingMemoryLocation();
    void testReset();
   
private:
    // create and delete Api
    void createVmbxApi();
    void createEntryApi();
    void deleteVmbxApi();
    void deleteEntryApi();
private:
    CVoiceMailbox *mVmbx;
    CVoiceMailboxEntry *mEntry;
};

#endif // UT_VMBXENGINE_H
