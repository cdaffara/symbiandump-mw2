/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*      CPsetContainer is the general access class for phonesettings.
*      Container must exists as long as PS objects are used.          
*
*
*/


// INCLUDES
#include    "psetcontainer.h" 
#include    "psetconstants.h" 
#include    "psetrefreshhandlerimpl.h" 
#include    <mmtsy_names.h>

// ---------------------------------------------------------------------------
// 
// Creates Call Waiting object
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetCallWaiting* CPsetContainer::CreateCWObjectL(
    MPsetCallWaitingObserver& aObserver )
    {
    return CPsetCallWaiting::NewL( iPhone, aObserver );
    }

// ---------------------------------------------------------------------------
// 
// Creates Diverting object
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetCallDiverting* CPsetContainer::CreateCFObjectL( 
    MPsetDivertObserver& aObserver )
    {
    return CPsetCallDiverting::NewL( aObserver, iPhone );
    }

// ---------------------------------------------------------------------------
// 
// Creates CLI object
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetCli* CPsetContainer::CreateCliObjectL( 
    MPsetCliObserver& aObserver )
    {
    return CPsetCli::NewL( iPhone, aObserver );
    }

// ---------------------------------------------------------------------------
// 
// Creates Call Barring object
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetCallBarring* CPsetContainer::CreateCBObjectL( 
    MPsetBarringObserver& aObserver )
    {
    return CPsetCallBarring::NewL( aObserver, iPhone );
    }

// ---------------------------------------------------------------------------
// 
// Creates Network object
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetNetwork* CPsetContainer::CreateNetworkObjectL( 
    MPsetNetworkInfoObserver& aObserver )
    {
    return CPsetNetwork::NewL( iPhone, aObserver );
    }


// -----------------------------------------------------------------------------
// 
// Creates Refresh Handler object
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CPSetRefreshHandler* CPsetContainer::CreateRefreshHandlerL()
    {
    return CPSetRefreshHandlerImpl::NewL(); 
    }


// ---------------------------------------------------------------------------
// 
// Connects to ETEL telephony server and initialises phone, 
// line and event monitors.
// 
// ---------------------------------------------------------------------------
//
void CPsetContainer::ConstructL()
    {
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    User::LeaveIfError( ConnectServer() );
    
    User::LeaveIfError( 
        iServer.SetExtendedErrorGranularity( RTelServer::EErrorExtended ) );
    
    TInt numPhones;
    
    User::LeaveIfError( iServer.EnumeratePhones( numPhones ) );
    if ( !numPhones )
        {
        User::Leave( KErrGeneral );
        }
    
    //match phone name to correct one
    RTelServer::TPhoneInfo phoneInfo;
    TName matchTsyName;
    TInt i = 0;
    for ( ; i < numPhones; i++ )
        {
        User::LeaveIfError( iServer.GetTsyName( i, matchTsyName ) );
        if    ( matchTsyName.CompareF( KMmTsyModuleName ) == 0 )
            {            
            User::LeaveIfError( iServer.GetPhoneInfo( i, phoneInfo ) );
            break;
            }
        }
    if ( i == numPhones )
        {
        User::Leave( KErrGeneral );
        }
    
    //open phone subsession
    User::LeaveIfError( iPhone.Open( iServer, phoneInfo.iName ) ); 
    
    // Open line - line is opened using phone and name is specified,
    User::LeaveIfError( iLine.Open( iPhone, KMmTsyVoice1LineName ) );
    }

// ---------------------------------------------------------------------------
// 
// Connects to Etel and loads Tsy module
// 
// ---------------------------------------------------------------------------
//
TInt CPsetContainer::ConnectServer()
    {   
    TInt err = KErrNone;
    for ( TInt a = 0 ; a < KPSetRetryCount ; a++ )
        {
        err = iServer.Connect();
        
        if ( err == KErrNone )
            {
            break;
            }
        
        User::After( KPSetRetryTimeout );
        }
    
    if ( err != KErrNone )
        {
        return err;
        }
    else
        {    
        // Access the phone
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        /*****************************************************
        *    Series 60 Customer / TSY
        *    Needs customer TSY implementation
        *****************************************************/
        return iServer.LoadPhoneModule( KMmTsyModuleName );
        }
    }

// ---------------------------------------------------------------------------
// 
// destructor
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetContainer::~CPsetContainer()
    {
    // Close line.
    iLine.Close();
    iPhone.Close();    
    iServer.Close();
    }

// ---------------------------------------------------------------------------
// 
// Symbian OS 2-phase constructor
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetContainer* CPsetContainer::NewL()                
    {
    CPsetContainer* self = new( ELeave ) CPsetContainer;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// ---------------------------------------------------------------------------
// 
// Creates Network object for Network Mode observer
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetNetwork* CPsetContainer::CreateNetworkModeObjectL(
            MPsetNetworkModeObserver& aModeObserver )
    {
    return CPsetNetwork::NewL( iPhone, aModeObserver );
    }

// End of file
