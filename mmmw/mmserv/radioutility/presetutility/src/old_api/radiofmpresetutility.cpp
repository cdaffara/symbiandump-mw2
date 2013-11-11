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
* Description:  Contains implementation of the CRadioFmPresetUtility class,
*                 which controlls the presets for an FM tuner.
*/


#include <RadioPresetUtility.h>
#include <RadioFmPresetUtility.h>
#include "presetutilitybody.h"
#include <preset.h>
#include "trace.h"

// #############################################################################
// This file has been deprecated! It is kept only for BC compatibility purposes
// Please use the new API instead of this one
// #############################################################################

// Dummy body
class CRadioFmPresetUtility::CBody : public CBase,
                                     public MRadioPreset,
                                     public MRadioFmPreset
    {
public:
    ~CBody();
    CBody();
    };
CRadioFmPresetUtility::CBody::CBody() {}
CRadioFmPresetUtility::CBody::~CBody() {}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CRadioFmPresetUtility* CRadioFmPresetUtility::NewL( MRadioPresetObserver& aObserver )
    {
    FUNC_LOG;
    CRadioFmPresetUtility* self = new (ELeave) CRadioFmPresetUtility( aObserver );
    CleanupStack::PushL(self);
    self->iBody = CPresetUtilityBody::NewL();
    self->iBody->SetObserver( self );
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CRadioFmPresetUtility::~CRadioFmPresetUtility()
    {
    FUNC_LOG;
    delete iBody;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CRadioFmPresetUtility::CRadioFmPresetUtility( MRadioPresetObserver& aObserver ):
    iObserver( aObserver )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CRadioFmPresetUtility::SetPresetL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmPresetUtility::SetPresetL( TInt aIndex, const TFmPresetName& aName, TInt aFrequency )
    {
    FUNC_LOG;
    TPreset radioPreset;
    TInt err( KErrNone );
    TRAP(err, iBody->ReadPresetL( aIndex, radioPreset ) );
    if (KErrNotFound == err)
        {
        // Preset doesn't need to exist yet.
        // Handling of err only to get rid of compile warning.
        err = KErrNone;
        }
    User::LeaveIfError(err);
    radioPreset.SetName( aName );
    radioPreset.SetFrequency( aFrequency );
    if ( 0 >= aFrequency )
        {
        // aFrequency values 0, -1(Or <=0) means that it is better to destroy
        // rest of parameters and also URL
        TFmPresetUrl url;
        radioPreset.SetUrl( url );
        radioPreset.SetGenre( 0 );
        radioPreset.SetPiCode( 0 );
        radioPreset.SetLocalStation( 0 );
        radioPreset.SetFavorite( EFalse );
        radioPreset.SetRenamedByUser( EFalse );             
        }
    User::LeaveIfError( iBody->SavePresetL( radioPreset, aIndex ) );
    }

// -----------------------------------------------------------------------------
// CRadioFmPresetUtility::SetPresetUrlL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmPresetUtility::SetPresetUrlL( TInt aIndex, const TFmPresetUrl& aUrl )
    {
    FUNC_LOG;
    TPreset radioPreset;
    iBody->ReadPresetL( aIndex, radioPreset );
    radioPreset.SetUrl( aUrl );
    User::LeaveIfError( iBody->SavePresetL( radioPreset, aIndex ) );
    }

// -----------------------------------------------------------------------------
// CRadioFmPresetUtility::SetPresetPtyL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmPresetUtility::SetPresetPtyL( TInt aIndex, TUint aPty )
    {
    FUNC_LOG;
    TPreset radioPreset;
    iBody->ReadPresetL( aIndex, radioPreset );
    radioPreset.SetGenre( aPty );
    User::LeaveIfError( iBody->SavePresetL( radioPreset, aIndex ) );
    }

// -----------------------------------------------------------------------------
// CRadioFmPresetUtility::SetPresetPICodeL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmPresetUtility::SetPresetPICodeL( TInt aIndex, TUint aPICode )
    {
    FUNC_LOG;
    TPreset radioPreset;
    iBody->ReadPresetL( aIndex, radioPreset );
    radioPreset.SetPiCode( aPICode );
    User::LeaveIfError( iBody->SavePresetL( radioPreset, aIndex ) );
    }
// -----------------------------------------------------------------------------
// CRadioFmPresetUtility::SetPresetFavouriteInfoL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmPresetUtility::SetPresetFavouriteInfoL( TInt aIndex, TBool aFavourite )
    {
    FUNC_LOG;
    TPreset radioPreset;
    iBody->ReadPresetL( aIndex, radioPreset );
    radioPreset.SetFavorite( aFavourite );
    User::LeaveIfError( iBody->SavePresetL( radioPreset, aIndex ) );
    }

// -----------------------------------------------------------------------------
// CRadioFmPresetUtility::SetPresetUserRenameInfoL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmPresetUtility::SetPresetUserRenameInfoL( TInt aIndex, TBool aRename )
    {
    FUNC_LOG;
    TPreset radioPreset;
    iBody->ReadPresetL( aIndex, radioPreset );
    radioPreset.SetRenamedByUser( aRename );
    User::LeaveIfError( iBody->SavePresetL( radioPreset, aIndex ) );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmPresetUtility::GetPresetL( TInt aIndex, TFmPresetName& aName, TInt& aFrequency )
    {
    FUNC_LOG;
    TPreset preset;
    iBody->ReadPresetL( aIndex, preset );
    aName.Copy( preset.Name() );
    aFrequency = preset.Frequency();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmPresetUtility::GetPresetNameL( TInt aIndex, TFmPresetName& aName)
    {
    FUNC_LOG;
    TInt unused;
    GetPresetL( aIndex, aName, unused );
    }

// -----------------------------------------------------------------------------
// CRadioFmPresetUtility::GetPresetUrlL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
EXPORT_C void CRadioFmPresetUtility::GetPresetUrlL( TInt aIndex, TFmPresetUrl& aUrl)
    {
    FUNC_LOG;
    TPreset radioPreset;
    iBody->ReadPresetL( aIndex, radioPreset);
    aUrl.Copy( radioPreset.Url() );
    }

// -----------------------------------------------------------------------------
// CRadioFmPresetUtility::GetPresetPtyL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
EXPORT_C void CRadioFmPresetUtility::GetPresetPtyL( TInt aIndex, TUint& aPty)
    {
    FUNC_LOG;
    TPreset radioPreset;
    iBody->ReadPresetL( aIndex, radioPreset);
    aPty = radioPreset.Genre();
    }

// -----------------------------------------------------------------------------
// CRadioFmPresetUtility::GetPresetPICodeL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
EXPORT_C void CRadioFmPresetUtility::GetPresetPICodeL( TInt aIndex, TUint& aPICode)
    {
    FUNC_LOG;
    TPreset radioPreset;
    iBody->ReadPresetL( aIndex, radioPreset);
    aPICode = radioPreset.PiCode();
    }


// -----------------------------------------------------------------------------
// CRadioFmPresetUtility::GetPresetFavouriteInfoL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
EXPORT_C void CRadioFmPresetUtility::GetPresetFavouriteInfoL( TInt aIndex, TBool& aFavourite)
    {
    FUNC_LOG;
    TPreset radioPreset;
    iBody->ReadPresetL( aIndex, radioPreset);
    aFavourite = radioPreset.Favorite();
    }

// -----------------------------------------------------------------------------
// CRadioFmPresetUtility::GetPresetUserRenameInfoL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
EXPORT_C void CRadioFmPresetUtility::GetPresetUserRenameInfoL( TInt aIndex, TBool& aRename)
    {
    FUNC_LOG;
    TPreset preset;
    iBody->ReadPresetL( aIndex, preset);
    aRename = preset.RenamedByUser();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmPresetUtility::GetPresetFrequencyL( TInt aIndex, TInt& aFrequency)
    {
    FUNC_LOG;
    TPreset preset;
    iBody->ReadPresetL( aIndex, preset );
    aFrequency = preset.Frequency();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioFmPresetUtility::GetMaxNumberOfPresets( TInt& aMaxNumOfPresets ) const
    {
    FUNC_LOG;
    aMaxNumOfPresets = iBody->MaxNumberOfPresets();
    return KErrNone;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioFmPresetUtility::GetNumberOfPresets( TInt& aNumOfPresets ) const
    {
    FUNC_LOG;
    aNumOfPresets = iBody->PresetCount();
    return KErrNone;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmPresetUtility::GetFirstPresetL( TInt& aIndex) const
    {
    FUNC_LOG;
    aIndex = iBody->FirstPreset();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmPresetUtility::GetNextPresetL( TInt aIndex, TInt& aNextIndex ) const
    {
    FUNC_LOG;
    aNextIndex = iBody->NextPreset( aIndex );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmPresetUtility::DeletePresetL( TInt aIndex )
    {
    FUNC_LOG;
    User::LeaveIfError( iBody->DeletePreset( aIndex ) );
    }

// -----------------------------------------------------------------------------
// CRadioFmPresetUtility::PresetChanged()
// Returns: None
// -----------------------------------------------------------------------------
void CRadioFmPresetUtility::PresetChanged( TPresetChangeEvent aChange, TInt aIndex )
    {
    FUNC_LOG;
    iObserver.MrpeoPresetChanged( (MRadioPresetObserver::TPresetChangeEvent)aChange, aIndex );
    }
