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



#ifndef C_COMASUPLCOMMONCONFIG_H
#define C_COMASUPLCOMMONCONFIG_H

#include <e32base.h>

 class COMASuplSettings;
 class COMASuplInitilizeRequestor;
 class COMASuplPosHandlerBase; 
 
 /**
 *  class for holding common config for protocol handler plugins. 
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 v3.1u
 */
 
class COMASuplCommonConfig : public CBase
{
public:

	/**
	* NewL Method.
	* @since S60 v3.1u
	* @param None.
	* @return Instance of COMASuplCommonConfig
	*/
	IMPORT_C static COMASuplCommonConfig* COMASuplCommonConfig::NewL();
	
	/**
	* ~COMASuplCommonConfig Method, Destructor
	* @since S60 v3.1u
	* @param None.
	* @return None
	*/
	~COMASuplCommonConfig();
	
	IMPORT_C void ReadOMASuplSettingsL();

private:  // Constructor 
	
	/**
	* C++ default constructor.
	*/
	COMASuplCommonConfig();
	
	/**
	* Symbian 2nd phase constuction  ConstuctL.
	* @since S60 v3.1u
	* @param None.
	* @return None
	*/
	void ConstructL();
	
	void ReadOMASuplConfigurationL();
	
	void LoadPosHandlerL();

public:

	//Tracing utility
	COMASuplTrace* iTrace;

	//Pos Handler
	COMASuplPosHandlerBase *iPosHandler;
	
	// OMA Supl Settings
	COMASuplSettings* iSuplSettings;
		
	// Container to store IMSI value
	RBuf    iIMSI;
	
	//	UT1 Timer for SUPL_START
	TInt iUT1_StartTimer;
	
	//	UT2 Timer for SUPL_POS_INIT
	TInt iUT2_PosInitTimer;
	
	//	UT3 Timer for SUPL_POS
	TInt iUT3_PosTimer;
	
	//Privacy Timer
	TInt iPrivacyTimer;
	
	//SET Mode
	TInt iSETMode;

    //SUPL Usage configuration
    TInt iSuplUsage;
    // Timer for persistent failure
    TInt iPersistFailTimer;
    
    TInt iSuplInitTimeOut;
    
};

#endif //C_COMASUPLCOMMONCONFIG_H
