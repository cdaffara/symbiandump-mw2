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
* Description:  GetInterdependentPropDesc operation
*
*/

#ifndef CGETINTERDEPENDENTPROPDESC_H
#define CGETINTERDEPENDENTPROPDESC_H

#include "crequestprocessor.h"

class MMmMtpDpConfig;
class CMTPTypeInterdependentPropDesc;

class CGetInterdependentPropDesc : public CRequestProcessor
    {
public:
    /**
     *
     */
    IMPORT_C static MMmRequestProcessor* NewL( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * Destructor
    */
    IMPORT_C virtual ~CGetInterdependentPropDesc();

protected:
    /**
    * Standard C++ Constructor
    * @param aFramework    The data provider framework
    * @param aConnection   The connection from which the request comes
    * @param aWrapper      Medadata access interface
    */
    CGetInterdependentPropDesc( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * 2nd Phase Constructor
    */
    void ConstructL();

protected:
    // from CRequestProcessor
    IMPORT_C TMTPResponseCode CheckRequestL();

    IMPORT_C void ServiceL();

protected:
    MMmMtpDpConfig& iDpConfig;

    CMTPTypeInterdependentPropDesc* iDataset;

    TUint32 iFormatCode;

    };

#endif // CGETINTERDEPENDENTPROPDESC_H
