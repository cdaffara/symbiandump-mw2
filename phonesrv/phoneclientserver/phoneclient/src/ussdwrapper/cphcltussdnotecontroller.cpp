/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Ussd note controller
*
*/


// INCLUDE FILES
#include <hbdevicemessageboxsymbian.h>
#include <hbdeviceprogressdialogsymbian.h>
#include <hbtextresolversymbian.h>

#include "phcltclientserver.h" 
#include "cphcltussdnotecontroller.h" 
#include "tflogger.h"

_LIT(KUssdLocFilename, "ussd_");
_LIT(KCommonLocFilename, "common_");
_LIT(KPath, "z:\\resource\\qt\\translations");
_LIT(KUssdRequesting, "txt_common_info_requesting"); // Requesting
_LIT(KUssdDone, "txt_ussd_dpopinfo_done"); // Done
_LIT(KUssdNotDone, "txt_ussd_dpopinfo_not_done"); // NotDone
_LIT(KUssdNotAllowed, "txt_ussd_dpopinfo_not_allowed"); //NotAllowed
_LIT(KUssdUnconfirmed, "txt_ussd_dpopinfo_request_not_confirmed"); // Unconfirmed
_LIT(KUssdNoService, "txt_ussd_dpopinfo_no_service"); // NoService
_LIT(KUssdOffline, "txt_ussd_dpopinfo_unable_to_use_network_phone_is"); // Offline
_LIT(KUssdHide, "txt_common_button_hide"); // Hide

const int KPhCltUssdProgressBarMaxLength = 10;
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhCltUssdNoteController* CPhCltUssdNoteController::NewL(
        MPhCltUssdNoteControllerCallBack& aCallBack )
    {
    TFLOGSTRING("CPhCltUssdNoteController: NewL call")
    CPhCltUssdNoteController* self = new( ELeave ) 
        CPhCltUssdNoteController( aCallBack );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    TFLOGSTRING("CPhCltUssdNoteController: NewL exit")
    return self;
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhCltUssdNoteController::ConstructL()
    {
    TFLOGSTRING( "CPhCltUssdNoteController: ConstructL call_exit" ) 
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::CPhCltUssdNoteController
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
CPhCltUssdNoteController::CPhCltUssdNoteController(
    MPhCltUssdNoteControllerCallBack& aCallBack )
    : iGlobalWaitNote( NULL ),
      iCallBack( aCallBack )
    {
    TFLOGSTRING("CPhCltUssdNoteController: CPhCltUssdNoteController call")
    TFLOGSTRING("CPhCltUssdNoteController: CPhCltUssdNoteController exit")
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController:~CPhCltUssdNoteController
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhCltUssdNoteController::~CPhCltUssdNoteController()
    {
    TFLOGSTRING("CPhCltUssdNoteController: ~CPhCltUssdNoteController call")
    DestroyGlobalWaitNote();
    TFLOGSTRING("CPhCltUssdNoteController: ~CPhCltUssdNoteController exit")
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::ShowGlobalInformationNoteL
// -----------------------------------------------------------------------------
void CPhCltUssdNoteController::ShowGlobalInformationNoteL( 
        const TPhCltUssdInformationType aInfoType )
    {
    TFLOGSTRING2("CPhCltUssdNoteController: ShowGlobalInformationNoteL\
            aInfoType = %d call", aInfoType)
    const TBool textResolver = HbTextResolverSymbian::Init( 
        KUssdLocFilename, KPath );
    TFLOGSTRING2("CPhCltUssdNoteController: ShowGlobalInformationNoteL\
        ussd textResolver = %d", textResolver ) 
    HBufC* temp(NULL);
    switch ( aInfoType )
        {
        case EPhCltUssdUnconfirme:
            {
            temp = HbTextResolverSymbian::LoadLC( KUssdUnconfirmed );
            break;
            }
        case EPhCltUssdNotallowed:
            {
            temp = HbTextResolverSymbian::LoadLC( KUssdNotAllowed );
            break;
            }
        case EPhCltUssdNoservice:
            {
            temp = HbTextResolverSymbian::LoadLC( KUssdNoService );
            break;
            }
        case EPhCltUssdOffline:
            {
            temp = HbTextResolverSymbian::LoadLC( KUssdOffline );
            break;
            }
        case EPhCltUssdDone:
            {
            temp = HbTextResolverSymbian::LoadLC( KUssdDone );
            break;
            }
        case EPhCltUssdNotDone:
            {
            temp = HbTextResolverSymbian::LoadLC( KUssdNotDone );
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        }
    CHbDeviceMessageBoxSymbian* dlg = CHbDeviceMessageBoxSymbian::NewL(
            CHbDeviceMessageBoxSymbian::EInformation);
    CleanupStack::PushL( dlg );
    dlg->SetTextL( temp->Des() );
    dlg->SetButton( CHbDeviceMessageBoxSymbian::EAcceptButton, ETrue );
    dlg->ExecL();
    CleanupStack::PopAndDestroy( dlg );
    CleanupStack::PopAndDestroy( temp );
    TFLOGSTRING("CPhCltUssdNoteController: ShowGlobalInformationNoteL exit")
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::ShowGlobalWaitNoteL
// -----------------------------------------------------------------------------
void CPhCltUssdNoteController::ShowGlobalWaitNoteL( )
    {
    TFLOGSTRING("CPhCltUssdNoteController: ShowGlobalWaitNoteL call")
    DestroyGlobalWaitNote();
    TBool textResolver = HbTextResolverSymbian::Init( 
        KCommonLocFilename, KPath );
    TFLOGSTRING2("CPhCltUssdNoteController: ConstructL\
        init common textResolver = %d", textResolver ) 
    //CHbDeviceProgressDialogSymbian
    iGlobalWaitNote = CHbDeviceProgressDialogSymbian::NewL(
            CHbDeviceProgressDialogSymbian::EProgressDialog );
    HBufC* context = HbTextResolverSymbian::LoadLC( KUssdRequesting );
    iGlobalWaitNote->SetTextL( context->Des() );
    HBufC* bottonText = HbTextResolverSymbian::LoadLC( KUssdHide );
    iGlobalWaitNote->SetButtonTextL( bottonText->Des() );
    iGlobalWaitNote->SetObserver( this );
    TFLOGSTRING("CPhCltUssdNoteController: ShowGlobalWaitNoteL before setactive")
    iGlobalWaitNote->SetRange(0,KPhCltUssdProgressBarMaxLength);
    iGlobalWaitNote->SetProgressValue( KPhCltUssdProgressBarMaxLength );
    iGlobalWaitNote->SetAutoClose( EFalse );
    iGlobalWaitNote->ShowL();
    TFLOGSTRING("CPhCltUssdNoteController: ShowGlobalWaitNoteL after ShowL")
    CleanupStack::PopAndDestroy( bottonText );
    CleanupStack::PopAndDestroy( context );
    TFLOGSTRING("CPhCltUssdNoteController: ShowGlobalWaitNoteL after setactive")
    TFLOGSTRING("CPhCltUssdNoteController: ShowGlobalWaitNoteL exit")
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::DestroyGlobalWaitNote
// -----------------------------------------------------------------------------
void CPhCltUssdNoteController::DestroyGlobalWaitNote()
    {
    TFLOGSTRING("CPhCltUssdNoteController: DestroyGlobalWaitNote call")
    if ( iGlobalWaitNote )
        {
        iGlobalWaitNote->Close();
        delete iGlobalWaitNote;
        iGlobalWaitNote = NULL;
        }
    TFLOGSTRING("CPhCltUssdNoteController: DestroyGlobalWaitNote exit")
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::ProgressDialogCancelled
//
// 
// -----------------------------------------------------------------------------
//
void CPhCltUssdNoteController::ProgressDialogCancelled(
        const CHbDeviceProgressDialogSymbian* /*aProgressDialog*/)
    {
    TFLOGSTRING("CPhCltUssdNoteController: ProgressDialogCancelled call")
    iCallBack.GlobalWaitNoteHidden();
    TFLOGSTRING("CPhCltUssdNoteController: ProgressDialogCancelled exit")
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::ProgressDialogClosed
//
// 
// -----------------------------------------------------------------------------
//
void CPhCltUssdNoteController::ProgressDialogClosed(
        const CHbDeviceProgressDialogSymbian* /*aProgressDialog*/)
    {
    TFLOGSTRING("CPhCltUssdNoteController: ProgressDialogClosed call")
    TFLOGSTRING("CPhCltUssdNoteController: ProgressDialogClosed exit")
    }

// End of file

