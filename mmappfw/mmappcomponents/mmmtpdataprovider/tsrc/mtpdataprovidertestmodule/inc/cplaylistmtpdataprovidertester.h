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
* Description:  Tester class for abstractmediamtpdp plugin
*
*/


#ifndef CABSTRACTMEDIAMTPDATAPROVIDERTESTER_H
#define CABSTRACTMEDIAMTPDATAPROVIDERTESTER_H

#include "cmmmtpdataprovidertester.h"

/**
*  CAbstractMediaMtpDataProviderTester class.
*  
*/
class CAbstractMediaMtpDataProviderTester : public CMmMtpDataProviderTester
    {
public:
    static CAbstractMediaMtpDataProviderTester* NewL();
    virtual ~CAbstractMediaMtpDataProviderTester();
private:
    TInt CheckSupportedL( TMTPSupportCategory aCategory,
            RArray<TUint> aResponseArray );
    
    TInt PrepareEnvironmentL();
    TInt CleanEnvironmentL();
    
    TBool ObjectEnumerationL_prepare( CMTPStorageMetaData* aPhysicalStorage,
            CMTPStorageMetaData* aLogicalStorage );
    TInt ObjectEnumerationL_checkresults( MMTPObjectMgr& aObjectManager );
    TInt DeleteObjectL_checkresults( MMTPObjectMgr& aObjectManager );
private:
    CAbstractMediaMtpDataProviderTester();
    void ConstructL();
private:
    CActiveSchedulerWait* iActiveSchedulerWait;
    RFs iFsSession;
    CFileMan* iFileMan;
    };

#endif // CABSTRACTMEDIAMTPDATAPROVIDERTESTER_H
