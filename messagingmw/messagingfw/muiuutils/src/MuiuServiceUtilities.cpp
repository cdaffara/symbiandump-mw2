/*
* Copyright (c) 2002 - 2005 Nokia Corporation and/or its subsidiary(-ies).
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
*     Static class to help mtm service queries.
*
*/




// INCLUDE FILES
#include <e32base.h>
#include <bldvariant.hrh>           // For feature flags  
#include <msvuids.h>                // KUidMsvServiceEntry
#include <data_caging_path_literals.hrh> // KDC_RESOURCE_FILES_DIR
#include <msvapi.h>
#include <msvstd.hrh>
#include <msvids.h>
#include <mtclreg.h>                // CClientMtmRegistry
#include <mtclbase.h>               // CBaseMtm
#include <centralrepository.h>      // CRepository 
#include <UiklafInternalCRKeys.h>   // KCRUidUiklaf, KUikOODDiskCriticalThreshold

#include <MuiuServiceUtilities.h>

// CONSTANTS
const TInt KMuiuCharQuote = '\"';
const TInt KMuiuCharBackSlash = '\\';
const TInt KMuiuCharDot = '.';
const TInt KMuiuCharSpace = ' ';
const TInt KMuiuCharDel = 127;
const TInt KMuiuCharAt = '@';
const TInt KMuiuSpecialCharStrLen = 12;
_LIT( KRFC822Specials,"()<>@,;:\\\"[]");



// ============================ MEMBER FUNCTIONS ===============================


// ---------------------------------------------------------
// MsvUiServiceUtilitiesr::GetListOfAccountsL
//
// ---------------------------------------------------------
//
EXPORT_C CMsvEntrySelection* MsvUiServiceUtilities::GetListOfAccountsL( CMsvSession& aSession, 
                                                                        TBool aAlwaysListHidden )
    { // static
    return DoGetListOfAccountsL( aSession, KNullUid, aAlwaysListHidden );
    }


// ---------------------------------------------------------
// MsvUiServiceUtilitiesr::GetListOfAccountsWithMTML
//
// ---------------------------------------------------------
//
EXPORT_C CMsvEntrySelection* MsvUiServiceUtilities::GetListOfAccountsWithMTML( 
                                                    CMsvSession& aSession, 
                                                    TUid aMtm, TBool aAlwaysListHidden )
    { // static
    return DoGetListOfAccountsL( aSession, aMtm, aAlwaysListHidden );
    }


// ---------------------------------------------------------
// MsvUiServiceUtilitiesr::DoGetListOfAccountsL
//
// ---------------------------------------------------------
//
CMsvEntrySelection* MsvUiServiceUtilities::DoGetListOfAccountsL( CMsvSession& aSession, 
                                                                 TUid aMtm, 
                                                                 TBool aAlwaysListHidden )
    {
    CMsvEntrySelection* sel = new( ELeave ) CMsvEntrySelection();
    CleanupStack::PushL( sel );
    CMsvEntry* entry = aSession.GetEntryL( KMsvRootIndexEntryIdValue );
    CleanupStack::PushL( entry );
    TBool visible = EFalse;
    TInt err;
    
    TInt cnt = entry->Count();
    if ( cnt != 0 )
        {
        entry->SetSortTypeL( TMsvSelectionOrdering( KMsvGroupByType | KMsvGroupByStandardFolders, 
                                                    EMsvSortByDetailsReverse, ETrue ) );
        const TMsvEntry* tentry;
        for (TInt cc = entry->Count(); --cc >= 0; )
            {
            tentry=&(*entry)[cc];
            if ( aMtm == KNullUid || tentry->iMtm == aMtm )
                {
                if ( tentry->iType.iUid == KUidMsvServiceEntryValue && 
                     tentry->Id() != KMsvLocalServiceIndexEntryIdValue )
                    {
                    const TBool noRelatedId = tentry->iRelatedId == KMsvNullIndexEntryId || 
                                tentry->iRelatedId == tentry->Id();
                    if ( tentry->Visible() || noRelatedId || aAlwaysListHidden )
                        {
                        // Add this service if:
                        // it is visible, or,there is no associated related service, or,
                        // we have been asked to list all services.
                        sel->AppendL( tentry->Id() );
                        }
                    else
                        {
                        // This service is invisible and has a related service.
                        // If the related service is visible we will add it later.
                        // If the related service is also invisible, and has not yet been added,
                        // add this service.
                        TRAP( err, visible = entry->ChildDataL( tentry->iRelatedId ).Visible() )
                            if ( err == KErrNone && !visible && 
                                 sel->Find(tentry->iRelatedId) != KErrNone )
                                {
                                sel->AppendL( tentry->Id() );
                                }
                        }
                    }
                }
            }
        }
    CleanupStack::PopAndDestroy( entry );
    CleanupStack::Pop( sel );
    return sel;
    }

// ----------------------------------------------------
// MsvUiServiceUtilities::IsValidEmailAddressL
//
// ----------------------------------------------------
EXPORT_C TBool MsvUiServiceUtilities::IsValidEmailAddressL( const TDesC& aAddress )
    {
    TInt c;
    TInt length = aAddress.Length ();
    TBufC<KMuiuSpecialCharStrLen> rfc822Specials ( KRFC822Specials );
        
    // first we validate the name portion (name@domain)
    if ( length && aAddress[0] == KMuiuCharDot )
        {
        return EFalse;
        }
    for ( c = 0 ; c < length ; c++ )
        {
        if ( aAddress[c] == KMuiuCharQuote && ( c == 0 || 
        aAddress[c-1] == KMuiuCharDot || aAddress[c-1] == KMuiuCharQuote ) )
            {
            while ( ++c < length )
                {
                if ( aAddress[c] == KMuiuCharQuote )
                    {
                    if( (c + 1) == length)
                        {
                        return EFalse;
                        }
                    break;
                    }
                if ( aAddress[c] == KMuiuCharBackSlash && 
                    ( aAddress[++c] == KMuiuCharSpace) ) 
                    {
                    continue;
                    }  
                if ( aAddress[c] <= KMuiuCharSpace || 
                    aAddress[c] >= KMuiuCharDel ) 
                    {
                    return EFalse;
                    }
                }
            if ( c++ == length )
                {
                return EFalse;
                }
            if ( aAddress[c] == KMuiuCharAt )
                {
                break;
                }
            if ( aAddress[c] != KMuiuCharDot )
                {
                return EFalse;
                }
            continue;
            }
        if ( aAddress[c] == KMuiuCharAt )
            {
            break; 
            }
        if ( aAddress[c] <= KMuiuCharSpace || aAddress[c] >= KMuiuCharDel )
            {
            return EFalse;
            }    
        if ( rfc822Specials.Locate ( aAddress[c] ) != KErrNotFound )
            {
            return EFalse;
            }
        }
    if ( c == 0 || aAddress[c-1] == KMuiuCharDot )
        {
        return EFalse;
        }
    // next we validate the domain portion (name@domain)
    if ( c == length )
        {
        return EFalse;
        }
    else
        {
        c++; 
        return IsValidDomainL ( aAddress.Mid ( ( c ) , length-c ) );
        }
    }


// -----------------------------------------------------------------------------
// MsvUiServiceUtilities::IsValidDomainL
// -----------------------------------------------------------------------------
EXPORT_C TBool MsvUiServiceUtilities::IsValidDomainL ( const TDesC& aDomain )
    {
    TInt c = 0;  
    TInt length = aDomain.Length ();
    TBufC<KMuiuSpecialCharStrLen> rfc822Specials ( KRFC822Specials );
    
    if ( length == 0 )
        {
        return EFalse;   
        }
    
    do
        {    
        if ( aDomain[c] == KMuiuCharDot )
            {
            if ( c == 0 || aDomain[c-1] == KMuiuCharDot )
                {
                return EFalse;
                }
            }
        if ( aDomain[c] <= KMuiuCharSpace || aDomain[c] >= KMuiuCharDel )
            {
            return EFalse;
            }
        if ( rfc822Specials.Locate( aDomain[c] ) != KErrNotFound )
            {
            return EFalse;
            }
        } 
    while ( ++c < length );
  
    return ( aDomain[length-1] != '.' );
    }
    

// ---------------------------------------------------------
// MsvUiServiceUtilitiesr::DiskSpaceBelowCriticalLevelL
//
// ---------------------------------------------------------
//
EXPORT_C TBool MsvUiServiceUtilities::DiskSpaceBelowCriticalLevelL( CMsvSession& aSession,
                                                                    TInt aBytesToWrite )
    {
    RFs& fs = aSession.FileSession();
    TInt err = KErrNone;
    TVolumeInfo vinfo;
    TInt currentDrive = TInt( aSession.CurrentDriveL() );
    err = fs.Volume( vinfo, currentDrive );
    if ( err != KErrNone )
        {
        User::LeaveIfError( err );
        }
    
    TInt64 freespace = vinfo.iFree;
    TInt64 newFree = freespace - ( TInt64 )aBytesToWrite;
    TInt criticalLevel;
    CRepository* repository = CRepository::NewLC( KCRUidUiklaf );
    repository->Get( KUikOODDiskCriticalThreshold, criticalLevel );
    CleanupStack::PopAndDestroy( repository ); //repository
    return ( newFree <= (TInt64)criticalLevel );
    }



// ---------------------------------------------------------
// MsvUiServiceUtilities::DiskSpaceBelowCriticalLevelWithOverheadL
//
// ---------------------------------------------------------
//
EXPORT_C TBool MsvUiServiceUtilities::DiskSpaceBelowCriticalLevelWithOverheadL( 
                                                                 CMsvSession& aSession,
                                                                 TInt aBytesToWrite,
                                                                 TInt aPhoneMemoryOverheadBytes )
    {
    RFs& fs = aSession.FileSession();
    TInt err = KErrNone;
    TBool belowCritical = EFalse;
    TVolumeInfo vinfo;
    TInt criticalLevel;
    CRepository* repository = CRepository::NewLC( KCRUidUiklaf );
    repository->Get( KUikOODDiskCriticalThreshold, criticalLevel );
    CleanupStack::PopAndDestroy( repository ); //repository
    
    TInt currentDrive = TInt( aSession.CurrentDriveL() );
    // Sending eat always some memory from C drive
    if( currentDrive != EDriveC )
        {
        err = fs.Volume( vinfo, EDriveC );
        if ( err != KErrNone )
            {
            User::LeaveIfError( err );
            }
        belowCritical = ( vinfo.iFree < ( (TInt64)aPhoneMemoryOverheadBytes + 
                                          (TInt64)criticalLevel ) );
        }

    if ( !belowCritical )
        {
        err = fs.Volume( vinfo, currentDrive );
        if ( err != KErrNone )
            {
            User::LeaveIfError( err );
            }
        if( currentDrive == EDriveC )
            {
            belowCritical = ( vinfo.iFree < ( (TInt64)aBytesToWrite + 
                                              (TInt64)aPhoneMemoryOverheadBytes + 
                                              (TInt64)criticalLevel ) );
            }
        else
            {
            belowCritical = ( vinfo.iFree < ( (TInt64)aBytesToWrite + (TInt64)criticalLevel ) );
            }
        }
        
    return belowCritical;
    }
    
// End of File
