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


#ifndef CABSTRACTMEDIAMTPDATAPROVIDERSETOBJECTPROPVALUE_H
#define CABSTRACTMEDIAMTPDATAPROVIDERSETOBJECTPROPVALUE_H

#include "csetobjectpropvalue.h"

/**
* Defines abstract media data provider SetObjectPropValue request processor
*/
class CAbstractMediaMtpDataProviderSetObjectPropValue: public CSetObjectPropValue
    {
public:
    /**
    * Two-phase construction method
    * @param aFramework    The data provider framework
    * @param aConnection    The connection from which the request comes
    * @param aWrapper Medadata access
    * @return a pointer to the created request processor object
    */
    static MMmRequestProcessor* NewL( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * Destructor
    */
    ~CAbstractMediaMtpDataProviderSetObjectPropValue();

private:
    /**
    * Standard c++ constructor
    * @param aFramework    The data provider framework
    * @param aConnection    The connection from which the request comes
    * @param aWrapper Medadata access
    */
    CAbstractMediaMtpDataProviderSetObjectPropValue( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * two-phase construction
    */
    void ConstructL();

protected:
    // from CSetObjectPropValue
    TBool IsSpecificPropCodeReadOnly( TUint16 aPropCode ) const;

    void ReceiveDataSpecificObjectPropertyL( TUint16 aPropCode );

    TMTPResponseCode ServiceSpecificObjectPropertyL( TUint16 aPropCode );

    };

#endif // CABSTRACTMEDIAMTPDATAPROVIDERSETOBJECTPROPVALUE_H
