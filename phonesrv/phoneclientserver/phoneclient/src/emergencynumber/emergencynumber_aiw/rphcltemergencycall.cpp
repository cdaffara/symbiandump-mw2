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
* Description: Implementation of RPhCltEmergencyCall class.
*
*/



// INCLUDE FILES
#include    <cphcltemergencycall.h> // for TPhCltEmergencyNumber
#include    "rphcltemergencycall.h" 
#include    "phcltclientserver.h" 
#include    "phcltutils.h" 

// CONSTANTS
_LIT( KPhCltDtmfCharp, "p" );
_LIT( KPhCltDtmfCharw, "w" );
_LIT( KPhCltSsCLIRSuppress, "*31#" );    // Outgoing CLI Release (release)
_LIT( KPhCltSsCLIRInvoke, "#31#" );   // Outgoing CLI Release (withhold)

// Emergency number (number part) max size
const TInt KPhCltMaxEmergencyNumberLength = 6; 
// Min length of CLIR
const TInt KPhCltMinSsStringLength = 4; 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RPhCltEmergencyCall::RPhCltEmergencyCall
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
RPhCltEmergencyCall::RPhCltEmergencyCall() 
    : RSubSessionBase()
    {    
    }

// -----------------------------------------------------------------------------
// RPhCltEmergencyCall::Open
// 
// Creates a subsession, as part of the given session.
// -----------------------------------------------------------------------------
//
TInt RPhCltEmergencyCall::Open( RPhCltServer& aServer )
    {
    __ASSERT_ALWAYS( aServer.Handle(), User::Panic( 
        KPhClientPanicCategory, 
        EPhCltClientSidePanicNullHandle ) );

    return CreateSubSession( 
        aServer, 
        EPhoneServerEmergencyNumberSubSessionOpen,
        TIpcArgs() );
    }

// -----------------------------------------------------------------------------
// RPhCltEmergencyCall::Close
// 
// Closes the subsession.
// -----------------------------------------------------------------------------
//
void RPhCltEmergencyCall::Close()
    {
    CloseSubSession( EPhoneServerEmergencyNumberSubSessionClose );
    }

// -----------------------------------------------------------------------------
// RPhCltEmergencyCall::IsEmergenyPhoneNumber
// 
// Checks if telephonenumber is an emergency number.
// -----------------------------------------------------------------------------
//
TInt RPhCltEmergencyCall::IsEmergencyPhoneNumber( 
    const TDesC& aNumber, 
    TBool& aIsEmergencyNumber )
    {
    aIsEmergencyNumber = EFalse;    
    TPhCltEmergencyNumber emNumber;
    
    // Maximum total number size check
    if ( aNumber.Length() > KPhCltEmergencyNumberSize )
        {
        return KErrArgument; 
        }
        
    emNumber.Copy( aNumber );        
    TBool writeEmergencyNumberBack( EFalse );
    
    // Actual check if the number provided is a valid emergency number
    TInt result = ValidateNumber( emNumber, aIsEmergencyNumber, 
                                writeEmergencyNumberBack );    

    return result;    
    }

// -----------------------------------------------------------------------------
// RPhCltEmergencyCall::IsEmergenyPhoneNumber
// 
// Checks if telephonenumber is an emergency number.
// -----------------------------------------------------------------------------
//
TInt RPhCltEmergencyCall::IsEmergencyPhoneNumber( 
    const TDesC& aNumber,
    TDes& aMatchedEmergencyNumber, 
    TBool& aIsEmergencyNumber )
    {
    aIsEmergencyNumber = EFalse;
    
    // Maximum total number size check
    if ( aNumber.Length() > KPhCltEmergencyNumberSize )
        {
        return KErrArgument; 
        }
        
    aMatchedEmergencyNumber.Copy( aNumber );   
    TBool writeEmergencyNumberBack( ETrue );

    // Actual check if the number provided is a valid emergency number    
    TInt result = ValidateNumber( aMatchedEmergencyNumber, aIsEmergencyNumber, 
                                writeEmergencyNumberBack );
                                
    // Reset the matched number, if no match was found
    if ( !aIsEmergencyNumber )
        {
        aMatchedEmergencyNumber = KNullDesC;
        }
                                
    return result;    
    }
    
// -----------------------------------------------------------------------------
// RPhCltEmergencyCall::FindEmergencyPhoneNumber
// 
// Checks if telephonenumber is an emergency number.
// Found emergency number returned
// -----------------------------------------------------------------------------
//
TInt RPhCltEmergencyCall::FindEmergencyPhoneNumber( 
    TDes& aNumber, 
    TBool& aIsEmergencyNumber )
    {   
    TPckg< TBool > pIsEmergencyNumber( aIsEmergencyNumber );
    
    if ( aNumber.Length() > KPhCltTelephoneNumberLength )
        {
        return KErrArgument;
        }
    
    TPckg< TPhCltTelephoneNumber > pNumber( 
        static_cast<TPhCltTelephoneNumber&>( aNumber ) );
            
    return SendReceive( 
        EPhoneServerEmergencyNumberSubSessionIsEmergencyNumber, 
        TIpcArgs( 
            &pNumber,
            &pIsEmergencyNumber,
            EPhCltEmergencyNumberQueryModeAdvanced ) );
    }
    
// -----------------------------------------------------------------------------
// RPhCltEmergencyCall::DialEmergencyCall
// 
// Sends a dial request to the server.
// -----------------------------------------------------------------------------
//
void RPhCltEmergencyCall::DialEmergencyCall( 
    TRequestStatus& aReqStatus, 
    TDesC8& aNumber )
    {
    return SendReceive( 
        EPhoneServerEmergencyNumberSubsessionEmergencyDial,
        TIpcArgs( &aNumber ),
        aReqStatus );
    }

// -----------------------------------------------------------------------------
// RPhCltEmergencyCall::CancelDialEmergencyCall
// 
// Cancels an emergency call.
// -----------------------------------------------------------------------------
//
void RPhCltEmergencyCall::CancelDialEmergencyCall()
    {
    SendReceive( 
        EPhoneServerEmergencyNumberSubsessionEmergencyDialCancel,
        TIpcArgs() );
    }

// -----------------------------------------------------------------------------
// Extracts telephone number out of string. Valid emergency number can
// have CLIR SS code and DTMF in it.
// -----------------------------------------------------------------------------
//
TBool RPhCltEmergencyCall::ExtractTelephoneNumber( TDes& aString )
    {    
    // Emergency number may contain CLIR ss code, so remove
    // possible CLIR code from the number.
    RemoveCLIR( aString );
    
    // Emergency number may contain DTMF. 
    RemoveDTMF( aString );
    
    if ( aString.Length() )
        {
        // First character in emergency can be +, but after that all characters must be digits.
        const TChar KPlusChar = '+';
        TChar firstCharacter = aString[0];
        if ( firstCharacter == KPlusChar )
            {
            aString = aString.Right( aString.Length()-1 );
            }
        
        // Number should be only digits.
        for ( TInt i = 0; i < aString.Length(); i++ )
            {
            TChar character = aString[i];
            if ( !character.IsDigit() )
                {
                return EFalse;
                }
            }
        }

    // This might be a valid emergency number. Final check is
    // done by EnPolicy.
    if ( aString.Length() )
        {
        return ETrue;
        }
    // If string length is zero after parsing, no reason to check it
    // with EnPolicy, jus return EFalse.
    else
        {
        return EFalse;
        }
    
    }
    
// -----------------------------------------------------------------------------
// Finds start of DTMF string. DTMF string starts with p or w character.
// -----------------------------------------------------------------------------
//
TInt RPhCltEmergencyCall::FindStartOfDtmfString( const TDesC& aString )
    {
    // If string contains 'p' or 'w' returns true.
    TInt indexp; //char 'p' index
    TInt indexw;// char 'w' index
    TPtrC ptrDtmfCharp( KPhCltDtmfCharp );
    TPtrC ptrDtmfCharw( KPhCltDtmfCharw );

    indexp = aString.Locate( ptrDtmfCharp[ 0 ] );
    indexw = aString.Locate( ptrDtmfCharw[ 0 ] ); 

    TInt dtmfIndex = KErrNotFound;
    if ( indexp != KErrNotFound || indexw != KErrNotFound )
        {
        // dtmf char found
        if ( indexw == KErrNotFound )
            {
            // char p index
            dtmfIndex = indexp;    
            }   
        else if ( indexp == KErrNotFound )
            {
            // char w index
            dtmfIndex = indexw;    
            }                 
        else
            {
            // Both chars p and w found, smaller index selected
            indexp < indexw ? dtmfIndex = indexp : dtmfIndex = indexw;    
            }         
        }      
    
    return dtmfIndex; 
    }

// -----------------------------------------------------------------------------
// Removes CLIR SS code
// -----------------------------------------------------------------------------
//    
void RPhCltEmergencyCall::RemoveCLIR( TDes& aString )
    {
    
    // Check for valid SS codes. Compare returns zero if strings match.
    if ( !aString.Left( KPhCltMinSsStringLength ).Compare( KPhCltSsCLIRInvoke ) ||
        !aString.Left( KPhCltMinSsStringLength ).Compare( KPhCltSsCLIRSuppress ) )        
        {
        // Extract string after the CLIR ss code.  
        aString = aString.Right( aString.Length() - KPhCltSsCLIRInvoke().Length() );
        }
    }

// -----------------------------------------------------------------------------
// Removes DTMF from string.
// -----------------------------------------------------------------------------
// 
void RPhCltEmergencyCall::RemoveDTMF( TDes& aString )
    {
    const TInt dtmfIndex  = FindStartOfDtmfString( aString );
    if ( dtmfIndex != KErrNotFound )
        {               
        aString = aString.Left( dtmfIndex );
        }
    }

// -----------------------------------------------------------------------------
// RPhCltEmergencyCall::ValidateNumber
//
// -----------------------------------------------------------------------------
//    
TInt RPhCltEmergencyCall::ValidateNumber( TDes& aString,
                                        TBool& aIsEmergencyNumber,
                                        const TBool aWriteEmergencyNumBack )
    {
    TPckg<TBool> pIsEmergencyNumber( aIsEmergencyNumber );
    
    // Check and remove additional characters from the phone number
    TBool isPotentialEmNumber = ExtractTelephoneNumber( aString ); 
    
    // Pass the number on to EnPolicy for further check
    if ( isPotentialEmNumber )
        {
        // Maximum length of digit part check
        if ( aString.Length() > KPhCltMaxEmergencyNumberLength )
            {
            return KErrArgument;
            }
                   
        // Use write-back mode if requested            
        if ( aWriteEmergencyNumBack )
            {
            TPckg< TPhCltTelephoneNumber > pNumber( 
                static_cast<TPhCltTelephoneNumber&>( aString ) );
            return SendReceive( 
                EPhoneServerEmergencyNumberSubSessionIsEmergencyNumber, 
                TIpcArgs( 
                    &pNumber,
                    &pIsEmergencyNumber,
                    EPhCltEmergencyNumberQueryModeNormalWriteback ) );                    
            }
        // Else use the normal mode
        else
            {
            TPckgC<TPhCltEmergencyNumber> pNumber( aString ); 
            return SendReceive( 
                EPhoneServerEmergencyNumberSubSessionIsEmergencyNumber, 
                TIpcArgs( 
                    &pNumber,
                    &pIsEmergencyNumber,
                    EPhCltEmergencyNumberQueryModeNormal ) );
            }                     
        }
    else
        {
        return KErrNone;
        }
    }
    
//  End of File

