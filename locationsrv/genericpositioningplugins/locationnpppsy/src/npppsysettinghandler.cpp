/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This is NPP PSY Positioning Plug-in ( PSY ).
*
*/



// INCLUDE FILES
#include <e32std.h>            
#include <centralrepository.h>

#include "npppsysettinghandler.h"
#include "npppsystatushandler.h"
#include "npppsyinternalcrkeys.h"
#include "npppsylogging.h"
#include "npppsypanic.h"
#include "npppsy.hrh"

// Constants
// Sepperator used in setting
_LIT( KNppPsySettingSeperator, " " );

// The width of each PSY uid in PSY list
const TInt KNppPsyPsyUidWidth = 8;

// The length of buffer which is used for logging into file
const TInt KBufferSize = 64;

// PSY list length when the list is frozen and no fallback
const TInt KNppPsyListLengthWhenFrozenNoFallback = 1;

const TText KNppPsySpaceCharacter = ' ';

// ========================== == MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CNppPsySettingHandler::CNppPsySettingHandler
// C++default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CNppPsySettingHandler::CNppPsySettingHandler( CNppPsyStatusHandler& aStatusHandler )
    : CActive( CActive::EPriorityStandard ),
      iStatusHandler( aStatusHandler )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CNppPsySettingHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CNppPsySettingHandler::ConstructL()
    {
    TRACESTRING( "CNppPsySettingHandler::ConstructL start" )
    //Central repository
    iRepository = CRepository::NewL( KCRUidNppPsy );

    //Start notification
    iRepository->NotifyRequest( KNppPsyPsyList, iStatus );
    SetActive();
    
    //Parse settings
    ParseSettingL( iListFrozen, iPsyList );

    //Check if key value is changed in ROM key
    ValidateRomKeyValueL();

    //Check if the list is empty
    iStatusHandler.SetPsyListStatus( iPsyList.Count() != 0 );
    
    TRACESTRING( "CNppPsySettingHandler::ConstructL end" )
    }


// -----------------------------------------------------------------------------
// CNppPsySettingHandler::NewL
// Two - phased constructor.
// -----------------------------------------------------------------------------
//
CNppPsySettingHandler* CNppPsySettingHandler::NewL( 
        CNppPsyStatusHandler& aStatusHandler )
    {
    CNppPsySettingHandler* self = new( ELeave ) 
        CNppPsySettingHandler( aStatusHandler );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

    
// -----------------------------------------------------------------------------
// CNppPsySettingHandler::~CNppPsySettingHandler
// -----------------------------------------------------------------------------
//
CNppPsySettingHandler::~CNppPsySettingHandler()
    {
    Cancel();
    
    delete iRepository;
    
    iPsyList.Close();
    
    TRACESTRING( "CNppPsySettingHandler:: destructed" )
    }


// -----------------------------------------------------------------------------
// CNppPsySettingHandler::GetPsyListL
// -----------------------------------------------------------------------------
//
void CNppPsySettingHandler::GetPsyListL( RArray < TUid> &aList )
    {
    aList.Reset();
    TInt count = iPsyList.Count();
    
    if(iListFrozen == ENppPsyPsyListFrozenNoFallback)
        {
        //If list is frozen and no fallback, we return only the first PSY
        count = ( count == 0 ? 0 : KNppPsyListLengthWhenFrozenNoFallback );
        }
        
   TBuf<KBufferSize> buffer;
   buffer.Copy(_L("Get PSY List..."));
   TRACETEXT(buffer)
    for ( TInt i = 0; i < count; i++ )
        {
        User::LeaveIfError( aList.Append( iPsyList[i] ) );
	   
	    buffer.Copy(iPsyList[i].Name());
	    TRACETEXT(buffer)
        }
    }

// -----------------------------------------------------------------------------
// CNppPsySettingHandler::MovePsyLast
// -----------------------------------------------------------------------------
//
void CNppPsySettingHandler::MovePsyLast( TUid aPsy )
    {
    if ( !iListFrozen )
        {
        TInt index = iPsyList.Find( aPsy );
        if ( index != KErrNotFound )
            {
            iPsyList.Remove( index );
            if ( iPsyList.Append( aPsy ) == KErrNone )
                {
                WriteSetting();
                }
            }
        }

    }

// -----------------------------------------------------------------------------
// CNppPsySettingHandler::RemovePsy
// -----------------------------------------------------------------------------
//
void CNppPsySettingHandler::RemovePsy( TUid aPsy )
    {
    TInt index = iPsyList.Find( aPsy );
    if ( index != KErrNotFound )
        {
        iPsyList.Remove( index );
        WriteSetting();
        }
    }

// -----------------------------------------------------------------------------
// CNppPsySettingHandler::RunL
// -----------------------------------------------------------------------------
//
void CNppPsySettingHandler::RunL()
    {
    ParseSettingL( iListFrozen, iPsyList );

    //Check if the list is empty
    iStatusHandler.SetPsyListStatus( iPsyList.Count() != 0 );

    //issue another request
    iRepository->NotifyRequest( KNppPsyPsyList, iStatus );
    SetActive();
    }
        
// -----------------------------------------------------------------------------
// CNppPsySettingHandler::DoCancel
// -----------------------------------------------------------------------------
//
void CNppPsySettingHandler::DoCancel()
    {
    if ( iRepository )
        {
        iRepository->NotifyCancelAll(); //error ignored
        }
    }

// -----------------------------------------------------------------------------
// CNppPsySettingHandler::RunError
// -----------------------------------------------------------------------------
//
TInt CNppPsySettingHandler::RunError( TInt aError )
    {
    //This function may be called when PSY list is corrupted
    if ( aError != KErrNoMemory && aError != KErrCancel )
        {
        iStatusHandler.SetPsyListStatus( EFalse );
        }
        
    //issue another request
    iRepository->NotifyRequest( KNppPsyPsyList, iStatus );
    SetActive();

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CNppPsySettingHandler::ParseSettingL
// -----------------------------------------------------------------------------
//
void CNppPsySettingHandler::ParseSettingL(
            TInt& aListFrozen, 
            RArray < TUid >& aPsyList )
    {
    TBuf < KNppPsyPsyListMaxLength > setting;
    User::LeaveIfError( iRepository->Get( KNppPsyPsyList, setting ) );

    TRACESTRING("PSY List from CenRep...")
    TRACETEXT(setting)
    //Reset list
    aPsyList.Reset();

    TLex lex( setting );
    lex.SkipSpace();
    TInt frozen;
    User::LeaveIfError( lex.Val( frozen ) ); //frozen status
    aListFrozen = frozen;

    lex.SkipSpace();
    TUint32 psyUid;
    while ( !lex.Eos() )
        {
        User::LeaveIfError( lex.Val( psyUid, EHex ) );
        User::LeaveIfError( aPsyList.Append( TUid::Uid( psyUid ) ) );
        lex.SkipSpace();
        }
    }

// -----------------------------------------------------------------------------
// CNppPsySettingHandler::ValidateRomKeyValueL
// -----------------------------------------------------------------------------
//
void CNppPsySettingHandler::ValidateRomKeyValueL()
    {
	TInt romListFrozen;
    RArray < TUid > romPsyList;
    CleanupClosePushL(romPsyList);

	TBuf < KNppPsyPsyListMaxLength > setting;
	User::LeaveIfError( iRepository->Get( KNppPsyPsyList, setting ) );
	setting.TrimRight();
	setting.Append(KNppPsySpaceCharacter);
	User::LeaveIfError(iRepository->Set( KNppPsyPsyList,setting ));
	
    iRepository->Reset( KNppPsyPsyList );
    ParseSettingL( romListFrozen, romPsyList );
    
    RemoveDuplicatePsyUid(iPsyList,romPsyList);
	iListFrozen = romListFrozen;
	
    CleanupStack::PopAndDestroy(&romPsyList);
    }

// -----------------------------------------------------------------------------
// CNppPsySettingHandler::WriteSetting
// -----------------------------------------------------------------------------
//
void CNppPsySettingHandler::WriteSetting()
    {
    TBuf < KNppPsyPsyListMaxLength > setting;

    //Frozen status
    setting.AppendNum( iListFrozen );
    setting.Append( KNppPsySettingSeperator );

    TInt count = iPsyList.Count();
    for ( TInt i = 0; i < count; i++ )
        {
        setting.AppendNumFixedWidth( iPsyList[i].iUid, EHex, KNppPsyPsyUidWidth );
        setting.Append( KNppPsySettingSeperator );
        }
        
	TRACESTRING("Writing PSY List...")
	TRACETEXT(setting)
 
    iRepository->Set( KNppPsyPsyList, setting ); //error ignored
    }

// -----------------------------------------------------------------------------
// CNppPsySettingHandler::RemoveDuplicatePsyUid
// -----------------------------------------------------------------------------
//
void CNppPsySettingHandler::RemoveDuplicatePsyUid(
            RArray < TUid >& aToPsyList ,
            RArray < TUid >& aFromPsyList )
    {
    
	    TInt count = aFromPsyList.Count();
	    for ( TInt i = 0; i < count; i++ )
	        {
                TInt index = aToPsyList.Find( aFromPsyList[i] );
	            if (  index == KErrNotFound )
        	  	{
        	  		aToPsyList.Append( aFromPsyList[i] );		
        	  	}
	        }
	    WriteSetting();    
    }
    
//  End of File  
