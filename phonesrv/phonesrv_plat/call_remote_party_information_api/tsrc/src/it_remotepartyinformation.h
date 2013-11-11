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
* Description:
*
*/

#ifndef __IT_REMOTEPARTYINFORMATION_H__
#define __IT_REMOTEPARTYINFORMATION_H__

// INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>
#include <MCallInformationObserver.h>
#include <mcallremotepartyinfoobserver.h>
#include <etel.h>

// FORWARD DECLARATIONS
class CCallRemotePartyInformation;
class CDialer;
class CCallInformation;


// CLASS DEFINITION
/**
 * Generated EUnit test suite class.
 */
NONSHARABLE_CLASS( IT_RemotePartyInformation ) : public CEUnitTestSuiteClass,
                                                 public MCallInformationObserver,
                                                 public MCallRemotePartyInformationObserver
    {
    public:  // Constructors and destructor

        static IT_RemotePartyInformation* NewL();
        static IT_RemotePartyInformation* NewLC();
        ~IT_RemotePartyInformation();

    private: // Constructors

        IT_RemotePartyInformation();
        void ConstructL();

    private: // New methods
        
        void CallInformationChanged();  //from MCallInformationObserver        
        void CallRemotePartyInformationChanged( ); //from MCallRemotePartyInformationObserver 

        void SetupL();        
        void Teardown();
            
        void TestNoCallsL();         
        void TestSingleCallNoContactL();        

    private: // Data
        
        RTelServer iTelServer;
        CCallRemotePartyInformation* iRemotePartyInfo;
        CDialer* iDialer;
        CActiveSchedulerWait* iWait;
        CCallInformation* iCallInfos;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __IT_REMOTEPARTYINFORMATION_H__
