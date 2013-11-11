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
* Description:   class for handling SUPL_REPORT message...
*
*/



#ifndef C_COMASUPLREPORTSTATE_H
#define C_COMASUPLREPORTSTATE_H

#include <e32base.h>

#include "epos_comasuplreport.h"
#include "epos_comasuplstate.h"

class COMASuplAsnHandlerBase;

/**
 *	COMASuplReportState handles all SUPL_END related processing.
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 v3.1u
 */


class COMASuplReportState : public COMASuplState
{
	public :  // Constructor 
	
		/**
		* NewL Method.
		* @since S60 3.1u
		* @param None
		* @return Instance of COMASuplReportState
		*/ 
		static COMASuplReportState* NewL(COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase,
                                         HBufC8* aEncodedSuplInit = NULL,
                                         HBufC8* aHSLPAddress = NULL);
		
		/**
		* NewL Method.
		* @since S60 3.1u
		* @param None
		* @return Instance of COMASuplReportState
		*/ 
		static COMASuplReportState* NewL(TInt aMsgTimeout, COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase,
                                         HBufC8* aEncodedSuplInit = NULL,
                                         HBufC8* aHSLPAddress = NULL);
		
		/**
		* Destructor.
		*/
		~COMASuplReportState();
		
    public:		//COMASuplState
    
   	/**
		* GenerateMessageL Method,Generates SUPL_END message...
		* @since S60 3.1u
		* @param None
		* @return system wide error
		*/ 
		virtual TInt GenerateMessageL();

   	/**
		* EncodeMessageL Method,Encodes SUPL_END message...
		* @since S60 3.1u
		* @param aSuplVersion, version of SUPL
		* @param aSessionId, sessionid of session.
		* @param aError, error code if any error occures while conversion.
		* @return encoded data, HBufC8*
		*/ 
		virtual HBufC8* EncodeMessageL(TOMASuplVersion &aSuplVersion,COMASuplSessionID* aSessionId,
									   TInt &aError);
		
    /**
		* CancelOperation  Method,Cancels Operation...which are going on...
		* @since S60 3.1u
		* @param None
		* @return None
		*/ 
		virtual void CancelOperation();

    /**
		* ProcessMessage Method,Process SUPL_END message...
		* @since S60 3.1u
		* @param aDecodedMessage, COMASuplAsnMessageBase ...decoded message
		* @return system wide error
		*/ 
		virtual TInt ProcessMessageL(COMASuplAsnMessageBase* aDecodedMessage); 

		
		/**
		* AddToReportData Method, add report data to report list 
		* @since S60 3.1u
		* @param None
		* @return None
		*/ 
        void AddToReportDataL(COMASuplReportData aReportData);

       /**
		* ComputeSuplReportVer Method,Computes the Verof SUPL INIT
		* in case of Network Initiated Location requests.
		* @since S60 v3.1u
		* @param None
		* @return None
		*/
		
		TBool ComputeSuplReportVerL();

		/**
		* GetPosition Method,Return the Position pointer
		* @since S60 3.1u
		* @param None
		* @return COMASuplPosition*
		*/ 
        COMASuplPosition* GetPositionL() const;
	private:
	
 	   /**
		* LogReceivedPosition Method,Logs position information
		* @since S60 v3.1
		* @param None
		* @return None
		*/
		void PrintHex(const TDesC8& aBuffer,TInt aLine);
		
	protected : 
	
		
		/**
		* Constuctor
		* @since S60 3.1u
		* @param Status code,if any error 
		* @return None
		*/ 
		COMASuplReportState(COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase, HBufC8* aEncodedSuplInit = NULL, HBufC8 *aHSLPAddress = NULL);
		
		/**
		* Constuctor
		* @since S60 3.1u
		* @param Status code,if any error 
		* @return None
		*/ 
		COMASuplReportState(TInt aMsgTimeout, COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase, HBufC8* aEncodedSuplInit = NULL, HBufC8 *aHSLPAddress = NULL);
		
    /**
    * ConstructL Method, 2nd phase construction
    * @since S60 3.1u
    * @param None
    * @return None
    */
    void ConstructL();	

	private: //Data
		COMASuplAsnHandlerBase* iOMASuplAsnHandlerBase;
        RPointerArray<COMASuplReportData> iReportDataList;

        HBufC8 *iEncodedSuplInit;
		HBufC8 *iHSLPAddress;
        TBool iReportDataPresent;

        TBool iVerPresent;
		TBuf8<64> iRes;
};

#endif //C_COMASUPLREPORTSTATE_H
