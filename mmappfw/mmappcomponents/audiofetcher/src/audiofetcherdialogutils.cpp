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

// INCLUDE FILES

#include "audiofetcherdialogutils.h"
#include "audiofetcherlog.h"

#include <bautils.h>  // bafl.lib
#include <StringLoader.h>
#include <driveinfo.h>
#include <pathinfo.h>

#include <aknnavide.h>          // CAknNavigationDecorator
#include <aknnavilabel.h>       // CAknNaviLabel
#include <akntitle.h>           // CAknTitlePane
#include <AknQueryDialog.h>
#include <aknnotewrappers.h>
#include <AknsUtils.h>          // AknsUtils

#include <data_caging_path_literals.hrh> // KDC_APP_RESOURCE_DIR


/******************************************************************************
 * class CActiveCaller
 ******************************************************************************/


// -----------------------------------------------------------------------------
// CActiveCaller::NewL
// 
// -----------------------------------------------------------------------------
//
CActiveCaller* CActiveCaller::NewL( MActiveCallerObserver* aObserver )
    {
    CActiveCaller* self = new (ELeave) CActiveCaller( aObserver );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );

 	return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//
CActiveCaller::~CActiveCaller()
    {
    WLOG("CActiveCaller::~CActiveCaller >");
 	Cancel();
	iTimer.Close();
	WLOG("CActiveCaller::~CActiveCaller <");
    }


// -----------------------------------------------------------------------------
// CActiveCaller::CActiveCaller
// 
// -----------------------------------------------------------------------------
//
CActiveCaller::CActiveCaller(MActiveCallerObserver* aObserver) : CActive(CActive::EPriorityStandard)
    {
	iObserver = aObserver;
    }


// -----------------------------------------------------------------------------
// CActiveCaller::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CActiveCaller::ConstructL()
    {
    WLOG("CActiveCaller::ConstructL >");
	User::LeaveIfError( iTimer.CreateLocal() );
	CActiveScheduler::Add( this );
	WLOG("CActiveCaller::ConstructL <");
    }


// -----------------------------------------------------------------------------
// CActiveCaller::DoCancel
//
// -----------------------------------------------------------------------------
//
void CActiveCaller::DoCancel()
    {
    WLOG("CActiveCaller::DoCancel >");
 	iTimer.Cancel();
 	WLOG("CActiveCaller::DoCancel <");
    }


// -----------------------------------------------------------------------------
// CActiveCaller::RunL
//
// -----------------------------------------------------------------------------
//
void CActiveCaller::RunL()
    {
    WLOG("CActiveCaller::RunL >");
    TRAP_IGNORE( iObserver->HandleActiveCallL( iCallId ) );
    WLOG("CActiveCaller::RunL <");
    }


// -----------------------------------------------------------------------------
// CActiveCaller::Request
//
// This function calls this class RunL.
// -----------------------------------------------------------------------------
//
void CActiveCaller::Request()
    {
	Cancel();
	SetActive();
	TRequestStatus* status = &iStatus;
	User::RequestComplete( status, KErrNone );
    }


// -----------------------------------------------------------------------------
// CActiveCaller::Start
//
// -----------------------------------------------------------------------------
//
void CActiveCaller::Start( TInt aCallId, TInt aMilliseconds )
    {
    WLOG("CActiveCaller::Start >");
    const TInt KThousand = 1000;
    Cancel();
	
	iCallId = aCallId;

	if ( aMilliseconds <= 0 )
		{
		Request();  // no delay - complete asap
		}
	else
		{
	    iTimer.After( iStatus, aMilliseconds*KThousand );
	    SetActive();
		}
	WLOG("CActiveCaller::Start <");
    }


// -----------------------------------------------------------------------------
// CActiveCaller::Stop
//
// -----------------------------------------------------------------------------
//
void CActiveCaller::Stop()
    {
    WLOG("CActiveCaller::Stop >");
    Cancel();
    WLOG("CActiveCaller::Stop <");
    }




/*******************************************************************************
 * class CNaviPaneHandler
 *******************************************************************************/


// -----------------------------------------------------------------------------
// Destructor
//
// -----------------------------------------------------------------------------
//
CNaviPaneHandler::~CNaviPaneHandler()
    {
    WLOG("CNaviPaneHandler::~CNaviPaneHandler >");
    if ( iNaviDecorator )
        {
        if ( iNaviPane && iNavidecoratorPushed )
            {
            iNaviPane->Pop( iNaviDecorator );
            }
    
        delete iNaviDecorator;
        }
        
    if ( iNaviPane && iNavipanePushed )
        {
        iNaviPane->Pop(); // restore previous navi pane
        }
    WLOG("CNaviPaneHandler::~CNaviPaneHandler <");
    }


// -----------------------------------------------------------------------------
// CNaviPaneHandler::CNaviPaneHandler
//
// -----------------------------------------------------------------------------
//
CNaviPaneHandler::CNaviPaneHandler( CEikStatusPane* aStatusPane ) : iStatusPane( aStatusPane )
    {
    iNavidecoratorPushed = EFalse;
    iNavipanePushed = EFalse;
    
    if ( iStatusPane )
        {
        TRAP_IGNORE( iNaviPane = (CAknNavigationControlContainer*)iStatusPane->ControlL(TUid::Uid(EEikStatusPaneUidNavi)) );
        }
    }


// ----------------------------------------------------------------------------
// CNaviPaneHandler::SetNaviPaneTitleL
// 
// ----------------------------------------------------------------------------
//
void CNaviPaneHandler::SetNaviPaneTitleL(const TDesC& aTitle)
    {
    if ( !iStatusPane || !iNaviPane )
        {
        return;
        }

    if ( iNaviDecorator )
        {
        CAknNaviLabel* naviLabel = static_cast<CAknNaviLabel*>(iNaviDecorator->DecoratedControl());
        if ( naviLabel )
            {
            naviLabel->SetTextL( aTitle );
            iNaviDecorator->DrawDeferred();
            }
        }
     else
        {
        iNaviDecorator = iNaviPane->CreateNavigationLabelL( aTitle ) ;
        iNaviPane->PushL( *iNaviDecorator ); // activate navi label in navi pane
        iNavidecoratorPushed = ETrue;
        }
    }


// ----------------------------------------------------------------------------
// CNaviPaneHandler::StoreNavipaneL
// 
// ----------------------------------------------------------------------------
//
void CNaviPaneHandler::StoreNavipaneL()
    {
    if ( !iNaviPane || !iStatusPane || iNavipanePushed )
        {
        return;
        }

    if ( iStatusPane->PaneCapabilities(TUid::Uid(EEikStatusPaneUidTitle)).IsPresent() )
        {
        iNaviPane->PushDefaultL();
        iNavipanePushed = ETrue;
        }
    }




/*******************************************************************************
 * class CStatusPaneHandler
 *******************************************************************************/

// -----------------------------------------------------------------------------
// CStatusPaneHandler::NewL
//
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CStatusPaneHandler* CStatusPaneHandler::NewL( CAknAppUi* aAppUi )
    {
    CStatusPaneHandler* self = new (ELeave) CStatusPaneHandler( aAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// Destructor
//
// -----------------------------------------------------------------------------
//
CStatusPaneHandler::~CStatusPaneHandler()
    {
    WLOG("CStatusPaneHandler::~CStatusPaneHandler >");
    TRAP_IGNORE( RestoreOriginalTitleL() );
    
    delete iNaviPaneHandler;
    WLOG("CStatusPaneHandler::~CStatusPaneHandler <");
    }


// -----------------------------------------------------------------------------
// CStatusPaneHandler::ConstructL
//
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CStatusPaneHandler::ConstructL()
    {
    WLOG("CStatusPaneHandler::ConstructL >");
    iNaviPaneHandler = new (ELeave) CNaviPaneHandler( iAppUi->StatusPane() );
    WLOG("CStatusPaneHandler::ConstructL <");
    }


// -----------------------------------------------------------------------------
// CStatusPaneHandler::CStatusPaneHandler
//
// -----------------------------------------------------------------------------
//
CStatusPaneHandler::CStatusPaneHandler( CAknAppUi* aAppUi ) : iAppUi(aAppUi )
    {
    //__ASSERT_ALWAYS( iAppUi, User::Panic( KErrGeneral ) );

    iOriginalTitleStored = EFalse;
    iOriginalTitle = KNullDesC;
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::GetTitleL
// 
// ----------------------------------------------------------------------------
//
TBool CStatusPaneHandler::GetTitleL( CAknAppUi* aAppUi, TDes& aTitle )
    {
    aTitle = KNullDesC;

    CEikStatusPane* statusPane = aAppUi->StatusPane();
    if (statusPane && statusPane->PaneCapabilities( TUid::Uid( EEikStatusPaneUidTitle ) ).IsPresent() )
        {
        CAknTitlePane* titlePane = static_cast<CAknTitlePane*>
            ( statusPane->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
        if ( titlePane->Text() ) 
            {
            StrCopy( aTitle, *titlePane->Text() );
            return ETrue;
            }
        }

    return EFalse;
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::SetTitleL
// 
// ----------------------------------------------------------------------------
//
TBool CStatusPaneHandler::SetTitleL( CAknAppUi* aAppUi, const TDesC& aTitle )
    {
    CEikStatusPane* statusPane = aAppUi->StatusPane();
    if ( statusPane && statusPane->PaneCapabilities( TUid::Uid( EEikStatusPaneUidTitle ) ).IsPresent() )
        {
        CAknTitlePane* titlePane = static_cast<CAknTitlePane*>
            ( statusPane->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
        if ( titlePane->Text() ) 
            {
            titlePane->SetTextL( aTitle );
            return ETrue;
            }
        }
    return EFalse;
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::SetNaviPaneTitleL
// 
// ----------------------------------------------------------------------------
//
void CStatusPaneHandler::SetNaviPaneTitleL( const TDesC& aTitle )
    {
    iNaviPaneHandler->SetNaviPaneTitleL( aTitle );
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::SetNaviPaneTitle
// 
// ----------------------------------------------------------------------------
//
void CStatusPaneHandler::SetNaviPaneTitle( const TDesC& aTitle )
    {
    TRAP_IGNORE( iNaviPaneHandler->SetNaviPaneTitleL( aTitle ) );
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::SetNaviPaneTitleL
// 
// ----------------------------------------------------------------------------
//
void CStatusPaneHandler::SetNaviPaneTitleL( TInt aResource )
    {
    HBufC* hBuf = StringLoader::LoadLC( aResource) ;
    SetNaviPaneTitleL( hBuf->Des() );
    CleanupStack::PopAndDestroy( hBuf );
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::StoreNavipaneL
// 
// ----------------------------------------------------------------------------
//
void CStatusPaneHandler::StoreNavipaneL()
    {
    iNaviPaneHandler->StoreNavipaneL();
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::StoreOriginalTitleL
// 
// ----------------------------------------------------------------------------
//
void CStatusPaneHandler::StoreOriginalTitleL()
    {
    TBool ret = GetTitleL( iAppUi, iOriginalTitle );
    
    if ( ret )
        {
        iOriginalTitleStored = ETrue;
        }
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::RestoreOriginalTitleL
// 
// ----------------------------------------------------------------------------
//
void CStatusPaneHandler::RestoreOriginalTitleL()
    {
    if ( iOriginalTitleStored )
        {
        SetTitleL( iAppUi, iOriginalTitle );
        }
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::SetTitleL
// 
// ----------------------------------------------------------------------------
//
void CStatusPaneHandler::SetTitleL( const TDesC& aText )
    {
    if ( iOriginalTitleStored )
        {
        SetTitleL( iAppUi, aText );
        }
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::SetTitleL
// 
// ----------------------------------------------------------------------------
//
void CStatusPaneHandler::SetTitleL( TInt aResourceId )
    {
    HBufC* hBuf = StringLoader::LoadLC( aResourceId );
    SetTitleL( hBuf->Des() );
    CleanupStack::PopAndDestroy( hBuf );
    }


// -----------------------------------------------------------------------------
// CStatusPaneHandler::StrCopy
//
// -----------------------------------------------------------------------------
//
void CStatusPaneHandler::StrCopy( TDes& aTarget, const TDesC& aSource )
    {
    WLOG("CStatusPaneHandler::StrCopy >");
    TInt len = aTarget.MaxLength();
    if( len < aSource.Length() ) 
        {
        aTarget.Copy( aSource.Left(len) );
        return;
        }
    aTarget.Copy( aSource );
    WLOG("CStatusPaneHandler::StrCopy <");
    }



/******************************************************************************
 * class CDriveUtil
 ******************************************************************************/


// -----------------------------------------------------------------------------
// CDriveUtil::NewL
// 
// -----------------------------------------------------------------------------
//
CDriveUtil* CDriveUtil::NewL()
    {
    CDriveUtil* self = new (ELeave) CDriveUtil();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//
CDriveUtil::~CDriveUtil()
    {
    WLOG("CDriveUtil::~CDriveUtil >");
    iFsSession.Close();
    WLOG("CDriveUtil::~CDriveUtil <");
    }


// -----------------------------------------------------------------------------
// CDriveUtil::CDriveUtil
// 
// -----------------------------------------------------------------------------
//
CDriveUtil::CDriveUtil()
    {
    }


// -----------------------------------------------------------------------------
// CDriveUtil::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CDriveUtil::ConstructL()
    {
    WLOG("CDriveUtil::ConstructL >");
    User::LeaveIfError( iFsSession.Connect() );
    WLOG("CDriveUtil::ConstructL <");
    }


// -----------------------------------------------------------------------------
// CDriveUtil::IsRom
// 
// -----------------------------------------------------------------------------
//
TBool CDriveUtil::IsRom( const TDesC& aFullPath )
    {
    const TUint KMassStorageBits = DriveInfo::EDriveRom;

    TDriveUnit drive( aFullPath );
    TUint driveStatus( 0 );
    
    TInt err = DriveInfo::GetDriveStatus( iFsSession, drive, driveStatus );
    if ( err != KErrNone )
        {
        return EFalse;
        }
        
    if ( (driveStatus & KMassStorageBits) == KMassStorageBits )
        {
        return ETrue;
        }
        
    return EFalse;
    }


// -----------------------------------------------------------------------------
// CDriveUtil::IsMassStorage
// 
// -----------------------------------------------------------------------------
//
TBool CDriveUtil::IsMassStorage( const TDesC& aFullPath )
    {
    const TUint KMassStorageBits = DriveInfo::EDriveInternal |
                                   DriveInfo::EDriveExternallyMountable;

    TDriveUnit drive( aFullPath );
    TUint driveStatus( 0 );
    
    TInt err = DriveInfo::GetDriveStatus( iFsSession, drive, driveStatus );
    if ( err != KErrNone )
        {
        return EFalse;
        }
        
    if ( (driveStatus & KMassStorageBits) == KMassStorageBits )
        {
        return ETrue;
        }
        
    return EFalse;
    }


// -----------------------------------------------------------------------------
// CDriveUtil::IsMemoryCard
// 
// -----------------------------------------------------------------------------
//
TBool CDriveUtil::IsMemoryCard( const TDesC& aFullPath )
    {
    const TUint KMemoryCardBits = DriveInfo::EDriveRemovable |
                                  DriveInfo::EDriveExternallyMountable;

    TDriveUnit drive( aFullPath );
    TUint driveStatus( 0 );
    
    TInt err = DriveInfo::GetDriveStatus( iFsSession, drive, driveStatus );
    if ( err != KErrNone )
        {
        return EFalse;
        }
        
    if ( (driveStatus & KMemoryCardBits) == KMemoryCardBits )
        {
        return ETrue;
        }
        
    return EFalse;
    }

//  End of File  
