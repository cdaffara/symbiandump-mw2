/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Abstract base class for queries.
*
*/


// INCLUDE FILES

#include "lpdquerylauncherbase.h"
#include "lpdresulthandler.h"
#include "lpdrequestorprocessor.h"

#include "lpdglobalplugindialog.h"


// CONSTANTS

// Unnamed namespace for local definitions

#ifdef _DEBUG
//_LIT( KPanicText, "CLpdQueryLauncherBase" );
enum TPanicCode
    {
    KLpdErrGeneral = 1
    };
#endif

// INCLUDE FILES
#include <s32mem.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::CLpdQueryLauncherBase
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdQueryLauncherBase::CLpdQueryLauncherBase( MLpdResultHandler& aHandler )
    : iHandler( aHandler ),
      iResult( KErrArgument ),
      iQueryDlg( NULL )
    {
    }



// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::~CLpdQueryLauncherBase
// Destructor
// -----------------------------------------------------------------------------
//
CLpdQueryLauncherBase::~CLpdQueryLauncherBase()
    {
    delete iQueryDlg;
    iQueryDlg = NULL;
    }


// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::ConstructL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::ConstructL()
    {
    iQueryDlg = CLpdGlobalPluginDialog::NewL(*this);
    }
// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::SetQueryTextArray
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::SetRequestInfoL( CLpdRequestorProcessor* aRequestInfo )
    {
    iQueryDlg->SetRequestInfoL(aRequestInfo);
    }

void CLpdQueryLauncherBase::RequestType(TRequestType& aRequestType)
    {
    aRequestType = EUnknown;
    }
// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::StartQueryL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::StartQueryL()
    {
    TRequestType type;
    RequestType( type );
    if( EVerification == type )
        {
        iQueryDlg->RunVerificationQueryL();
        }
    else if( ENotification == type )
        {
        iQueryDlg->RunNotificationL();
        }
    else
        {
        // do nothing now
        }

    }

void CLpdQueryLauncherBase::Cancel()
    {
    iQueryDlg->Cancel();
    }
// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::HandleDlgDismissedL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::HandleDlgDismissedL(    
    TInt aResult )
    {
    
    iResult = aResult;
      
    TRAPD( err, ExtendedHandleDlgDismissedL() );
         if ( err )
             { 
             iHandler.HandleLeave( err );
             }
      
    }

//  End of File
