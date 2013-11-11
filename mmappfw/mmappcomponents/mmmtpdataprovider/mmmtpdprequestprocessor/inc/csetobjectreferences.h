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
* Description:  Set object references operation.
*
*/


#ifndef CSETOBJECTREFERENCES_H
#define CSETOBJECTREFERENCES_H

#include <badesca.h>

#include "crequestprocessor.h"

// forward declaration
class CMTPTypeArray;
class CMmMtpDpMetadataAccessWrapper;
class MMmMtpDpConfig;

/**
* Defines SetObjectReferences request processor
*/
class CSetObjectReferences : public CRequestProcessor
    {
public:
    /**
    * Two-phase construction method
    * @param aFramework    The data provider framework
    * @param aConnection    The connection from which the request comes
    * @param aWrapper    medadata access interface
    * @return a pointer to the created request processor object
    */
    IMPORT_C static MMmRequestProcessor* NewL( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * Destructor
    */
    IMPORT_C virtual ~CSetObjectReferences();

protected:
    /**
    * Standard c++ constructor
    * @param aFramework    The data provider framework
    * @param aConnection    The connection from which the request comes
    * @param aWrapper    medadata access interface
    */
    CSetObjectReferences( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

protected:
    /**
    * set references to DB
    * @param aObject, the reference owner
    */
    void DoSetObjectReferencesL( const CMTPObjectMetaData& aObject );

private:
    //from CRequestProcessor
    /**
    * SetReferences request handler
    * start receiving reference data from the initiator
    */
    IMPORT_C void ServiceL();

    /**
    * Apply the references to the specified object
    * @return EFalse
    */
    IMPORT_C TBool DoHandleResponsePhaseL();

    /**
    * Derived from CRequestProcessor, can not be neglected
    * @return ETrue
    */
    IMPORT_C TBool HasDataphase() const;

private:
    /**
    * Verify if the references are valid handles to objects
    * @return ETrue if all the references are good, otherwise, EFalse
    */
    TBool VerifyReferenceHandlesL() const;

    /**
    * Two-phase construction
    */
    void ConstructL();

private:
    // Array to store the references to set to the object
    CMTPTypeArray* iReferences;
    MMmMtpDpConfig& iDpConfig;
    CDesCArray* iReferenceSuids;

    };

#endif // CSETOBJECTREFERENCES_H
