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
* Description:   class for holding SUPL settings
*
*/



#ifndef C_COMASUPLSETTINGS_H
#define C_COMASUPLSETTINGS_H

#include <e32base.h>
#include "epos_csuplsettings.h"


 /**
 *  Setting class for holding HSLP address & IAP Id. 
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 v3.1u
 */
 
class COMASuplSettings : public CBase
{
public:

	/**
	* NewL Method.
	* @since S60 v3.1u
	* @param None.
	* @return Instance of COMASuplSettings
	*/
	static COMASuplSettings* NewL();
	
	/**
	* ~COMASuplSettings Method, Destructor
	* @since S60 v3.1u
	* @param None.
	* @return None
	*/
	~COMASuplSettings();
	
private:  // Constructor 
	
	/**
	* C++ default constructor.
	*/
	COMASuplSettings();
	
	/**
	* Symbian 2nd phase constuction  ConstuctL.
	* @since S60 v3.1u
	* @param None.
	* @return None
	*/
	void ConstructL();
	
public: //Setter Methods

	/**
	* SetHSLPAddress Method.
	* @since S60 v3.1u
	* @param aHSLPIPAddress , HSLPIPAddress to set.
	* @return None 
	*/
	void SetHSLPAddress(const TDesC& aHSLPIPAddress);

/**
	* SetHSLPAddress from IMSI Method.
	* @since S60 v3.1u
	* @param aHSLPIPAddress , HSLPIPAddress to set.
	* @return None 
	*/
	void SetHSLPAddressFromIMSI(const TDesC& aHSLPfromImsi);
	/**
	* SetIAPId Method.
	* @since S60 v3.1u
	* @param aIAPID, IAP id to set
	* @return None 
	*/
	void SetIAPId(TInt aIAPId);

	/**
	* SetSUPLUsage Method.
	* @since S60 v3.2
	* @param aSUPLUsage 
	* @return None 
	*/
  //  void SetSUPLUsage(CSuplSettings::TSuplSettingsUsage& aSUPLUsage );
	
	/**
	* SetUIActive Method.
	* @since S60 v3.1u
	* @param aIsActive, flag to indicate the activation of UI
	* @return None 
	*/
	void SetUIActive(TBool aIsActive);
	
public: //Getter Methods		

	/**
	* IPAddress Method.
	* @since S60 v3.1u
	* @param None
	* @return IP Address 
	*/
	const TDesC8& SLPAddress() const;
	/**
	* Retrieves the HSLP address generated from IMSI
	* @since S60 v3.1u
	* @param None
	* @return IP Address 
	*/
	const TDesC8& SLPAddressfromImsi() const;
	
	/**
	* IAPId Method.
	* @since S60 v3.1u
	* @param None
	* @return IAP Id
	*/
	TInt IAPId() const;

	/**
	* SUPL Usage Method.
	* @since S60 v3.2
	* @param None
	* @return SUPL Usage
	*/
  //  CSuplSettings::TSuplSettingsUsage SUPLUsage() const;
	
	/**
	* IAPId Method.
	* @since S60 v3.1u
	* @param None
	* @return True / False
	*/
	TBool IsUIActive() const;
	
	/**
     * ConvertIAPNameToId,Converts IAP Name to IAP Id
     * @since S60 v3.1u
     * @param       aIAPName -  Name of IAP
     * @param       aIAPId - Id of APN
     * @return      TUint32 - Id of the IAP Name
     */  														
	TBool ConvertIAPNameToIdL(const TDesC& aIAPName, TUint32& aIAPId);
	TBool IsHslpNull();

private:

	// HSLPIP Address
	RBuf8 iHSLPIPAddress;
	RBuf8 iHSLPfromImsi;
	// IAP Id 
	TInt  	 iIAPId;

    // SUPL Usage
 //   CSuplSettings::TSuplSettingsUsage iSUPLUsage; 
	// Flag indicating the activation of settings UI
	TBool    iSettingsUIActive;
};

#endif //C_COMASUPLSETTINGS_H
