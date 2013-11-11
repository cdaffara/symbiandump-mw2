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
* Description:  Provides DOS emergency number policy API implementation.
*
*/


// INCLUDE FILES
#include "cdosemergencynumberpolicy.h" 
#include <mmtsy_names.h>
#include <telservicesvariant.hrh>

#include <centralrepository.h>
#include <telservicesinternalcrkeys.h>
#include <defaultemergencynumberscrkeys.h>

// CONSTANT
const TInt KDosEmergencyNumberMaxLength = 6;
//Prefix for Sip.
_LIT( KSipPrefix, "sip:" );
//Prefix for Tel.
_LIT( KTelPrefix, "tel:" );
//Prefix for Sos.
_LIT( KSosPrefix, "sos:" );
// @ char
_LIT( KAt, "@" );
// Prefix length
const TInt KPrefixLength = 4;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CDosEmergencyNumberPolicy::~CDosEmergencyNumberPolicy()
// -----------------------------------------------------------------------------
//
CDosEmergencyNumberPolicy::~CDosEmergencyNumberPolicy()
    {
    if ( iListener )
        {
        iListener->StopListening();
        delete iListener;
        }
    delete iCenRep;

    iProductSpecificList.Zero();

    iCustomPhone.Close();
    iMobilePhone.Close();
    }

// -----------------------------------------------------------------------------
// CDosEmergencyNumberPolicy::ConstructL()
// -----------------------------------------------------------------------------
//
void CDosEmergencyNumberPolicy::ConstructL( RTelServer& aETelServer )
    {
    iETelServer = aETelServer;

    User::LeaveIfError( iMobilePhone.Open( iETelServer , KMmTsyPhoneName ) );
    User::LeaveIfError( iCustomPhone.Open( iMobilePhone) );

    TInt variantReadOnlyValues = 0;
    TInt err = KErrNone;

    CRepository* variationFlags = CRepository::NewL( KCRUidTelSrvVariation );
    err = variationFlags->Get( KTelSrvVariationFlags , variantReadOnlyValues );

    if ( err != KErrNotFound )
        {
        User::LeaveIfError( err );
        }  

    if ( KTelSrvLVFlagAdvancedENCheck & variantReadOnlyValues )
        {
        iAdvancedCheck = ETrue;
        }
    else
        {
        iAdvancedCheck = EFalse;
        }
    
    delete variationFlags;

    iCenRep = CRepository::NewL( KCRUidDefaultEmergencyNumbers );    
    iListener = CCenRepNotifyHandler::NewL( 
        *this, 
        *iCenRep, 
        CCenRepNotifyHandler::EStringKey, 
        KDefaultEmergencyNumbersList );
    
    iListener->StartListeningL();
    
    ReadProductSpecificNumbers();
    }


// -----------------------------------------------------------------------------
// CDosEmergencyNumberPolicy::IsEmergencyNumberL 
// 
// -----------------------------------------------------------------------------
//
TInt CDosEmergencyNumberPolicy::IsEmergencyNumber ( 
                                    TPhCltTelephoneNumber& aTelNumber, 
                                    TPhCltEmergencyNumberQueryMode aMode,
                                    TBool& aIsEmergencyNumber )
    {   
    TPhCltTelephoneNumber checkedTelNumber(aTelNumber);
    ParseNumber( checkedTelNumber );
    aIsEmergencyNumber = EFalse;
    RMmCustomAPI::TEmerNumberCheckMode  checkedNumberMode;
    if ( checkedNumberMode.iNumber.MaxLength() >= checkedTelNumber.Length() ) // aTelNumber -> checkedTelNumber
        {
        checkedNumberMode.iNumber = checkedTelNumber; // aTelNumber -> checkedTelNumber
        }
    else
        {
        // too long, cannot be checked
        return KErrArgument;
        }

    // set check type
    switch ( aMode )
        {
        case EPhCltEmergencyNumberQueryModeAdvanced:
            {
            // check if it is a product specific number.
            if ( IsInListByAdvancedMode( checkedTelNumber ) ) // aTelNumber -> checkedTelNumber
                {
                aTelNumber = checkedTelNumber; // aTelNumber -> checkedTelNumber
                aIsEmergencyNumber = ETrue;
                return KErrNone;
                }
            
            // if TSY doesn't support advanced mode, then check by S60 advanced mode
            // based on TSY normal mode check.
            if ( ! iAdvancedCheck )
                {
                TInt length = 
                    Min( checkedTelNumber.Length(), KDosEmergencyNumberMaxLength ); // aTelNumber -> checkedTelNumber
                checkedNumberMode.iCheckMode = RMmCustomAPI::EEmerNumberCheckNormal;

                TInt result = KErrArgument;
                while ( ( length > 0 ) && ! aIsEmergencyNumber )
                    {
                    checkedNumberMode.iNumber = checkedTelNumber.Right( length ); // aTelNumber -> checkedTelNumber
                    result = iCustomPhone.CheckEmergencyNumber( 
                            checkedNumberMode, aIsEmergencyNumber );

                    if ( result == KErrNone && aIsEmergencyNumber )
                        {
                        aTelNumber = checkedNumberMode.iNumber;
                        }

                    length--;
                    }

                return result;
                }
            else
                {
                checkedNumberMode.iCheckMode = 
                    RMmCustomAPI::EEmerNumberCheckAdvanced;
                }
            }
            break;
        case EPhCltEmergencyNumberQueryModeNormal:
        case EPhCltEmergencyNumberQueryModeNormalWriteback:
            // check if it is a product specific number.
            if ( IsInListByNormalMode( checkedTelNumber ) ) // aTelNumber -> checkedTelNumber
                {
                aTelNumber = checkedTelNumber; // aTelNumber -> checkedTelNumber
                aIsEmergencyNumber = ETrue;
                return KErrNone;
                }

            // flow thru
        default:
            checkedNumberMode.iCheckMode = 
                RMmCustomAPI::EEmerNumberCheckNormal;
            break;
        }

    // do checking   
    TInt err = iCustomPhone.CheckEmergencyNumber ( 
      checkedNumberMode, aIsEmergencyNumber );

    aTelNumber = checkedNumberMode.iNumber;
   
    return err;
    }

// -----------------------------------------------------------------------------
// CDosEmergencyNumberPolicy::ReadProductSpecificNumbers
// 
// Reads product specific values from central repository
// -----------------------------------------------------------------------------
//
void CDosEmergencyNumberPolicy::ReadProductSpecificNumbers()
    {
    // This is done only once in bootup phase.
    iProductSpecificList.Zero();

    TInt err = KErrNone;


        err = iCenRep->Get( KDefaultEmergencyNumbersList, 
                                iProductSpecificList );

    if ( err != KErrNone )
        {
        // just to make sure, if central repository has modified it,
        // even when there was an error.
        iProductSpecificList.Zero();
        }
    }

// -----------------------------------------------------------------------------
// CDosEmergencyNumberPolicy::IsInListByAdvancedMode
// 
// Checks if the number is in list by advanced mode check.
//
// -----------------------------------------------------------------------------
//
TBool CDosEmergencyNumberPolicy::IsInListByAdvancedMode ( TDes& aNumber )
    {    
    TLex list( iProductSpecificList );
    TLexMark mark;
    TPtrC token_candidate( KNullDesC() ) ; 
    
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
// CDosEmergencyNumberPolicy::IsInListByNormalMode
// 
// Checks if the number is in list by normal mode check.
//
// -----------------------------------------------------------------------------
//
TBool CDosEmergencyNumberPolicy::IsInListByNormalMode( const TDesC& aNumber )
    {
    TInt length = aNumber.Length();
    TInt place = iProductSpecificList.Find( aNumber ); // first number of aNumber

    // _DDPRINT( 4, "ENPolicy.SDM.IsInListNormal.ok.", place );    // debug print

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
        if ( iProductSpecificList[ place -1 ] != KEnPolicySDNumberSeparator )
            {
            // this was just a substring numbers before that
            return EFalse;
            }
        }

    // beginning was ok, so we can continue
    
    // now checking the end
    // there must be atleast one empty space in the end that
    // we do the checking of number end.
    if ( endOfMatch < ( iProductSpecificList.Length() - 2 ) )
        {
        if ( iProductSpecificList[ endOfMatch ] != KEnPolicySDNumberSeparator )
            {
            return EFalse;
            }
        }
        
    //match is wrong, only part of ememergency number is matched
    int newmatchlen = 0;
    for (int i = place; i < iProductSpecificList.Length(); i++)
        {
        if(iProductSpecificList [i] != KEnPolicySDNumberSeparator )
            newmatchlen++;
        else
            break;  
        }
        
        if (length != newmatchlen)
            {
            return EFalse;
            }
        

    // if we get so far, its ok.
    return ETrue;
    }
    
// -----------------------------------------------------------------------------
// CDosEmergencyNumberPolicy::ParserNumber
// 
// Removes sip prefixis beging of the number and end of the number after @ char.
// @ character is also removed.
//
// -----------------------------------------------------------------------------
//
void CDosEmergencyNumberPolicy::ParseNumber( TPhCltTelephoneNumber& aTelNumber )
    {
    // Does number contains prefix. If yes then remove it
    
    if ( 0 == aTelNumber.FindC( KSipPrefix ) ||
         0 == aTelNumber.FindC( KTelPrefix ) ||
           0 == aTelNumber.FindC( KSosPrefix ) )
        {
        aTelNumber.Delete( 0, KPrefixLength );
          }
        // Does Number contains domain part. If yes remove it at beging of @ character.
      TInt atIndex = aTelNumber.Find( KAt );
      TInt telNumberLength = aTelNumber.Length();
        if ( 0 < atIndex && 
             atIndex < telNumberLength )
        {
            aTelNumber.Delete( atIndex, telNumberLength - atIndex );
            }
    }

// -----------------------------------------------------------------------------
// CDosEmergencyNumberPolicy::HandleNotifyString
// 
// Inherited from MCenRepNotifyHandlerCallback.
// -----------------------------------------------------------------------------
//
void CDosEmergencyNumberPolicy::HandleNotifyString( 
    TUint32 aId, const TDesC16& aNewValue )
    {
    if( aId == KDefaultEmergencyNumbersList )
        {
        iProductSpecificList.Zero();
        iProductSpecificList.Copy( aNewValue );
        }

    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// NewEnPolicy()
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CEmergencyNumberPolicy* NewEnPolicyL()
    {
    return new (ELeave) CDosEmergencyNumberPolicy;
    }

// End of File
