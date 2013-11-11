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

#ifndef __MT_CALLINFORMATION_H__
#define __MT_CALLINFORMATION_H__

// INCLUDES
#include <ceunittestsuiteclass.h>
#include <eunitdecorators.h>

#include <etel.h>
#include <CCallInformation.h>
#include <MCallInformationObserver.h>

// FORWARD DECLARATIONS
class CDialer;
#ifdef __WINSCW__
class CPhoneEngineCreator;
class MPEPhoneModel;
class CPEMessageWaiter;
#endif    

// CLASS DEFINITION
/**
 * Generated EUnit test suite class.
 */
NONSHARABLE_CLASS( MT_CallInformation )
	: public CEUnitTestSuiteClass, public MCallInformationObserver
    {
    public:  // Constructors and destructor

        static MT_CallInformation* NewL();
        static MT_CallInformation* NewLC();
        ~MT_CallInformation();

    private: // Constructors

        MT_CallInformation();
        void ConstructL();        
        void CallInformationChanged();        
        

    private: // New methods

         void SetupL();
        
         void Teardown();
        
         void TestNoCallsL();
         void TestActiveCallL();
         void TestSingleCallL();
         void TestEmergencyCallL();
         void TestMultiCallL();
         void TwoCallsExistL();
         
        

    private: // Data
        
        CCallInformation* iCallInfos;        
        CDialer*          iDialer;
        CActiveSchedulerWait* iWait;        
        RTelServer iTelServer;
        
#ifdef __WINSCW__
        CPhoneEngineCreator*    iPECreator;
        MPEPhoneModel*          iPhoneModel;
        CPEMessageWaiter*       iMsgWaiter;
#endif    

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __MT_CALLINFORMATION_H__
