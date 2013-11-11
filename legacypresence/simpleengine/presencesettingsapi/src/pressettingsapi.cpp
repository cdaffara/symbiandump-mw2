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
* Description:    Presence Settings API designed for creating and editing
*  setting sets for presence. Usually used by UI components.
*
*/




// Includes
#include <centralrepository.h>

#include "pressettingsapi.h"
#include "pressettingsapilogger.h"


// ---------------------------------------------------------------------------
// PresSettingsApi::GetAllSetsNamesLC
// ---------------------------------------------------------------------------
//
EXPORT_C CDesCArray* PresSettingsApi::GetAllSetsNamesLC( RArray<TInt>& aSetIds )
    {
    OPENG_DP(D_OPENG_LIT( "PresSettingsApi::GetAllSetsNamesLC"));
    
    CDesCArrayFlat* sets = new ( ELeave ) CDesCArrayFlat(KPresNormalListLength);
    CleanupStack::PushL( sets );
    
    CRepository* repository = CRepository::NewL( KCRUIDSimpleOTA );
    CleanupStack::PushL( repository );
    
    RArray<TUint32> idKeys;
    TInt error = repository->FindL( KSimpleSetId, KOnlyColumnInfo, idKeys );
    CleanupClosePushL( idKeys );
    
    TInt idCount = idKeys.Count();
    
    OPENG_DP(D_OPENG_LIT( " idCount = %d"), idCount);
    
    TBuf<KPresSetNameMaxLength> setName;   
    TInt setId(NULL); 
    
    TInt errID(KErrNone);
    TInt errName(KErrNone);
    TUint32 error2(KErrNone);
    TUint32 row;
    
    for( TInt i = 0;i < idCount;i++ )
        {
        // avoid the 0th and 256th rows
        if ((idKeys[i]<KUnitRow) || (idKeys[i]>KLastUseableRow))
            continue;
        
        row = idKeys[i] & KOnlyRowInfo;

        errName = repository->Get( row + KSimpleSetName, setName );
        errID = repository->Get( idKeys[i], setId );
        
        if( (errName != KErrNone) || (errID != KErrNone)) // if some error in getting
            {
            // try deleting the troubled row
            repository->Delete( row, KOnlyRowInfo, error2);            
            OPENG_DP(D_OPENG_LIT( "     Error in Get(name): i = %d"),i);
            OPENG_DP(D_OPENG_LIT( "     Error in Get(id)  : i = %d"),i);
            continue;
            }
        else
            {
            sets->AppendL(setName);
            aSetIds.Append(setId);
            }
        setName.Zero();           
        }

    CleanupStack::PopAndDestroy(&idKeys);   //idKeys
    CleanupStack::PopAndDestroy(repository); //repository
    return sets;
    
    }

// ---------------------------------------------------------------------------
// PresSettingsApi::CreateSetL
// ---------------------------------------------------------------------------
//
EXPORT_C TInt PresSettingsApi::CreateSetL(TPresSettingsSet aSet )
    {
    OPENG_DP(D_OPENG_LIT( "PresSettingsApi::CreateSetL"));
    CRepository* repository = CRepository::NewL( KCRUIDSimpleOTA );
    CleanupStack::PushL( repository );
    
    TUint32 row = GetFreshRawInHex(repository);
    OPENG_DP(D_OPENG_LIT( " row: %d"),row);
    TInt error(KErrNone);
    TUint32 error2(KErrNone);

    
    // Create the set id in different way
    TInt id = CreateUniqueIdL( repository, row );
    
    OPENG_DP(D_OPENG_LIT( " id: %d"),id);
    
    error = error + repository->Create( row+KSimpleSetName, aSet.iSetName);
    error = error + repository->Create( row+KSimpleSipProfile, aSet.iSipProfile );
    error = error + repository->Create( row+KSimpleXDMSet, aSet.iXDMSetting );
    error = error + repository->Create( row+KSimpleClientObjDataLimit, aSet.iObjectSize );
    error = error + repository->Create( row+KSimpleContentSvrAdd, aSet.iContSrvrAdd );
    error = error + repository->Create( row+KSimpleStatusofProps, aSet.iStatusOfProps );
    error = error + repository->Create( row+KSimpleSourceThrottlePublish, aSet.iPublicationInt );
    error = error + repository->Create( row+KSimpleMaxNbrPresenceSubs, aSet.iMaxSubscriptions );
    error = error + repository->Create( row+KSimpleMaxNbrSubsInList, aSet.iMaxContactsInList );
    error = error + repository->Create( row+KSimpleServiceUriTemplate, aSet.iDomainSyntax );
    error = error + repository->Create( row+KSimpleProviderID, aSet.iProviderID );

    if (error<0) // was there one of more errors then try deleting this currupt row
        {
        error = repository->Delete( row, KOnlyRowInfo, error2);
        User::Leave(KErrCorrupt);
        }

    CleanupStack::PopAndDestroy(repository);  //repository
    
    OPENG_DP(D_OPENG_LIT( " id: %d"),id);
    return id;
    }
    
// ---------------------------------------------------------------------------
// PresSettingsApi::UpdateSetL
// ---------------------------------------------------------------------------
//
EXPORT_C TInt PresSettingsApi::UpdateSetL(TPresSettingsSet aSet, TInt aSetID)
    {
    OPENG_DP(D_OPENG_LIT( "PresSettingsApi::UpdateSetL(%d)"),aSetID);
    
    CRepository* repository = CRepository::NewL( KCRUIDSimpleOTA );
    CleanupStack::PushL( repository );
    
    TInt ret(KErrNotFound);
    TInt error(KErrNone);
    TUint32 error2(KErrNone);
    
    TUint32 row =  PresSettingsApi::GetRawInHexL(repository,aSetID);
    OPENG_DP(D_OPENG_LIT( " row: %d, id: %d"),row, aSetID);
    if(row>0) // if a valid row is found
        {
        error = error + repository->Set( row+KSimpleSetName, aSet.iSetName);
        error = error + repository->Set( row+KSimpleSipProfile, aSet.iSipProfile );
        error = error + repository->Set( row+KSimpleXDMSet, aSet.iXDMSetting );
        error = error + repository->Set( row+KSimpleClientObjDataLimit, aSet.iObjectSize );
        error = error + repository->Set( row+KSimpleContentSvrAdd, aSet.iContSrvrAdd );
        error = error + repository->Set( row+KSimpleStatusofProps, aSet.iStatusOfProps );
        error = error + repository->Set( row+KSimpleSourceThrottlePublish, aSet.iPublicationInt );
        error = error + repository->Set( row+KSimpleMaxNbrPresenceSubs, aSet.iMaxSubscriptions );
        error = error + repository->Set( row+KSimpleMaxNbrSubsInList, aSet.iMaxContactsInList );
        error = error + repository->Set( row+KSimpleServiceUriTemplate, aSet.iDomainSyntax );
        error = error + repository->Set( row+KSimpleProviderID, aSet.iProviderID );

        
        if (error<0) // was there one of more errors then try deleting this currupt row
            {
            error = repository->Delete( row, KOnlyRowInfo, error2);
            ret = KErrCorrupt;
            }
        else
            ret = KErrNone;
        }
    
    CleanupStack::PopAndDestroy(repository);   //repository
    
    OPENG_DP(D_OPENG_LIT( " ret: %d"),ret);
    return ret;
    }

// ---------------------------------------------------------------------------
// PresSettingsApi::RemoveSetL
// ---------------------------------------------------------------------------
//
EXPORT_C TInt PresSettingsApi::RemoveSetL( TInt aSetID )
    {
    OPENG_DP(D_OPENG_LIT( "PresSettingsApi::RemoveSetL"));
    
    CRepository* repository = CRepository::NewL( KCRUIDSimpleOTA );
    CleanupStack::PushL( repository );
    
    TInt ret(KErrNotFound);
    TUint32 error(KErrNone);
    TUint32 error2(KErrNone);
    
    TUint32 row =  PresSettingsApi::GetRawInHexL(repository,aSetID);
    OPENG_DP(D_OPENG_LIT( " row: %d, id: %d"),row, aSetID);
    if(row>0) // if a valid row is found
        {
        //Delete all entries on the row
        TInt retError = repository->Delete( row, KOnlyRowInfo, error);
        if (retError<0) // was there one of more errors then try deleting this currupt row
            {
            error = repository->Delete( row, KOnlyRowInfo, error2);
            ret = KErrCorrupt;
            }
        else
            ret = KErrNone;
        }
    
    CleanupStack::PopAndDestroy(repository);   //repository
    
    OPENG_DP(D_OPENG_LIT( " ret: %d"),ret);
    return ret;
    }
    
// ---------------------------------------------------------------------------
// PresSettingsApi::UpdatePropertyL
// ---------------------------------------------------------------------------
//
EXPORT_C TInt PresSettingsApi::UpdatePropertyL( TInt aSetID, 
                                            TPresSettingsProperty aPropertyName,
                                            TInt aPropValueInt
                                            )
    {
    OPENG_DP(D_OPENG_LIT( "PresSettingsApi::UpdatePropertyL"));
    
    CRepository* repository = CRepository::NewL( KCRUIDSimpleOTA );
    CleanupStack::PushL( repository );
    
    TInt ret(KErrNotFound);
    TInt error(KErrNone);
    TUint32 error2(KErrNone);
    
    TUint32 row =  PresSettingsApi::GetRawInHexL(repository,aSetID);
    OPENG_DP(D_OPENG_LIT( " row: %d, id: %d"),row, aSetID);
    if(row>0) // if a valid row is found
        {
        if((IsStr(aPropertyName))||(aPropertyName==EPresPropSetID))
            {
            // because property and its value doesnt match
            ret = KErrArgument;
            }
        else
            {
            error = repository->Set( row+aPropertyName, aPropValueInt );
            if (error<0) // was there one of more errors then try deleting this currupt row
                {
                error = repository->Delete( row, KOnlyRowInfo, error2);
                ret = KErrCorrupt;
                }
            else
                ret = KErrNone;
            }
        }
    
    CleanupStack::PopAndDestroy(repository);   //repository
    
    OPENG_DP(D_OPENG_LIT( " ret: %d"),ret);
    return ret;
    }

// ---------------------------------------------------------------------------
// PresSettingsApi::UpdatePropertyL
// ---------------------------------------------------------------------------
//
EXPORT_C TInt PresSettingsApi::UpdatePropertyL( TInt aSetID, 
                                            TPresSettingsProperty aPropertyName,
                                            const TDesC& aPropValueDes
                                            )
    {
    OPENG_DP(D_OPENG_LIT( "PresSettingsApi::UpdatePropertyL"));
    
    CRepository* repository = CRepository::NewL( KCRUIDSimpleOTA );
    CleanupStack::PushL( repository );
    
    TInt ret(KErrNotFound);
    TInt error(KErrNone);
    TUint32 error2(KErrNone);
    
    TUint32 row =  PresSettingsApi::GetRawInHexL(repository,aSetID);
    OPENG_DP(D_OPENG_LIT( " row: %d, id: %d"),row, aSetID);
    if(row>0) // if a valid row is found
        {
        if((!IsStr(aPropertyName))||(aPropertyName==EPresPropSetID))
            {
            // because property and its value doesnt match
            ret = KErrArgument;
            }
        else
            {
            error = repository->Set( row+aPropertyName, aPropValueDes );
            if (error<0) // was there one of more errors then try deleting this currupt row
                {
                error = repository->Delete( row, KOnlyRowInfo, error2);
                ret = KErrCorrupt;
                }
            else
                ret = KErrNone;
            }
        }
    
    CleanupStack::PopAndDestroy(repository);   //repository
    OPENG_DP(D_OPENG_LIT( " ret: %d"),ret);
    return ret;
    }

// ---------------------------------------------------------------------------
// PresSettingsApi::SettingsSetL
// ---------------------------------------------------------------------------
//
EXPORT_C TInt PresSettingsApi::SettingsSetL(TInt aSetID, TPresSettingsSet& aSet)
    {
    OPENG_DP(D_OPENG_LIT( "PresSettingsApi::SettingsSetL"));
    
    CRepository* repository = CRepository::NewL( KCRUIDSimpleOTA );
    CleanupStack::PushL( repository );
    
    TInt ret(KErrNotFound);
    TInt error(KErrNone);
    TUint32 error2(KErrNone);
    
    TUint32 row =  PresSettingsApi::GetRawInHexL(repository,aSetID);
    OPENG_DP(D_OPENG_LIT( " row: %d, id: %d"),row, aSetID);
    if(row>0) // if a valid row is found
        {
        error = error + repository->Get( row+KSimpleSetName, aSet.iSetName);
        error = error + repository->Get( row+KSimpleSipProfile, aSet.iSipProfile );
        error = error + repository->Get( row+KSimpleXDMSet, aSet.iXDMSetting );
        error = error + repository->Get( row+KSimpleClientObjDataLimit, aSet.iObjectSize );
        error = error + repository->Get( row+KSimpleContentSvrAdd, aSet.iContSrvrAdd );
        error = error + repository->Get( row+KSimpleStatusofProps, aSet.iStatusOfProps );
        error = error + repository->Get( row+KSimpleSourceThrottlePublish, aSet.iPublicationInt );
        error = error + repository->Get( row+KSimpleMaxNbrPresenceSubs, aSet.iMaxSubscriptions );
        error = error + repository->Get( row+KSimpleMaxNbrSubsInList, aSet.iMaxContactsInList );
        error = error + repository->Get( row+KSimpleServiceUriTemplate, aSet.iDomainSyntax );
        error = error + repository->Get( row+KSimpleProviderID, aSet.iProviderID );
        
        if (error<0) // was there one of more errors then try deleting this currupt row
            {
            error = repository->Delete( row, KOnlyRowInfo, error2);
            ret = KErrCorrupt;
            }
        else
            ret = KErrNone;
        }
    
    CleanupStack::PopAndDestroy(repository);   //repository
    OPENG_DP(D_OPENG_LIT( " ret: %d"),ret);
    return ret;
    }
    
// ---------------------------------------------------------
// PresSettingsApi::DecodePropertyStatus
// ---------------------------------------------------------
//
EXPORT_C TPresPropStatus PresSettingsApi::DecodePropertyStatus(TInt aStatusOfProps,
                                              TPresSettingsProperty aPropertyName)
    {
    TInt shiftNumber = (aPropertyName-1) * 2;
    TInt maskNumber = 0x3 << shiftNumber; 
    return TPresPropStatus((aStatusOfProps & maskNumber)>>shiftNumber);  
    }
                                              
// ---------------------------------------------------------
// PresSettingsApi::EncodePropertyStatus
// ---------------------------------------------------------
//
EXPORT_C void PresSettingsApi::EncodePropertyStatus(TInt& aStatusOfProps,
                                              TPresPropStatus aPropertyStatus,
                                           TPresSettingsProperty aPropertyName)
    {
    TInt shiftNumber = (aPropertyName-1) * 2;
    TInt maskNumber = (0x3 << shiftNumber)^KFullValue;
    aStatusOfProps = (aStatusOfProps & maskNumber) | (aPropertyStatus << shiftNumber);
    }
    
// ---------------------------------------------------------
// PresSettingsApi::IsStr
// ---------------------------------------------------------
//
EXPORT_C TBool PresSettingsApi::IsStr( TPresSettingsProperty aProperty )
    {
    OPENG_DP(D_OPENG_LIT( "PresSettingsApi::IsStr"));
    TBool ret(EFalse);
    if((aProperty==KSimpleSetName)||(aProperty==KSimpleContentSvrAdd)||
       (aProperty==KSimpleProviderID)||(aProperty==KSimpleServiceUriTemplate))
        ret = ETrue;
    
    OPENG_DP(D_OPENG_LIT( " ret: %d"),ret);
    return ret;   
    }    
    
// ---------------------------------------------------------------------------
// PresSettingsApi::GetRawInHexL
// ---------------------------------------------------------------------------
//
TUint32 PresSettingsApi::GetRawInHexL( CRepository* aRepository, TInt aSetID )
    {
    OPENG_DP(D_OPENG_LIT( " PresSettingsApi::GetRawInHexL"));
    TInt error(KErrNone);
    TInt tempValue(0);
    TUint32 ret(0);
    
    // Find all SetId keys
    RArray<TUint32> idKeys;
    error = aRepository->FindL( KSimpleSetId, KOnlyColumnInfo, idKeys );
    CleanupClosePushL( idKeys );
    User::LeaveIfError(error);
    
    TInt count = idKeys.Count();
    OPENG_DP(D_OPENG_LIT( "     count: %d"),count);

    // start from 1st row and search the whole range of rows
    for (TInt i=0; i<count; i++)
        {
        // avoid the 0th and 256th rows
        if ((idKeys[i]<KUnitRow) || (idKeys[i]>KLastUseableRow))
            continue;
        error = aRepository->Get(idKeys[i], tempValue);
        User::LeaveIfError(error);
        
        if(tempValue == aSetID)
            {
            ret = idKeys[i] & KOnlyRowInfo; // remove column info and return
            break;
            }
        }
    
    CleanupStack::PopAndDestroy(); //idKeys
    OPENG_DP(D_OPENG_LIT( "     ret: %d"),ret);
    return ret;
    }
    
// ---------------------------------------------------------
// PresSettingsApi::CreateUniqueIdL
// ---------------------------------------------------------
//
TInt PresSettingsApi::CreateUniqueIdL( CRepository* aRepository, 
                                                        TUint32 aRowInHex )
    {
    OPENG_DP(D_OPENG_LIT( " PresSettingsApi::CreateUniqueIdL aRowInHex:%d"),aRowInHex);

    TInt error(KErrNone);
    
    // At row = KLastRow(0x10000), column = KSimpleSetId, we store the Set Ids which have been allocated
    // each time we +1 to get a fresh Set Id
    TInt setId(0);
    error = aRepository->Get(KLastRow+KSimpleSetId, setId);
    if (error==KErrNotFound) // first time writing
        {
        error = aRepository->Create(KLastRow+KSimpleSetId, setId);
        }
    //if we cant create nor get then leave
    User::LeaveIfError(error);
    
    //increase the id to have the next one
    setId++;
    
    // set this id back to allocated ids storage
    error = aRepository->Set(KLastRow+KSimpleSetId, setId);
    User::LeaveIfError(error);
    
    //put id to the provided row
    error = aRepository->Create( aRowInHex+KSimpleSetId, setId );
    User::LeaveIfError(error);
    
    OPENG_DP(D_OPENG_LIT( "     setId: %d"),setId);
    return setId;
    }

// ---------------------------------------------------------
// PresSettingsApi::GetFreshRawInHex
// ---------------------------------------------------------
//
TUint32 PresSettingsApi::GetFreshRawInHex( CRepository* aRepository )
    {
    OPENG_DP(D_OPENG_LIT( " PresSettingsApi::GetFreshRawInHex"));
    TInt error(KErrNone);
    TInt tempValue(NULL);
    TUint32 i(NULL);
    
    //search for ids on all rows
    for (i=KUnitRow+KSimpleSetId; i<KLastRow; i+=KUnitRow)
        {
        error = aRepository->Get(i, tempValue);
        
        // if this value is 0 or row doesnt exists
        if((tempValue==0)||(error==KErrNotFound))
            {
            OPENG_DP(D_OPENG_LIT( " found at, error:%d, i:%d"),error, i);
            error = KErrNone;
            break;
            }
        
        tempValue = NULL;    
        
        //break if there is any other error
        if(error<0)
            return error;
        }
    i = i & KOnlyRowInfo; // remove the column info from row
    OPENG_DP(D_OPENG_LIT( "     i: %d"),i);
    return i;
    }
    
//end of file
