/*
 * Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
 * Description:   SUPL settings respository class
 *
 */




#include <utf.h>

#include "epos_comasupltrace.h"
#include "epos_comasupldmrepository.h"

#include "epos_csuplsettingparams.h"
#include "epos_tomasuplsettingsmap.h"

_LIT( KTraceSuplDmRepository, "epos_comasupldmrepository.cpp" );

const TInt KMaxBufferLength = 255 ;

//integer values used to set or reset a boolen flag ti indicate which parameters were set NULL
const TInt KNameBitMask = 1;
const TInt KPrefConRefBitMask = 2;
const TInt KProvIdBitMask = 4;
const TInt KSlpBitMask = 8;
const TInt KToConRefBitMask = 32;
const TInt KConRefBitMask = 64;
const TInt KAddrTypeBitMask = 128;

// -----------------------------------------------------------------------------
// COMASuplSettingsRepository::NewL
// Factory method that creates an instance of COMASuplSettings
// -----------------------------------------------------------------------------
//
COMASuplDMRepository* COMASuplDMRepository::NewL()
    {
    COMASuplDMRepository* self = new ( ELeave ) COMASuplDMRepository();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// -----------------------------------------------------------------------------    
// Destructor
//
// -----------------------------------------------------------------------------
//
COMASuplDMRepository::~COMASuplDMRepository()
    {
    if(iSettings)
        {
        delete iSettings;
        iSettings = NULL;
        }
    if(iTrace)
        {
        delete iTrace;
        iSettings = NULL;
        }
    if(iServerAddress)
        {
        delete iServerAddress;
        iServerAddress = NULL;
        }
    if(iIapName)
        {
        delete iIapName;
        iIapName = NULL;
        }
    if(iServerId)
        {
        delete iServerId;
        iServerId = NULL;
        }

    if(iIap1)
        {
        delete iIap1;
        iIap1 = NULL;
        }

    if(iIap2)
        {
        delete iIap2;
        iIap2 = NULL;
        }

    if(iSlp)
        {
        delete iSlp;
        iSlp = NULL;
        }

    if(iServerAddType)
        {
        delete iServerAddType;
        iServerAddType = NULL;
        }
    }


// -----------------------------------------------------------------------------
// Retrieves IAP name
//
// -----------------------------------------------------------------------------
//
void COMASuplDMRepository::GetIAPNameLC( TDesC8*& aIAP )
    {

    iTrace->Trace( _L("COMASuplDMRepository::GetIAPNameLC - Start"), 
            KTraceSuplDmRepository, __LINE__ );


    CServerParams* paramValues = CServerParams::NewL();
    CleanupStack::PushL(paramValues);
    TInt64 slpId;
    HBufC* serverAddress = HBufC::NewLC(KMaxBufferLength);
    HBufC* iapName = HBufC::NewLC(KMaxBufferLength);
    TBool serverEnabled;
    TBool simChangeRemove;
    TBool usageInHomeNw;
    TBool editable;

    TInt err = iSettings->GetDefaultServer( paramValues );


    if ( KErrNone != err)
        { 
        TBuf<64> errcode;
        errcode.Append(_L("GetDefaultServer failed with error code :"));
        errcode.AppendNum(err);
        iTrace->Trace(errcode,KTraceSuplDmRepository, __LINE__ );
        User::Leave( err );
        }    
    paramValues->Get(slpId,serverAddress->Des(),iapName->Des(),serverEnabled,simChangeRemove,usageInHomeNw,editable);

    HBufC8* IAPName = CnvUtfConverter::ConvertFromUnicodeToUtf8L( *iapName );


    aIAP = IAPName;    
    iTrace->Trace( _L("COMASuplDMRepository::GetIAPNameLC - End"), 
            KTraceSuplDmRepository, __LINE__ );
    CleanupStack::PopAndDestroy(3);

    CleanupStack::PushL( IAPName );
    }


// -----------------------------------------------------------------------------
// Retrieves HSLP address
//
// -----------------------------------------------------------------------------
//
void COMASuplDMRepository::GetHSLPAddressLC( TDesC8*& aHSLP )
    {

    iTrace->Trace( _L("COMASuplDMRepository::GetHSLPAddressLC - Start"), 
            KTraceSuplDmRepository, __LINE__ );


    CServerParams* paramValues = CServerParams::NewL();
    CleanupStack::PushL(paramValues);
    TInt64 slpId;
    HBufC* serverAddress = HBufC::NewLC(KMaxBufferLength);
    HBufC* iapName = HBufC::NewLC(KMaxBufferLength);
    TBool serverEnabled;
    TBool simChangeRemove;
    TBool usageInHomeNw;
    TBool editable;

    TInt err = iSettings->GetDefaultServer( paramValues );

    if ( KErrNone != err && KErrNotFound != err )
        {
        TBuf<64> errcode;
        errcode.Append(_L("GetDefaultServer failed with error code :"));
        errcode.AppendNum(err);
        iTrace->Trace(errcode,KTraceSuplDmRepository, __LINE__ );
        User::Leave( err );
        }

    paramValues->Get(slpId,serverAddress->Des(),iapName->Des(),serverEnabled,simChangeRemove,usageInHomeNw,editable);

    HBufC8* HSLPAddr = CnvUtfConverter::ConvertFromUnicodeToUtf8L( *serverAddress );


    aHSLP = HSLPAddr;
    iTrace->Trace( _L("COMASuplDMRepository::GetHSLPAddressLC - End"), 
            KTraceSuplDmRepository, __LINE__ );    
    CleanupStack::PopAndDestroy(3);



    CleanupStack::PushL( HSLPAddr );
    }


// -----------------------------------------------------------------------------
// Starts an atomic transaction
//
// -----------------------------------------------------------------------------
//
void COMASuplDMRepository::StartTransactionL()
    {
#if 0
    User::LeaveIfError( iSettings->LockSettingsL() );    
#endif
    }


// -----------------------------------------------------------------------------
// Commits changes in an atomic transaction
//
// -----------------------------------------------------------------------------
//
void COMASuplDMRepository::CommitTransactionL()
    {
#if 0
    User::LeaveIfError( iSettings->UnlockSettings() );    
#endif
    }


// -----------------------------------------------------------------------------
// Reverts changes made in an atomic transaction
//
// -----------------------------------------------------------------------------
//
void COMASuplDMRepository::RollbackTransactionL()
    {
#if 0
    User::LeaveIfError( iSettings->UnlockSettings( EFalse ) );
#endif
    }


// -----------------------------------------------------------------------------
// Saves SUPL Settings information
//
// -----------------------------------------------------------------------------
//
void COMASuplDMRepository::SaveL( const TDesC& aValue,TOMASuplSettingsMap::TSuplSettingType aType,TBool aIsAtomic )
    {
    if( !aIsAtomic )
        {
        StartTransactionL();    
        }
    TRAPD( err, SaveSettingsL( aValue,aType) );
    if( KErrNone != err )
        {
        if ( !aIsAtomic )
            {
            RollbackTransactionL();    
            }
        User::Leave( err );
        }
    if ( !aIsAtomic )
        {
        CommitTransactionL();
        }
    }


// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
COMASuplDMRepository::COMASuplDMRepository()
    {
    }


// -----------------------------------------------------------------------------    
// Symbian 2nd phase constructor
//
// -----------------------------------------------------------------------------
//
void COMASuplDMRepository::ConstructL()
    {
    iSettings = CSuplSettingsInternal::NewL();
    iTrace = COMASuplTrace::NewL();
    iState = ENotInitialized;
    iSlpId = -1;
    iServerAddress = HBufC::NewL(KMaxBufferLength);
    iIapName = HBufC::NewL(KMaxBufferLength);
    iServerId = HBufC::NewL(KMaxBufferLength);
    iIap1 = HBufC::NewL(KMaxBufferLength);
    iIap2 = HBufC::NewL(KMaxBufferLength);
    iSlp = HBufC::NewL(KMaxBufferLength);
    iServerAddType = HBufC::NewL(KMaxBufferLength);
    iNullMask = 0;
    }


// -----------------------------------------------------------------------------
// Saves SUPL Settings information
//
// -----------------------------------------------------------------------------
//
void COMASuplDMRepository::SaveSettingsL( const TDesC& aValue,TOMASuplSettingsMap::TSuplSettingType aType)
    {

    iTrace->Trace( _L( "COMASuplSettingsRepository::SaveL() Start" ), 
            KTraceSuplDmRepository, __LINE__ );

    if( iState != EInitialized )
        {
        iSettings->Initialize( iStatus );
        User::WaitForRequest( iStatus );

        if ( KErrNone != iStatus.Int() )
            {
            iTrace->Trace( _L( "Error Initializing SUPL Settings" ), 
                    KTraceSuplDmRepository, __LINE__ );

            User::Leave( KErrNotReady );
            }
        iState = EInitialized;
        }

    switch(aType)
        {
        case TOMASuplSettingsMap::EName :
            {
            if ( aValue.Length() )
                {
                iTrace->Trace( _L( "Received Server Address of no zero length" ), 
                        KTraceSuplDmRepository, __LINE__ );
                *iServerAddress = aValue;
                CServerParams* paramValues = CServerParams::NewL();
                CleanupStack::PushL(paramValues);
                TInt err =  paramValues->Set(*iServerAddress,*iIapName);
                paramValues->SetServerConfigurationType(EFalse);
                err = iSettings->AddNewServer(paramValues,iSlpId);
                if(err == KErrAlreadyExists)
                    {
                    TInt getErr = iSettings->GetSlpInfoAddress(*iServerAddress,paramValues);
                    if(getErr == KErrNone)
                        {
                        HBufC* servername = HBufC::NewLC(KMaxBufferLength);
                        HBufC* iapName = HBufC::NewL(KMaxBufferLength);
                        TBool serverEnabled;
                        TBool simChangeRemove;
                        TBool usageInHomeNw;
                        TBool editable;

                        paramValues->Get(iSlpId,servername->Des(),iapName->Des(),serverEnabled,simChangeRemove,usageInHomeNw,editable);
                        delete iapName;
                        CleanupStack::PopAndDestroy();//servername
                        }
                    }

                if ((err == KErrNone || err == KErrAlreadyExists) && iSlpId!= -1)
                    iSettings->SetTopPriority(iSlpId);


                if (err!=KErrNone && err != KErrAlreadyExists)
                    {
                    TBuf<64> errcode;
                    errcode.Append(_L( "Error updating SUPL settings" ));
                    errcode.AppendNum(err);
                    iTrace->Trace( errcode,KTraceSuplDmRepository, __LINE__ );
                    User::Leave( err );
                    }
                CleanupStack::PopAndDestroy(1); //params
                }
            else
                {
                iTrace->Trace( _L( "Server Name is NULL" ), 
                        KTraceSuplDmRepository, __LINE__ );
                iNullMask |= KNameBitMask;
                }
            }
            break;
        case TOMASuplSettingsMap::EPrefConRef :
            {
            if( aValue.Length() )
                {
                iTrace->Trace( _L( "Received EPrefConRef of non zero length" ), 
                        KTraceSuplDmRepository, __LINE__ );
                }
            else
                {
                iTrace->Trace( _L( "EPrefConRef Name is NULL" ), 
                        KTraceSuplDmRepository, __LINE__ );
                iNullMask |= KPrefConRefBitMask;
                }
            *iIapName = aValue; 
            }
            break;
        case TOMASuplSettingsMap::EProviderId :
            {
            if( aValue.Length() )
                {
                iTrace->Trace( _L( "Received ProviderId of non zero length" ), 
                        KTraceSuplDmRepository, __LINE__ );
               
                }
            else
                {
                iTrace->Trace( _L( "Provider Id is NULL" ), 
                        KTraceSuplDmRepository, __LINE__ );
                iNullMask |= KProvIdBitMask;
                }
            *iServerId = aValue; 
            }
            break;
        case TOMASuplSettingsMap::ESlp: 
            {
            if( aValue.Length() )
                {
                iTrace->Trace( _L( "Received name of non zero length" ), 
                        KTraceSuplDmRepository, __LINE__ );
                }
            else
                {
                iTrace->Trace( _L( "name is NULL" ), 
                        KTraceSuplDmRepository, __LINE__ );
                iNullMask |= KSlpBitMask;
                }
            *iSlp = aValue; 
            }
            break;
        case TOMASuplSettingsMap::EToConRef :
            {
            if( aValue.Length() )
                {
                iTrace->Trace( _L( "Received IAP1 of non zero length" ), 
                        KTraceSuplDmRepository, __LINE__ );
                }
            else
                {
                iTrace->Trace( _L( "IAP1 is NULL" ), 
                        KTraceSuplDmRepository, __LINE__ );
                iNullMask |= KToConRefBitMask;
                }
            *iIap1 = aValue; 
            }
            break;
        case TOMASuplSettingsMap::EConRef :
            {
            if( aValue.Length() )
                {
                iTrace->Trace( _L( "Received name of non zero length" ), 
                        KTraceSuplDmRepository, __LINE__ );
                }
            else
                {
                iTrace->Trace( _L( "IAP2 is NULL" ), 
                        KTraceSuplDmRepository, __LINE__ );
                iNullMask |= KConRefBitMask;
                }
            *iIap2 = aValue; 
            }
            break;
        case TOMASuplSettingsMap::EAddrType :
            {
            if( aValue.Length() )
                {
                iTrace->Trace( _L( "Received Address type of non zero length" ), 
                        KTraceSuplDmRepository, __LINE__ );
                }
            else
                {
                iTrace->Trace( _L( "Address type is NULL" ), 
                        KTraceSuplDmRepository, __LINE__ );
                iNullMask |= KAddrTypeBitMask;
                }
            *iServerAddType = aValue; 
            }
            break;
        }




    if(iSlpId != -1) // a server name was received and added now add buffered values,if any to database
        {
        if(iIapName->Length() != 0 || iNullMask & KPrefConRefBitMask)// if IAP Name was recieved add it to the settings DB
            {
            iSettings->SetIapName(iSlpId,*iIapName);
            iSettings->SetTopPriority(iSlpId);
            }


        if(iServerId->Length() != 0 && iIap1->Length() != 0 && iIap2->Length() != 0 && 
                iSlp->Length() != 0 && iServerAddType->Length() != 0) // if all extended parameters exist add to settings DB directly
            {
            CServerExtensionParams* params = CServerExtensionParams::NewL();
            TInt err=params->Set( iServerId->Des(),iIap1->Des(),iIap2->Des(),iSlp->Des(),iServerAddType->Des());  

            iSettings->SetServerExtensionParams(iSlpId,params);
            iSettings->SetTopPriority(iSlpId);
            delete params;

            User::LeaveIfError(err);

            }
        // if 1 or more extended parameters exist but not all of them add to settings DB after retrieving existing params
        else if(iServerId->Length() != 0 || iIap1->Length() != 0 || iIap2->Length() != 0 || 
                iSlp->Length() != 0 || iServerAddType->Length() != 0 || iNullMask) 
            {
            CServerExtensionParams* params = CServerExtensionParams::NewLC();

            iSettings->GetServerExtensionParams(iSlpId,params); //get the already set parameters

            HBufC* serverid=HBufC::NewLC(KMaxBufferLength);
            HBufC* iap1=HBufC::NewLC(KMaxBufferLength);
            HBufC* iap2=HBufC::NewLC(KMaxBufferLength);
            HBufC* serveradd=HBufC::NewLC(KMaxBufferLength);
            HBufC* serveraddtype=HBufC::NewLC(KMaxBufferLength);
            params->Get(serverid->Des(),iap1->Des(),iap2->Des(),serveradd->Des(),serveraddtype->Des());

            if(iServerId->Length() == 0 && !(iNullMask & KProvIdBitMask))
                {
                *iServerId = *serverid;                          
                }
            if(iIap1->Length() == 0 && !(iNullMask & KToConRefBitMask))
                {
                *iIap1 = *iap1;
                }
            if(iIap2->Length() == 0 && !(iNullMask & KConRefBitMask))
                {
                *iIap2 = *iap2;
                }
            if(iSlp->Length() == 0 && !(iNullMask & KSlpBitMask))
                {
                *iSlp = *serveradd;
                }
            if(iServerAddType->Length() == 0 && !(iNullMask & KAddrTypeBitMask))
                {
                *iServerAddType = *serveraddtype;
                }
            CleanupStack::PopAndDestroy(5);//serverid,iap1,iap2,serveradd,serveraddtype

            TInt err=params->Set( iServerId->Des(),iIap1->Des(),iIap2->Des(),iSlp->Des(),iServerAddType->Des());  

            iSettings->SetServerExtensionParams(iSlpId,params);
            iSettings->SetTopPriority(iSlpId);
            CleanupStack::PopAndDestroy(1);//params
            User::LeaveIfError(err);

            }

        }



    }

// -----------------------------------------------------------------------------
// COMASuplDMRepository::GetExtnParamsLC(TDesC8*& aValue,TSuplDMExtnParamType aType)
// Retrieves SUPL Settings Extended Params information
//
// -----------------------------------------------------------------------------
//
void COMASuplDMRepository::GetExtnParamsLC(TDesC8*& aValue,TOMASuplSettingsMap::TSuplSettingType aType)
    {
    TInt64 defaultSlpId = -1;
    TInt err = iSettings->GetDefaultSlpId(defaultSlpId);
    User::LeaveIfError(err);
    CServerExtensionParams* servExtnParams = CServerExtensionParams::NewLC();
    err = iSettings->GetServerExtensionParams(defaultSlpId,servExtnParams);
    User::LeaveIfError(err);

    HBufC* serverid=HBufC::NewLC(KMaxBufferLength);
    HBufC* iap1=HBufC::NewLC(KMaxBufferLength);
    HBufC* iap2=HBufC::NewLC(KMaxBufferLength);
    HBufC* serveradd=HBufC::NewLC(KMaxBufferLength);
    HBufC* serveraddtype=HBufC::NewLC(KMaxBufferLength);
    servExtnParams->Get(serverid->Des(),iap1->Des(),iap2->Des(),serveradd->Des(),serveraddtype->Des());
    switch(aType)
        {
        case TOMASuplSettingsMap::EProviderId:
            {
            HBufC8* provId = CnvUtfConverter::ConvertFromUnicodeToUtf8L( *serverid );
            aValue = provId;

            CleanupStack::PopAndDestroy(6);//serverid,iap1,iap2,serveradd,serveraddtype,servExtnParams
            CleanupStack::PushL(provId);
            }
            break;
        case TOMASuplSettingsMap::ESlp:
            {
            HBufC8* slpAdd = CnvUtfConverter::ConvertFromUnicodeToUtf8L( *serveradd );
            aValue = slpAdd;

            CleanupStack::PopAndDestroy(6);//serverid,iap1,iap2,serveradd,serveraddtype,servExtnParams
            CleanupStack::PushL(slpAdd);
            }
            break;
        case TOMASuplSettingsMap::EToConRef:
            {
            HBufC8* toConRef = CnvUtfConverter::ConvertFromUnicodeToUtf8L( *iap1 );
            aValue = toConRef;

            CleanupStack::PopAndDestroy(6);//serverid,iap1,iap2,serveradd,serveraddtype,servExtnParams
            CleanupStack::PushL(toConRef);
            }
            break;
        case TOMASuplSettingsMap::EConRef:
            {
            HBufC8* conRef = CnvUtfConverter::ConvertFromUnicodeToUtf8L( *iap2 );
            aValue = conRef;

            CleanupStack::PopAndDestroy(6);//serverid,iap1,iap2,serveradd,serveraddtype,servExtnParams
            CleanupStack::PushL(conRef);
            }
            break;
        case TOMASuplSettingsMap::EAddrType:
            {
            HBufC8* addrType = CnvUtfConverter::ConvertFromUnicodeToUtf8L( *serveraddtype );
            aValue = addrType;

            CleanupStack::PopAndDestroy(6);//serverid,iap1,iap2,serveradd,serveraddtype,servExtnParams
            CleanupStack::PushL(addrType);
            }
            break;

        }
    }

