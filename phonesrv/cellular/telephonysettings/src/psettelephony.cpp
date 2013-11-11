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
* Description:  ETEL connection and line opening class.
*
*
*/


// INCLUDE FILES
#include <etelmm.h>
#include <e32svr.h>
#include <mmtsy_names.h>
#include "psetconstants.h" 
#include "psettelephony.h" 
#include "phonesettingslogger.h" 

//  MEMBER FUNCTIONS
// ---------------------------------------------------------------------------
// 
//  First phase constructor
// 
// ---------------------------------------------------------------------------
//
CPsetTelephony* CPsetTelephony::NewL( 
    RMobilePhone& aPhone, const TDesC* aTsyName )
    {
    CPsetTelephony* self = new ( ELeave ) CPsetTelephony( aPhone );
    CleanupStack::PushL( self );
    self->ConstructL( aTsyName );
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// 
//  Destructor
// 
// ---------------------------------------------------------------------------
//
CPsetTelephony::~CPsetTelephony()
    {
    if ( iOwnConnection )
        {
        if ( iPhone )
            {
            iPhone->Close();
            }
        
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        /*****************************************************
        *    Series 60 Customer / TSY
        *    Needs customer TSY implementation
        *****************************************************/
        if ( iTelServer )
            {
            iTelServer->UnloadPhoneModule( iTsyName );
            iTelServer->Close();
            }
        delete iTelServer;
        iTelServer = NULL;
        delete iPhone;
        iPhone = NULL;
        }
    } 

// ---------------------------------------------------------------------------
// 
//  Constructor.
// 
// ---------------------------------------------------------------------------
//
void CPsetTelephony::ConstructL( const TDesC* aTsyName )                   
    {
    User::LeaveIfError( ConnectToEtelL( aTsyName ) );
    iOwnConnection = ETrue;
    }

// ---------------------------------------------------------------------------
// 
// Connects to ETel server and loads TSY module, then opens phone object
// if phone(s) found.
// 
// ---------------------------------------------------------------------------
TInt CPsetTelephony::ConnectToEtelL( const TDesC* aTsyName )
    {
    __PHSLOGSTRING("[PHS]--> CPsetTelephony::ConnectToEtelL");
    if ( aTsyName )
        {
        iTsyName= *aTsyName; // use specific tsy name
        }
    else
        {
        iTsyName = KMmTsyModuleName; // use default tsy name
        }
    
    iTelServer = new ( ELeave ) RTelServer;    
    iPhone = new ( ELeave ) RMobilePhone;
    TInt err = KErrNone;
    
    for ( TInt a = 0 ; a < KPSetRetryCount ; a++ )
        {
        err = iTelServer->Connect();
        
        if ( err == KErrNone )
            {
            break;
            }
        
        User::After( KPSetRetryTimeout );
        }
    
    User::LeaveIfError( err );
    
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    /*****************************************************
    *    Series 60 Customer / TSY
    *    Needs customer TSY implementation
    *****************************************************/
    err = iTelServer->LoadPhoneModule( iTsyName );
    
    if ( err == KErrNone )
        {
        iTelServer->SetExtendedErrorGranularity( RTelServer::EErrorExtended );
        
        RTelServer::TPhoneInfo phoneInfo;
        err = iTelServer->GetPhoneInfo( 0, phoneInfo );  
        if ( err == KErrNone )
            {
            // open phone subsession
            err = iPhone->Open( *iTelServer, phoneInfo.iName ); 
            }
        }

    __PHSLOGSTRING("[PHS] <--CPsetTelephony::ConnectToEtelL");
    return err;
    }

// ---------------------------------------------------------------------------
// 
// Checks that correct line for diverting is open. If not, lines
// are gone through and when one with correct capabilities is found,
// it is opened.
// 
// ---------------------------------------------------------------------------
//
void CPsetTelephony::CheckLineModeL( const TServiceGroup aServiceGroup,
                                    RMobilePhone* aPhone, RMobileLine* aLine)
    {
    __PHSLOGSTRING("[PHS]--> CPsetTelephony::CheckLineModeL");
    if (!aPhone || !aLine)
        {
        User::Leave(KErrArgument);
        }
    
    RLine::TCaps lineCaps;    
    lineCaps.iFlags = 0;
    TBool oldMode( EFalse ); // if true, another line must be opened    
    
    if ( aLine->SubSessionHandle() == 0 )
        {
        oldMode = ETrue;
        }
    else
        {
        User::LeaveIfError( aLine->GetCaps( lineCaps ) );
        }

    TUint32 capsFlags( 0 );
    capsFlags = SetCapFlagsL( aServiceGroup, oldMode, lineCaps );
    
    if ( oldMode )
        { // open line in right mode according to service group        
        if ( aLine->SubSessionHandle() )
            {
            aLine->Close();        
            }
        
        TInt lines = 0;
        User::LeaveIfError( aPhone->EnumerateLines( lines ) );
        //lines is no of lines, to check each line, we need to 
        // check all lines from 0 to lines-1
        for (lines--; KPSetNoLinesAvailable < lines; lines-- )
            {
            RPhone::TLineInfo lineInfo;
            User::LeaveIfError( aPhone->GetLineInfo( lines, lineInfo ) );            
            
            if ( lineInfo.iLineCapsFlags & capsFlags )
                {
                User::LeaveIfError( aLine->Open( *aPhone, lineInfo.iName ) );
                return;
                }
            }        
        }
    __PHSLOGSTRING("[PHS] <--CPsetTelephony::CheckLineModeL");
    }
// ---------------------------------------------------------------------------
// 
// Sets cap flags.
// 
// ---------------------------------------------------------------------------
//
TUint32 CPsetTelephony::SetCapFlagsL( 
    const TServiceGroup aServiceGroup, TBool& aMode, RLine::TCaps& aCaps )
    {
    __PHSLOGSTRING("[PHS]--> CPsetTelephony::SetCapFlagsL");
    TUint32 flags(0);

    switch ( aServiceGroup )
        {
        case EServiceGroupVoice:
            __PHSLOGSTRING("[PHS]    CPsetTelephony::SetCapFlagsL: EServiceGroupVoice");
            // always open voice line even because it could have been another 
            // ALS line which was open
            aMode = ETrue;
            flags |= RLine::KCapsVoice;
            break;
        case EServiceGroupData:
            __PHSLOGSTRING("[PHS]    CPsetTelephony::SetCapFlagsL: EServiceGroupData");
            if ( !( aCaps.iFlags & RLine::KCapsData ) )
                {
                aMode = ETrue;
                }
            flags |= RLine::KCapsData;
            break;
        case EServiceGroupFax:
            __PHSLOGSTRING("[PHS]    CPsetTelephony::SetCapFlagsL: EServiceGroupFax");
            if ( !( aCaps.iFlags & RLine::KCapsFax ) )
                {
                aMode = ETrue;
                }
            flags |= RLine::KCapsFax;
            break;
        case EServiceGroupAllTeleservices:
            __PHSLOGSTRING("[PHS]    CPsetTelephony::SetCapFlagsL: EServiceGroupAllTeleservices");
            aMode = ETrue;
            flags |= RLine::KCapsData;
            flags |= RLine::KCapsFax;
            flags |= RLine::KCapsVoice;
            break;
        default:
            User::Leave( KErrNotSupported );
        }

    __PHSLOGSTRING("[PHS] <--CPsetTelephony::SetCapFlagsL");
    return flags;
    }

// ---------------------------------------------------------------------------
// 
// C++ constructor.
// 
// ---------------------------------------------------------------------------
//
CPsetTelephony::CPsetTelephony
                ( RMobilePhone& aPhone ) : iPhone ( &aPhone )
    {
    }
//End of file
