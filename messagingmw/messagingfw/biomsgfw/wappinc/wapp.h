// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Header file for the WAP Access Point Parser
// 
//

#ifndef __WAPP_H__
#define __WAPP_H__


#include <bsp.h>
#include <commsdattypesv1_1.h>
#include <commsdat.h>
using namespace CommsDat;
#include <cdbcols.h>
#include <cdblen.h>
#include <charconv.h>

class MMsvAttachmentManager;
class MMsvAttachmentManagerSync;

#define BIO_MSG_ENTRY_UNPARSED	 0 // iMtmData3=0 - an untouched, a virgin message entry!
#define BIO_MSG_ENTRY_PARSED	 1 // iMtmData3=1 - entry parsed and externalised (has a store)
#define BIO_MSG_ENTRY_PROCESSED	 2 // iMtmData3=2 - entry been parsed,externalised and processed


_LIT8(KDummyModemInitStr, "Modem Init String - final version to be decided!!!");
_LIT(KWapDefaultSettingsName, "WAP Settings");
_LIT(KWappDefaultBMName, "Unknown Bookmark"); // Default name for a bookmark.
_LIT(KWappDll, "KWAPP-DLL");

const TInt KGsmCsdSuffixLen = 8;
_LIT(KGsmCsdSuffix, "-GsmCsd");	// Suffix - append to Settings Name for Gsm/Csd ISP/IAP entry
_LIT(KGsmUssdSuffix, "-GsmUssd");	// Suffix - append to Settings Name for Gsm/Ussd ISP/IAP entry
_LIT(KIS136CsdSuffix, "-Is136Csd"); // Suffix - append to Settings Name for IS136/Csd ISP/IAP entry
_LIT(KGprsSuffix, "-Gprs"); // Suffix - append to Settings Name for GPRS ISP/IAP entry


enum TWappPanic  // Assert Debug panic codes
	{
	EWappLimitsExceeded,
	EWappErrMessageProcessed,
	EWappEmptyBuffer,
	EWappInvalidEntry,
	EWappNullPointer
	};
/*
enum TBearer
	{
	EAll,
	EIP,
	ESMS7,
	ESMS
	};
*/
enum TCsdCallType
	{
	EAnalogue,
	EIsdn
	};

enum TCsdCallSpeed
	{
	EAutobauding,
	E9600baud,
	E14400baud,
	E19200baud,
	E28800baud,
	E38400baud,
	E43200baud,
	E57600baud
	};


// Forward declarations
class RResourceFile;
class CRichText;
class CCommsDatabase;
class CCommsDbTableView;
class CIspTableData;
class TBearerSpeed;
class CWappBookmark;
class CWapIpBearerData;
class CWapSmsBearerData;
class CWapGprsBearerData;
class CWapCharsetConverter;

class CMsvBIOWapAccessParser : public CBaseScriptParser2
/**
@internalComponent
@released
*/
    {
// Enum and inline class defines
private:
    enum TParseSession
        { 
		EStart,				// Don't do anything-just check message not empty & set Scheduler going
        EUnfoldMessage,     //  Don't have to unfold the message, just save it
        EParseMessage,      //  Parsing 
		EProcessInitialise,	//  Start ProcessL - restore data if necessary, PreProcess and Validate it 
		EProcessMessage,	//	Actual Processing of message data
        ECompleteMessage    //  Completing ..
        };

	enum TWappBearerType
		{
		EWappUnknownBearer,
		EWappGsmCsdBearer,
		EWappGsmSmsBearer,
		EWappGsmUssdBearer,
		EWappIS136CsdBearer,
		EWappGprsBearer
		};


// Member Variables
public:	
	//Defines which type of characteristic we're dealing with
	enum TWappCharacteristicType
		{
		EWappUnknown,				// Don't know the type yet
		EWappAddressCharacteristic, // An address, but specific type undetermined
		EWappGsmCsdCharacteristic,		// A GSM/CSD address 
		EWappGsmSmsCharacteristic,		// A GSM/SMS address
		EWappGsmUssdCharacteristic,		// A GSM/USSD address
		EWappIS136CsdCharacteristic,    // A IS136/CSD address
		EWappGprsCharacteristic,		// A GPRS address
		EWappBookmarkCharacteristic, // A Bookmark	
		EWappURLCharacteristic,		 // A URL
		EWappNameCharacteristic,	// A name characteristic
		EWappIDCharacteristic,		// A ID characteristic
		EWappMMSURLCharacteristic   // A Multi Media Service Centre Url characteristic
		};


    IMPORT_C static CMsvBIOWapAccessParser* NewL(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs);
    ~CMsvBIOWapAccessParser();

    void ParseL(TRequestStatus& aStatus, const TDesC& aSms);
    void ProcessL(TRequestStatus& aStatus);

private:
	//Constructors
    CMsvBIOWapAccessParser(CRegisteredParserDll& aRegisteredParserDll, // Only constructor-passes
									CMsvEntry& aEntry, RFs& aFs);	   // parameters to base const

    void ConstructL();

	// Active Object stuff
    void DoCancel();
    void RunL();

	// State Engine functions

	void CallChangeStateL(TInt aState); // Calls ChangeStateL with the right state
    void ChangeStateL(TParseSession aState);    // Advances State Machine to next state

    void ParseMessageL(); // Only checks & saves parsed data - calls DoParseL 

    void CompleteMessageL();  //Sets the flag on TMsvEntry to show message parsed.

    void RequestComplete(TRequestStatus& aStatus, TInt aError); // Complete our async op.
	
	// *****************	Additional helper functions added by AHF	*********************
private:	

	void StoreParsedDataL();	// Saves CParsedField data in a store
	
	void RestoreParsedDataL();	// Loads the CParsedField data from store

	void ResetProcessVariablesL();  // Resets variables, delete and create if necessary
	
	void DoParseL();			// Does the actual parsing of the message. 
	
	void DoProcessL();		// Get data from array, commit to COMMDB
	
	void PreProcessL(); //  Called by DoProcessL,extracts data from array into 
						//  Mapping object,rejects duplicate data

	// *******************	Helper Functions for Parsing	**********************

	void ReadCharacteristicL();	// Gets a CHARACTERISTIC from the message 

	void ReadParmL();			// Reads in a PARM	from the message

	void SettingsNamePresentL(); // Ensures that a NAME characteristic is present. 

	
	TUint32 Read_mb_u_int32L();	// Extract a multiple byte encoded 32 bit int
	
	HBufC* ReadStringTableReferenceLC(TUint32 anOffset);// Gets string from the string table
 											// using a 32 bit integer offset

	void ReadASCIIorUTF8StringL(); //finds the end of a string encoded in UTF-8 or ASCII
	void ReadUCS2StringL();	//finds end of a string encoded in 2-byte Unicode
	HBufC* ReadInlineStringLC();		 // Extracts an inline string from the message	
	
	void AppendDataPairL(const TDesC& aFieldName, const TDesC& aFieldValue);// Creates CParsedField,
																			// & appends to array

	void AppendNameFieldL(const TDesC& aFieldName); // Creates a CParsedField,
													// & append to array
	
	void InsertDataPairL(const TDesC& aFieldName,const TDesC& aFieldValue,TInt anIndex);// Inserts CParsedField
																				// at specified position. 

	TChar GetNextMessageByteL();   // Gets next character from TLex - leaves if at end of buffer

	// ******************	 Helper functions for ProcessL	*****************************
	void ExtractBearerDataL(TInt& aCurrentPos);//Extracts data from iParsedFieldArray 
											   // & stores in WAP & ISP table objects. 
											   // Increments the current pos ref as it goes 

	void ExtractBookmarkDataL(TInt& aIndex);  // Extracts bookmark data from array to create
											  // a bookmark. Rejects invalid bookmarks.
											  // Ensures that array positioned at next characteristic

	void ValidateCharacteristicsL();		  // Ensures that don't have invalid combinations
											  // of characteristic eg URLs, IDs, NAMES must have a bearer type
											  // but Bookmarks can be by themselves

	TBool IsCharacteristicName(TInt anIndex); // True if name at index positino is a Characteristic name
	

	TInt IsValidTokenNameL(const TInt anArrayIndex);	// Validates the name against defined strings 
												// returns 0 if OK, error otherwise
	
	void WriteDataToDBL();			// Creates entries in the tables using 
									// data in the arrays of table info	
	
	TBool DeleteTableEntryL(const TDesC& aTableName, const TDesC& aEntryName);   // returns True if there are records in the CommDB table
	void WriteToCommDatDialOutISPTableL(const TDesC& aRecName); // Writes/amends Dial Out ISP table
	void WriteToCommsDatIAPTableL(const TDesC& aRecName); // Writes/amends WAP table
	void WriteToCommDatGprsTableL(const TDesC& aRecName); // Writes/amends Gprs table
	void WriteToCommDatWapAccessPointTableL(const TDesC& aRecName); // Writes/amends WAP table
	void WriteToCommDatWapIpBearerTableL();
	void WriteToCommDatWapSmsBearerTableL(); // Writes/amends Dial-Out WAP table	
	void DeleteCommDatTableEntryL(TMDBElementId aElementName, const TDesC& aCsdNameStr);
	

	void WriteBookmarksToFileL();  // Stores bookmarks as an attachment file in the .eBM format

	void WriteMMSURLL(); // Write the MMS URL to the setting in the message server.

	TCommsDbWapWspOption GetWSPType(TDesC& aPortValue);   // Returns the appropriate enum for the Port value
	
	TBool UseSecurity(TDesC& aPortValue);  //Returns the security level for the Port Value

	void SetWapIspEntryIdL(const TDesC& aEntryName);  // Set the iCurrentBearerType BEFORE calling this!!!!
													 //Sets variables holding ID of Dial Out ISP ID entry 

	void SetWapIapEntryIdL(const TDesC& aEntryName, CWapIpBearerData& aWapIpTable);  // Sets variables holding ID of Dial Out IAP ID entry 
	
	void SetWapAPEntryIdL();	// Sets ID of entry in WAP Access Point Table. Uses iWAPSettings

	void SetModemInitStringL(); // Sets the modem initialisation string

	void UpdateStartPageUrlL(); // Updates the URL of start page in WAP table

	// ******************    General Functions    *****************************
	
	// Locate a Name value, default is from start of array, returns -1 if search fails
	TInt LocateParsedFieldName(const TDesC& aFieldName, const TInt aStartIndex=0); 

	void RemoveAttachmentIfExistL(const TDesC& aFileName, MMsvAttachmentManager& aManager, MMsvAttachmentManagerSync& aManagerSync);

	void Panic(TInt aPanic);
	//Member variables
private:
    TInt            iState;          // Current session state
    TRequestStatus* iReport;
    TInt            iCompleted;      // Stores completion code from previous session state

private: 
	// Additional variables added by AHF

	TWappCharacteristicType iCurrentCharacteristic;

	TUint32				iWBXMLversionX10;   // WBXML version *10 to give an int eg 1.0 -> 10
	TUint32				iPublicID;			//PublicId of document
	// Internal Symbian character set identifier for incoming strings
	TUint32				iCharacterSet;		
	HBufC*				iStringTable;		// Table of strings to be used for str table reference

	CArrayPtrSeg<CWappBookmark>* iBookmarkList;   // Array of bookmarks to store
	TPtrC				iWAPSettingsName;	// Name for these WAP settings-get from NAME characteristic
	TPtrC				iUrl;				// URL of the start page
	TPtrC				iMMSUrl;			// URL of the MMSC
	TPtrC				iID;				// ID used by these settings
	HBufC8*				iModemInitString;	// String used to hold Modem Initialisation string

	CWapIpBearerData*	iGsmCsdWapIpTable;		//  Wap IP table data GSM/CSD 
	CWapIpBearerData*	iGprsWapIpTable;		//  Wap IP table data for GPRS

	CIspTableData*		iWapIpISPTable;		// 1 WAP GSM/CSD table entry = 1 ISP table entry
	CWapSmsBearerData*	iWapSmsTable;		// No links to ISP table.
	CWapGprsBearerData*	iWapGprsTable;				

	TBool				iGsmCsdData;
	TBool				iGsmSmsData;
	TBool				iGprsData;
	TBool				iURLData;
	TBool				iNameData;
	TBool				iIdData;
	TBool				iMMSURLData; 

	//commit data
	CMDBSession* 		iDBSession;
	
	
	TUint32				iRecordId; 
	
	TUint32				iWapAccessPtID;		// ID of entry in WAP Access Point Table

	TUint32				iWapLocation; 
	TUint32				iWapChargeCard;
	TUint32				iWapModem;

	CWapCharsetConverter* iCharsetConverter;
	TWappBearerType		iCurrentBearerType;
	};


class CWapCharsetConverter: public CBase
/**
@internalComponent
@released
*/
	{
	public:
		static CWapCharsetConverter* NewL(RFs& aFs, TUint aMibEnum);
		void ConvertL(const TDesC& aSource, TDes& aSink);
		~CWapCharsetConverter();
	private:
		void ConstructL(TInt aMibEnum);
		CWapCharsetConverter(RFs& aFs) : iFs(aFs){}
	private:
		RFs& iFs;
		CCnvCharacterSetConverter* iConverter;
		CArrayFix<CCnvCharacterSetConverter::SCharacterSet>*  iAvailableCharsets;
		TBool iParseable;
	};


#endif // __WAPP_H__
