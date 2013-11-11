/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Call diverting class. Sets and gets call diverting status. 
*
*
*/


//  Include Files
#include <etelmm.h>           
#include <e32math.h>
#include <e32svr.h>
#include <badesca.h>
#include <centralrepository.h>
#include <settingsinternalcrkeys.h> 

#include "psetcalldiverting.h" 
#include "psetcontainer.h" 
#include "mpsetdivertobs.h" 
#include "psettelephony.h" 
#include "psetpanic.h" 
#include "phonesettingslogger.h" 

#include "psetcalldivertingcreator.h" 
#include "mcalldiverting.h" 

//  MEMBER FUNCTIONS
// ---------------------------------------------------------------------------
// First phase constructor
// ---------------------------------------------------------------------------       
EXPORT_C CPsetCallDiverting* CPsetCallDiverting::NewL( MPsetDivertObserver& aObserver,
                  RMobilePhone& aPhone )
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallDiverting::NewL");
    CPsetCallDiverting* self = new ( ELeave ) CPsetCallDiverting( aPhone );
    CleanupStack::PushL( self );
    self->ConstructL( aObserver );
    CleanupStack::Pop( self );
    __PHSLOGSTRING("[PHS] <--CPsetCallDiverting::NewL");
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
EXPORT_C CPsetCallDiverting::~CPsetCallDiverting()
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallDiverting::~CPsetCallDiverting");
    iReqObserver = NULL;

    delete iRepository;
    
    delete iDivert;
    iDivert = NULL;
    __PHSLOGSTRING("[PHS]<-- CPsetCallDiverting::~CPsetCallDiverting");
    } 

// ---------------------------------------------------------------------------
// Symbian OS constructor
// ---------------------------------------------------------------------------
void CPsetCallDiverting::ConstructL( MPsetDivertObserver& aObserver )
    {
    __PHSLOGSTRING("[PHS] -->CPsetCallDiverting::ConstructL" );
    iDivert= CPSetCallDivertingCreator::CreateCallDivertingL( aObserver, iPhone, this );
    iRepository = CRepository::NewL( KCRUidCallForwarding );
    __PHSLOGSTRING("[PHS] <--CPsetCallDiverting::ConstructL" );
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CPsetCallDiverting::CPsetCallDiverting( RMobilePhone& aPhone ) :
                    iPhone ( aPhone )
    {
    }

// ---------------------------------------------------------------------------
// Sets diverting. 
// ---------------------------------------------------------------------------
EXPORT_C void CPsetCallDiverting::SetDivertingL( 
    const TCallDivertSetting& aDivert, 
    TBasicServiceGroups aBsc, TBool /*aVmbx*/ )
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallDiverting::SetDivertingL");    
    __PHSLOGSTRING1("[PHS]    CPsetCallDiverting::SetDivertingL: aDivert.iCondition: %d", aDivert.iCondition );
    __PHSLOGSTRING1("[PHS]    CPsetCallDiverting::SetDivertingL: aDivert.iStatus: %d", aDivert.iStatus );
    __PHSLOGSTRING1("[PHS]    CPsetCallDiverting::SetDivertingL: aDivert.iServiceGroup: %d", aDivert.iServiceGroup );
    iDivert->SetDivertingL( aDivert, aBsc );
    __PHSLOGSTRING("[PHS] <--CPsetCallDiverting::SetDivertingL" );
    }

// --------------------------------------------------------------------------- 
// Request diverting status from network asynhronously.
// ---------------------------------------------------------------------------
EXPORT_C void CPsetCallDiverting::GetDivertingStatusL( 
    const TServiceGroup aServiceGroup,
    const TCallDivertingCondition aCondition, 
    TBasicServiceGroups aBsc ) 
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallDiverting::GetDivertingStatusL" );
    __PHSLOGSTRING1("[PHS]    CPsetCallDiverting::GetDivertingStatusL: aServiceGroup: %d", aServiceGroup );
    __PHSLOGSTRING1("[PHS]    CPsetCallDiverting::GetDivertingStatusL: aCondition: %d", aCondition );
    __PHSLOGSTRING1("[PHS]    CPsetCallDiverting::GetDivertingStatusL: aBsc: %d", aBsc );
    iDivert->GetDivertingStatusL( aServiceGroup, aCondition, aBsc );
    __PHSLOGSTRING("[PHS] <--CPsetCallDiverting::GetDivertingStatusL" );
    }

// ---------------------------------------------------------------------------
// Cancels pending request.
// ---------------------------------------------------------------------------
EXPORT_C TInt CPsetCallDiverting::CancelCurrentRequest()
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallDiverting::CancelCurrentRequest" );
    TInt err(KErrNone);
    err = iDivert->CancelCurrentRequest();
    __PHSLOGSTRING("[PHS] <--CPsetCallDiverting::CancelCurrentRequest" );
    return err; 
    }

// ---------------------------------------------------------------------------
// The default (last forwarded-to) numbers can be retrieved through this method.
// The amount of available numbers is returned.
// Relies on default numbers having right order in shared data.
// ---------------------------------------------------------------------------
EXPORT_C TInt CPsetCallDiverting::GetDefaultNumbersL( 
    CDesC16ArrayFlat& aDefNumbers )
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallDiverting::GetDefaultNumbersL" );
    return iDivert->GetNumbersFromSharedDataL( NULL, aDefNumbers );
    }

// ---------------------------------------------------------------------------
// Sets new default number the the default numbers list, erases the oldest one.
// ---------------------------------------------------------------------------
EXPORT_C TInt CPsetCallDiverting::SetNewDefaultNumberL( TDes& aNumber )
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallDiverting::SetNewDefaultNumberL" );
    TInt retValue(KErrNone);
    retValue = iDivert->SetNewDefaultNumberL( aNumber );
    __PHSLOGSTRING("[PHS] <--CPsetCallDiverting::SetNewDefaultNumberL" );
    return retValue; //return value is not used
    }

// ---------------------------------------------------------------------------
// Gets the timer value index related to "cfnry" from shared data
// In error cases, return default value (30).
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPsetCallDiverting::GetTimerValueL()
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallDiverting::GetTimerValueL" );
    TInt timerValue(0);
    timerValue = iDivert->GetTimerValueL();
    __PHSLOGSTRING("[PHS] <--CPsetCallDiverting::GetTimerValueL" );
    return timerValue;
    }

// ---------------------------------------------------------------------------
// Sets the default time for "divert when not answered" to .ini file
// ---------------------------------------------------------------------------
EXPORT_C TInt CPsetCallDiverting::SetTimerValueL( const TInt& aValue )
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallDiverting::SetTimerValueL" );
    TInt retValue(KErrNone);
    retValue = iDivert->SetTimerValueL( aValue );
    __PHSLOGSTRING1("[PHS] <--CPsetCallDiverting::SetTimerValueL: retValue: %d", retValue );
    return retValue; 
    }

// ---------------------------------------------------------------------------
// Swaps the most recently used number to first in the shared data file.
// ---------------------------------------------------------------------------
EXPORT_C void CPsetCallDiverting::SwapDefaultNumberL( const TInt& aLocation )
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallDiverting::SwapDefaultNumberL" );
    iDivert->SwapDefaultNumberL( aLocation );
    __PHSLOGSTRING("[PHS] <--CPsetCallDiverting::SwapDefaultNumberL" );
    }

// ---------------------------------------------------------------------------
// Saves fax call diverting number to shareddata.
// ---------------------------------------------------------------------------
EXPORT_C TInt CPsetCallDiverting::SetUsedDataNumberL(  TTelNumber& aNumber ) 
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallDiverting::SetUsedDataNumberL" );
    TInt retValue = KErrNone;
    retValue = iDivert->SaveKey( KSettingsCFUsedDataNumber, aNumber );
    __PHSLOGSTRING1("[PHS]    CPsetCallDiverting::SetUsedDataNumberL: aNumber: %S", &aNumber );
    __PHSLOGSTRING1("[PHS]    CPsetCallDiverting::SetUsedDataNumberL: retValue: %d", retValue );
    __PHSLOGSTRING("[PHS] <--CPsetCallDiverting::SetUsedDataNumberL" );
    return retValue;
    }

// ---------------------------------------------------------------------------
// Returns used number for data call forwardings.
// ---------------------------------------------------------------------------
EXPORT_C HBufC* CPsetCallDiverting::GetUsedDataNumberLC( ) 
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallDiverting::GetUsedDataNumberLC" );
    TTelNumber readNumber;

    iRepository->Get( KSettingsCFUsedDataNumber, readNumber );

    __PHSLOGSTRING1("[PHS]    CPsetCallDiverting::GetUsedDataNumberLC: readNumber: %S", &readNumber );
    __PHSLOGSTRING("[PHS] <--CPsetCallDiverting::GetUsedDataNumberLC" );
    
    return readNumber.AllocLC();
    }

// ---------------------------------------------------------------------------
// Saves fax call diverting number to shareddata.
// ---------------------------------------------------------------------------
EXPORT_C TInt CPsetCallDiverting::SetUsedFaxNumberL(  TTelNumber& aNumber ) 
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallDiverting::SetUsedFaxNumberL" );
    TInt retValue = KErrNone;

    retValue =  iDivert->SaveKey( KSettingsCFUsedFaxNumber, aNumber );

    __PHSLOGSTRING1("[PHS]    CPsetCallDiverting::SetUsedFaxNumberL: aNumber: %S", &aNumber );
    __PHSLOGSTRING1("[PHS]    CPsetCallDiverting::SetUsedFaxNumberL: retValue: %d", retValue );
    __PHSLOGSTRING("[PHS] <--CPsetCallDiverting::SetUsedFaxNumberL" );

    return retValue;
    }

// ---------------------------------------------------------------------------
// Returns used number for fax call forwardings.
// ---------------------------------------------------------------------------
EXPORT_C HBufC* CPsetCallDiverting::GetUsedFaxNumberLC() 
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallDiverting::GetUsedFaxNumberLC" );
    TTelNumber readNumber;

    iRepository->Get( KSettingsCFUsedFaxNumber, readNumber );

    __PHSLOGSTRING1("[PHS]    CPsetCallDiverting::GetUsedFaxNumberLC: readNumber: %S", &readNumber );
    __PHSLOGSTRING("[PHS] <--CPsetCallDiverting::GetUsedFaxNumberLC" );

    return readNumber.AllocLC();
    }

// ---------------------------------------------------------------------------
// Sets request observer.
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCallDiverting::SetRequestObserver( MPsetRequestObserver* aObs )
    {
    iReqObserver = aObs;
    iDivert->SetRequestObserver(aObs);
    }

// ---------------------------------------------------------------------------
// Queries for voice mail box number
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCallDiverting::VoiceMailQueryL( TDes& /*aTelNumber*/ )
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallDiverting::VoiceMailQueryL" );

    __PHSLOGSTRING("[PHS] <--CPsetCallDiverting::VoiceMailQueryL" );
    }

// ---------------------------------------------------------------------------
// Queries for voice mail box number
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCallDiverting::VideoMailQueryL( TDes& /*aTelNumber*/ )
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallDiverting::VideoMailQueryL" );

    __PHSLOGSTRING("[PHS] <--CPsetCallDiverting::VideoMailQueryL" );
    }    
    
// ---------------------------------------------------------------------------
// Opens Vmbx. Leaves vmbx to the stack. 
// ---------------------------------------------------------------------------
//
TInt CPsetCallDiverting::OpenVideoMailboxLC( TDes& /*aTelNumber*/, RVmbxNumber& /*aVmbx*/ )
    {
    return 0;
    }
    
// End of File
