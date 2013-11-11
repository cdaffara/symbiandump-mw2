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


#ifndef CABSTRACTMEDIAMTPDATAPROVIDERGETOBJECTPROPLIST_H
#define CABSTRACTMEDIAMTPDATAPROVIDERGETOBJECTPROPLIST_H

#include "cgetobjectproplist.h"

class MMmMtpDpConfig;

/**
* Defines abstract media data provider GetObjectPropList request processor
*/
class CAbstractMediaMtpDataProviderGetObjectPropList : public CGetObjectPropList
    {
public:
    /**
    * @param aFramework, The data provider framework
    * @param aConnection, The connection from which the request comes
    * @param aWrapper Medadata access
    * @return A pointer to the created request processor object
    */
    static MMmRequestProcessor* NewL( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * Destructor
    */
    ~CAbstractMediaMtpDataProviderGetObjectPropList();

private:
    /**
    * Standard C++ Constructor
    * @param aFramework, The data provider framework
    * @param aConnection, The connection from which the request comes
    * @param aWrapper Medadata access
    */
    CAbstractMediaMtpDataProviderGetObjectPropList( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * 2nd Phase Constructor
    */
    void ConstructL();

protected:
    // from CGetObjectPropList
    TInt ServiceSpecificObjectPropertyL( TUint16 aPropCode, TUint32 aHandle );

    };

#endif // CABSTRACTMEDIAMTPDATAPROVIDERGETOBJECTPROPLIST_H

