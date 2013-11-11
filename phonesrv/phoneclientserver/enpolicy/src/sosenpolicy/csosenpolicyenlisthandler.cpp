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
* Description:  Shared Data Monitor
*
*/


// INCLUDES
#include "csosenpolicyenlisthandler.h" 
#include <e32property.h>
#include <centralrepository.h>
#include <telservicesinternalcrkeys.h>
#include <defaultemergencynumberscrkeys.h>

// FUNCTIONS

// -----------------------------------------------------------------------------
// CSosEnPolicyENListHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSosEnPolicyENListHandler* CSosEnPolicyENListHandler::NewL( 
     )
    {
    CSosEnPolicyENListHandler* self = new ( ELeave ) 
        CSosEnPolicyENListHandler();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CSosEnPolicyENListHandler::~CSosEnPolicyENListHandler()
// -----------------------------------------------------------------------------
//
CSosEnPolicyENListHandler::~CSosEnPolicyENListHandler()
    {
    if ( iListener )
        {
        iListener->StopListening();
        delete iListener;
        }
    delete iRepository;
    
    iProductSpecificList.Zero();
    iCurrentList.Zero();
    }

// -----------------------------------------------------------------------------
// CSosEnPolicyENListHandler::CSosEnPolicyENListHandler
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSosEnPolicyENListHandler::CSosEnPolicyENListHandler( 
    )
    {
    }

// -----------------------------------------------------------------------------
// CSosEnPolicyENListHandler::ConstructL
// Construction, 2nd phase.
// -----------------------------------------------------------------------------
//        
void CSosEnPolicyENListHandler::ConstructL()
    {
    ClearCurrentList();
    iRepository = CRepository::NewL( KCRUidDefaultEmergencyNumbers );
    iListener = CCenRepNotifyHandler::NewL( 
        *this, 
        *iRepository, 
        CCenRepNotifyHandler::EStringKey, 
        KDefaultEmergencyNumbersList);
    
    iListener->StartListeningL();
    }


// -----------------------------------------------------------------------------
// CSosEnPolicyENListHandler::AddToList
// 
// Adds number to list with separator.
// If numbers are set to permanent, also list start index is updated. 
// Permanent numbers must be in the beginning of the list,
// so SIM numbers are removed before writing!
// -----------------------------------------------------------------------------
//
void CSosEnPolicyENListHandler::AddToList( 
    const TDesC& aNumber, 
    TBool aPermanent ) 
    {
    TInt length = aNumber.Length();
    if ( length > 0 ) // for null string do nothing
        {
        // Remove non permanent numbers
        if ( aPermanent )
            {
            ClearCurrentList();
            }
        // Write new ones
    
        for ( TInt i = 0; i < length; i++ )
            {
            iCurrentList[ iCurrentListPlace ] = aNumber[i];
            iCurrentListPlace++;
            }
        iCurrentListPlace++;
        // Update starting point
        if ( aPermanent )
            {
            iCurrentListStart = iCurrentListPlace;
            }
        }
    }

// -----------------------------------------------------------------------------
// CSosEnPolicyENListHandler::ClearCurrentList
// 
// Clears list to be written to Central Repository.
// Does not clear permanent numbers, from the begining of list
// There is always 112 or 911 (WCDMA) and optionally product specific numbers.
// Basically only deleted numbers are SIM emergency numbers.
// -----------------------------------------------------------------------------
//
void CSosEnPolicyENListHandler::ClearCurrentList()
    {
    iCurrentList.SetLength( KEnPolicySDEmergencyNumberListLength );
    iCurrentListPlace = iCurrentListStart;
    for ( TInt i = iCurrentListStart; 
          i < KEnPolicySDEmergencyNumberListLength; 
          i++ )
        {
        iCurrentList[i] = KEnPolicySDNumberSeparator;
        }
    }

// -----------------------------------------------------------------------------
// CSosEnPolicyENListHandler::IsInListByAdvancedMode
// 
// Checks if the number is in list by advanced mode check.
//
// -----------------------------------------------------------------------------
//
TBool CSosEnPolicyENListHandler::IsInListByAdvancedMode ( TDes& aNumber )
    {    
    TLex list( iCurrentList );
    TLexMark mark;
    TPtrC token_candidate( KNullDesC() ); 
    
    for ( TInt i = 0; i < KEnPolicySDEmergencyNumberListLength; i++ )
        {
        list.Mark( mark );
        list.SkipCharacters();
        TPtrC token;
        token.Set( list.MarkedToken( mark ) );
        if ( token.Length() && ( token == aNumber.Right( token.Length() ) ) )
            {
            // _DDPRINT( 4, "ENPolicy.SDM.IsInListAdvanced.ok.", place );
            if( token_candidate == KNullDesC )
                { 
                token_candidate.Set( token ); 
                } 
            else 
                { 
                if( token.Length() > token_candidate.Length() ) 
                    { 
                    token_candidate.Set( token );     
                    } 
                } 
            } 
        list.Inc(); 
        } 

    if( token_candidate != KNullDesC ) 
        { 
        aNumber = token_candidate; 
        return ETrue; 
        }    
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSosEnPolicyENListHandler::IsInListByNormalMode
// 
// Checks if the number is in list by normal mode check.
//
// -----------------------------------------------------------------------------
//
TBool CSosEnPolicyENListHandler::IsInListByNormalMode( 
    const TDesC& aNumber )
    {
    TInt length = aNumber.Length();
    TInt place = iCurrentList.Find( aNumber ); // first number of aNumber

    // _DDPRINT( 4, "ENPolicy.SDM.IsInListNormal.ok.", place );  // debug print

    if ( place < 0 )
        {
        // negative value is an error, not found!
        // return immediately
        return EFalse;
        }

    TInt endOfMatch = place + length; // end of number + 1

    // we check the begining
    if ( place > 0 )
        {
        // we are not in the begining of buffer, so we have to check
        // char before that
        if ( iCurrentList[ place -1 ] != KEnPolicySDNumberSeparator )
            {
            // this was just a substring numbers before that
            return EFalse;
            }
        }

    // beginning was ok, so we can continue
    
    // now checking the end
    // there must be atleast one empty space in the end that
    // we do the checking of number end.
    if ( endOfMatch < ( iCurrentList.Length() - 2 ) )
        {
        if ( iCurrentList[ endOfMatch ] != KEnPolicySDNumberSeparator )
            {
            return EFalse;
            }
        }

    // if we get so far, its ok.
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CSosEnPolicyENListHandler::ReadProductSpecificNumbers
// 
// Reads product specific values from Central Repository.
// -----------------------------------------------------------------------------
//
const TDesC& CSosEnPolicyENListHandler::ReadProductSpecificNumbers()
    {
    // This is done only once in bootup phase.
    iProductSpecificList.Zero();

    TInt err = KErrNone;
    err = iRepository->Get( KDefaultEmergencyNumbersList, 
                            iProductSpecificList );

    if ( err != KErrNone )
        {
        iProductSpecificList.Zero();
        }

    // _DDPRINT( 4, "ENPolicy.SDM.ReadPSN.list", iProductSpecificList );

    return iProductSpecificList;
    }

// -----------------------------------------------------------------------------
// CDosEmergencyNumberPolicy::HandleNotifyString
// 
// Inherited from MCenRepNotifyHandlerCallback.
// -----------------------------------------------------------------------------
//
void CSosEnPolicyENListHandler::HandleNotifyString( 
    TUint32 aId, const TDesC16& aNewValue )
    {
    if( aId == KDefaultEmergencyNumbersList )
        {
        iProductSpecificList.Zero();
        iProductSpecificList.Copy( aNewValue );
        
        ClearCurrentList();
        AddToList( iProductSpecificList );
        }
    }

//  End of File
