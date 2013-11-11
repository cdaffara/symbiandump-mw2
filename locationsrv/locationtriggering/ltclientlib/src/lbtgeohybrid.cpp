/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of Geographical hybrid area class.
*
*/

#include <lbtgeohybrid.h>
#include <lbtgeocircle.h>
#include <lbtgeorect.h>
#include <lbtgeocell.h>
#include <lbterrors.h>


// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtGeoHybrid::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoHybrid* CLbtGeoHybrid::NewL()
    {
    CLbtGeoHybrid* self = CLbtGeoHybrid::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtGeoHybrid::NewLC()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoHybrid* CLbtGeoHybrid::NewLC()
    {
    CLbtGeoHybrid* self = new( ELeave ) CLbtGeoHybrid;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoHybrid::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoHybrid* CLbtGeoHybrid::NewL( RPointerArray<CLbtGeoAreaBase>& 
                                             aHybridArea )
    {
    CLbtGeoHybrid* self = CLbtGeoHybrid::NewLC( aHybridArea );
    CleanupStack::Pop( self );
    return self;
    }  
    
// ---------------------------------------------------------------------------
// CLbtGeoHybrid::NewLC()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoHybrid* CLbtGeoHybrid::NewLC( RPointerArray<CLbtGeoAreaBase>&
                                              aHybridArea )
    {
    CLbtGeoHybrid* self = new( ELeave ) CLbtGeoHybrid();
    CleanupStack::PushL( self );
    self->ConstructL( aHybridArea );
    return self;
    }      

// ---------------------------------------------------------------------------
// CLbtGeoHybrid::CLbtGeoHybrid()
//
// Default constructor
// ---------------------------------------------------------------------------
//
CLbtGeoHybrid::CLbtGeoHybrid()
    {
                                 
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoHybrid::ConstructL()
//
// ---------------------------------------------------------------------------
//
void CLbtGeoHybrid::ConstructL( RPointerArray<CLbtGeoAreaBase>&
                                aHybridArea )
    {
    SetHybridArea( aHybridArea );
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoHybrid::ConstructL()
//
// ---------------------------------------------------------------------------
//
void CLbtGeoHybrid::ConstructL()
    {
    
    }      
     


// ---------------------------------------------------------------------------
// CLbtGeoHybrid::~CLbtGeoHybrid()
//
// Destructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoHybrid::~CLbtGeoHybrid()
    {
    iHybridArea.ResetAndDestroy();
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoHybrid::Type()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CLbtGeoAreaBase::TGeoAreaType CLbtGeoHybrid::Type() const   
    {
    return CLbtGeoAreaBase::EHybrid;
    } 
    
// ---------------------------------------------------------------------------
// CLbtGeoHybrid::SetHybridArea()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtGeoHybrid::SetHybridArea( const RPointerArray<CLbtGeoAreaBase>& 
                                            aHybridArea )
    {
    iHybridArea.ResetAndDestroy();
    if( aHybridArea.Count() < 2 )
        {
        User::Panic( KLbtClientPanicCategory,ELbtErrArgument );
        }
    for( TInt i=0;i<aHybridArea.Count();i++ )
        {
        if( aHybridArea[i]->Type() != CLbtGeoAreaBase::ECellular )
            {
            User::Panic( KLbtClientPanicCategory,ELbtErrArgument );
            }
        iHybridArea.Append( aHybridArea[i] );
        }
    }

// ---------------------------------------------------------------------------
// CLbtGeoHybrid::HybridArea()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CLbtGeoAreaBase>& CLbtGeoHybrid::HybridArea()
    {
    return iHybridArea;
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoHybrid::DoExternalizeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CLbtGeoHybrid::DoExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteInt8L( iHybridArea.Count() );
    for( TInt i=0;i<iHybridArea.Count();i++ )
        {
        CLbtGeoAreaBase::TGeoAreaType areaType= iHybridArea[i]->Type();
        switch( areaType )
            {
            case CLbtGeoAreaBase::ECircle:
                aStream.WriteInt8L(1);
                break;
            case CLbtGeoAreaBase::ERectangular:
                aStream.WriteInt8L(2);
                break;
            case CLbtGeoAreaBase::ECellular:    
                aStream.WriteInt8L(3);
                break;
            }
        iHybridArea[i]->ExternalizeL( aStream );
        }
    }    
    
// ---------------------------------------------------------------------------
// CLbtGeoHybrid::DoInternalizeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CLbtGeoHybrid::DoInternalizeL( RReadStream& aStream ) 
    {
    TInt count = aStream.ReadInt8L();
    for( TInt i=0;i<count;i++ )
        {
        TInt areaType = aStream.ReadInt8L();
        if( areaType == 1 )
            {
            CLbtGeoCircle* area= CLbtGeoCircle::NewL();
            area->InternalizeL(aStream);
            iHybridArea.Append( area );
            }
        else if( areaType == 2 )
            {
            CLbtGeoRect* area= CLbtGeoRect::NewL();
            area->InternalizeL(aStream);
            iHybridArea.Append( area );
            }
        else if( areaType == 3 )
            {
            CLbtGeoCell* area= CLbtGeoCell::NewL();
            area->InternalizeL(aStream);
            iHybridArea.Append( area );
            }
        }
    }     

// ---------------------------------------------------------------------------
// CLbtGeoHybrid::ValidateHybridInformationL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CLbtGeoHybrid::ValidateHybridInformationL()
    {
    if( iHybridArea.Count() < 2 )
        {
        User::Leave( KErrArgument );
        }
    for( TInt i=0;i<iHybridArea.Count();i++ )
        {
        switch( iHybridArea[i]->Type() )
            {
            case CLbtGeoAreaBase::ECircle:
                {
                CLbtGeoCircle* circle = static_cast<CLbtGeoCircle*> ( iHybridArea[i] );
                circle->ValidateCircleInformationL();
                break;
                }
            case CLbtGeoAreaBase::ECellular:    
                {
                CLbtGeoCell* cell = static_cast<CLbtGeoCell*> ( iHybridArea[i] );
                cell->ValidateCellInformationL();
                break;
                }
            }
        }
    }
// end of file



