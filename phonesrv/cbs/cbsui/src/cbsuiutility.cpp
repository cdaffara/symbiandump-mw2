/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
*   Implementation of CbsUiClenaupHandler     
*
*
*/


// INCLUDES
#include    "CbsUiUtility.h"
#include    "MSaveMessageController.h"
#include    "CbsUiDebug.h"
#include    "MCbs.h"



// =================== MEMBER FUNCTIONS ====================


// ---------------------------------------------------------
// CbsUiUtility::ShowConfirmationNoteL
// 
// 
// ---------------------------------------------------------
//
void CbsUiUtility::ShowConfirmationNoteL(
    const TInt aResourceId, TBool aWaitingDialog )
    {
    /*HBufC* text = CCoeEnv::Static()->AllocReadResourceLC(aResourceId);
    CAknConfirmationNote* note = 
        new (ELeave) CAknConfirmationNote(aWaitingDialog);
    note->ExecuteLD(*text);
    CleanupStack::PopAndDestroy(text);*/
    }

// ---------------------------------------------------------
// CbsUiUtility::ShowErrorNoteL
// 
// 
// ---------------------------------------------------------
//
void CbsUiUtility::ShowErrorNoteL( const TInt aResourceId )
    {
    /*HBufC* text = CCoeEnv::Static()->AllocReadResourceLC( aResourceId );
    CAknErrorNote* note = new (ELeave) CAknErrorNote;
    note->ExecuteLD( *text );
    CleanupStack::PopAndDestroy( text );*/
    }

// ---------------------------------------------------------
// CbsUiUtility::ShowWarningNoteL
// 
// 
// ---------------------------------------------------------
//
void CbsUiUtility::ShowWarningNoteL( const TInt aResourceId )
    {
   /* HBufC* text = CCoeEnv::Static()->AllocReadResourceLC( aResourceId );
    CAknWarningNote* note = new (ELeave) CAknWarningNote;
    note->ExecuteLD( *text );
    CleanupStack::PopAndDestroy( text );*/
    }

// ---------------------------------------------------------
// CbsUiUtility::CreateEmptyNavigationPaneL
// 
// 
// ---------------------------------------------------------
//
/*CAknNavigationDecorator* CbsUiUtility::CreateEmptyNavigationPaneL()
    {
    CEikStatusPane* statusPane = 
        STATIC_CAST( CAknAppUi*, 
            CEikonEnv::Static()->EikAppUi() )->StatusPane();

    CAknNavigationControlContainer* naviContainer = 
        STATIC_CAST( CAknNavigationControlContainer*, statusPane->
        ControlL(TUid::Uid(EEikStatusPaneUidNavi)) );

    CAknNavigationDecorator* decorator;
    decorator = naviContainer->CreateNavigationLabelL();

    naviContainer->PushL(*decorator);

    return decorator;
    }*/

// ---------------------------------------------------------
// CbsUiUtility::IsSavedL
// 
// 
// ---------------------------------------------------------
//
TBool CbsUiUtility::IsSavedL( const TCbsMessageHandle aHandle, MCbs& aServer )
    {
    /*__ASSERT_DEBUG(&aServer, CbsUiPanic(EErrNullPointer) );
    __ASSERT_DEBUG(aServer.Connected(), CbsUiPanic(EErrServerDisconnected) );

    TCbsMessage message;
    
    TInt ret = aServer.FindMessageByHandle(aHandle, message);

    __CBSUI_ASSERT( ret == KErrNone, CbsUiPanic(EErrMsgHandleNotFound) );

    User::LeaveIfError(ret);
    TBool saved(EFalse);

    if ( message.iPermanent )
        {
        saved = ETrue;
        }
           */
    return EFalse; //saved;
    }

// ---------------------------------------------------------
// CbsUiUtility::SaveCurrentMessageL
// 
// 
// ---------------------------------------------------------
//
TInt CbsUiUtility::SaveCurrentMessageL(
    const TCbsMessageHandle aHandle, MSaveMessageController& aSaver )
    {    
   /* __ASSERT_DEBUG(&aSaver, CbsUiPanic(EErrNullPointer) );

    TInt resultCode( aSaver.SaveMessage(aHandle) );
    
    switch ( resultCode )
        {
        case KErrDiskFull:
            {
            User::Leave( KErrDiskFull );
            break;
            }
        case KErrNone: // Normal case
            {
            // do nothing
            break;
            }
        default:
            {          
            //CbsUiUtility::ShowErrorNoteL(R_TEXT_NOTE_MEMORY_FULL_ERROR);
            break;
            } 
        }  */

    return 0; //resultCode;
    }

//  End of File  
