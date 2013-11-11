/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   SUPL Settings class
*
*/



#include <f32file.h>
#include <badesca.h>    // CDesCArrayFlat
#include <bautils.h>    // file helpers
#include <eikenv.h>
#include <e32cmn.h>
#include <centralrepository.h> //for central repository

//#define PRINT_MESSAGE           //for logging 
#define ENABLE_DEBUG

#include "epos_csuplsettings.h"
#include "epos_csettingsdatabasehandler.h"
#include "epos_csuplsettingparams.h"
#include "epos_csuplsettingextensionparams.h"
#include "epos_csuplsettingsconstants.h"
#include "epos_csuplsettingsinternalconstants.h"
#include "epos_csuplserverprivatecrkeys.h" //for keys of default setting values stored in cen rep
#include "epos_csuplsettingsinternalcrkeys.h"     //for keys of setting change events used for notification

// Implementation specific constants

//some string length contants
const int KDateStringLen = 64;         
const int KGenericStringLen = 255;
const int KShortStringLen = 32;

const TInt KMaxLenSLPAddress = 64;
const TInt KMaxLenIAPName = 16;

//integer values used to set or reset a boolen flag corresponding to slp properties
const TInt KEmergencySupport = 1;
const TInt KTlsEnabled = 2;
const TInt KPskTlsUsed = 4;
const TInt KServerEnabled = 8;
const TInt KSimChangeRemove = 32;
const TInt KUsageInHomeNw = 64;
const TInt KEditable = 128;
const TInt KProductConfigured= 256;

//values used to set and retrieve major and minor version numbers from a single int
const TInt KVersionMinorMask = 255;
const TInt KVersionMajorMask = 8;

//values used to set and retrieve mcc,mnc values from a single int
const TInt KMccShift = 20;
const TInt KMncMask = 1023;
const TInt KMncShift= 10;


const TInt KMinPriorityValue = 1; //The minimum value for the priority field

//all strings related to SQL Statements used to form valid SQL statements

_LIT(KSecureDBDrive,"c:");
_LIT(KSecureDBName,"settings.db");
_LIT(KCreateTable, "CREATE TABLE");
_LIT(KCreateTableName," SUPLSETTINGS ");
_LIT(KInsertIntoTable," INSERT INTO ");
_LIT(KValues," VALUES");
_LIT(KDeleteRecord,"DELETE FROM ");
_LIT(KUpdateRecord,"UPDATE ");
_LIT(KWhere," WHERE ");
_LIT(KSET," SET ");
_LIT(KSelect,"SELECT ");
_LIT(KFrom," FROM ");
_LIT(KMax," MAX ");
_LIT(KOrderBy," ORDER BY ");
_LIT(KOpeningBracket,"(");
_LIT(KClosingBracket,")");
_LIT(KCommaSeparator,",");
_LIT(KQuotes,"'");
_LIT(KEquals," = ");
_LIT(KGreaterOrEquals," >= ");
_LIT(KGreaterThan," > ");
_LIT(KStar," * ");
_LIT(KCount," Count ");
_LIT(KColSLPId,"SlpID");
_LIT(KColSLPAddress,"SLPName");
_LIT(KColManuName, "ManuName");
_LIT(KColProtocolVersionMajor,"MajorVersion");
_LIT(KColIap,"IAP");      
_LIT(KColTimeLastUse,"LastTimeUsed");
_LIT(KColTimeLastTried,"LastTimeTried");
_LIT(KColNetInfoLastUse,"NetInfoLastUse");
_LIT(KColNetLastUseCId,"NwInfoLastUseCId");
_LIT(KColNetLastUseParams,"NwInfoLastUseParams");
_LIT(KColNetInfoLastSucess,"NetInfoLastSucess");
_LIT(KColNetLastSuccParams,"NwInfoLastSuccParams");
_LIT(KColNetLastSuccCId,"NwInfoLastSuccCId");
//EEmergencySupport,ETlsEnabled,EPskTlsUsed,EServerEnabled,ESimChangeRemove,EUsageInHomeNw,EEditable
_LIT(KColOtherProperties,"OtherProperties");
_LIT(KColPriority,"Priority");
_LIT(KCaseInsensitive," COLLATE NOCASE ");
_LIT(KValuePriority,":Priority");
_LIT(KValueSlpId,":SlpId");
_LIT(KDataTypeInteger," INTEGER ");
_LIT(KDataTypeString," VARCHAR");
_LIT(KDataTypeTime," TIMESTAMP NULL");
_LIT(KPrimaryKey,"PRIMARY KEY AUTOINCREMENT");
_LIT(KDateTimeFormat,"%M%D%Y%/0%1%/1%2%/2%3%/3 %-B%:0%J%:1%T%:2%S%:3%+B");   // MM/DD/YYYY hh:mm:ss AM/PM

_LIT(KSessionTableName," SESSIONSETTINGS ");
_LIT(KColSessionId,"SessionID");
_LIT(KColSessionName,"SessionName");
_LIT(KColNotificationPresent,"NotificationPresent");
_LIT(KColTriggerNotificationStatus, "NotificationStatus");
_LIT(KColTriggerType,"TriggerType");
_LIT(KColRequestType,"RequestType");
_LIT(KColOutstandingTrigger,"OutstandingTrigger");      
_LIT(KColInterval,"Interval");
_LIT(KColEndTime,"EndTime");

_LIT(KColServerId,"ServerId");
_LIT(KColIap1,"Iap1");
_LIT(KColIap2,"Iap2");
_LIT(KColServerAddress,"ServerAddress");
_LIT(KColServerAddType,"ServerAddType");



// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::NewL()
// ---------------------------------------------------------------------------
CSettingsDatabaseHandler* CSettingsDatabaseHandler::NewL() 
    {
    CSettingsDatabaseHandler* tmp = new (ELeave)CSettingsDatabaseHandler();
    CleanupStack::PushL(tmp);
    tmp->ConstructL();
    CleanupStack::Pop();
    return tmp;
    }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::~CSettingsDatabaseHandler()
//
// Destructor method
// ---------------------------------------------------------------------------
CSettingsDatabaseHandler::~CSettingsDatabaseHandler()
    {
				delete iSettingsRep;
				iSettingsRep = NULL;
				
				delete iSettingsNotifierRep;
				iSettingsNotifierRep = NULL;
				
				Close();
				
				LogQuery(_L("Deleted Settings DB Handle CLosed"));
				
    }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::ConstructL()
//
// Second phase construction. Can Leave.
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::ConstructL()
    {
    iSettingsRep = CRepository::NewL(KCRUidSuplConfiguration);
    iSettingsNotifierRep = CRepository::NewL(KCRUidSuplSettings);
    CreateOpenSecureDatabaseL();
    }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::CSettingsDatabaseHandler()
//
// Constructor
// ---------------------------------------------------------------------------
CSettingsDatabaseHandler::CSettingsDatabaseHandler()
    {

    }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::CreateOpenSecureDatabaseL()
//
// Create a new database and open. The database will be in exclusive access mode.
// ---------------------------------------------------------------------------

void CSettingsDatabaseHandler::CreateOpenSecureDatabaseL(/*const TDesC& aDbFile*/)
    {
    //create a security policy for the database
    const TSecurityPolicy KPolicy1(ECapabilityReadUserData,ECapabilityWriteUserData);
    const TSecurityPolicy KPolicy2(ECapabilityReadUserData);

    TSecurityPolicy defaultPolicy(TSecurityPolicy::EAlwaysPass);
    RSqlSecurityPolicy securityPolicy;
    CleanupClosePushL(securityPolicy);
    TInt err = securityPolicy.Create(defaultPolicy);

    PrintErrorMessage(_L(""),err,4);
 
    
    User::LeaveIfError(err);

    securityPolicy.SetDbPolicy(RSqlSecurityPolicy::ESchemaPolicy, KPolicy1);
    securityPolicy.SetDbPolicy(RSqlSecurityPolicy::EWritePolicy, KPolicy2);
   
    

    TBuf<KShortStringLen> dbPath;
    TBuf<KShortStringLen> pathBuf;
    err = GetDBPathFromCR(pathBuf);

    PrintErrorMessage(pathBuf,err,7);
    
    if(pathBuf.Length()) //if there is a DB path existing in the CenRep try to open that
        {
        dbPath.Copy(KSecureDBDrive);
        dbPath.Append(pathBuf);
        dbPath.Append(KSecureDBName);
        err = iDb.Open(dbPath); // Check if DB exists
        PrintErrorMessage(dbPath,err,3);       
        }       
    							

    if( KErrNotFound == err || pathBuf.Length() <= 0) // if there is no DB Path in the cen rep or the Db does not exist
        {
        RProcess process;
        TBuf<KShortStringLen> uidname;
        TInt error = process.SecureId().iId;
        if(error < 0) // if there is no secure id associated with the app use the UID3 value
            {
            TUidType tempUidType = process.Type();
            const TUid& uid = tempUidType[2];
            uidname.Copy(uid.Name());
            }
        else // use the secure id value to create the DB
            {
            TSecureId secureID = process.SecureId();
            TUid uid = TUid::Uid(error);
            uidname.Copy(uid.Name());
            
            }
        dbPath.Zero();
        dbPath.Append(KSecureDBDrive);
        dbPath.Append(uidname);
        dbPath.Append(KSecureDBName);
        err = iDb.Create(dbPath, securityPolicy); // create the db with the path that was formed

        PrintErrorMessage(dbPath,err,0);
                
        User::LeaveIfError(err);
                
        //DB created successfully copy the DB path to CenRep
        StoreDBPathToCR(uidname);
        CreateTableL();
        CreateSessionTableL();
        }
    CleanupStack::PopAndDestroy();//securityPolicy;
  
    PrintErrorMessage(dbPath,err,3);
    User::LeaveIfError(err);
    }
 
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::Close()
//
// Close the database.
// ---------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::Close()
    {
    iDb.Close();
    LogQuery(_L("CLOSED DB"));
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::CreateTableL()
//
// Creates the SUPL Settings table. Leaves, if the table cannot be created.
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::CreateTableL()
    {
    
    iSQLString.Copy(KCreateTable);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KOpeningBracket);

    iSQLString.Append(KColSLPId);
    iSQLString.Append(KDataTypeInteger);
    iSQLString.Append(KPrimaryKey);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColSLPAddress);
    iSQLString.Append(KDataTypeString);
    iSQLString.Append(KOpeningBracket);
    iSQLString.AppendNum(KMaxLenSLPAddress);
    iSQLString.Append(KClosingBracket);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColProtocolVersionMajor);
    iSQLString.Append(KDataTypeInteger);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColPriority);
    iSQLString.Append(KDataTypeInteger);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColManuName);
    iSQLString.Append(KDataTypeString);
    iSQLString.Append(KOpeningBracket);
    iSQLString.AppendNum(KGenericStringLen);
    iSQLString.Append(KClosingBracket);
    iSQLString.Append(KCommaSeparator); 

    iSQLString.Append(KColIap);
    iSQLString.Append(KDataTypeString);
    iSQLString.Append(KOpeningBracket);
    iSQLString.AppendNum(KMaxLenIAPName);
    iSQLString.Append(KClosingBracket);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColTimeLastUse);
    iSQLString.Append(KDataTypeTime);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColTimeLastTried);
    iSQLString.Append(KDataTypeTime);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColNetInfoLastUse);
    iSQLString.Append(KDataTypeInteger);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColNetInfoLastSucess);
    iSQLString.Append(KDataTypeInteger);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColNetLastUseCId);
    iSQLString.Append(KDataTypeInteger);
    iSQLString.Append(KCommaSeparator);
    iSQLString.Append(KColNetLastUseParams);
    iSQLString.Append(KDataTypeInteger);
    iSQLString.Append(KCommaSeparator);
    iSQLString.Append(KColNetLastSuccCId);
    iSQLString.Append(KDataTypeInteger);
    iSQLString.Append(KCommaSeparator);
    iSQLString.Append(KColNetLastSuccParams);
    iSQLString.Append(KDataTypeInteger);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColOtherProperties);
    iSQLString.Append(KDataTypeInteger);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColServerId);
    iSQLString.Append(KDataTypeString);
    iSQLString.Append(KOpeningBracket);
    iSQLString.AppendNum(KMaxServerIdLen);
    iSQLString.Append(KClosingBracket);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColIap1);
    iSQLString.Append(KDataTypeString);
    iSQLString.Append(KOpeningBracket);
    iSQLString.AppendNum(KMaxLenIAPName);
    iSQLString.Append(KClosingBracket);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColIap2);
    iSQLString.Append(KDataTypeString);
    iSQLString.Append(KOpeningBracket);
    iSQLString.AppendNum(KMaxLenIAPName);
    iSQLString.Append(KClosingBracket);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColServerAddress);
    iSQLString.Append(KDataTypeString);
    iSQLString.Append(KOpeningBracket);
    iSQLString.AppendNum(KMaxServerAddLen);
    iSQLString.Append(KClosingBracket);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColServerAddType);
    iSQLString.Append(KDataTypeString);
    iSQLString.Append(KOpeningBracket);
    iSQLString.AppendNum(KMaxServerAddTypeLen);
    iSQLString.Append(KClosingBracket);
    
    iSQLString.Append(KClosingBracket);
   
    TInt err = iDb.Exec(iSQLString);

    PrintErrorMessage(iSQLString,err,1);							
        
    User::LeaveIfError(err);

    }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::InsertSLPRecordL()
//
// This method is used to insert SLP record into the SUPL Settings DB
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::InsertSLPRecordL(const CServerParams* aServParamValues,TInt64& aSlpId)
    {    
   
    HBufC* SLPAddress = HBufC::NewLC( KMaxHSLPAddrLen);
    HBufC* IAPName = HBufC::NewLC( KMaxIAPLen);      
    HBufC* manuName = HBufC::NewLC(KMaxHSLPAddrLen);
    
    TBuf<KMaxServerAddTypeLen> serverAddTypeDefault;
    serverAddTypeDefault.Copy(_L("FQDN"));
    
    TUint16* buf = NULL;
    TPtr nullDefault(buf,0);
        
    TBuf<KDateStringLen> lastUseDateTimeFormat;
    TBuf<KDateStringLen> lasttriedDateTimeFormat;
    
    TInt err = KErrNone;
    
    TInt64 slpId;
    TInt protocolVersion = 0; 
    TTime  timeLastUse;
    TTime  timeLastTried;
    
    TInt32  netInfoLastUse = 0;
    TInt32  netInfoLastSucess = 0;
    TBool   serverEnabled,simChangeRemove,usageInHomeNw,editable,prodConfig;
    
    TInt ret = aServParamValues->Get(slpId,SLPAddress->Des(),IAPName->Des(),serverEnabled,simChangeRemove,usageInHomeNw,editable);
    					 aServParamValues->GetServerConfigurationType(prodConfig); 
    
    if(ret == KErrNotFound)
        {
        CleanupStack::PopAndDestroy(3); //SLPAddress,IAPName,manuName
        aSlpId = -1;
        PrintErrorMessage(_L(""),KErrNotFound,8);
        User::Leave(KErrGeneral);        
        }
    if(IsSlpExists(*SLPAddress))
        {
        PrintErrorMessage(_L(""),KErrAlreadyExists,8);
        User::Leave(KErrAlreadyExists);  
        }
    err = GetDefaultCRValues(KSuplProtoclVersion,protocolVersion); //get default values to be set from CR
    PrintErrorMessage(_L(""),err,8);
    User::LeaveIfError(err);
    TBool   emergencySupport;
    err = GetDefaultCRValues(KSuplEmergencySupport,emergencySupport); //get default values to be set from CR
    PrintErrorMessage(_L(""),err,8);
    User::LeaveIfError(err);
    TBool   tlsEnabled;
    err = GetDefaultCRValues(KSuplTlsEnable,tlsEnabled); //get default values to be set from CR
    PrintErrorMessage(_L(""),err,8);
    User::LeaveIfError(err);
    TBool   pskTlsUsed;
    err = GetDefaultCRValues(KSuplPskTlsEnable,pskTlsUsed); //get default values to be set from CR
    PrintErrorMessage(_L(""),err,8);
    User::LeaveIfError(err);

    timeLastUse.FormatL(lastUseDateTimeFormat,KDateTimeFormat); 
    timeLastTried.FormatL(lasttriedDateTimeFormat,KDateTimeFormat); 
    //form other properties int based on the returned TBool Values
    TInt OtherProperties = 0; //set to all false
   
    if(emergencySupport)
        {
        OtherProperties |= KEmergencySupport;
        }
    if(tlsEnabled)
        {
        OtherProperties |= KTlsEnabled;
        }
    if(pskTlsUsed)
        {
        OtherProperties |= KPskTlsUsed;
        }
    if(serverEnabled)
        {
        OtherProperties |= KServerEnabled;
        }
    if(simChangeRemove)
        {
        OtherProperties |= KSimChangeRemove;
        }
    if(usageInHomeNw)
        {
        OtherProperties |= KUsageInHomeNw;
        }
    if(editable)
        {
        OtherProperties |= KEditable;
        }
    if(prodConfig)
        {
        OtherProperties |= KProductConfigured;
        }
    
    TBuf<KMaxHSLPAddrLen> tempBuf;
    TBuf<KMaxIAPLen> tempIapBuf;
    tempBuf.Copy(*SLPAddress);
    tempIapBuf.Copy(*IAPName);
    tempBuf.TrimAll();
    tempIapBuf.TrimAll();
    
    if( tempBuf.Length() == 0 )
	    {
	    CleanupStack::PopAndDestroy(3); //SLPAddress,IAPName,manuName
        User::Leave(KErrArgument);        
	    }
    
    
    iSQLString.Copy(KInsertIntoTable);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KOpeningBracket);

    iSQLString.Append(KColSLPAddress);
    iSQLString.Append(KCommaSeparator);


    iSQLString.Append(KColPriority);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColProtocolVersionMajor);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColManuName);
    iSQLString.Append(KCommaSeparator);
        
    iSQLString.Append(KColIap);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColTimeLastUse);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColTimeLastTried);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColNetInfoLastUse);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColNetInfoLastSucess);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColOtherProperties);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColServerId);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColIap1);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColIap2);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColServerAddress);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColServerAddType);
        
    iSQLString.Append(KClosingBracket);

    iSQLString.Append(KValues);

    iSQLString.Append(KOpeningBracket);

    iSQLString.Append(KQuotes);
    iSQLString.Append(tempBuf);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);

    TInt priority = 0;
    GetMaxPriorityL(priority);
    iSQLString.AppendNum(priority); // set all servers to max priority by default   
    iSQLString.Append(KCommaSeparator);

    iSQLString.AppendNum(protocolVersion);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KQuotes);
    iSQLString.Append(*manuName);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KQuotes);
    iSQLString.Append(tempIapBuf);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);


    iSQLString.Append(KQuotes);
    iSQLString.Append(lastUseDateTimeFormat);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KQuotes);
    iSQLString.Append(lasttriedDateTimeFormat);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);

    iSQLString.AppendNum(netInfoLastUse);
    iSQLString.Append(KCommaSeparator);

    iSQLString.AppendNum(netInfoLastSucess);
    iSQLString.Append(KCommaSeparator);

    iSQLString.AppendNum(OtherProperties);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KQuotes);
    iSQLString.Append(nullDefault);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KQuotes);
    iSQLString.Append(nullDefault);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KQuotes);
    iSQLString.Append(nullDefault);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KQuotes);
    iSQLString.Append(nullDefault);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KQuotes);
    iSQLString.Append(serverAddTypeDefault);
    iSQLString.Append(KQuotes);
        
    iSQLString.Append(KClosingBracket);

    CleanupStack::PopAndDestroy(3); //SLPAddress,IAPName,manuName
    
    err = iDb.Exec(_L("BEGIN")); 
    err = iDb.Exec(iSQLString);
    PrintErrorMessage(iSQLString,err,2);                                         
    if(err < 0)
        User::Leave(KErrGeneral);
    aSlpId  = GetLastInsertedSLPID();
    iDb.Exec(_L("COMMIT")); 

    err = NotifyDBChange(MSuplSettingsObserver::ESuplSettingsDBAddEvent,aSlpId);
    
    }
    
// ----------------------------------------------------------------------------
// CSettingsDatabaseHandler::DeleteSLPRecordL()
//
// This method is used to delete a SLP record corresponding to the given SLP ID
// ----------------------------------------------------------------------------
void CSettingsDatabaseHandler::DeleteSLPRecordL(TInt64 aSLPId)
    {
    if(!IsSlpExists(aSLPId)) //no slp exists with the given id so return error
        {
        PrintErrorMessage(_L(""),KErrNotFound,9);
        User::Leave(KErrNotFound);
        }

    iSQLString.Copy(KDeleteRecord);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(aSLPId);

    TInt err = iDb.Exec(iSQLString);
    PrintErrorMessage(iSQLString,err,5);
    if(err < 0)
        User::Leave(KErrGeneral);
    NormalizePriorityL();
    err = NotifyDBChange(MSuplSettingsObserver::ESuplSettingsDBDeleteEvent,aSLPId); // notify about a change in the database

    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::UpdateSLPRecordL()
//
// This method is used to update a column of string value corresponding 
// to a particular slp
// ---------------------------------------------------------------------------
       
void CSettingsDatabaseHandler::UpdateSLPRecordL(TInt64 aSLPId,CSuplSettingsInternal::TServerParam aParameter,const TDesC& aValue)
    {
    if(!IsSlpExists(aSLPId)) //no slp exists with the given id so return error
        {
        PrintErrorMessage(_L(""),KErrArgument,9);
        User::Leave(KErrArgument);
        }
    switch(aParameter)
        {
        case CSuplSettingsInternal::EIap:
            if(aValue.Length() > KMaxIAPLen || aValue.Length() <= 0) //check for max length of IAP 
                {
                PrintErrorMessage(_L(""),KErrArgument,9);
                User::Leave(KErrArgument);
                }

            break;
        case CSuplSettingsInternal::EServerAddress:
            {
            TInt64 slpId;
            if(IsSlpExists(aValue,slpId))
                {
                if(slpId == aSLPId)//the requested server already has the server name set
                    return;
                PrintErrorMessage(_L(""),KErrAlreadyExists,9);
                User::Leave(KErrAlreadyExists);
                }
            if(aValue.Length() > KGenericStringLen || aValue.Length() <= 0)
                {
                PrintErrorMessage(_L(""),KErrArgument,9);
                User::Leave(KErrArgument);
                }
            }
            break;
        default:
            if(aValue.Length() > KGenericStringLen || aValue.Length() <= 0)
                {
                PrintErrorMessage(_L(""),KErrArgument,9);
                User::Leave(KErrArgument);
                }
            break;
        
        }


    iSQLString.Copy(KUpdateRecord);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KSET);

    TBuf<KGenericStringLen> tempBuf;
    tempBuf.Copy(aValue);
    //remove additional spaces
    tempBuf.Trim();
    FormSLPQueryL(aParameter,tempBuf,iSQLString);

    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(aSLPId);

    TInt err = iDb.Exec(iSQLString);
    PrintErrorMessage(iSQLString,err,6);				
    if(err < 0)
           User::Leave(KErrGeneral);
    err = NotifyDBChange(MSuplSettingsObserver::ESuplSettingsDBUpdateEvent,aSLPId);

    }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::UpdateSLPRecordL()
//
// This method is used to update a column of integer value corresponding 
//  to a particular slp
// ---------------------------------------------------------------------------
       
void CSettingsDatabaseHandler::UpdateSLPRecordL(TInt64 aSLPId,CSuplSettingsInternal::TServerParam aParameter,TInt aValue)
    {
    
    if(!IsSlpExists(aSLPId))
        {
        PrintErrorMessage(_L(""),KErrArgument,9);
        User::Leave(KErrArgument);
        }
    iSLPId = aSLPId;

    iSQLString.Copy(KUpdateRecord);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KSET);

    FormSLPQueryL(aParameter,aValue,iSQLString);

    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(aSLPId);

    TInt err = iDb.Exec(iSQLString);
    PrintErrorMessage(iSQLString,err,6);	
    if(err < 0)
           User::Leave(KErrGeneral);
    err = NotifyDBChange(MSuplSettingsObserver::ESuplSettingsDBUpdateEvent,aSLPId);
    }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::UpdateSLPRecordL()
//
// This method is used to update a column of TTime value corresponding to 
// a particular slp
// ---------------------------------------------------------------------------
        
void CSettingsDatabaseHandler::UpdateSLPRecordL(TInt64 aSLPId,CSuplSettingsInternal::TServerParam aParameter,TTime aValue)
    {

    iSQLString.Copy(KUpdateRecord);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KSET);

    FormSLPQueryL(aParameter,aValue,iSQLString);

    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(aSLPId);

    TInt err = iDb.Exec(iSQLString); 

    PrintErrorMessage(iSQLString,err,6);	
    
    if(err < 0)
           User::Leave(KErrGeneral);
    err = NotifyDBChange(MSuplSettingsObserver::ESuplSettingsDBUpdateEvent,aSLPId);
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::UpdateSLPRecordL()
//
// This method is used to update all configurable parameters corresponding 
// to a partiular SLP
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::UpdateSLPRecordL(const CServerParams* aParamValues)
    {

    TInt64 slpId;
    HBufC* SLPAddress = HBufC::NewLC( KMaxHSLPAddrLen);
    
    HBufC* IAPName = HBufC::NewLC( KMaxIAPLen);
    TBool  serverEnabled,simChangeRemove,usageInHomeNw,editable;
    
    TInt ret = aParamValues->Get(slpId,SLPAddress->Des(),IAPName->Des(),serverEnabled,simChangeRemove,usageInHomeNw,editable);
    if(ret == KErrNotFound || slpId == -1 )
        {
        CleanupStack::PopAndDestroy(2); //SLPAddress,IAPName
        PrintErrorMessage(_L(""),KErrArgument,9);
        User::Leave(KErrArgument); 
        }
    if(!IsSlpExists(slpId))
        {
        CleanupStack::PopAndDestroy(2); //SLPAddress,IAPName
        PrintErrorMessage(_L(""),KErrNotFound,9);
        User::Leave(KErrNotFound);
        }
    
    //form other properties int based on the returned TBool Values
    TInt OtherProperties = 0; //set to all false
    ret = GetOtherPropertyValue(slpId,OtherProperties); //get the current value of all flags
    if(ret != KErrNone)
        {
        CleanupStack::PopAndDestroy(2);//SLPAddress,IAPName
        PrintErrorMessage(_L(""),KErrGeneral,9);
        User::Leave(KErrGeneral);
        }

    if(serverEnabled)
         {
         OtherProperties |= KServerEnabled;
         }
     else
         {
         OtherProperties &= ~KServerEnabled;
         }
     if(simChangeRemove)
         {
         OtherProperties |= KSimChangeRemove;
         }
     else
         {
         OtherProperties &= ~KSimChangeRemove;
         }
     if(usageInHomeNw)
         {
         OtherProperties |= KUsageInHomeNw;
         }
     else
         {
         OtherProperties &= ~KUsageInHomeNw;
         }
     if(editable)
         {
         OtherProperties |= KEditable;
         }
     else
         {
         OtherProperties &= ~KEditable;
         }
     //product configuration flag is not altered here since it should not be modified while updation
     //and gets set only during addition of a new server through product configuration
     
     TBuf<KGenericStringLen> tempBuf;
     tempBuf.Copy(*SLPAddress);
     tempBuf.Trim();
    iSQLString.Copy(KUpdateRecord);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KSET);
    iSQLString.Append(KColSLPAddress);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.Append(tempBuf);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColIap);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.Append(*IAPName);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);


    iSQLString.Append(KColOtherProperties);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.AppendNum(OtherProperties);
    iSQLString.Append(KQuotes);

    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(slpId);
    
    TInt err = iDb.Exec(iSQLString); 
    PrintErrorMessage(iSQLString,err,6); 
    CleanupStack::PopAndDestroy(2);
    if(err < 0)
        {
        PrintErrorMessage(_L(""),KErrGeneral,9);
        User::Leave(KErrGeneral);
        }

    err = NotifyDBChange(MSuplSettingsObserver::ESuplSettingsDBUpdateEvent,slpId);
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::UpdateSLPRecordL()
//
// This method is used to update a column  corresponding to version number 
// of a particular slp
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::UpdateSLPRecordL(TInt64 aSLPId,TInt aMajor, TInt aMinor)
    {
    if(!IsSlpExists(aSLPId))
        {
        PrintErrorMessage(_L(""),KErrArgument,9);
        User::Leave(KErrArgument); 
        }
    if(aMajor < 0 || aMajor > 255 || aMinor < 0 || aMinor > 255 )
        {
        PrintErrorMessage(_L(""),KErrArgument,9);
        User::Leave(KErrArgument); 
        }
    TInt protocolVersion = aMajor<< KVersionMajorMask | aMinor;

    iSQLString.Copy(KUpdateRecord);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KSET);
    iSQLString.Append(KColProtocolVersionMajor);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.AppendNum(protocolVersion);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(aSLPId);

    TInt err = iDb.Exec(iSQLString);

    PrintErrorMessage(iSQLString,err,6); 

    if(err < 0)
        User::Leave(KErrGeneral);
    err = NotifyDBChange(MSuplSettingsObserver::ESuplSettingsDBUpdateEvent,aSLPId);
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::UpdateSLPRecordL()
//
// This method is used to update a column corresponding to N/w Info Last Use/ 
//  Last Success of a particular slp
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::UpdateSLPRecordL(TInt64 aSLPId,CSuplSettingsInternal::TServerParam aParamType ,
                                                const CSuplSettingsInternal::TSuplSettingsNetworkType aNetType, 
                                                const TInt aMcc, const TInt aMnc, const TInt aCid, const TInt aLac)
    {
    if(!IsSlpExists(aSLPId))
        {
        PrintErrorMessage(_L(""),KErrArgument,9);
        User::Leave(KErrArgument); 
        }
    if(aMcc < 0 || aMcc > 999 || aMnc < 0 || aMnc > 999 || aLac < 0 || aLac > 65535)//these params are 8 bit ints and cant go beyond 255
        {
        PrintErrorMessage(_L(""),KErrArgument,9);
        User::Leave(KErrArgument); 
        }
    TInt64 nwparams = aMcc << KMccShift | aMnc << KMncShift | aLac;
    

    iSQLString.Copy(KUpdateRecord);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KSET);
    switch(aParamType)
        {
        case CSuplSettingsInternal::ENetInfoLastUse:
            iSQLString.Append(KColNetInfoLastUse);
            iSQLString.Append(KEquals);
            iSQLString.Append(KQuotes);
            iSQLString.AppendNum(aNetType);
            iSQLString.Append(KQuotes);
            iSQLString.Append(KCommaSeparator);

            iSQLString.Append(KColNetLastUseCId);
            iSQLString.Append(KEquals);
            iSQLString.Append(KQuotes);
            iSQLString.AppendNum(aCid);
            iSQLString.Append(KQuotes);
            iSQLString.Append(KCommaSeparator);

            iSQLString.Append(KColNetLastUseParams);
            iSQLString.Append(KEquals);
            iSQLString.Append(KQuotes);
            iSQLString.AppendNum(nwparams);
            iSQLString.Append(KQuotes);
            break;
        case CSuplSettingsInternal::ENetInfoLastSucess:
            iSQLString.Append(KColNetInfoLastSucess);
            iSQLString.Append(KEquals);
            iSQLString.Append(KQuotes);
            iSQLString.AppendNum(aNetType);
            iSQLString.Append(KQuotes);
            iSQLString.Append(KCommaSeparator);

            iSQLString.Append(KColNetLastSuccCId);
            iSQLString.Append(KEquals);
            iSQLString.Append(KQuotes);
            iSQLString.AppendNum(aCid);
            iSQLString.Append(KQuotes);
            iSQLString.Append(KCommaSeparator);

            iSQLString.Append(KColNetLastSuccParams);
            iSQLString.Append(KEquals);
            iSQLString.Append(KQuotes);
            iSQLString.AppendNum(nwparams);
            iSQLString.Append(KQuotes);
            break;
        default:
            User::Leave(KErrGeneral);
            break;
        }
    
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(aSLPId);

    TInt err = iDb.Exec(iSQLString);

    PrintErrorMessage(iSQLString,err,6); 
    if(err < 0)
        User::Leave(KErrGeneral);
    err = NotifyDBChange(MSuplSettingsObserver::ESuplSettingsDBUpdateEvent,aSLPId);
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::UpdateDefaultServerL()
//
// This method is used to update all configurable parameters corresponding 
// to a default SLP
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::UpdateDefaultServerL(const CServerParams* aParamValues)
    {

    TInt64 defaultSlpId;
    GetDefaultSLPId(defaultSlpId);
    if(defaultSlpId == -1) // no server set as default
        {
        PrintErrorMessage(_L(""),KErrNotFound,10);
        User::Leave(KErrNotFound);
        }
        
    HBufC* SLPAddress = HBufC::NewLC( KMaxHSLPAddrLen);           
    HBufC* IAPName = HBufC::NewLC( KMaxIAPLen);
    
    TBool  serverEnabled,simChangeRemove,usageInHomeNw,editable;


    TInt ret = 0;
    TInt64 slpId;
    ret = aParamValues->Get(slpId,SLPAddress->Des(),IAPName->Des(),serverEnabled,simChangeRemove,usageInHomeNw,editable);
    if(ret == KErrNotFound )
        {
        CleanupStack::PopAndDestroy(2); //SLPAddress,IAPName
        PrintErrorMessage(_L(""),KErrArgument,10);
        User::Leave(KErrArgument);
        }
    
    //form other properties int based on the returned TBool Values
    TInt OtherProperties = 0; //set to all false
    ret = GetOtherPropertyValue(defaultSlpId,OtherProperties); //get the current value of all flags
    if(ret != KErrNone)
        {
        CleanupStack::PopAndDestroy(2); //SLPAddress,IAPName
        PrintErrorMessage(_L(""),KErrGeneral,10);
        User::Leave(KErrGeneral);
        }
        
    
    if(serverEnabled)
        {
        OtherProperties |= KServerEnabled;
        }
    else
        {
        OtherProperties &= ~KServerEnabled;
        }
    if(simChangeRemove)
        {
        OtherProperties |= KSimChangeRemove;
        }
    else
        {
        OtherProperties &= ~KSimChangeRemove;
        }
    if(usageInHomeNw)
        {
        OtherProperties |= KUsageInHomeNw;
        }
    else
        {
        OtherProperties &= ~KUsageInHomeNw;
        }
    if(editable)
        {
        OtherProperties |= KEditable;
        }
    else
        {
        OtherProperties &= ~KEditable;
        }
    //product configuration flag is not altered here since it should not be modified while updation
    //and gets modified only during addition of a new server
    
    TBuf<KGenericStringLen> tempBuf;
    tempBuf.Copy(*SLPAddress);
    tempBuf.Trim();
    iSQLString.Copy(KUpdateRecord);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KSET);
    iSQLString.Append(KColSLPAddress);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.Append(tempBuf);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColIap);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.Append(*IAPName);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColOtherProperties);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.AppendNum(OtherProperties);
    iSQLString.Append(KQuotes);

    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(defaultSlpId);
    
    TInt err = iDb.Exec(iSQLString);
    PrintErrorMessage(iSQLString,err,6); 
    CleanupStack::PopAndDestroy(2); //SLPAddress,IAPName
    if(err < 0)
        {
        User::Leave(KErrGeneral);
        }
   
    err = NotifyDBChange(MSuplSettingsObserver::ESuplSettingsDBUpdateEvent,defaultSlpId);
    
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::SetSLPAsDefaultL()
//
// This method is used to set a SLP with a particular id as default
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::SetSLPAsDefaultL(TInt64 aSLPId)
    {
    if(!IsSlpExists(aSLPId))
        {
        PrintErrorMessage(_L(""),KErrNotFound,12);
        User::LeaveIfError(KErrNotFound);
        }

    SetPriorityL(aSLPId,KMinPriorityValue);
    PrintErrorMessage(_L(""),KErrNone,12);
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetDefaultSLPId()
//
// This method is used to get SLP Id of the default server
// ---------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::GetDefaultSLPId(TInt64& aSlpId)
    {
    aSlpId = -1;

    iSQLString.Copy(KSelect);
    iSQLString.Append(KStar);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KOrderBy);
    iSQLString.Append(KColPriority);
    iSQLString.Append(KCommaSeparator);
    iSQLString.Append(KColSLPId);

    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);
    TInt columnIndexSLPID = stmt.ColumnIndex(KColSLPId);
    TInt columnIndexPriority = stmt.ColumnIndex(KColPriority);
    //Get the first slp in the list in the order of priority/slp id
    while((err=stmt.Next())==KSqlAtRow)
        {
        aSlpId = stmt.ColumnInt(columnIndexSLPID);
        break;
        }
     
    stmt.Close();
    PrintErrorMessage(iSQLString,err,13);
    if(aSlpId == -1)
        {
        PrintErrorMessage(_L(""),KErrNotFound,14);
        return KErrNotFound;
        }
    PrintErrorMessage(_L(""),KErrNone,14);
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::FormSLPQueryL()
//
// Forms part of the query string, used to form a query string that contains 
// a TTIme value in it 
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::FormSLPQueryL(CSuplSettingsInternal::TServerParam aParameter,TTime aValue,TDes& aQueryString)
    {
    TBuf<64> dateTimeFormat;   
    aValue.FormatL(dateTimeFormat,KDateTimeFormat); 

    switch(aParameter)
        {
        case CSuplSettingsInternal::ETimeLastTried: 
            {
            aQueryString.Append(KColTimeLastTried);
            break;
            }
        case CSuplSettingsInternal::ETimeLastUse:
            {
            aQueryString.Append(KColTimeLastUse);
            break;
            }
        default:User::Leave(KErrArgument);
        }

    aQueryString.Append(KEquals);
    aQueryString.Append(KQuotes);
    aQueryString.Append(dateTimeFormat);
    aQueryString.Append(KQuotes);
    }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::FormSLPQueryL()
//
// Forms part of the query string, used to form a query string that contains 
// a TInt value in it 
// ---------------------------------------------------------------------------
    
void CSettingsDatabaseHandler::FormSLPQueryL(CSuplSettingsInternal::TServerParam aParameter,TInt aValue,TDes& aQueryString)
    { 
    switch(aParameter)
        {
        case CSuplSettingsInternal::ENetInfoLastUse:
            {
            aQueryString.Append(KColNetInfoLastUse);
            break;
            }
        case CSuplSettingsInternal::ENetInfoLastSucess:
            {
            aQueryString.Append(KColNetInfoLastSucess);
            break;
            }
        case CSuplSettingsInternal::EEmergencySupport: 
        case CSuplSettingsInternal::ETlsEnabled: 
        case CSuplSettingsInternal::EPskTlsUsed: 
        case CSuplSettingsInternal::EServerEnabled: 
        case CSuplSettingsInternal::EManufacturer: 
        case CSuplSettingsInternal::ESimChangeRemove:
        case CSuplSettingsInternal::EUsageInHomeNw:
        case CSuplSettingsInternal::EEditable:
            {
            aValue = SetOtherPropertiesL(aParameter,aValue);
            aQueryString.Append(KColOtherProperties);
            break;
            }
        default:
            {
            User::Leave(KErrArgument);
            break;
            }


        }	
    aQueryString.Append(KEquals);
    aQueryString.AppendNum(aValue);
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::FormSLPQueryL()
//
// Forms part of the query string, used to form a query string that contains 
// a String value in it 
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::FormSLPQueryL(CSuplSettingsInternal::TServerParam aParameter,const TDesC& aValue,TDes& aQueryString)
    {
    switch(aParameter)
        {

        case CSuplSettingsInternal::EServerAddress: 
            {
            aQueryString.Append(KColSLPAddress);
            break;
            }

        case CSuplSettingsInternal::EIap: 
            {
            aQueryString.Append(KColIap);
            break;
            }
        case CSuplSettingsInternal::EManufacturer: 
            {
            aQueryString.Append(KColManuName);
            break;
            }

        default:User::Leave(KErrArgument);
        }

    aQueryString.Append(KEquals);
    aQueryString.Append(KQuotes);
    aQueryString.Append(aValue);
    aQueryString.Append(KQuotes);
    }				



// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetNwInfoPropertiesL()
//
// This method is used to retrieve a colummn corresponding to N/w Info Last 
//  Use/Last Success of a particular slp
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::GetNwInfoPropertiesL(TInt64 aSLPId,CSuplSettingsInternal::TServerParam& aParamType ,
        CSuplSettingsInternal::TSuplSettingsNetworkType& aNetType,TInt& aMcc,TInt& aMnc, 
        TInt& aCid, TInt& aLac)
    {
    if(!IsSlpExists(aSLPId))
        {
        PrintErrorMessage(_L(""),KErrArgument,14);
        User::Leave(KErrArgument);
        }

    iSQLString.Copy(KSelect);
    iSQLString.Append(KStar);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(aSLPId);

    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);
    if(err != KErrNone)
        {
        PrintErrorMessage(_L(""),KErrGeneral,14);
        User::Leave(KErrGeneral);
        }
           
    TInt columnIndexNwType = 0;
    TInt columnIndexCid = 0;
    TInt columnIndexParams = 0;
    TInt64 nwParams = 0;

                
    switch(aParamType)
        {
        case CSuplSettingsInternal::ENetInfoLastSucess:
            columnIndexNwType = stmt.ColumnIndex(KColNetInfoLastSucess);
            columnIndexParams = stmt.ColumnIndex(KColNetLastSuccParams);
            columnIndexCid = stmt.ColumnIndex(KColNetLastSuccCId);
            break;
        case CSuplSettingsInternal::ENetInfoLastUse:
            columnIndexNwType = stmt.ColumnIndex(KColNetInfoLastUse);
            columnIndexParams = stmt.ColumnIndex(KColNetLastUseParams);
            columnIndexCid = stmt.ColumnIndex(KColNetLastUseCId);
            break;
        default:
            User::Leave(KErrGeneral);
        }
    while((err=stmt.Next())==KSqlAtRow)
        {
        aNetType = (CSuplSettingsInternal::TSuplSettingsNetworkType) stmt.ColumnInt(columnIndexNwType);
        aCid = stmt.ColumnInt(columnIndexCid);
        nwParams = stmt.ColumnInt64(columnIndexParams);
        }
    aMcc = nwParams>>KMccShift;
    aMnc = (nwParams>>KMncShift)& KMncMask;
    aLac = nwParams & KMncMask;
    PrintErrorMessage(_L(""),KErrNone,14);
    #ifdef PRINT_MESSAGE
        TBuf<5> mccBuf;
        mccBuf.AppendNum(aMcc);
        TBuf<5> mncBuf;
        mncBuf.AppendNum(aMnc);
        TBuf<5> lacBuf;
        lacBuf.AppendNum(aLac);
        LogQuery(mccBuf);
        LogQuery(mncBuf);
        LogQuery(lacBuf);
    #endif
    stmt.Close();
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::SetOtherPropertiesL()
//
//
// ---------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::SetOtherPropertiesL(CSuplSettingsInternal::TServerParam aParameter,TInt aValue)
    {
    TInt OtherProperty = 0;
    TBuf<KGenericStringLen>  sqlString;
    sqlString.Copy(KSelect);
    sqlString.Append(KColOtherProperties);
    sqlString.Append(KFrom);
    sqlString.Append(KCreateTableName);
    sqlString.Append(KWhere);
    sqlString.Append(KColSLPId);
    sqlString.Append(KEquals);
    sqlString.AppendNum(iSLPId);

    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, sqlString);
    if(err != KErrNone)
        User::Leave(KErrGeneral);
    TInt columnIndex0=0; 
    while((err=stmt.Next())==KSqlAtRow)
        {
        OtherProperty = stmt.ColumnInt(columnIndex0);
        }	

    if(aValue == 1)
        {
        switch(aParameter)
            {
            case CSuplSettingsInternal::EEmergencySupport: 
                OtherProperty = OtherProperty | KEmergencySupport; break;
            case CSuplSettingsInternal::ETlsEnabled: 
                OtherProperty = OtherProperty | KTlsEnabled; break;
            case CSuplSettingsInternal::EPskTlsUsed: 
                OtherProperty = OtherProperty | KPskTlsUsed; break;
            case CSuplSettingsInternal::EServerEnabled: 
                OtherProperty = OtherProperty | KServerEnabled; break;
            
            case CSuplSettingsInternal::ESimChangeRemove:
                OtherProperty = OtherProperty | KSimChangeRemove; break;
            case CSuplSettingsInternal::EUsageInHomeNw:
                OtherProperty = OtherProperty | KUsageInHomeNw; break;
            case CSuplSettingsInternal::EEditable:
                OtherProperty = OtherProperty | KEditable; break;
            default:{ User::Leave(KErrArgument);}
            }	
        }
    else
        {
        switch(aParameter)
            {
            case CSuplSettingsInternal::EEmergencySupport: 
                OtherProperty = OtherProperty & ~KEmergencySupport; break;
            case CSuplSettingsInternal::ETlsEnabled: 
                OtherProperty = OtherProperty & ~KTlsEnabled; break;
            case CSuplSettingsInternal::EPskTlsUsed: 
                OtherProperty = OtherProperty & ~KPskTlsUsed; break;
            case CSuplSettingsInternal::EServerEnabled: 
                OtherProperty = OtherProperty & ~KServerEnabled; break;
            
            case CSuplSettingsInternal::ESimChangeRemove:
                OtherProperty = OtherProperty & ~KSimChangeRemove; break;
            case CSuplSettingsInternal::EUsageInHomeNw:
                OtherProperty = OtherProperty & ~KUsageInHomeNw; break;
            case CSuplSettingsInternal::EEditable:
                OtherProperty = OtherProperty & ~KEditable; break;
            default:User::Leave(KErrArgument);
            }
        }	 
    stmt.Close();
    return OtherProperty;			
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetOtherPropertiesL()
//
// This method is used to get other flags associated with a slp
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::GetOtherPropertiesL(TInt64 aSLPId,CSuplSettingsInternal::TServerParam aParameter,TInt& aValue)
    {
    if(!IsSlpExists(aSLPId))
        User::Leave(KErrArgument);
    TInt OtherProperty = 0;

    iSQLString.Copy(KSelect);
    iSQLString.Append(KColOtherProperties);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(aSLPId);

    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);
    if(err != KErrNone)
        User::Leave(KErrGeneral);
    TInt columnIndex0=0; 
    while((err=stmt.Next())==KSqlAtRow)
        {
        OtherProperty = stmt.ColumnInt(columnIndex0);
        }   
    
    switch(aParameter)
        {
        case CSuplSettingsInternal::EEmergencySupport: 
            OtherProperty = OtherProperty & KEmergencySupport; break;
        case CSuplSettingsInternal::ETlsEnabled: 
            OtherProperty = OtherProperty & KTlsEnabled; break;
        case CSuplSettingsInternal::EPskTlsUsed: 
            OtherProperty = OtherProperty & KPskTlsUsed; break;
        case CSuplSettingsInternal::EServerEnabled: 
            OtherProperty = OtherProperty & KServerEnabled; break;
        case CSuplSettingsInternal::ESimChangeRemove:
            OtherProperty = OtherProperty & KSimChangeRemove; break;
        case CSuplSettingsInternal::EUsageInHomeNw:
            OtherProperty = OtherProperty & KUsageInHomeNw; break;
        case CSuplSettingsInternal::EEditable:
            OtherProperty = OtherProperty & KEditable; break;
        default:
            User::Leave(KErrArgument);break;
        }   
    if(OtherProperty > 0)
        {
        aValue = ETrue;
        }
    else
        aValue = EFalse;

    stmt.Close();
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetOtherPropertyValue()
//
// Used to retrieve other parameters associated with a slp
// ---------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::GetOtherPropertyValue(TInt64 aSLPId,TInt& aOtherPropertyValue)
    {

    TInt OtherProperty = 0;


    iSQLString.Copy(KSelect);
    iSQLString.Append(KColOtherProperties);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(aSLPId);

    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);
    if(err != KErrNone)
        return KErrGeneral;
    TInt columnIndex0=0; 
    while((err=stmt.Next())==KSqlAtRow)
        {
        OtherProperty = stmt.ColumnInt(columnIndex0);
        }   
    
    aOtherPropertyValue = OtherProperty;
    stmt.Close();
    return KErrNone;
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetLastInsertedSLPID()
//
// Returns the last inserted slp id
// ---------------------------------------------------------------------------
TInt64 CSettingsDatabaseHandler::GetLastInsertedSLPID()
    {

    iSQLString.Copy(KSelect);
    iSQLString.Append(KMax);
    iSQLString.Append(KOpeningBracket);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KClosingBracket);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);

    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);
    if(err != KErrNone)
        return KErrGeneral;
    TInt64 SLPId =  -1;
    TInt columnIndex0 = 0; 

    while((err=stmt.Next())==KSqlAtRow)
        {
        SLPId = stmt.ColumnInt(columnIndex0);
        }	

    PrintErrorMessage(iSQLString,err,-1);				

    LogQuery(iSQLString);

    TBuf<KShortStringLen> buf(_L("Last SLP ID... "));
    buf.AppendNum(SLPId);
    LogQuery(buf);
    stmt.Close();
    return SLPId;
    }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetAllSLP()
//
// Returns a list of all SLPs in the order of priority
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::GetAllSLPL(RPointerArray<CServerParams>& aParamValues)
    {
    TInt count = 0;
    SlpCount(count);
    if(count<= 0)
        User::Leave(KErrNotFound);
        

    iSQLString.Copy(KSelect);
    iSQLString.Append(KStar);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName); // retrieve list of all SLPs in order of priority
    iSQLString.Append(KOrderBy);
    iSQLString.Append(KColPriority);
    iSQLString.Append(KCommaSeparator);
    iSQLString.Append(KColSLPId);
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);
    PrintErrorMessage(iSQLString,err,13);
    if(err != KErrNone)
            User::Leave(KErrGeneral);
    TInt64 SLPId =  -1;
    TBuf<KGenericStringLen> ServerAddress,Iap;

    TInt otherprops;
    TBuf<KShortStringLen> otherpropbuf;

    
    TBool serverEnabled = EFalse;
    TBool simChangeRemove = EFalse;
    TBool usageInHomeNw = EFalse;
    TBool editable = EFalse;
    TBool prodConfig = ETrue;

    TInt columnIndexSLPID = stmt.ColumnIndex(KColSLPId);
    TInt columnIndexSLPAddress = stmt.ColumnIndex(KColSLPAddress);
    TInt columnIndexIAP = stmt.ColumnIndex(KColIap);
    TInt columnIndexOtherProperties = stmt.ColumnIndex(KColOtherProperties);

    while((err=stmt.Next())==KSqlAtRow)
        {
        CServerParams *params = CServerParams::NewL();
        CleanupStack::PushL(params);
        SLPId = stmt.ColumnInt(columnIndexSLPID);
        stmt.ColumnText(columnIndexSLPAddress,ServerAddress);
              
        stmt.ColumnText(columnIndexIAP,Iap);            
        otherprops = stmt.ColumnInt(columnIndexOtherProperties);
        otherpropbuf.Zero();
        otherpropbuf.AppendNum(otherprops);
        LogQuery(ServerAddress);
        LogQuery(Iap);
        LogQuery(otherpropbuf);
        serverEnabled = otherprops & KServerEnabled;
        simChangeRemove = otherprops & KSimChangeRemove; 
        usageInHomeNw = otherprops & KUsageInHomeNw; 
        editable = otherprops & KEditable;
        prodConfig = otherprops & KProductConfigured;
        params->Set(ServerAddress,Iap,serverEnabled,simChangeRemove,usageInHomeNw,editable,SLPId);
        params->SetServerConfigurationType(prodConfig);
        
        aParamValues.AppendL(params);//ownership transferred to RPointerArray
        CleanupStack::Pop(params);
        }   

    stmt.Close();
    LogAllSLP();
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetAllSLP()
//
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::GetAllSLPL(RPointerArray<CServerParams>& aParamValues,TRequestStatus& aStatus)
    {
    iStatus = &aStatus;
    TInt count;
    SlpCount(count);
    if(count<= 0)
        User::RequestComplete(iStatus,KErrNotFound);

    iSQLString.Copy(KSelect);
    iSQLString.Append(KStar);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName); // retrieve list of all SLPs 
    iSQLString.Append(KOrderBy);
    iSQLString.Append(KColPriority);
    iSQLString.Append(KCommaSeparator);
    iSQLString.Append(KColSLPId);
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);
    PrintErrorMessage(iSQLString,err,13);
    if(err != KErrNone)
        User::RequestComplete(iStatus,KErrGeneral);

    TInt64 SLPId =  -1;
    TBuf<KGenericStringLen> ServerAddress,Iap;

    TInt otherprops;
    TBuf<KShortStringLen> otherpropbuf;

    
    TBool serverEnabled = EFalse;
    TBool simChangeRemove = EFalse;
    TBool usageInHomeNw = EFalse;
    TBool editable = EFalse;
    TBool prodConfig = ETrue;

    TInt columnIndexSLPID = stmt.ColumnIndex(KColSLPId);
    TInt columnIndexSLPAddress = stmt.ColumnIndex(KColSLPAddress);
    TInt columnIndexIAP = stmt.ColumnIndex(KColIap);
    TInt columnIndexOtherProperties = stmt.ColumnIndex(KColOtherProperties);

    while((err=stmt.Next())==KSqlAtRow)
        {
        CServerParams *params = CServerParams::NewL();
        CleanupStack::PushL(params);
        SLPId = stmt.ColumnInt(columnIndexSLPID);
        stmt.ColumnText(columnIndexSLPAddress,ServerAddress);
              
        stmt.ColumnText(columnIndexIAP,Iap);            
        otherprops = stmt.ColumnInt(columnIndexOtherProperties);
        otherpropbuf.Zero();
        otherpropbuf.AppendNum(otherprops);
        LogQuery(ServerAddress);
        LogQuery(Iap);
        LogQuery(otherpropbuf);
        serverEnabled = otherprops & KServerEnabled;
        simChangeRemove = otherprops & KSimChangeRemove; 
        usageInHomeNw = otherprops & KUsageInHomeNw; 
        editable = otherprops & KEditable;
        prodConfig = otherprops & KProductConfigured;
        params->Set(ServerAddress,Iap,serverEnabled,simChangeRemove,usageInHomeNw,editable,SLPId);
        params->SetServerConfigurationType(prodConfig);
        aParamValues.AppendL(params);//ownership transferred to RPointerArray
        CleanupStack::Pop(params);
        }   

    stmt.Close();
    LogAllSLP();
    User::RequestComplete(iStatus,KErrNone);
    }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetServerAddress()
//
// This method is used to retrieve server address for a slp of a particular id
// ---------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::GetServerAddress(const TInt64 aSlpId, TDes& aServerAddress) 
    {
    if(!IsSlpExists(aSlpId))
        return KErrArgument;
    TBuf<KGenericStringLen> serveraddress;

    iSQLString.Copy(KSelect);
    iSQLString.Append(KColSLPAddress);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.AppendNum(aSlpId);
    iSQLString.Append(KQuotes);
    
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb,iSQLString);
    PrintErrorMessage(iSQLString,err,13);
    if(err != KErrNone)
        return KErrGeneral;
       
    TInt columnIndexSLPName = stmt.ColumnIndex(KColSLPAddress);
    aServerAddress.Zero();
    
    while((err=stmt.Next())==KSqlAtRow)
        {
        stmt.ColumnText(columnIndexSLPName,serveraddress);
        }   
    
    stmt.Close();
    if(aServerAddress.MaxLength() < serveraddress.Length())
        return KErrOverflow;
    aServerAddress = serveraddress;
    return KErrNone;
    }
// -------------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetLastTriedTime()
//
// This method is to last tried time of a SLP corresponding to a particular Slp Id
// --------------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::GetLastTriedTime(const TInt64 aSlpId, TTime& aLastTriedTime)
    {
    if(!IsSlpExists(aSlpId))
        return KErrNotFound;

    iSQLString.Copy(KSelect);
    iSQLString.Append(KColTimeLastTried);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.AppendNum(aSlpId);
    iSQLString.Append(KQuotes);
    
    TBuf<KGenericStringLen> timeString;
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb,iSQLString);
    PrintErrorMessage(iSQLString,err,13);
    if(err != KErrNone)
        return KErrGeneral;
       
    TInt columnIndexSLPName = stmt.ColumnIndex(KColTimeLastTried);
    
    while((err=stmt.Next())==KSqlAtRow)
        {
        stmt.ColumnText(columnIndexSLPName,timeString);
        }   
    aLastTriedTime.Parse(timeString);
    stmt.Close();
    
    return KErrNone;
    }
// ------------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetLastUsedTime()
//
// This method is to last used time of a SLP corresponding to a particular Slp Id
// ------------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::GetLastUsedTime(const TInt64 aSlpId, TTime& aLastUsedTime)
    {
    if(!IsSlpExists(aSlpId))
        return KErrArgument;

    iSQLString.Copy(KSelect);
    iSQLString.Append(KColTimeLastUse);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.AppendNum(aSlpId);
    iSQLString.Append(KQuotes);
    
    TBuf<KGenericStringLen> timeString;
    
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb,iSQLString);
    PrintErrorMessage(iSQLString,err,13);
    if(err != KErrNone)
        return KErrGeneral;
       
    TInt columnIndexSLPName = stmt.ColumnIndex(KColTimeLastUse);
    
    while((err=stmt.Next())==KSqlAtRow)
        {
        stmt.ColumnText(columnIndexSLPName,timeString);
        }   
    aLastUsedTime.Parse(timeString);
    stmt.Close();

    return KErrNone;
    }
// -------------------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetManufacturer()
//
// This method is to Get manufacturer name of a SLP corresponding to a particular Slp Id
// --------------------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::GetManufacturer(const TInt64 aSlpId, TDes& aManuName)
    {
    if(!IsSlpExists(aSlpId))
            return KErrArgument;
    TBuf<KGenericStringLen> manuname;

    iSQLString.Copy(KSelect);
    iSQLString.Append(KColManuName);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.AppendNum(aSlpId);
    iSQLString.Append(KQuotes);
    
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb,iSQLString);
    PrintErrorMessage(iSQLString,err,13);
    if(err != KErrNone)
        return KErrGeneral;
       
    TInt columnIndexSLPName = stmt.ColumnIndex(KColManuName);
    aManuName.Zero();

    while((err=stmt.Next())==KSqlAtRow)
        {
        stmt.ColumnText(columnIndexSLPName,manuname);
        }   
    
    stmt.Close();
    
    if(aManuName.MaxLength() < manuname.Length())
        return KErrOverflow;
    aManuName = manuname;
    return KErrNone;    
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetIAPName()
//
// This method is to Get IAP name of a SLP corresponding to a particular Slp Id
// ---------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::GetIAPName(const TInt64 aSlpId, TDes& aIAPName)
    {
    if(!IsSlpExists(aSlpId))
        return KErrArgument;
    TBuf<KGenericStringLen> iapname;

    iSQLString.Copy(KSelect);
    iSQLString.Append(KColIap);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.AppendNum(aSlpId);
    iSQLString.Append(KQuotes);
    
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb,iSQLString);
    PrintErrorMessage(iSQLString,err,13);
    if(err != KErrNone)
        return KErrGeneral;
       
    TInt columnIndexSLPName = stmt.ColumnIndex(KColIap);
    aIAPName.Zero();
    while((err=stmt.Next())==KSqlAtRow)
        {
        stmt.ColumnText(columnIndexSLPName,iapname);
        }   
    
    stmt.Close();
    if(aIAPName.MaxLength() < iapname.Length())
        return KErrOverflow;
    aIAPName = iapname;
    return KErrNone;    
    }
// ------------------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetVersion()
//
// This method is to Get Protocol Version of a SLP corresponding to a particular Slp Id
// ------------------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::GetVersion(const TInt64 aSlpId,TInt& aMajor, TInt& aMinor)
    {
    if(!IsSlpExists(aSlpId))
        return KErrArgument;

    iSQLString.Copy(KSelect);
    iSQLString.Append(KColProtocolVersionMajor);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.AppendNum(aSlpId);
    iSQLString.Append(KQuotes);
    
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb,iSQLString);
    PrintErrorMessage(iSQLString,err,13);
       
    TInt columnIndexVersion = stmt.ColumnIndex(KColProtocolVersionMajor);
    TInt protocolVersion = 0;
    while((err=stmt.Next())==KSqlAtRow)
        {
        protocolVersion = stmt.ColumnInt(columnIndexVersion);
        }   
    
    stmt.Close();
    aMajor = protocolVersion >> KVersionMajorMask;
    aMinor = protocolVersion & KVersionMinorMask;
    return KErrNone;    
    }
// -------------------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetSlpInfoFromId()
//
// This method is retrieve all configurable parameters corresponding to a particular SLP
// -------------------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::GetSlpInfoFromId(const TInt64 aSlpId,CServerParams* aParamValues)
    {
    if(!IsSlpExists(aSlpId))
        return KErrNotFound;

    iSQLString.Copy(KSelect);
    iSQLString.Append(KStar);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.AppendNum(aSlpId);
    iSQLString.Append(KQuotes);

    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);
    PrintErrorMessage(iSQLString,err,13);
    if(err != KErrNone)
        return KErrGeneral;
    
    TInt64 SLPId =  -1;
    TBuf<KMaxHSLPAddrLen> serverAddress;
    TBuf<KMaxIAPLen> Iap;
       TInt otherprops;
    TBuf<KShortStringLen> otherpropbuf;

    
    TBool serverEnabled = EFalse;
    TBool simChangeRemove = EFalse;
    TBool usageInHomeNw = EFalse;
    TBool editable = EFalse;
    TBool prodConfig = ETrue;
    
    TInt columnIndexSLPID = stmt.ColumnIndex(KColSLPId);
    TInt columnIndexSLPAddress = stmt.ColumnIndex(KColSLPAddress);
    
    TInt columnIndexIAP = stmt.ColumnIndex(KColIap);
    
    TInt columnIndexOtherProperties = stmt.ColumnIndex(KColOtherProperties);

    while((err=stmt.Next())==KSqlAtRow)
        {
        SLPId = stmt.ColumnInt(columnIndexSLPID);
        stmt.ColumnText(columnIndexSLPAddress,serverAddress);
        stmt.ColumnText(columnIndexIAP,Iap);  
        
        otherprops = stmt.ColumnInt(columnIndexOtherProperties);
        
        
        serverEnabled = otherprops & KServerEnabled;
        simChangeRemove = otherprops & KSimChangeRemove;
        usageInHomeNw = otherprops & KUsageInHomeNw;
        editable = otherprops & KEditable;
        prodConfig = otherprops & KProductConfigured;
        //logging
        otherpropbuf.Zero();
        otherpropbuf.AppendNum(otherprops);
        LogQuery(serverAddress);
        LogQuery(Iap)   ;     
        LogQuery(otherpropbuf);
        }   

    stmt.Close();
    
    aParamValues->Set(serverAddress,Iap,serverEnabled,simChangeRemove,usageInHomeNw,editable,SLPId);
    aParamValues->SetServerConfigurationType(prodConfig);
    
    return KErrNone;
    }
// --------------------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetSlpInfoFromAddress()
//
// This method is retrieve all configurable parameters corresponding to a particular SLP
// --------------------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::GetSlpInfoFromAddress(const TDesC& aServerAddress,CServerParams* aParamValues)
    {

    TBuf<KGenericStringLen> tempBuf;
    tempBuf.Copy(aServerAddress);
    tempBuf.Trim();
    iSQLString.Copy(KSelect);
    iSQLString.Append(KStar);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPAddress);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.Append(tempBuf);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCaseInsensitive);

    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);
    PrintErrorMessage(iSQLString,err,13);
    if(err != KErrNone)
        return KErrGeneral;
    
    TInt64 SLPId =  -1;
    TBuf<KMaxHSLPAddrLen> serverAddress;
    TBuf<KMaxIAPLen> Iap;
    
    TInt otherprops;
    TBuf<KShortStringLen> otherpropbuf;

    TBool serverEnabled = EFalse;
    TBool simChangeRemove = EFalse;
    TBool usageInHomeNw = EFalse;
    TBool editable = EFalse;
    TBool prodConfig = ETrue;
    
    TInt columnIndexSLPID = stmt.ColumnIndex(KColSLPId);
    TInt columnIndexSLPAddress = stmt.ColumnIndex(KColSLPAddress);
    
    TInt columnIndexIAP = stmt.ColumnIndex(KColIap);
    TInt columnIndexOtherProperties = stmt.ColumnIndex(KColOtherProperties);

    while((err=stmt.Next())==KSqlAtRow)
        {
        SLPId = stmt.ColumnInt(columnIndexSLPID);
        stmt.ColumnText(columnIndexSLPAddress,serverAddress);
             
        stmt.ColumnText(columnIndexIAP,Iap);  
        
        otherprops = stmt.ColumnInt(columnIndexOtherProperties);
        
        
        serverEnabled = otherprops & KServerEnabled;
        simChangeRemove = otherprops & KSimChangeRemove;
        usageInHomeNw = otherprops & KUsageInHomeNw;
        editable = otherprops & KEditable;
        prodConfig = otherprops & KProductConfigured;
        //logging
        otherpropbuf.Zero();
        otherpropbuf.AppendNum(otherprops);
        LogQuery(serverAddress);
        LogQuery(Iap);

        LogQuery(otherpropbuf);
        }   
    
    stmt.Close();
    
    if(SLPId == -1)//we did not find any server by the slp id that was requested for
        return KErrNotFound;
    
    aParamValues->Set(serverAddress,Iap,serverEnabled,simChangeRemove,usageInHomeNw,editable,SLPId);
    aParamValues->SetServerConfigurationType(prodConfig);
    return KErrNone;
    }
// ------------------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetDefaultServer()
//
// This method is retrieve all configurable parameters corresponding to the default SLP
// -----------------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::GetDefaultServer(CServerParams* aParamValues)
    {

    iSQLString.Copy(KSelect);
    iSQLString.Append(KStar);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KOrderBy);
    iSQLString.Append(KColPriority);
    iSQLString.Append(KCommaSeparator);
    iSQLString.Append(KColSLPId);
    
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);
    PrintErrorMessage(iSQLString,err,13);
    if(err != KErrNone)
        return KErrGeneral;
    TInt64 SLPId =  -1;
    TBuf<KMaxHSLPAddrLen> serverAddress;
    TBuf<KMaxIAPLen> Iap;
    TInt otherprops;
    TBuf<KShortStringLen> otherpropbuf; //for logging
    

    
    TBool serverEnabled = EFalse;
    TBool simChangeRemove = EFalse;
    TBool usageInHomeNw = EFalse;
    TBool editable = EFalse;
    TBool prodConfig = ETrue;
    
    TInt columnIndexSLPID = stmt.ColumnIndex(KColSLPId);
    TInt columnIndexSLPAddress = stmt.ColumnIndex(KColSLPAddress);
    TInt columnIndexIAP = stmt.ColumnIndex(KColIap);
    TInt columnIndexOtherProperties = stmt.ColumnIndex(KColOtherProperties);

    while((err=stmt.Next())==KSqlAtRow)
        {
        SLPId = stmt.ColumnInt(columnIndexSLPID);
        stmt.ColumnText(columnIndexSLPAddress,serverAddress);
             
        stmt.ColumnText(columnIndexIAP,Iap);  
        otherprops = stmt.ColumnInt(columnIndexOtherProperties);
       
        serverEnabled = otherprops & KServerEnabled;
        simChangeRemove = otherprops & KSimChangeRemove;
        usageInHomeNw = otherprops & KUsageInHomeNw;
        editable = otherprops & KEditable;
        prodConfig = otherprops & KProductConfigured;
        //logging
        otherpropbuf.Zero();
        otherpropbuf.AppendNum(otherprops);
        LogQuery(serverAddress);
        LogQuery(Iap);
        LogQuery(otherpropbuf);
        break; //obtain only the first record as that would be the default server
        }   
    if(SLPId == -1) // no record obtained so return error
        {
        PrintErrorMessage(_L(""),KErrNotFound,17);
        return KErrNotFound;
        }
    stmt.Close();

    aParamValues->Set(serverAddress,Iap,serverEnabled,simChangeRemove,usageInHomeNw,editable,SLPId);
    aParamValues->SetServerConfigurationType(prodConfig);
    return KErrNone;
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::SlpCount()
//
// This method is retrieve the count of slp entries present
// ---------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::SlpCount(TInt& aCount)
    {
    //Select all slps existing in the records
    TBuf<KGenericStringLen> sqlString;

    sqlString.Copy(KSelect);
    sqlString.Append(KCount);
    sqlString.Append(KOpeningBracket);
    sqlString.Append(KStar);
    sqlString.Append(KClosingBracket);
    sqlString.Append(KFrom);
    sqlString.Append(KCreateTableName);
    
    
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, sqlString);
    if(err != KErrNone)
        return KErrGeneral;
    TInt count = 0;
    TInt columnIndex0 = 0; 
    while((err=stmt.Next())==KSqlAtRow)
        {
        count = stmt.ColumnInt(columnIndex0); //count number of rows
        }
    stmt.Close();
    PrintErrorMessage(_L(""),count,16);
    if(count == 0 )
        {
        return KErrNotFound;
        }
    aCount = count;    
    return KErrNone;       
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::IsSlpExists()
//
// This method is to check is a slp entry exists corresponding to a particular SLP Id
// ---------------------------------------------------------------------------
TBool CSettingsDatabaseHandler::IsSlpExists(const TInt64 aSlpId)
    {
    //Select all slps existing in the records corresponing to the Slp Id
    TBuf<KGenericStringLen> sqlString;
    sqlString.Copy(KSelect);
    sqlString.Append(KCount);
    sqlString.Append(KOpeningBracket);
    sqlString.Append(KStar);
    sqlString.Append(KClosingBracket);
    sqlString.Append(KFrom);
    sqlString.Append(KCreateTableName);
    sqlString.Append(KWhere);
    sqlString.Append(KColSLPId);
    sqlString.Append(KEquals);
    sqlString.Append(KQuotes);
    sqlString.AppendNum(aSlpId);
    sqlString.Append(KQuotes);

		
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, sqlString);

    TInt count = 0;
    TInt columnIndex0 = 0; 
    while((err=stmt.Next())==KSqlAtRow)
        {
        count = stmt.ColumnInt(columnIndex0); //count number of rows
        }
    stmt.Close();
    if(count == 0)
        {
        PrintErrorMessage(_L(""),EFalse,15);
        return EFalse;
        }
    PrintErrorMessage(_L(""),ETrue,15);
    return ETrue;
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::IsSlpExists()
//
// This method is to check is a slp entry exists corresponding to a particular 
// SLP Address
// ---------------------------------------------------------------------------
TBool CSettingsDatabaseHandler::IsSlpExists(const TDesC& aServerAdress)
    {
    //Select all slps existing in the records corresponding to the Server Address 

    TBuf<KGenericStringLen> tempBuf;
    tempBuf.Copy(aServerAdress);
    tempBuf.Trim();
    iSQLString.Copy(KSelect);
    iSQLString.Append(KCount);
    iSQLString.Append(KOpeningBracket);
    iSQLString.Append(KStar);
    iSQLString.Append(KClosingBracket);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPAddress);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.Append(tempBuf);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCaseInsensitive);

    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);

    TInt count = 0;
    TInt columnIndex0 = 0; 
    while((err=stmt.Next())==KSqlAtRow)
        {
        count = stmt.ColumnInt(columnIndex0); //count number of rows
        }
    stmt.Close();
    if(count == 0)
        {
        PrintErrorMessage(_L(""),EFalse,15);
        return EFalse;
        }
    PrintErrorMessage(_L(""),ETrue,15);
    return ETrue;    
    }
TBool  CSettingsDatabaseHandler::IsSlpExists(const TDesC& aServerAddress,TInt64& aSlpId)
    {
    //Select all slps existing in the records corresponding to the Server Address 

    iSQLString.Copy(KSelect);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPAddress);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.Append(aServerAddress);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCaseInsensitive);

    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);
    aSlpId = -1;
    TInt count = 0; // count the number of slps present in the records
    TInt columnIndexSLPID = stmt.ColumnIndex(KColSLPId);
    while((err=stmt.Next())==KSqlAtRow)
        {
        aSlpId = stmt.ColumnInt(columnIndexSLPID);
        count++;
        }
    stmt.Close();
    if(count == 0)
        {
        PrintErrorMessage(_L(""),EFalse,15);
        return EFalse;
        }
    PrintErrorMessage(_L(""),ETrue,15);
    return ETrue; 
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetDefaultCRValues()
//
// This method is used to retrieve default settings value from the cen rep
// ---------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::GetDefaultCRValues(TInt aKey,TInt& aValue)
    {
    TInt err;            

    err = iSettingsRep->Get(aKey, aValue);
    if (err != KErrNone)
        return err;         
    return KErrNone;
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetDBPathFromCR()
//
// This method is used to retrieve settings db path value from the cen rep
// ---------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::GetDBPathFromCR(TDes& aPath)
    {
    TInt err;            

    err = iSettingsNotifierRep->Get(KSuplSettigsDBPath, aPath);
    if (err != KErrNone)
        return err;         
    return KErrNone;
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::StoreDBPathToCR()
//
// This method is used to store settings db path value to the cen rep
// ---------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::StoreDBPathToCR(TDes& aPath)
    {
    TInt err;            
    err = iSettingsNotifierRep->Set(KSuplSettigsDBPath, aPath);
    if (err != KErrNone)
        return err;   
    return KErrNone;
    }

//-----------------------------------------------------------------------------
// CSettingsDatabaseHandler::NotifyDBChange()
//
// This method is used to notify that a change has occured in the DB via the CR 
// ----------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::NotifyDBChange(MSuplSettingsObserver::TSuplSettingsEventType aChangedEventType,TInt64 aSlpId)
    {
    TInt err;
    TReal idChanged = aSlpId;
    err = iSettingsNotifierRep->Set(KSuplSettingsDBChangedSLPId,idChanged);
    if(err != KErrNone)
        return err;
    TInt curEvent;
    TInt keyValue;
    err = iSettingsNotifierRep->Get(KSuplSettingsDBEvent,curEvent);
    if(aChangedEventType == (MSuplSettingsObserver::TSuplSettingsEventType)curEvent)
        {
        keyValue = aChangedEventType+1;
        }
    else
        keyValue = aChangedEventType;
    err = iSettingsNotifierRep->Set(KSuplSettingsDBEvent,keyValue);
    if(err != KErrNone)
        return err;
    return KErrNone;
    }
//----------------------------------------------------------------------------
// CSettingsDatabaseHandler::ChangePriority()
//
// This method is used to alter priority of a particular server
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::ChangePriorityL(TInt aSlpId,TInt aValue,TBool aDirection)
    {
    if(!IsSlpExists(aSlpId))
        User::Leave(KErrNotFound);
    if(aValue <= 0)
        User::Leave(KErrArgument);
    TInt count;
    SlpCount(count);

    
    TInt priorityToSet;
    
    GetPriorityL(aSlpId,priorityToSet);
    
    if(aDirection)
        {
        priorityToSet -= aValue; //move up 
        } 
    else
        {
        priorityToSet += aValue;//move down
        }
    if(priorityToSet > count || priorityToSet <= 0)
        User::Leave(KErrArgument);
    
    //Select all slps existing in the records corresponding to the Server Address 
   
    iSQLString.Copy(KSelect);
    iSQLString.Append(KStar);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColPriority);
    if(aDirection)
        iSQLString.Append(KGreaterOrEquals);
    else
        iSQLString.Append(KGreaterThan);
    iSQLString.Append(KQuotes);
    iSQLString.AppendNum(priorityToSet);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KOrderBy);
    iSQLString.Append(KColPriority);
    iSQLString.Append(KCommaSeparator);
    iSQLString.Append(KColSLPId);
    
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);
    TInt64 SLPId;
    TInt priority;
    TInt columnIndexSLPID = stmt.ColumnIndex(KColSLPId);
    TInt columnIndexPriority = stmt.ColumnIndex(KColPriority);
    count = 0; // count the number of slps present in the records
    RArray<TInt64> slpIndexArray;
    //Get all slp ids whose priorities wil get affected
    while((err=stmt.Next())==KSqlAtRow)
        {
        SLPId = stmt.ColumnInt64(columnIndexSLPID);
        if(SLPId != aSlpId)
            slpIndexArray.AppendL(stmt.ColumnInt64(columnIndexSLPID));
        }
    stmt.Reset(); 
    stmt.Close();
    //update the priority of the requested server first
    if(!aDirection)
        priorityToSet++;
    iSQLString.Zero();
    iSQLString.Append(KUpdateRecord);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KSET);
    iSQLString.Append(KColPriority);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(priorityToSet); 
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(aSlpId);
    
    err = iDb.Exec(iSQLString);
    iSQLString.Zero();
        
    iSQLString.Append(KUpdateRecord);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KSET);
    iSQLString.Append(KColPriority);
    iSQLString.Append(KEquals);
    iSQLString.Append(KValuePriority);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.Append(KValueSlpId);
    
    err = stmt.Prepare(iDb,iSQLString);
    //update priority of other slps
    TInt priorityParamIndex = stmt.ParameterIndex(KValuePriority);
    TInt slpIdParamIndex = stmt.ParameterIndex(KValueSlpId);
    
    priority = priorityToSet + 1;
    for(TInt i=0 ;i < slpIndexArray.Count(); i++,priority++)
        {
        err = stmt.BindInt(priorityParamIndex, priority);
        err = stmt.BindInt(slpIdParamIndex, slpIndexArray[i]);
        err = stmt.Exec();
        err = stmt.Reset();
        }
    slpIndexArray.Close();
    stmt.Close();
    
    NormalizePriorityL();
    
    err = NotifyDBChange(MSuplSettingsObserver::ESuplSettingsDBUpdateEvent,0); //indicates that all the records may have been altered
    LogAllSLP();
    }
//----------------------------------------------------------------------------
//CSettingsDatabaseHandler::GetPriority()
//This method is used to retrieve priority of a particular SLP
//
//----------------------------------------------------------------------------
void CSettingsDatabaseHandler::GetPriorityL(TInt aSlpId,TInt& aPriority)
    {

    iSQLString.Copy(KSelect);
    iSQLString.Append(KStar);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.AppendNum(aSlpId);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KOrderBy);
    iSQLString.Append(KColPriority);
    iSQLString.Append(KCommaSeparator);
    iSQLString.Append(KColSLPId);
    
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);
    if(err != KErrNone)
            User::LeaveIfError(KErrGeneral);
    TInt priorityParamIndex = stmt.ColumnIndex(KColPriority);
    while((err=stmt.Next())==KSqlAtRow)
        {
        aPriority = stmt.ColumnInt(priorityParamIndex);
        break;
        }
    stmt.Close();
    }

//----------------------------------------------------------------------------
// CSettingsDatabaseHandler::SetPriority()
//
// This method is used to alter priority of a particular server
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::GetMaxPriorityL(TInt& aPriorityValue)
    {
    TInt count;
    SlpCount(count);
    if(count <= 0)//no slp entries exist
        {
        aPriorityValue = KMinPriorityValue;
        return;
        }
    
    //Select all slps existing in the records corresponding to the Server Address 
    TBuf<KGenericStringLen> sqlString;
    sqlString.Copy(KSelect);
    sqlString.Append(KMax);
    sqlString.Append(KOpeningBracket);
    sqlString.Append(KColPriority);
    sqlString.Append(KClosingBracket);
    sqlString.Append(KFrom);
    sqlString.Append(KCreateTableName);
    

    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, sqlString);
    TInt columnIndex0 = 0; 

    while((err=stmt.Next())==KSqlAtRow)
        {
        aPriorityValue = stmt.ColumnInt(columnIndex0);
        }   
    aPriorityValue++;
    stmt.Close();
    }

//----------------------------------------------------------------------------
// CSettingsDatabaseHandler::SetPriority()
//
// This method is used to alter priority of a particular server
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::SetPriorityL(TInt aSlpId,TInt aValue)
    {
    if(aValue <= 0)
        User::Leave(KErrArgument);
    TInt count;
    SlpCount(count);
    if(aValue > count)
        User::Leave(KErrArgument);
    //Select all slps existing in the records corresponding to the Server Address 

    iSQLString.Copy(KSelect);
    iSQLString.Append(KStar);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColPriority);
    iSQLString.Append(KGreaterOrEquals);
    iSQLString.Append(KQuotes);
    iSQLString.AppendNum(aValue);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KOrderBy);
    iSQLString.Append(KColPriority);
    iSQLString.Append(KCommaSeparator);
    iSQLString.Append(KColSLPId);
    
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);
    TInt64 SLPId;
    TInt priority;
    TInt columnIndexSLPID = stmt.ColumnIndex(KColSLPId);
    TInt columnIndexPriority = stmt.ColumnIndex(KColPriority);
    count = 0; // count the number of slps present in the records
    RArray<TInt64> slpIndexArray;
    //Get all slp ids whose priorities wil get affected
    while((err=stmt.Next())==KSqlAtRow)
        {
        SLPId = stmt.ColumnInt64(columnIndexSLPID);
        if(SLPId != aSlpId)
            slpIndexArray.AppendL(stmt.ColumnInt64(columnIndexSLPID));
        }
    stmt.Reset(); 
    stmt.Close();
    //update the priority of the requested server first
    iSQLString.Zero();
    iSQLString.Append(KUpdateRecord);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KSET);
    iSQLString.Append(KColPriority);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(aValue); 
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(aSlpId);
    
    err = iDb.Exec(iSQLString);
    iSQLString.Zero();
        
    iSQLString.Append(KUpdateRecord);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KSET);
    iSQLString.Append(KColPriority);
    iSQLString.Append(KEquals);
    iSQLString.Append(KValuePriority);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.Append(KValueSlpId);
    
    err = stmt.Prepare(iDb,iSQLString);
    //update priority of other slps
    TInt priorityParamIndex = stmt.ParameterIndex(KValuePriority);
    TInt slpIdParamIndex = stmt.ParameterIndex(KValueSlpId);
    
    priority = aValue + 1;
    for(TInt i=0 ;i < slpIndexArray.Count(); i++,priority++)
        {
        err = stmt.BindInt(priorityParamIndex, priority);
        err = stmt.BindInt(slpIdParamIndex, slpIndexArray[i]);
        err = stmt.Exec();
        err = stmt.Reset();
        }
    slpIndexArray.Close();
    stmt.Close();
    
    err = NotifyDBChange(MSuplSettingsObserver::ESuplSettingsDBUpdateEvent,0); //indicates that all the records may have been altered
    LogAllSLP();
    }
//--------------------------------------------------------------------------------
// CSettingsDatabaseHandler::NormalizePriorityL()
//
// This method is used to order the priority values in proper incremental sequence
// -------------------------------------------------------------------------------
void CSettingsDatabaseHandler::NormalizePriorityL()
    { 
    TInt count;
    SlpCount(count);
    if(count <= 0)
        return;

    iSQLString.Copy(KSelect);
    iSQLString.Append(KStar);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KOrderBy);
    iSQLString.Append(KColPriority);
    iSQLString.Append(KCommaSeparator);
    iSQLString.Append(KColSLPId);

    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);
    User::LeaveIfError(err);

    TInt columnIndexSLPID = stmt.ColumnIndex(KColSLPId);
    RArray<TInt64> slpIdArray;
    //Get all slp ids currently in the database
    while((err=stmt.Next())==KSqlAtRow)
        {
        slpIdArray.AppendL(stmt.ColumnInt64(columnIndexSLPID));
        }
    stmt.Reset();
    
    iSQLString.Zero();

    iSQLString.Append(KUpdateRecord);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KSET);
    iSQLString.Append(KColPriority);
    iSQLString.Append(KEquals);
    iSQLString.Append(KValuePriority);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.Append(KValueSlpId);

    err = stmt.Prepare(iDb,iSQLString);
    //update priority of other slps
    TInt priorityParamIndex = stmt.ParameterIndex(KValuePriority);
    TInt slpIdParamIndex = stmt.ParameterIndex(KValueSlpId);

    TInt priorityToSet = KMinPriorityValue;
    for(TInt i=0 ;i < slpIdArray.Count(); i++,priorityToSet++)
        {
        err = stmt.BindInt(priorityParamIndex, priorityToSet);
        err = stmt.BindInt(slpIdParamIndex, slpIdArray[i]);
        err = stmt.Exec();
        err = stmt.Reset();
        }
    slpIdArray.Close();
    stmt.Close();
   
    }
//----------------------------------------------------------------------------
// CSettingsDatabaseHandler::RemoveOnSimChangeL()
//
// This method is used to delete all entries that have the sim change remove 
// property set
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::RemoveOnSimChangeL()
    {        

    iSQLString.Copy(KSelect);
    iSQLString.Append(KStar);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName); // retrieve list of all SLPs 
    iSQLString.Append(KOrderBy);
    iSQLString.Append(KColPriority);
    iSQLString.Append(KCommaSeparator);
    iSQLString.Append(KColSLPId);
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);
    if(err != KErrNone)
        User::Leave(KErrGeneral);
    TInt64 slpId = -1;
    TInt columnIndexSLPID = stmt.ColumnIndex(KColSLPId);
    TBool simChangeRemove;
    while((err=stmt.Next())==KSqlAtRow)
        {
        
        slpId = stmt.ColumnInt64(columnIndexSLPID);
        GetOtherPropertiesL(slpId,CSuplSettingsInternal::ESimChangeRemove,simChangeRemove);
        if(simChangeRemove) //if sim change remove flag is set delete it
            DeleteSLPRecordL(slpId); 
        }
        
    stmt.Close();
    LogAllSLP();
    }


// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::CreateSessionTableL()
//
// Creates the SUPL session settings table. Leaves, if the table cannot be created.
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::CreateSessionTableL()
    {
    iSQLString.Copy(KCreateTable);
    iSQLString.Append(KSessionTableName);
    iSQLString.Append(KOpeningBracket);

    iSQLString.Append(KColSessionId);
    iSQLString.Append(KDataTypeInteger);
    iSQLString.Append(KPrimaryKey);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColSessionName);
    iSQLString.Append(KDataTypeString);
    iSQLString.Append(KOpeningBracket);
    iSQLString.AppendNum(KMaxTriggerSessionNameLen);
    iSQLString.Append(KClosingBracket);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColNotificationPresent);
    iSQLString.Append(KDataTypeInteger);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColTriggerNotificationStatus);
    iSQLString.Append(KDataTypeInteger);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColTriggerType);
    iSQLString.Append(KDataTypeInteger);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColRequestType);
    iSQLString.Append(KDataTypeInteger);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColOutstandingTrigger);
    iSQLString.Append(KDataTypeInteger);
    iSQLString.Append(KCommaSeparator); 

    iSQLString.Append(KColInterval);
    iSQLString.Append(KDataTypeInteger);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColEndTime);
    iSQLString.Append(KDataTypeInteger);
        
    iSQLString.Append(KClosingBracket);
    
    TInt err = iDb.Exec(iSQLString);
    
    PrintErrorMessage(iSQLString,err,1);                            
            
    User::LeaveIfError(err);
    
    }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::InsertSessionRecordL()
//
// This method is used to insert session record into the SUPL session DB
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::InsertSessionRecordL(const CTriggerParams* aSessionParamValues)
    {    
    if( !aSessionParamValues )
        {
        User::Leave(KErrArgument);
        }
    HBufC* sessionName = HBufC::NewL( KMaxTriggerSessionNameLen);
    
    
    TInt64 sessionId;
    TBool notificationPresent;
    TBool triggerNotificationStatus;
    CTriggerParams::TTriggerType triggerType;
    CTriggerParams::TRequestType requestType;
    TUint64 outstandingTrigger;
    TUint64 interval;
    TTime endTime;
    TInt ret = aSessionParamValues->Get(sessionId,sessionName->Des(),notificationPresent,triggerNotificationStatus,
                             triggerType,requestType,outstandingTrigger,interval );
                             
         aSessionParamValues->GetTriggerEndTime(endTime);
         
    TInt64 endTimeInt;
    endTimeInt = endTime.Int64();
    if(ret == KErrNotFound)
        {
        delete sessionName;
        User::Leave(KErrGeneral);        
        }
          
    iSQLString.Copy(KInsertIntoTable);
    iSQLString.Append(KSessionTableName);
    iSQLString.Append(KOpeningBracket);

    iSQLString.Append(KColSessionId);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColSessionName);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColNotificationPresent);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColTriggerNotificationStatus);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColTriggerType);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColRequestType);
    iSQLString.Append(KCommaSeparator);
        
    iSQLString.Append(KColOutstandingTrigger);
    iSQLString.Append(KCommaSeparator); 

    iSQLString.Append(KColInterval);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColEndTime);
    
    iSQLString.Append(KClosingBracket);
    
    iSQLString.Append(KValues);

    iSQLString.Append(KOpeningBracket);

    iSQLString.AppendNum(sessionId);
    iSQLString.Append(KCommaSeparator);
            
    iSQLString.Append(KQuotes);
    iSQLString.Append(sessionName->Des());
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.AppendNum(notificationPresent);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.AppendNum(triggerNotificationStatus);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.AppendNum(triggerType);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.AppendNum(requestType);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.AppendNum(outstandingTrigger);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.AppendNum(interval);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.AppendNum(endTimeInt);
    
    iSQLString.Append(KClosingBracket);

    delete sessionName;   
    TInt err = iDb.Exec(_L("BEGIN")); 
    err = iDb.Exec(iSQLString);
    if(err < 0)
            User::Leave(KErrGeneral);  
    iDb.Exec(_L("COMMIT")); 

    err = NotifySessionDBChange(MSuplSessionObserver::ESuplSessionDBAddEvent,sessionId);
    
    }
    
// ----------------------------------------------------------------------------
// CSettingsDatabaseHandler::DeleteSessionRecordL()
//
// This method is used to delete a session record corresponding to the given session ID
// ----------------------------------------------------------------------------
void CSettingsDatabaseHandler::DeleteSessionRecordL(TInt64 aSessionId)
    {
    if(!IsSessionExists(aSessionId)) //no session exists with the given id so return error
        {
        User::Leave(KErrNotFound);
        }

    iSQLString.Copy(KDeleteRecord);
    iSQLString.Append(KSessionTableName);
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSessionId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(aSessionId);

    TInt err = iDb.Exec(iSQLString);
    
    if(err < 0)
        User::Leave(KErrGeneral);
    err = NotifySessionDBChange(MSuplSessionObserver::ESuplSessionDBDeleteEvent,aSessionId); // notify about a change in the database

    }

// ----------------------------------------------------------------------------
// CSettingsDatabaseHandler::DeleteAllSessionRecordsL()
//
// This method is used to delete all session records from session table
// ----------------------------------------------------------------------------
void CSettingsDatabaseHandler::DeleteAllSessionRecordsL()
    {
    iSQLString.Copy(KDeleteRecord);
    iSQLString.Append(KSessionTableName);
   
    TInt err = iDb.Exec(iSQLString);
    
    if(err < 0)
      User::Leave(KErrGeneral);
     }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::UpdateSessionRecordL()
//
// This method is used to update a column of string value corresponding 
// to a particular session
// ---------------------------------------------------------------------------
       
void CSettingsDatabaseHandler::UpdateSessionRecordL(TInt64 aSessionId,CSuplSettingsInternal::TSessionParam aParameter,TUint64 aValue)
    {
    if(!IsSessionExists(aSessionId)) //no session exists with the given id so return error
        {
        User::Leave(KErrNotFound);
        }
   
    iSQLString.Copy(KUpdateRecord);
    iSQLString.Append(KSessionTableName);
    iSQLString.Append(KSET);

    FormSessionQueryL(aParameter,aValue,iSQLString);

    iSQLString.Append(KWhere);
    iSQLString.Append(KColSessionId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(aSessionId);

    TInt err = iDb.Exec(iSQLString);
    if(err < 0)
        User::Leave(KErrGeneral);
    err = NotifySessionDBChange(MSuplSessionObserver::ESuplSessionDBUpdateEvent,aSessionId);

    }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::FormSessionQueryL()
//
// Forms part of the query string, used to form a query string that contains 
// a String value in it 
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::FormSessionQueryL(CSuplSettingsInternal::TSessionParam aParameter,TUint64 aValue,TDes& aQueryString)
    {
    switch(aParameter)
        {
        case CSuplSettingsInternal::ETriggerNotificationStatus: 
            {
            aQueryString.Append(KColTriggerNotificationStatus);
            break;
            }
        case CSuplSettingsInternal::EOutstandingTriggers: 
            {
            aQueryString.Append(KColOutstandingTrigger);
            break;
            }    

        default:
            User::Leave(KErrArgument);
        }

    aQueryString.Append(KEquals);
    aQueryString.Append(KQuotes);
    aQueryString.AppendNum(aValue);
    aQueryString.Append(KQuotes);
    }               


// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetAllSessionsL()
//
// Returns a list of all sessions in the order of priority
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::GetAllSessionsL(RPointerArray<CTriggerParams>& aParamValues)
    {
    iSQLString.Copy(KSelect);
    iSQLString.Append(KStar);
    iSQLString.Append(KFrom);
    iSQLString.Append(KSessionTableName); // retrieve list of all session in order of priority
    iSQLString.Append(KOrderBy);
    iSQLString.Append(KColSessionId);
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, iSQLString);
    
    if(err != KErrNone)
        {
        stmt.Close();
        User::Leave(KErrGeneral);
        }
            
    
    TInt64 sessionId;
    TBool notificationPresent;
    TBool triggerNotificationStatus;
    CTriggerParams::TTriggerType triggerType;
    CTriggerParams::TRequestType requestType;
    TUint64 outstandingTrigger;
    TUint64 interval;
    TBuf<KGenericStringLen> sessionName;
    TTime endTime;
    TInt64 endTimeInt;
        
    TInt columnIndexSessionID = stmt.ColumnIndex(KColSessionId);
    TInt columnIndexSessionName = stmt.ColumnIndex(KColSessionName);
    TInt columnIndexNotificationPresent = stmt.ColumnIndex(KColNotificationPresent);
    TInt columnIndexTriggerNotificationStatus = stmt.ColumnIndex(KColTriggerNotificationStatus);
    TInt columnIndexTriggerType = stmt.ColumnIndex(KColTriggerType);
    TInt columnIndexRequestType = stmt.ColumnIndex(KColRequestType);
    TInt columnIndexOutstandingTrigger = stmt.ColumnIndex(KColOutstandingTrigger);
    TInt columnIndexInterval = stmt.ColumnIndex(KColInterval);
    TInt columnIndexEndTime = stmt.ColumnIndex(KColEndTime);

    // Reset client array
    aParamValues.Reset();
    while((err=stmt.Next())==KSqlAtRow)
        {
        CTriggerParams *params = CTriggerParams::NewL();
        CleanupStack::PushL(params);
        sessionId = stmt.ColumnInt(columnIndexSessionID);
        stmt.ColumnText(columnIndexSessionName,sessionName);
        notificationPresent = stmt.ColumnInt(columnIndexNotificationPresent);
        triggerNotificationStatus = stmt.ColumnInt(columnIndexTriggerNotificationStatus);
        triggerType = (CTriggerParams::TTriggerType) stmt.ColumnInt(columnIndexTriggerType);
        requestType = (CTriggerParams::TRequestType) stmt.ColumnInt(columnIndexRequestType);
        outstandingTrigger = stmt.ColumnInt(columnIndexOutstandingTrigger);
        interval = stmt.ColumnInt(columnIndexInterval);
        endTimeInt = stmt.ColumnInt64(columnIndexEndTime);
        endTime = endTimeInt;
        params->Set(sessionId,sessionName,notificationPresent,triggerNotificationStatus,triggerType,requestType,outstandingTrigger,interval);
				params->SetTriggerEndTime(endTime);        
        aParamValues.AppendL(params);//ownership transferred to RPointerArray
        CleanupStack::Pop(params);
        }   

    stmt.Close();
    if(aParamValues.Count()==0)
        {
        User::Leave(KErrNotFound);
        }
    
    }


// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetSessionL()
//
// Returns the session parameters for a given session id
// ---------------------------------------------------------------------------
void CSettingsDatabaseHandler::GetSessionL(TInt64 aSessionId,
                                                CTriggerParams*& aParamValues)
    {
    if( !IsSessionExists(aSessionId) ) //no session exists with the given id so return error
        {
        User::Leave(KErrNotFound);
        }
    iSQLString.Copy(KSelect);
    iSQLString.Append(KStar);
    iSQLString.Append(KFrom);
    iSQLString.Append(KSessionTableName); // retrieve list of all session in order of priority
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSessionId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(aSessionId);
        
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb,iSQLString);
    if(err != KErrNone)
        {
        stmt.Close();
        User::Leave(KErrGeneral);
        }
    
    TInt64 sessionId;
    TBool notificationPresent;
    TBool triggerNotificationStatus;
    CTriggerParams::TTriggerType triggerType;
    CTriggerParams::TRequestType requestType;
    TUint64 outstandingTrigger;
    TUint64 interval;
    TBuf<KGenericStringLen> sessionName;
    TTime endTime;
    TInt64 endTimeInt;
    
        
    TInt columnIndexSessionID = stmt.ColumnIndex(KColSessionId);
    TInt columnIndexSessionName = stmt.ColumnIndex(KColSessionName);
    TInt columnIndexNotificationPresent = stmt.ColumnIndex(KColNotificationPresent);
    TInt columnIndexTriggerNotificationStatus = stmt.ColumnIndex(KColTriggerNotificationStatus);
    TInt columnIndexTriggerType = stmt.ColumnIndex(KColTriggerType);
    TInt columnIndexRequestType = stmt.ColumnIndex(KColRequestType);
    TInt columnIndexOutstandingTrigger = stmt.ColumnIndex(KColOutstandingTrigger);
    TInt columnIndexInterval = stmt.ColumnIndex(KColInterval);
    TInt columnIndexEndTime = stmt.ColumnIndex(KColEndTime);
    
    while((err=stmt.Next())==KSqlAtRow)
        {
        sessionId = stmt.ColumnInt(columnIndexSessionID);
        stmt.ColumnText(columnIndexSessionName,sessionName);
        notificationPresent = stmt.ColumnInt(columnIndexNotificationPresent);
        triggerNotificationStatus = stmt.ColumnInt(columnIndexTriggerNotificationStatus);
        triggerType = (CTriggerParams::TTriggerType) stmt.ColumnInt(columnIndexTriggerType);
        requestType = (CTriggerParams::TRequestType) stmt.ColumnInt(columnIndexRequestType);
        outstandingTrigger = stmt.ColumnInt(columnIndexOutstandingTrigger);
        interval = stmt.ColumnInt(columnIndexInterval);
        endTimeInt = stmt.ColumnInt64(columnIndexEndTime);
        endTime = endTimeInt;
        aParamValues->Set(sessionId,sessionName,notificationPresent,triggerNotificationStatus,triggerType,requestType,outstandingTrigger,interval);
        aParamValues->SetTriggerEndTime(endTime);
        }   

    stmt.Close();
    }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::SessionCount()
//
// This method is retrieve the count of session entries present
// ---------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::SessionCount(TInt& aCount)
    {
    //Select all slps existing in the records
    TBuf<KGenericStringLen> sqlString;

    sqlString.Copy(KSelect);
    sqlString.Append(KCount);
    sqlString.Append(KOpeningBracket);
    sqlString.Append(KStar);
    sqlString.Append(KClosingBracket);
    sqlString.Append(KFrom);
    sqlString.Append(KSessionTableName);
    
    
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, sqlString);
    if(err != KErrNone)
        {
        stmt.Close();
        return KErrGeneral;
        }
        
    aCount = 0;
    TInt columnIndex0 = 0; 
    while((err=stmt.Next())==KSqlAtRow)
        {
        aCount = stmt.ColumnInt(columnIndex0); //count number of rows
        }
    stmt.Close();
    
    if(aCount == 0 )
        {
        return KErrNotFound;
        }
    return KErrNone;       
    }
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::IsSessionExists()
//
// This method is to check if a session entry exists corresponding to a particular session Id
// ---------------------------------------------------------------------------
TBool CSettingsDatabaseHandler::IsSessionExists(const TInt64 aSessionId)
    {
    //Select all sessions existing in the records corresponing to the session Id
    
    TBuf<KGenericStringLen> sqlString;
    
    sqlString.Copy(KSelect);
    sqlString.Append(KCount);
    sqlString.Append(KOpeningBracket);
    sqlString.Append(KStar);
    sqlString.Append(KClosingBracket);
    sqlString.Append(KFrom);
    sqlString.Append(KSessionTableName);
    sqlString.Append(KWhere);
    sqlString.Append(KColSessionId);
    sqlString.Append(KEquals);
    sqlString.AppendNum(aSessionId);
   
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb, sqlString);
   
    TInt count = 0;
    TInt columnIndex0 = 0; 
    while((err=stmt.Next())==KSqlAtRow)
        {
        count = stmt.ColumnInt(columnIndex0); //count number of rows
        }
    stmt.Close();
    if(count == 0)
        {
        return EFalse;
        }
    
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::GetServerExtensionParamsL()
//
// This method is used to retrieve extended parameters of given server
// ---------------------------------------------------------------------------
void  CSettingsDatabaseHandler::GetServerExtensionParamsL( TInt64 aSlpId,CServerExtensionParams* aServerExtnParams )
	{
	if(!IsSlpExists(aSlpId))
		{
		User::Leave(KErrNotFound);
		}
    iSQLString.Copy(KSelect);
    iSQLString.Append(KStar);
    iSQLString.Append(KFrom);
    iSQLString.Append(KCreateTableName); // retrieve list of all session in order of priority
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(aSlpId);
        
    RSqlStatement stmt;
    TInt err = stmt.Prepare(iDb,iSQLString);
    if(err != KErrNone)
        {
        stmt.Close();
        User::Leave(err);
        }
    
    TBuf<KMaxServerIdLen> serverId;
    TBuf<KMaxIAPLen> iap1;
    TBuf<KMaxIAPLen> iap2;
    TBuf<KMaxServerAddLen> serverAddr;
    TBuf<KMaxServerAddTypeLen> serverAddrType;
   
    TInt columnIndexServerId = stmt.ColumnIndex(KColServerId);
    TInt columnIndexIap1 = stmt.ColumnIndex(KColIap1);
    TInt columnIndexIap2 = stmt.ColumnIndex(KColIap2);
    TInt columnIndexServerAddress = stmt.ColumnIndex(KColServerAddress);
    TInt columnIndexServerAddType = stmt.ColumnIndex(KColServerAddType);
    
    while((err=stmt.Next())==KSqlAtRow)
        {
        stmt.ColumnText(columnIndexServerId,serverId);
        stmt.ColumnText(columnIndexIap1,iap1);
        stmt.ColumnText(columnIndexIap2,iap2);
        stmt.ColumnText(columnIndexServerAddress,serverAddr);
        stmt.ColumnText(columnIndexServerAddType,serverAddrType);
                
        aServerExtnParams->Set(serverId,iap1,iap2,serverAddr,serverAddrType);
        }   

    stmt.Close();
	}
	
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::UpdateServerExtensionParamsL()
//
// This method is used to set extended parameters of a given server.
// ---------------------------------------------------------------------------
void  CSettingsDatabaseHandler::UpdateServerExtensionParamsL( TInt64 aSlpId,CServerExtensionParams* aServerExtnParams )
	{
	if(!IsSlpExists(aSlpId))
		{
		User::Leave(KErrNotFound);
		}
	
    HBufC* serverId = HBufC::NewLC( KMaxServerIdLen);
    HBufC* iap1 = HBufC::NewLC( KMaxIAPLen);
    HBufC* iap2 = HBufC::NewLC( KMaxIAPLen);
    HBufC* serverAddr = HBufC::NewLC( KMaxServerAddLen);
    HBufC* serverAddrType = HBufC::NewLC( KMaxServerAddTypeLen);
        
    TInt ret = 0;
    ret = aServerExtnParams->Get(serverId->Des(),iap1->Des(),iap2->Des(),serverAddr->Des(),serverAddrType->Des());
    if(ret == KErrNotFound)
        {
        CleanupStack::PopAndDestroy(5); 
        User::Leave(KErrArgument); 
        }
   
    iSQLString.Copy(KUpdateRecord);
    iSQLString.Append(KCreateTableName);
    iSQLString.Append(KSET);
    
    iSQLString.Append(KColServerId);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.Append(*serverId);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);

    iSQLString.Append(KColIap1);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.Append(*iap1);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColIap2);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.Append(*iap2);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColServerAddress);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.Append(*serverAddr);
    iSQLString.Append(KQuotes);
    iSQLString.Append(KCommaSeparator);
    
    iSQLString.Append(KColServerAddType);
    iSQLString.Append(KEquals);
    iSQLString.Append(KQuotes);
    iSQLString.Append(*serverAddrType);
    iSQLString.Append(KQuotes);
    
    iSQLString.Append(KWhere);
    iSQLString.Append(KColSLPId);
    iSQLString.Append(KEquals);
    iSQLString.AppendNum(aSlpId);  
    
    TInt err = iDb.Exec(iSQLString); 
    PrintErrorMessage(iSQLString,err,6); 
    CleanupStack::PopAndDestroy(5);
    if(err < 0)
        {
        User::Leave(err);
        }
	}	



//-----------------------------------------------------------------------------
// CSettingsDatabaseHandler::NotifySessionDBChange()
//
// This method is used to notify that a change has occured in the DB via the CR 
// ----------------------------------------------------------------------------
TInt CSettingsDatabaseHandler::NotifySessionDBChange(MSuplSessionObserver::TSuplSessionEventType aChangedEventType,TInt64 aSessionId)
    {
    TInt err;
    TReal idChanged = aSessionId;
    err = iSettingsNotifierRep->Set(KSuplSessionDBChangedSessionId,idChanged);
    if(err != KErrNone)
        {
        return err;
        }
    TInt curEvent;
    TInt keyValue;
    err = iSettingsNotifierRep->Get(KSuplSessionDBEvent,curEvent);
    if(err != KErrNone)
        {
        return err;
        }
    if(aChangedEventType == (MSuplSessionObserver::TSuplSessionEventType)curEvent)
        {
        keyValue = aChangedEventType+1;
        }
    else
        keyValue = aChangedEventType;
    err = iSettingsNotifierRep->Set(KSuplSessionDBEvent,keyValue);
    return err;
    }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::PrintErrorMessage(const TDesC& aQueryFired,
//                                          TInt aErrorCode,TInt aOperationCode)
//
// Used to print error messages. Used for logging purposes
// (other items were commented in a header)
// --------------------------------------------------------------------------- 
void CSettingsDatabaseHandler::PrintErrorMessage(
        const TDesC& 
#ifdef PRINT_MESSAGE 
        aQueryFired
#endif
        ,TInt 
#ifdef PRINT_MESSAGE 
        aErrorCode
#endif    
        ,TInt 
#ifdef PRINT_MESSAGE 
        aOperationCode
#endif    
    )    
    {
    #ifdef PRINT_MESSAGE 
    
        LogQuery(aQueryFired);
        TBuf<64> buf;
    
        if(!(aOperationCode == 4 || aOperationCode == 3 || aOperationCode == 7))
            {                   
            buf.Copy(iDb.LastErrorMessage());
            TRACETEXT(buf)
            }
    
        switch(aOperationCode)
            {
            case 0:
                {
                buf.Copy(_L("DB Created with error code..."));
                break;
                }
            case 1 :
                {
                buf.Copy(_L("Table Created With Error Code..."));
                break;
                }
            case 2 : 
                {
                buf.Copy(_L("Record Inserted With Error Code..."));
                break;
                }
            case 3:
                {
                buf.Copy(_L("DB Opened with error code..."));
                break;
                }
            case 4:
                {
                buf.Copy(_L("Security Policy error code ... "));
                break;
                }
    
            case 5:
                {
                buf.Copy(_L("Record Deleted with error code..."));
                break;
                }
            case 6:
                {
                buf.Copy(_L("Record Updated with error code..."));
                break;
                }
            case 7:
                {
                buf.Copy(_L("Retrieved from CRep with error code"));
                }
                break;
            case 8:
                {
                buf.Copy(_L("InsertSLPRecordL completed. Reason  "));
                }
                break;
            case 9:
                {
                buf.Copy(_L("UpdateDefaultServerL completed. Reason "));
                }
                break;
            case 10:
                {
                buf.Copy(_L("UpdateSLPRecordL completed. Reason "));
                }
                break;
            case 11:
                {
                buf.Copy(_L("DeleteSLPRecordL completed. Reason "));
                }
            case 12 :
                {
                buf.Copy(_L("SetSLPAsDefaultL completed. Reason "));
                }
                break;
            case 13 : 
                {
                buf.Copy(_L("Record Retreived with error code..."));
                break;
                }
            case 14:
                {
                buf.Copy(_L("GetDefaultSLPId completed. Reason"));
                }
                break;
            case 15 :
                {
                buf.Copy(_L("IsSlpExists completed. Return Code "));
                }
                break;
            case 16:
                {
                buf.Copy(_L("SlpCount Completed. Count "));
                }
                break;
            case 17:
                {
                buf.Copy(_L("GetDefaultServer completed. Reason "));
                }
                break;
            case 18:
                {
                buf.Copy(_L("GetNwInfoPropertiesL completed. Reason "));
                }
                break;
            default:
                {
                buf.Copy(_L("Unknown Operation Completed With Error Code..."));
                }
                break;
            }
    
        buf.AppendNum(aErrorCode);
        TRACETEXT(buf)
    
    #endif                              
    }
    
// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::LogQuery(const TDesC& aBuffer)
//
// Used to print error messages. Used for logging purposes
// ---------------------------------------------------------------------------   
void CSettingsDatabaseHandler::LogQuery(const TDesC& 
#ifdef PRINT_MESSAGE
        aBuffer
#endif
        )
    {
    #ifdef PRINT_MESSAGE
        const TInt MaxLength = 125;
        TBuf<MaxLength> buff;
        TInt len = aBuffer.Length();
        if(len == 0) return;
        TInt start = 0;
        while(start<=len)
            {
            if( len - start > MaxLength)
                buff.Copy(aBuffer.Mid(start,MaxLength));
            else
                buff.Copy(aBuffer.Mid(start));
    
            TRACETEXT(buff) 
            start = start + MaxLength;
    
            }
    #endif
    }

// ---------------------------------------------------------------------------
// CSettingsDatabaseHandler::LogAllSLP()
//
// 
// ---------------------------------------------------------------------------

void CSettingsDatabaseHandler::LogAllSLP()
    {
    #ifdef PRINT_MESSAGE
        TBuf<KCustomSqlMaxLength> sqlString;
        sqlString.Append(KSelect);
        sqlString.Append(KStar);
        sqlString.Append(KFrom);
        sqlString.Append(KCreateTableName);
        sqlString.Append(KOrderBy);
        sqlString.Append(KColPriority);
        sqlString.Append(KCommaSeparator);
        sqlString.Append(KColSLPId);
    
        RSqlStatement stmt;
        TInt err = stmt.Prepare(iDb, sqlString);
       
        TInt64 SLPId =  -1;
        TBuf<256> ServerAddress,Manufacturer1,Iap;
        TBuf<10> priorityBuf;
        TBuf<256> lastusetime;
        TBuf<256> lasttried;
        TInt otherprops;
        TBuf<10> otherpropbuf;
        TBuf<10> slpIdBuf;
    
        TTime TimeLastUse;
        TTime TimeLastTried;
        TInt priority = 0;
        TInt columnIndexSLPID = stmt.ColumnIndex(KColSLPId);
        TInt columnIndexSLPAddress = stmt.ColumnIndex(KColSLPAddress);
        TInt columnIndexMajorVersion = stmt.ColumnIndex(KColProtocolVersionMajor);
        TInt columnIndexIAP = stmt.ColumnIndex(KColIap);
        TInt columnIndexTimeLastUse = stmt.ColumnIndex(KColTimeLastUse);
        TInt columnIndexTimeLastTried = stmt.ColumnIndex(KColTimeLastTried);
    
        TInt columnIndexNetInfoLastUse = stmt.ColumnIndex(KColNetInfoLastUse);
        TInt columnIndexNetInfoLastSucess = stmt.ColumnIndex(KColNetInfoLastSucess);
        TInt columnIndexOtherProperties = stmt.ColumnIndex(KColOtherProperties);
        TInt columnIndexPriority = stmt.ColumnIndex(KColPriority);
        while((err=stmt.Next())==KSqlAtRow)
            {
            SLPId = stmt.ColumnInt(columnIndexSLPID);
            priority = stmt.ColumnInt(columnIndexPriority);
            stmt.ColumnText(columnIndexSLPAddress,ServerAddress);
            stmt.ColumnText(columnIndexIAP,Iap);            
            stmt.ColumnText(columnIndexTimeLastUse,lastusetime);
            stmt.ColumnText(columnIndexTimeLastTried,lasttried);
            otherprops = stmt.ColumnInt(columnIndexOtherProperties);
            otherpropbuf.Zero();
            otherpropbuf.AppendNum(otherprops);
            priorityBuf.Zero();
            priorityBuf.AppendNum(priority);
            slpIdBuf.Zero();
            slpIdBuf.AppendNum(SLPId);
            TRACETEXT(slpIdBuf)
            TRACETEXT(priorityBuf)
            TRACETEXT(ServerAddress)
            TRACETEXT(Iap)
            TRACETEXT(Manufacturer1)
            TRACETEXT(lastusetime)
            TRACETEXT(lasttried)
            TRACETEXT(otherpropbuf)
            
            }   
    
        stmt.Close();
    #endif
    }
