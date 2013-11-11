/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Test class for STIF TestScripter module
*
*/


#ifndef CMMMTPDATAPROVIDERTESTMODULE_H
#define CMMMTPDATAPROVIDERTESTMODULE_H

//  INCLUDES

#include <stiflogger.h>
#include <testscripterinternal.h>
#include <stiftestmodule.h>
#include <testclassassert.h>
#include <mtp/tmtptyperequest.h>

#include "ctestobjectmanager.h"
#include "ctestconnectionchannel.h"
#include "ctestobserver.h"

// CONSTANTS

_LIT( KTestModuleName, "MMMTP" );

// MACROS

#define TEST_CLASS_VERSION_MAJOR 1
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// FORWARD DECLARATIONS

class CMTPDataProviderPlugin;
class CMmMtpDataProviderTester;

// CLASS DECLARATION

/**
*  CMtpDataProviderTestModule test class for STIF Test Framework TestScripter.
*
*/
NONSHARABLE_CLASS(CMmMtpDataProviderTestModule) : public CScriptBase
    {
public:  // Constructors and destructor
    static CMmMtpDataProviderTestModule* NewL( CTestModuleIf& aTestModuleIf );
    virtual ~CMmMtpDataProviderTestModule();
public: // Functions from base classes
    /**
    * From CScriptBase Runs a script line.
    * @since ?Series60_version
    * @param aItem Script line containing method name and parameters
    * @return Symbian OS error code
    */
    virtual TInt RunMethodL( CStifItemParser& aItem );
private:
    CMmMtpDataProviderTestModule( CTestModuleIf& aTestModuleIf );
    
    void ConstructL();
    void Delete();
    
    /**
    * Test methods are listed below. 
    */
    virtual TInt LoadPluginL( CStifItemParser& aItem );
    virtual TInt PrepareEnvironmentL( CStifItemParser& aItem );
    virtual TInt CleanEnvironmentL( CStifItemParser& aItem );
    virtual TInt OpenSessionL( CStifItemParser& aItem );
    virtual TInt CloseSessionL( CStifItemParser& aItem );
    virtual TInt ObjectEnumerationL( CStifItemParser& aItem );
    virtual TInt StorageEnumerationL( CStifItemParser& aItem );
    virtual TInt StartTransactionL( CStifItemParser& aItem );
    virtual TInt CancelTransactionL( CStifItemParser& aItem );
    virtual TInt CompleteTransactionL( CStifItemParser& aItem );
    virtual TInt GetObjectInfoL( CStifItemParser& aItem );
    virtual TInt GetObjectL( CStifItemParser& aItem );
    virtual TInt GetObjectPropValueL( CStifItemParser& aItem );
    virtual TInt GetObjectPropListL( CStifItemParser& aItem );
    virtual TInt GetObjectPropDescL( CStifItemParser& aItem );
    virtual TInt SendObjectInfoL( CStifItemParser& aItem );
    virtual TInt SendObjectL( CStifItemParser& aItem );
    virtual TInt SetObjectPropValueL( CStifItemParser& aItem );
    virtual TInt SetObjectPropListL( CStifItemParser& aItem );
    virtual TInt DeleteObjectL( CStifItemParser& aItem );
    virtual TInt CopyObjectL( CStifItemParser& aItem );
    virtual TInt MoveObjectL( CStifItemParser& aItem );
    virtual TInt RenameObjectL ( CStifItemParser& aItem );
    virtual TInt GetPartialObjectL( CStifItemParser& aItem );
    virtual TInt SetObjectReferencesL( CStifItemParser& aItem );
    virtual TInt GetObjectReferencesL( CStifItemParser& aItem );
    virtual TInt GetObjectPropsSupportedL( CStifItemParser& aItem );
    void SendTestClassVersion();
public: // data
    CMmMtpDataProviderTester* iPluginTester;
    CMTPDataProviderPlugin* iDataProviderPlugin;
    CTestConnectionChannel* iTestConnChannel;
private:
    TMTPTypeRequest* iRequest;
    TUint iSessionId;
    TBool iSessionOpen;
    TUint iTransactionId;
    TBool iTransactionOpen;
    
    CTestObserver* iTestObserver;
    CActiveScheduler* iActiveScheduler;
    };

#endif // CMMMTPDATAPROVIDERTESTMODULE_H
