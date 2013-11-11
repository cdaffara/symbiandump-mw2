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


#ifndef CMEDIAMTPDATAPROVIDERGETOBJECTPROPVALUE_H
#define CMEDIAMTPDATAPROVIDERGETOBJECTPROPVALUE_H

#include "cgetobjectpropvalue.h"

class MMmMtpDpConfig;

/**
* Defines media data provider GetObjectPropValue request processor
*/
class CMediaMtpDataProviderGetObjectPropValue: public CGetObjectPropValue
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
    ~CMediaMtpDataProviderGetObjectPropValue();

protected:
    // from CGetObjectPropValue
    void ServiceSpecificObjectPropertyL( TUint16 aPropCode );

private:
    /**
    * Standard c++ constructor
    * @param aFramework   The data provider framework
    * @param aConnection  The connection from which the request comes
    * @param aWrapper Medadata access
    */
    CMediaMtpDataProviderGetObjectPropValue( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * two-phase construction
    */
    void ConstructL();

    };

#endif // CMEDIAMTPDATAPROVIDERGETOBJECTPROPVALUE_H
