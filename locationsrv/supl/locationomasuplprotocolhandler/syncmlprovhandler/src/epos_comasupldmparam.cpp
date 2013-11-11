/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Implementation of COMASuplDMParam
*
*/




#include "epos_comasupldmparam.h"


// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// COMASuplDMParam::NewL
// COMASuplDMParam instantiation method.
// -----------------------------------------------------------------------------
//
COMASuplDMParam* COMASuplDMParam::NewL( const TDesC8& aURI,
                                        const TDesC8& aLUID, 
                                        const TDesC8* aObject, 
                                        const TDesC8& aType, 
                                        const TInt aStatusRef,
                                        const TInt aResultRef,						  
                                        const TCommand aCommand )
    {
    COMASuplDMParam* self = new( ELeave ) COMASuplDMParam();
    CleanupStack::PushL( self );
    self->ConstructL( aURI, aLUID, aObject, aType, aStatusRef, aResultRef, aCommand );
    CleanupStack::Pop( self );
    return self;	
    }


// -----------------------------------------------------------------------------
// COMASuplDMParam::~COMASuplDMParam()
// Default destructor.
// -----------------------------------------------------------------------------
//
COMASuplDMParam::~COMASuplDMParam()
    {
    delete iURI;				
    delete iLUID;		
    delete iObject;				
    delete iType;	
    }


// -----------------------------------------------------------------------------
// COMASuplDMParam::Get()
// Retrieves SUPL DM parameter values
// -----------------------------------------------------------------------------
//
void COMASuplDMParam::Get( HBufC8*& aURI,
                            HBufC8*& aLUID,
                            HBufC8*& aObject,
                            HBufC8*& aType, 
                            TInt& aStatusRef,
                            TInt& aResultRef,
                            TCommand& aCommand )
    {
    aURI = iURI;
    aLUID = iLUID;
    aObject = iObject;
    aType = iType;
    aStatusRef = iStatusRef;
    aResultRef = iResultRef;
    aCommand = iCommand;
    }


// -----------------------------------------------------------------------------
// COMASuplDMParam::Set()
// Initializes SUPL DM parameters
// -----------------------------------------------------------------------------
//
void COMASuplDMParam::Set( HBufC8* aURI,
                            HBufC8* aLUID, 
                            HBufC8* aObject, 
                            HBufC8* aType, 
                            TInt aStatusRef,
                            TInt aResultRef,
                            TCommand aCommand )
    {
    delete iURI;
    iURI = NULL;
    if ( aURI && aURI->Length() )
        {
        iURI = aURI->Alloc();
        }

    delete iLUID;
    iLUID = NULL;
    if ( aLUID && aLUID->Length() )
        {
        iLUID = aLUID->Alloc();
        }

    delete iObject;
    iObject = NULL;
    if ( aObject && aObject->Length() )
        {
        iObject = aObject->Alloc();
        }
        
    delete iType;
    iType = NULL;
    if ( aType && aType->Length() )
        {
        iType = aType->Alloc();
        }

    iStatusRef = aStatusRef;

    iResultRef = aResultRef;

    iCommand = aCommand;
    }


// -----------------------------------------------------------------------------
// COMASuplDMParam::COMASuplDMParam()
// C++ default constructor.
// -----------------------------------------------------------------------------
//
COMASuplDMParam::COMASuplDMParam()
    {
    iURI = NULL;				
    iLUID = NULL;		
    iObject = NULL;				
    iType = NULL;			
    }


// -----------------------------------------------------------------------------
// COMASuplDMParam::ConstructL()
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
//
void COMASuplDMParam::ConstructL( const TDesC8& aURI,
                                    const TDesC8& aLUID, 
                                    const TDesC8* aObject, 
                                    const TDesC8& aType, 
                                    const TInt aStatusRef,
                                    const TInt aResultRef,
                                    const TCommand aCommand )
    {
    if( aURI.Length() ) 
        {
        iURI = aURI.AllocL();   
        }

    if( aLUID.Length() ) 
        {
        iLUID = aLUID.AllocL();    
        }

    if( aObject && aObject->Length() ) 
        {
        iObject = aObject->AllocL();    
        }

    if( aType.Length() ) 
        {
        iType = aType.AllocL();
        }

    iStatusRef = aStatusRef;

    iResultRef = aResultRef;

    iCommand = aCommand;
    }
