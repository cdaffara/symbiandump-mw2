/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   class for handling SUPL_START message...
*
*/



#ifndef C_COMASUPLSTARTSTATE_H
#define C_COMASUPLSTARTSTATE_H

#include <e32base.h>
#include <lbs/epos_comasuplpossessionbase.h>
#include "lbs/epos_tomasuplallowedcapabilities.h"
#include "epos_comasuplstate.h"
#include "epos_comasupletel.h"

class COMASuplLocationId;
class COMASuplETel;
class COMASuplSETCapabilities;
class RMobilePhone;
class COMASuplPOSRequestor;
class TSuplTerminalQop;
class TOMASuplECellInfo;
class COMASuplAsnHandlerBase;
class TOMASuplAllowedCapabilities;
/**
*  COMASuplStartState handles all SUPL_START related processing.   
*  @lib eposomasuplprotocolhandler.lib
*  @since S60 v3.1
*/
 
class COMASuplStartState : public COMASuplState,
						   public MOMASuplETelObserver,
						   public MOMASuplMsgStateObserver
{
	
	public :  // Constructor 
	
			/**
			* NewL Method.
			* @since S60 v3.1u
			* @param aCapabilities,Allowed Capabilities
			* @param aMobilePhone,
			* @param aMsgTimeout,Timeout for message
			* @param aAllowedCapabilities,Allowed capabilities
			* @return Instance of COMASuplStartState
			*/ 
			static COMASuplStartState* NewL(RMobilePhone& aMobilePhone,TInt aMsgTimeout,
											COMASuplPOSRequestor* aPosRequestor,COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase,
											TInt aAllowedCapabilities);
			/**
			* Destructor.
			*/
			~COMASuplStartState();
	
	public:		//COMASuplState
			/**
			* GenerateMessageL Method,Generates SUPL_START message.
			* @since S60 v3.1u
			* @param None
			* @return None
			*/ 
			virtual TInt GenerateMessageL();
			
			/**
			* CancelOperation  Method,Cancels Operation...which are going on...
			* @since S60 v3.1u
			* @param None
			* @return None
			*/ 
			virtual void CancelOperation();
			
			/**
			* EncodeMessageL Method,Encodes SUPL_START message.
			* @since S60 v3.1u
			* @param aSuplVersion, version of SUPL
			* @param aSessionId, sessionid of session.
			* @param aError, error code if any error occures while conversion.
			* @return encoded data, HBufC8*
			*/ 
			virtual HBufC8* EncodeMessageL(TOMASuplVersion &aSuplVersion,
						   COMASuplSessionID* aSessionId,TInt &aError);
			
			/**
			* Capabilities Method, returns the Capabilities pointer
			* @since S60 v3.1u
			* @param None
			* @return SET capabilities retrieved during the creation of SUPL START
			*/
			COMASuplSETCapabilities* Capabilities();
			
			/**
			* SetQop Method, Sets QoP to Encode with SUPL_END
			* @since S60 v3.1
			* @param aQop , QoP parameter which got form client.
			* @return None
			*/
			void SetQop(TSuplTerminalQop& aQop);
			
			/*
             * SetStaleCellIdToUse Method is used to set the Stale Cell id to use in case of
             *                     retrieving position for an already retrieved cell id.
             *                     For example : in case of Geo Tagged Images 
             * @since S60 v5.2
             * @param aLocationId This is the locatiod id parameter containing the stale cell id,ownership is transferred.
             * @return None                     
             */
			void SetStaleCellIdToUse(COMASuplLocationId* aLocationId);
	
	public: //MOMASuplETelObserver
	
			/**
			* RequestCompleted Method, Location Id request completed succesfully
			* @since S60 v3.1u
			* @param aLocationId, all location related information
			* @param aErrorCode, Failure reason code        
			* @return None
			*/
			virtual void LocationIDRequestCompletedL(COMASuplLocationId* aLocationId,TInt aErrorCode);

			/**
			* RequestCompleted Method, E-Cell Id request completed succesfully
			* @since S60 v3.2
			* @param aECId TOMASuplECellInfo, all E-Cell id related information
			* @param aErrorCode, Failure reason code        
			* @return None
			*/
			virtual void LocationECellIdRequestCompletedL(TOMASuplECellInfo& aECId,TInt aErrorCode);
			
			/**
			 * RequestCompleted Method, Location Id request for comparision completed succesfully
			 * @since S60 v3.1u
			 * @param aLocationId, all location related information
			 * @param aErrorCode, Failure reason code        
			 * @return None
			 */
			virtual void ComparisionLocationIDRequestCompletedL(COMASuplLocationId* aLocationId,TInt aErrorCode);
			
			/**
	  	 	* OperationCompleteL,Notifies about complaetion of operation with or without error.
			*@since Series 60 3.1u
			*@param aError,error code occured while completion of operation.
			*/
			virtual void OperationCompleteL(TInt aError);
			
public:			
			/**
			* GetCurrentCellID Method, to get current cell id
			* @since Series 60 3.1
			* @param  TCellIdInfo
			* @return None
			*/ 
			void GetCurrentCellID(TCellIdInfo& aCurrentCellId,TInt& aType);		
			
	private:
			/**
			* FillAllowedCapabilities Method.
			* @since S60 v3.1u
			* @param aAllowedCapabilities,Allowed capabilities of SET 
			* @return None
			*/
			void FillAllowedCapabilities(TInt aAllowedCapabilities);
			
			/**
			* InitilizeAllowedCapabilities Method.
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void InitilizeAllowedCapabilities();

			/**
			* ComputeCapabilities Method,Computes capabilities.. does SETCaps = SETCaps & AllowedCaps
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void ComputeCapabilities();
			
			/**
			* ValidateSetCapabilities Method to validate the capabilities of
			* the SET against the Capabilities retrieved during the construction
			* of SUPL START
			* @since S60  v3.1u
			* @param None
			* @return None
			*/
			TInt ValidateSetCapabilities();
			
			/**
			* SetPrefferedBack Method to Set Preffered method to the one which POS had given
			* @since S60  v3.1u
			* @param None
			* @return None
			*/
			void SetPrefferedBack();

	protected : 
	
			/**
			* Constuctor Method.
			* @since S60 v3.1u
			* @param aMobilePhone,
			* @param aMsgTimeout,Timeout for message
			* @return None
			*/ 
			COMASuplStartState(RMobilePhone& aMobilePhone,
			   TInt aMsgTimeout,COMASuplPOSRequestor* aPosRequestor,COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase);
			/**
			* ConstructL Method, 2nd phase construction
			* @since S60 v3.1u
			* @param aAllowedCapabilities,Allowed capabilities
			* @return None
			*/
			void ConstructL(TInt aAllowedCapabilities);

			/**
			* GetPosParamsL Method, Get the SET Capabilities from POS
			* Message plugin
			* @since S60 v3.1u
			* @param None
			* @return SET capabilities retrieved during the 
			* creation of SUPL START
			*/
			void virtual GetPosParamsL();
	
			/**
			* GetPositionComplete Method, 
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void GetPositionComplete(TInt);
			
			/**
			* GetAssistceDataFromPluginL Method, 
			* @since S60 v3.2
			* @param aErrorCode , error code if any error occured during previous operation
			* @return None
			*/
			void GetAssistceDataFromPluginL(TInt aErrorCode);
			
	protected: //Data
	
			//LocationID Requestor
			COMASuplETel* iLocationIDRequestor;
			
			//Location ID
			COMASuplLocationId* iLocationId;
			
			//SET capabilities
			COMASuplSETCapabilities* iSETCapabilities;
			
			//Allowed capabilities
			COMASuplSETCapabilities* iAllowedCapabilities;
			COMASuplPosSessionBase::TOMASuplPositioningMethod iPositioningMethodforPOS;
			
			TOMASuplAllowedCapabilities iAllowedCapabilitiesforPOS;

			//Connected RMobilePhone session
			RMobilePhone& iMobilePhone;

			//POS Requestor
			COMASuplPOSRequestor* iPosRequestor;
			
			//Enhanaced Cell Id
			TBool  iECId; 
			
			//Generation Status
			TBool iGenerationStatus;
			
			//Received QoP from Client Lib
			TSuplTerminalQop iClientQop;
			
			//is Qop Present
			TBool iIsQoPPresent;
			
			//	SET Preffered method provided by POS
			COMASuplSETCapabilities::TOMASuplPrefMethod    iPOSMsgSETprefMethod;

			//To hold E-cell id data
			TOMASuplECellInfo iMmCellInfo;
			
			COMASuplAsnHandlerBase* iOMASuplAsnHandlerBase;
			
			//Indicates whether we need to create start message with Stale Cell Id
			TBool iIsStaleCellId;

};

#endif //C_COMASUPLSTARTSTATE_H
