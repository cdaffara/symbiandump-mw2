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
* Description:   Settings database handler...
*
*/



#ifndef __CSETTINGSDATABASEHANDLER__
#define __CSETTINGSDATABASEHANDLER__


#include <e32std.h>
#include <badesca.h>    // CDesCArrayFlat (cannot be forward declarated)
#include <e32des16.h> 
#include <sqldb.h>
#include <flogger.h>
#include <f32file.h>

#include "epos_csuplsettings.h" 
#include "epos_csuplsettingsinternal.h" //for TServerParam
#include "epos_msuplsettingsobserver.h"
#include "epos_msuplsessionobserver.h"

_LIT( KLoggingFolder, "suplsettingsdb" );
_LIT( KLoggingFile, "log.txt" );
_LIT( KLoggingFullName, "c:\\logs\\suplsettingsdb\\log.TXT" );

const int KCustomSqlMaxLength = 1024; //max len of a sql string
#define TRACESTRING( S1 )       { \
    _LIT( tempLogDes, S1 ); \
    RFileLogger::Write( KLoggingFolder(), KLoggingFile(), \
  EFileLoggingModeAppend, tempLogDes() );}
    
    
#define TRACETEXT( S1 )    {RFileLogger::Write( KLoggingFolder(),KLoggingFile(),EFileLoggingModeAppend, S1 ); }


class CServerParams;
class CRepository;
class CTriggerParams;
class CServerExtensionParams;

/**
*  This class is used for creating, accessing and storing SUPL Settings
*  onto the SUPL settings DB.
*
*  This is part of @lib epos_suplsettings.lib
*  @since 
*/
class CSettingsDatabaseHandler : public CBase
    {
public: // Creation and destruction

    /**
     * Function:    NewL
     *
     * Description: Get instance of a CSettingsDatabaseHandler object.
     */
    static CSettingsDatabaseHandler* NewL();

    /**
     * Function:    ~CSettingsDatabaseHandler
     *
     * Description: Destroy the object.
     */
    ~CSettingsDatabaseHandler();


public: // Public API for database operations


private: // Construction

    /**
     * Function:    ConstructL
     *
     * Description: Perform the second phase construction of a CSettingsDatabaseHandler
     *              object
     */
    void ConstructL();

    /**
     * Function:    CSettingsDatabaseHandler
     *
     * Description: Perform the first phase of two phase construction.
     */
    CSettingsDatabaseHandler();
		
private: // Helpers

    /**
     * This method is used to Create a new database and open. 
     * The database will be in exclusive access mode.
     *
     * @since 
     *
     * @param [IN] aDbFile The name of the database file to be opened
     *  
     */
    void   CreateOpenSecureDatabaseL(/*const TDesC& aDbFile*/); 
    
    /**
     * This method is used to close and remove the settings DB 
     *
     * @since       
     */
    TInt   RemoveDb();
    
    /**
     * This method is used to close and remove the settings DB 
     *
     * @since       
     */
    TInt   Close();
    
    /**
     *  Creates the SUPL Settings table. Leaves, if the table cannot be created. 
     *
     * @since       
     */
    void   CreateTableL();


    /**
     *  Forms part of the query string, used to form a query string that contains a TTIme value in it 
     *  
     * @since
     * 
     * @param [IN]  aParameter      The TServerParam enum which indicates which property is to be updated
     * @param [IN]  aValue          The TTime value to be included in the query string       
     * @param [OUT] aQueryString    The resultant Query String is returned in this   
     */
    void   FormSLPQueryL(CSuplSettingsInternal::TServerParam aParameter,TTime aValue,TDes& aQueryString);
    /**
     *  Forms part of the query string, used to form a query string that contains a TInt value in it 
     *  
     * @since
     * 
     * @param [IN]  aParameter      The TServerParam enum which indicates which property is to be updated
     * @param [IN]  aValue          The TInt value to be included in the query string       
     * @param [OUT] aQueryString    The resultant Query String is returned in this   
     */
    void   FormSLPQueryL(CSuplSettingsInternal::TServerParam aParameter,TInt aValue,TDes& aQueryString);
    /**
     *  Forms part of the query string, used to form a query string that contains a String value in it 
     *  
     * @since
     * 
     * @param [IN]  aParameter      The TServerParam enum which indicates which property is to be updated
     * @param [IN]  aValue          The string value to be included in the query string       
     * @param [OUT] aQueryString    The resultant Query String is returned in this   
     */
    void   FormSLPQueryL(CSuplSettingsInternal::TServerParam aParameter,const TDesC& aValue,TDes& aQueryString);
    

    /**
     * Used to form part of a query string and set one of the boolen flags as part of the query 
     * 
     * @param[IN]   aParameter  Indicates which property is being manipulated
     * @param[IN]   aValue      The value of the flag to be set as part of query 
     * @since
     */
    TInt   SetOtherPropertiesL(CSuplSettingsInternal::TServerParam aParameter,TInt aValue);

    /**
     *  Returns the SlpId of the Server that was last added into the records 
     *
     * @since       
     */
    TInt64 GetLastInsertedSLPID();
    
    /**
     * Used to get the integer representation of all boolean flags as stored in the database
     * 
     * @param[IN]   aSLPId                The slp id of the slp for which the value is required
     * @param[OUT]  aOtherPropertyValue   The value of the int representation of the flags is returned in this 
     * @since
     */
    TInt   GetOtherPropertyValue(TInt64 aSLPId,TInt& aOtherPropertyValue);

    /**
     * This method is used to log query strings. Used for logging purposes only
     * @since 
     *
     * @param [IN] aBuffer The buffer containing the string to be logged
     *  
     */
    void   LogQuery(const TDesC& aBuffer);
    
    /**
     * This method is used to log query strings along with the error code.
     * Used for logging purposes only
     * @since 
     *
     * @param [IN] aQueryFired The buffer containing the query that was fired.
     * @param [IN] aErrorCode The error code that resulted due to the query.
     * @param [IN] aOperationCode The operation code which the query was used to perform.
     *  
     */
    void   PrintErrorMessage(const TDesC& aQueryFired,TInt aErrorCode,TInt aOperationCode);
     
    /**
     * This method is used to log all SLPs currently in the database
     * @since 
     *  
     */
    void   LogAllSLP();
    
    /**
     * This method is used to retrieve default settings value from the cen rep
     * @since 
     * @param [IN] aKey The CenRep Key correspnding to the value that is to be retrieved
     * @param [OUT] aValue The value of the key is returned in this OUT parameter
     */
    TInt   GetDefaultCRValues(TInt aKey,TInt& aValue);
    
    /**
     * This method is used to retrieve Settings DB Path value from the cen rep
     * @since 
     * @param [OUT] aPath The value of the DB Path is returned in this OUT parameter
     */
    TInt   GetDBPathFromCR(TDes& aPath);
    
    /**
     * This method is used to store Settings DB Path value in the cen rep
     * @since 
     * @param [IN] aPath The value of the DB Path to be stored in CenRep
     */
    TInt StoreDBPathToCR(TDes& aPath);
    /*
     * This method is used to notify that a change has occured in the DB via the CR
     * @since
     * @param [IN] aChangeEventType The type of event that occured on the DB ex.Update Delete etc
     * @param [IN] aSlpId           The slpId for which the record was updated
     * 
     */
    TInt NotifyDBChange(MSuplSettingsObserver::TSuplSettingsEventType aChangedEventType,TInt64 aSlpId);
    
    /*
     * This method is used to retrieve priority of a particular SLP
     * @since
     * @param [IN]  aSlpId      The slp id for which the priority is required
     * @param [OUT] aPriority   The current priority is returned in this
     * 
     */
    void GetPriorityL(TInt aSlpId,TInt& aPriority);
    
    /**
     * This method is used to alter priority of a particular server
     * @since 
     * @param [IN] aSlpId The Slp Id for which new priority is to be assigned.
     * @param [IN] aValue The new priority value to be assigned to the server.     
     */
    void SetPriorityL(TInt aSlpId,TInt aValue);
    /**
     * This method is used to alter priority of a particular server
     * @since 
     * @param [IN] aSlpId The Slp Id for which new priority is to be assigned.
     * @param [IN] aValue The new priority value to be assigned to the server.     
     */
    void GetMaxPriorityL(TInt& aPriorityValue);
    /**
     * This method is used to order the priority values in proper incremental sequence
     * @since 
     *     
     */
    void NormalizePriorityL();
    
    /**
     *  Creates the SUPL session table. Leaves, if the table cannot be created. 
     *
     * @since       
     */
    void   CreateSessionTableL();


    /*  Forms part of the query string, used to form a query string that contains a TInt value in it 
     *  
     * @since
     * 
     * @param [IN]  aParameter      The TSessionParam enum which indicates which property is to be updated
     * @param [IN]  aValue          The TInt value to be included in the query string       
     * @param [OUT] aQueryString    The resultant Query String is returned in this   
     */
    void   FormSessionQueryL(CSuplSettingsInternal::TSessionParam aParameter,TUint64 aValue,TDes& aQueryString);
           
    /*
     * This method is used to notify that a change has occured in the DB via the CR
     * @since
     * @param [IN] aChangeEventType The type of event that occured on the DB ex.Update Delete etc
     * @param [IN] aSessionId           The aSessionId for which the record was updated
     * 
     */
    TInt NotifySessionDBChange(MSuplSessionObserver::TSuplSessionEventType aChangedEventType,TInt64 aSessionId);
public:		

    /**
     * This method is used to insert SLP record into the SUPL Settings DB
     * @since 
     *
     * @param [IN] aServParamValues The CServerParams pointer containing server params 
     *                              to be added to the DB
     * @param [OUT] aSlpId  The SLP ID of the SLP that was successfully inserted
     */
    void   InsertSLPRecordL(const CServerParams* aServParamValues,TInt64& aSlpId);
    
    /**
     * This method is used to delete a SLP record corresponding to the given SLP ID
     * @since 
     * @param [OUT] aSlpId  The SLP ID of the SLP that is to be deleted
     */
    void   DeleteSLPRecordL(TInt64 aSLPId);
    /**
     * This method is used to update a column of string value corresponding to a particular slp
     * @since 
     * @param [IN] aSlpId  The SLP ID of the SLP that is to be updated
     * @param [IN] aParameter  The TServerParam enum which indicates which property is to be updated
     * @param [IN] aValue  The new value to be updated to the setings db
     */
    void   UpdateSLPRecordL(TInt64 aSLPId,CSuplSettingsInternal::TServerParam aParameter,const TDesC& aValue);
    /**
     * This method is used to update a column  corresponding to version number of a particular slp
     * @since 
     * @param [IN] aSlpId  The SLP ID of the SLP that is to be updated
     * @param [IN] aMajor  The Major Version Number
     * @param [IN] aValue  The minor version number
     */
    void   UpdateSLPRecordL(TInt64 aSLPId,TInt aMajor, TInt aMinor);
    /**
     * This method is used to update a column corresponding to N/w Info Last Use/ lAst Success
     * of a particular slp
     * @since 
     * @param [IN] aNetType The network type 
     * @param [IN] aMcc Mobile country code.  It can have value from 0 to 999 
     * @param [IN] aMnc Mobile network code.  It can have value from 0 to 999 
     * @param [IN] aCid Cell Identity.  It can have value from 0 to 65535 if aNetType is GSM or
     *              it can have value from 0 to 268435455 if aNetType is WCDMA 
     * @param [IN] aLac Location Aread Code. This parameter is written to settings only if aNetType is GSM.
     */
    void   UpdateSLPRecordL(TInt64 aSLPId,CSuplSettingsInternal::TServerParam aParamType,const CSuplSettingsInternal::TSuplSettingsNetworkType aNetType, 
            const TInt aMcc, const TInt aMnc, const TInt aCid, const TInt aLac);
    /**
     * This method is used to update a column of integer value corresponding to a particular slp
     * @since 
     * @param [IN] aSlpId  The SLP ID of the SLP that is to be updated
     * @param [IN] aParameter  The TServerParam enum which indicates which property is to be updated
     * @param [IN] aValue  The new value to be updated to the setings db
     */
    void   UpdateSLPRecordL(TInt64 aSLPId,CSuplSettingsInternal::TServerParam aParameter,TInt aValue);
    /**
     * This method is used to update a column of TTime value corresponding to a particular slp
     * @since 
     * @param [IN] aSlpId  The SLP ID of the SLP that is to be updated
     * @param [IN] aParameter  The TServerParam enum which indicates which property is to be updated
     * @param [IN] aValue  The new value to be updated to the setings db
     */
    void   UpdateSLPRecordL(TInt64 aSLPId,CSuplSettingsInternal::TServerParam aParameter,TTime aValue);
    /**
     * This method is used to update all configurable parameters corresponding to a partiular SLP
     * @since 
     * @param [IN] aParamValues The CServerParams containing valid values to be updated to 
     *                          a particular SLP.
     */
    void   UpdateSLPRecordL(const CServerParams* aParamValues);
    /**
     * This method is used to update all configurable parameters corresponding to a default SLP
     * @since 
     * @param [IN] aParamValues The CServerParams containing valid values to be updated to 
     *                          default SLP.
     */
    void   UpdateDefaultServerL(const CServerParams* aParamValues);
    
    /**
     * This synchronous method is retrieve parameters corresponding to all existing SLP records
     * @since 
     * @param [OUT] aParamValues The Array of CServerParams which needs 
     *              to filled with server parameters
     */
    void   GetAllSLPL(RPointerArray<CServerParams>& aParamValues);
    /**
     * This asynchronous method is retrieve parameters corresponding to all existing SLP records
     * @since 
     * @param [OUT] aParamValues The Array of CServerParams which needs 
     *              to filled with server parameters
     */
    void   GetAllSLPL(RPointerArray<CServerParams>& aParamValues,TRequestStatus& aStatus);
    /**
     * This method is used to set a SLP with a particular id as default
     * @since 
     * @param [IN] aSLPId The SLP ID of the server entry to be set as default server
     */
    void   SetSLPAsDefaultL(TInt64 aSLPId);
    /**
     * This method is used to get SLP Id of the default server
     * @since 
     * @param [OUT] aSLPId The SLP ID of the default server
     */
    TInt   GetDefaultSLPId(TInt64& aSLPId);
    /**
     * This method is used to get other flags associated with a slp
     * @since 
     * @param [IN] aSLPId       The SLP ID of the default server
     * @param [IN] aParameter   Indicates which parameter is requested for
     * @param [OUT] aValue      The retrieved value is returned in this.
     */
    void   GetOtherPropertiesL(TInt64 aSLPId,CSuplSettingsInternal::TServerParam aParameter,TInt& aValue);
    
    /**
     * This method is to Get Server Addresss of a SLP corresponding to a particular Slp Id
     * @since 
     * @param [IN] aSlpId The Slp Id for which server address is required
     * @param [OUT] aServerAddress Is used to return the server address    
     */
    TInt   GetServerAddress(const TInt64 aSlpId, TDes& aServerAddress);
    /**
     * This method is to Get manufacturer name of a SLP corresponding to a particular Slp Id
     * @since 
     * @param [IN] aSlpId The Slp Id for which server address is required
     * @param [OUT] aManuName Is used to return the manufacturer name    
     */
    TInt   GetManufacturer(const TInt64 aSlpId, TDes& aManuName);
    /**
     * This method is to Get IAP name of a SLP corresponding to a particular Slp Id
     * @since 
     * @param [IN] aSlpId The Slp Id for which server address is required
     * @param [OUT] aIAPName Is used to return the IAP name    
     */
    TInt   GetIAPName(const TInt64 aSlpId, TDes& aIAPName);
    /**
     * This method is to last tried time of a SLP corresponding to a particular Slp Id
     * @since 
     * @param [IN] aSlpId The Slp Id for which server address is required
     * @param [OUT] aLastTriedTime Is used to return the last tried time    
     */
    TInt   GetLastTriedTime(const TInt64 aSlpId, TTime& aLastTriedTime);
    /**
     * This method is to last used time of a SLP corresponding to a particular Slp Id
     * @since 
     * @param [IN] aSlpId The Slp Id for which server address is required
     * @param [OUT] aLastUsedTime Is used to return the last used  time    
     */
    TInt   GetLastUsedTime(const TInt64 aSlpId, TTime& aLastUsedTime);

    /**
     * This method is retrieve all configurable parameters corresponding to a particular SLP
     * @since 
     * @param [IN] aSlpId The Slp Id for which values are required
     * @param [OUT] aParamValues The CServerParams is  updated with the values corresponding
     *                          to the  server.
     */
    TInt   GetSlpInfoFromId(const TInt64 aSlpId,CServerParams* aParamValues);
    /**
     * This method is retrieve all configurable parameters corresponding to a particular SLP
     * @since 
     * @param [IN] aServerAddress The server address of the slp for which values are required
     * @param [OUT] aParamValues The CServerParams is  updated with the values corresponding
     *                          to the  server.
     */
    TInt   GetSlpInfoFromAddress(const TDesC& aServerAddress,CServerParams* aParamValues);
    /**
     * This method is retrieve all configurable parameters corresponding to the default SLP
     * @since 
     * @param [IN] aParamValues The CServerParams to be updated with the values corresponding
     *                          to the default server.
     */
    TInt   GetDefaultServer(CServerParams* aParamValues);
    /**
     * This method is retrieve the count of slp entries present
     * @since 
     * @param [OUT] aCount Contains the number of slp entries as output
     */
    TInt   SlpCount(TInt& aCount);
    /**
     * This method is to check is a slp entry exists corresponding to a particular SLP Id
     * @since 
     * @param [IN] aSlpId The Slp ID whcih is to be serached for.
     */
    TBool  IsSlpExists(const TInt64 aSlpId);
    /**
     * This method is to check is a slp entry exists corresponding to a particular SLP Address
     * @since 
     * @param [IN] aServerAddress The Slp Address which is to be searched for.
     */
    TBool  IsSlpExists(const TDesC& aServerAddress);
    /**
     * This method is to check is a slp entry exists corresponding to a particular SLP Address
     * @since 
     * @param [IN] aServerAddress The Slp Address which is to be searched for.
     * @param [OUT] aSlpId The SLP Id of the server if found is returned in this
     */
    TBool  IsSlpExists(const TDesC& aServerAddress,TInt64& aSlpId);
    
    /**
     * This method is to Get Protocol Version of a SLP corresponding to a particular Slp Id
     * @since 
     * @param [IN] aSlpId The Slp Id for which version number is required
     * @param [OUT] aMajor Is used to return the Major Version Number]
     * @param [OUT] aMinor Is used to return the Minor Version Number
     */
    TInt   GetVersion(const TInt64 aSlpId,TInt& aMajor, TInt& aMinor);
    /**
     * This method is used to retrieve a colummn corresponding to N/w Info Last Use/Last Success
     * of a particular slp
     * @since 
     * @param [IN]  aParamType Used to indicate which property is requested for
     * @param [OUT] aNetType The network type 
     * @param [OUT] aMcc Mobile country code.  It can have value from 0 to 999 
     * @param [OUT] aMnc Mobile network code.  It can have value from 0 to 999 
     * @param [OUT] aCid Cell Identity.  It can have value from 0 to 65535 if aNetType is GSM or
     *              it can have value from 0 to 268435455 if aNetType is WCDMA 
     * @param [OUT] aLac Location Aread Code. This parameter is written to settings only if aNetType is GSM.
     */
    void   GetNwInfoPropertiesL(TInt64 aSLPId,CSuplSettingsInternal::TServerParam& aParamType ,
            CSuplSettingsInternal::TSuplSettingsNetworkType& aNetType,TInt& aMcc,TInt& aMnc, 
            TInt& aCid, TInt& aLac);

    /**
     * This method is used to alter priority of a particular server
     * @since 
     * @param [IN] aSlpId The Slp Id for which new priority is to be assigned.
     * @param [IN] aValue The new priority value to be assigned to the server.
     * @param [IN] aDirection Whether to increase or decrease priority.      
     */
    void ChangePriorityL(TInt aSlpId,TInt aValue,TBool aDirection);
    
    /**
     * This method deletes all slps which has the sim change remove flag set
     * @since 
     */
    void RemoveOnSimChangeL();
    
        /**
         * This method is used to insert Active session record into the SUPL session DB
         * @since S60 5.2
         *
         * @param [IN] aSessionParamValues The CServerParams pointer containing trigger session params 
         *                              to be added to the DB
         */
        void   InsertSessionRecordL(const CTriggerParams* aSessionParamValues);
        
        /**
         * This method is used to delete a session record corresponding to the given session ID
         * @since S60 5.2
         * @param [OUT] aSessionId  The session ID of the session that is to be deleted
         */
        void   DeleteSessionRecordL(TInt64 aSessionId);
        
        
        /**
         * This method is used to delete all session records from session table
         * @since S60 5.2
         */
        
        void DeleteAllSessionRecordsL();
        
        /**
         * This method is used to update a column of string value corresponding to a particular session record
         * @since S60 5.2
         * @param [IN] aSessionId  The session ID of the session that is to be updated
         * @param [IN] aParameter  The TSessionParam enum which indicates which property is to be updated
         * @param [IN] aValue  The new value to be updated to the session db
         */
        void   UpdateSessionRecordL(TInt64 aSessionId,CSuplSettingsInternal::TSessionParam aParameter,TUint64 aValue);
        
        /**
         * This synchronous method is retrieve parameters corresponding to all existing session records
         * @since S60 5.2
         * @param [OUT] aParamValues The Array of CTriggerParams which needs 
         *              to filled with session parameters
         */
        void   GetAllSessionsL(RPointerArray<CTriggerParams>& aParamValues);
        
        /**
         * This synchronous method is retrieve parameters corresponding to given session
         * @since S60 5.2
         * @param [IN] aSessionId Session id to get parameters
         * @param [OUT] aParamValues Parameters of a given session
         */
        void   GetSessionL(TInt64 aSessionId,
                           CTriggerParams*& aParamValues);
        
        /**
         * This method is retrieve the count of session entries present
         * @since S60 5.2
         * @param [OUT] aCount Contains the number of session entries as output
         */
        TInt SessionCount(TInt& aCount);
        /**
         * This method is to check is a session entry exists corresponding to a particular session Id
         * @since S60 5.2
         * @param [IN] aSessionId The session ID whcih is to be searched for.
         */
        TBool  IsSessionExists(const TInt64 aSessionId);
        
        /**
         * This method is used to retrieve extended parameters of given server
         * @since S60 5.2
         */
        void  GetServerExtensionParamsL( TInt64 aSlpId,CServerExtensionParams* aServerExtnParams );
        
        /**
         * This method is used to set extended parameters of a given server.
         * @since S60 5.2
         */
        void  UpdateServerExtensionParamsL( TInt64 aSlpId,CServerExtensionParams* aServerExtnParams );
    
private: // Member data

    
    // RSqlDatabase Instance
    RSqlDatabase iDb;
    
    //holds current slp id for which the request was made
    TInt64 iSLPId;
    
    
    TRequestStatus*  iStatus;
    
    //used to access central repository for retrieving default settings values
    CRepository*            iSettingsRep; 
    //used to access central repository for incdicating change in  settings values
    CRepository*            iSettingsNotifierRep; 
    //To hold the SQL Query String
    TBuf<KCustomSqlMaxLength> iSQLString;
    };

#endif // __CSETTINGSDATABASEHANDLER__


