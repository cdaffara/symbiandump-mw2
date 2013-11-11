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


#ifndef CMEDIAMTPDATAPROVIDERMOVEOBJECT_H
#define CMEDIAMTPDATAPROVIDERMOVEOBJECT_H

#include "cmoveobject.h"

class MMmMtpDpConfig;

/**
* Defines media data provider MoveObject request processor
*/
class CMediaMtpDataProviderMoveObject: public CMoveObject
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
    ~CMediaMtpDataProviderMoveObject();

private:
    /**
    * Standard c++ constructor
    * @param aFramework   The data provider framework
    * @param aConnection  The connection from which the request comes
    * @param aWrapper Medadata access
    */
    CMediaMtpDataProviderMoveObject( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * two-phase construction
    */
    void ConstructL();

protected:
    void ServiceGetSpecificObjectPropertyL( TUint16 aPropCode,
        TUint32 aHandle,
        const CMTPObjectMetaData& aObject );

    };

#endif // CMEDIAMTPDATAPROVIDERMOVEOBJECT_H
