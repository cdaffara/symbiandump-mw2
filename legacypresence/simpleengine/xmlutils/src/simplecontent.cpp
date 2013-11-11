/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    SIMPLE Engine simple content
*
*/





// *** system include files go here:
#include <e32std.h>
#include <s32strm.h>
#include "simplecontent.h"
#include "simplecommon.h"


// *** local constants go here:


// ----------------------------------------------------------
// CSimpleContent::CSimpleContent
// ----------------------------------------------------------
//
CSimpleContent::CSimpleContent( )
    {
    }
    
// ---------------------------------------------------------------------------
// CSimpleContent::~CSimpleContent
// ---------------------------------------------------------------------------
//
CSimpleContent::~CSimpleContent()
    {
    delete iContentID;
    delete iContentType;  
    delete iBody;
    }    

// ---------------------------------------------------------------------------
// CSimpleContent::NewL
// ---------------------------------------------------------------------------
//
CSimpleContent* CSimpleContent::NewL(
    const TDesC8& aContentID, const TDesC8& aContentType )
    {
    CSimpleContent* self = CSimpleContent::NewLC(
        aContentID, aContentType );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSimpleContent::NewLC
// ---------------------------------------------------------------------------
//
CSimpleContent* CSimpleContent::NewLC(
    const TDesC8& aContentID, const TDesC8& aContentType )
    {
    CSimpleContent* self = new( ELeave ) CSimpleContent;
    CleanupStack::PushL( self );
    self->ConstructL( aContentID, aContentType );    
    return self;
    }

// ---------------------------------------------------------------------------
// CSimpleContent::Body
// ---------------------------------------------------------------------------
//
TPtrC8 CSimpleContent::Body()
    {
    return iBody ? iBody->Des() : TPtrC8();
    }
    
// ---------------------------------------------------------------------------
// CSimpleContent::GiveBodyOwnerShip
// ---------------------------------------------------------------------------
//
HBufC8* CSimpleContent::GiveBodyOwnerShip()
    {
    HBufC8* body = iBody;
    iBody = NULL;
    return body;
    }
    
// ---------------------------------------------------------------------------
// CSimpleContent::Close
// ---------------------------------------------------------------------------
//
void CSimpleContent::Close()
    {
    delete this;
    }    
    
// ---------------------------------------------------------------------------
// CSimpleContent::ConstructL
// ---------------------------------------------------------------------------
//
void CSimpleContent::ConstructL(
    const TDesC8& aContentID, const TDesC8& aContentType )
    {
    iContentID = aContentID.AllocL();
    iContentType = aContentType.AllocL();  
    }    
      

        
// ---------------------------------------------------------------------------
// CSimpleContent::CopyBodyL
// ---------------------------------------------------------------------------
//
void CSimpleContent::CopyBodyL( const TDesC8& aData )   
    {
    delete iBody;
    iBody = NULL;
    iBody = aData.AllocL();
    }
       
// ---------------------------------------------------------------------------
// CSimpleContent::SetBody
// ---------------------------------------------------------------------------
//
void CSimpleContent::SetBody( HBufC8* aData )
    {
    delete iBody;   
    iBody = aData;
    }
    
// ---------------------------------------------------------------------------
// CSimpleContent::ContentID
// ---------------------------------------------------------------------------
//
TPtrC8 CSimpleContent::ContentID()
    {
    return iContentID ? iContentID->Des() : TPtrC8();
    }
    
// ---------------------------------------------------------------------------
// CSimpleContent::ContentType
// ---------------------------------------------------------------------------
//
TPtrC8 CSimpleContent::ContentType()
    {
    return iContentType ? iContentType->Des() : TPtrC8();
    }
    


