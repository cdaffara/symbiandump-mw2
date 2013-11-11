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


#ifndef CREQUESTUNKNOWN_H
#define CREQUESTUNKNOWN_H

#include "crequestprocessor.h"

/**
* Defines device data provider OpenSession request processor
*/
class CRequestUnknown : public CRequestProcessor
    {
public:
    /**
    * Two-phase construction method
    * @param aFramework   The data provider framework
    * @param aConnection  The connection from which the request comes
    * @return a pointer to the created request processor object
    */
    IMPORT_C static MMmRequestProcessor* NewL( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection );

    /**
    * Destructor
    */
    IMPORT_C ~CRequestUnknown();

protected:
    /**
    * Standard c++ constructor
    */
    CRequestUnknown( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection );

protected:
    // from CRequestProcessor
    /**
    * OpenSession request handler
    */
    IMPORT_C void ServiceL();

    /**
    * Override to match both the unknown requests
    * @param aRequest The request to match
    * @param aConnection The connection from which the request comes
    * @return always return ETrue to process all unknown request
    */
    IMPORT_C TBool Match( const TMTPTypeRequest& aRequest,
        MMTPConnection& aConnection ) const;

private:
    /**
    * Two-phase construction
    */
    void ConstructL();

    };

#endif // CREQUESTUNKNOWN_H
