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


#ifndef CABSTRACTMEDIAMTPDATAPROVIDERCOPYOBJECT_H
#define CABSTRACTMEDIAMTPDATAPROVIDERCOPYOBJECT_H

#include "ccopyobject.h"

/**
* Defines abstract media data provider CopyObject request processor
*/
class CAbstractMediaMtpDataProviderCopyObject: public CCopyObject
    {
public:
    /**
    * Two-phase construction method
    * @param aFramework   The data provider framework
    * @param aConnection  The connection from which the request comes
    * @param aWrapper Medadata access
    * @return a pointer to the created request processor object
    */
    static MMmRequestProcessor* NewL( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * Destructor
    */
    ~CAbstractMediaMtpDataProviderCopyObject();

private:
    /**
    * Standard c++ constructor
    * @param aFramework   The data provider framework
    * @param aConnection  The connection from which the request comes
    * @param aWrapper Medadata access
    */
    CAbstractMediaMtpDataProviderCopyObject( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * two-phase construction
    */
    void ConstructL();

protected:
    // from CCopyObject
    void ServiceGetSpecificObjectPropertyL( TUint16 aPropCode,
        TUint32 aHandle,
        const CMTPObjectMetaData& aObject );

    };

#endif // CABSTRACTMEDIAMTPDATAPROVIDERCOPYOBJECT_H
