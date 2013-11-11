/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface for Active object handling Pub&Sub notify events.
*
*/


// INCLUDE FILES
#include    "csssettingsactiveobject.h" 
#include    <e32property.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSSSettingsActiveObject::CSSSettingsActiveObject
// -----------------------------------------------------------------------------
//
CSSSettingsActiveObject::CSSSettingsActiveObject(
    const TUid aNotifyUid,
    const TUint32 aNotifyKey,
    MSSSettingsPubSubNotify& aNotifyHandler )
:   CActive( CActive::EPriorityStandard ),
    iNotifyUid ( aNotifyUid ),
    iNotifyKey( aNotifyKey ),
    iNotifyHandler( &aNotifyHandler ),
    iNotify( EFalse )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CSSSettingsActiveObject::ConstructL
// -----------------------------------------------------------------------------
//
void CSSSettingsActiveObject::ConstructL()
    {
    TInt err = iProperty.Attach(
        iNotifyUid,
        iNotifyKey );
    User::LeaveIfError( err );
    }

// -----------------------------------------------------------------------------
// CSSSettingsActiveObject::NewL
// -----------------------------------------------------------------------------
//
CSSSettingsActiveObject* CSSSettingsActiveObject::NewL(
    const TUid aNotifyUid,
    const TUint32 aNotifyKey,
    MSSSettingsPubSubNotify& aNotifyHandler )
    {
    CSSSettingsActiveObject* self = 
        new ( ELeave ) CSSSettingsActiveObject(
            aNotifyUid,
            aNotifyKey,
            aNotifyHandler );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CSSSettingsActiveObject::~CSSSettingsActiveObject
// -----------------------------------------------------------------------------
//
CSSSettingsActiveObject::~CSSSettingsActiveObject()
    {
    Cancel();
    iProperty.Close();
    }

// -----------------------------------------------------------------------------
// CSSSettingsActiveObject::NotifyKeyChange
// -----------------------------------------------------------------------------
//
TInt CSSSettingsActiveObject::NotifyKeyChange()
    {
    TInt err(KErrNone);
    if ( iNotify || IsActive() )
        {
        // Notify already active, so return error code.
        err = KErrInUse;
        }
    else
        {
        iProperty.Subscribe( iStatus );
        SetActive();
        iNotify = ETrue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CSSSettingsActiveObject::CancelNotifyCancelNotify
// -----------------------------------------------------------------------------
//
void CSSSettingsActiveObject::CancelNotify()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// CSSSettingsActiveObject::RunL
// -----------------------------------------------------------------------------
//
void CSSSettingsActiveObject::RunL()
    {
    TInt err = iStatus.Int();
    if ( err != KErrCancel && !iNotify )
        {
        // Reset the notification.
        iProperty.Subscribe( iStatus );
        SetActive();
        }
    
    if ( !err )
        {
        if ( iNotifyHandler )
            {
            iNotifyHandler->HandlePubSubNotify(
                iNotifyUid,
                iNotifyKey );
            }
        }
    }

// ---------------------------------------------------------
// CSSSettingsActiveObject::DoCancel
// ---------------------------------------------------------
//
void CSSSettingsActiveObject::DoCancel()
    {
    iNotify = EFalse;
    iProperty.Cancel();
    }

//  End of File  
