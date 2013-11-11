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
* Description:   GSM SMS Listener class definition
*
*/



#ifndef C_COMASUPLGSMSMSLISTENER_H
#define C_COMASUPLGSMSMSLISTENER_H

#include "epos_comasuplsmslistenerbase.h"
//#include "epos_comasuplbackuplistener.h"
#include "epos_mproductconfigobserver.h"
#include "epos_msuplimsiobserver.h"

class COMASuplIMSIObserver;
class COMASuplProductConfig;
/**
*  Class definition for OMA SUPL GSM SMS Listener implementation
*  @since S60 v3.1
*/
class COMASuplGsmSmsListener : public COMASuplSmsListenerBase, public MProductConfigObserver,
                               public MHandleImsiObserver
    {

public:

    /**
     * Factory function that instantiates a new object of 
     * COMASuplGsmSmsListener
     *
     */
	static COMASuplSmsListenerBase* NewL();
	
    /**
     * Destructor
     *
     */
	~COMASuplGsmSmsListener();
	
	/**
	 * Notifier
	 */
	void HandleProductConfigCompleteL();
	
	/*
	     *Handles the deletion of ImsiObserver object after its purpose is achieved
	     */
	void HandleImsiObserverCompleteL();

private:

    /**
     * Default constructor
     *
     */
	COMASuplGsmSmsListener();
	
    /**
     * 2nd phase constuctor for instantiating member variables
     *
     * @since S60 v3.1
     */
	void ConstructL();

    // from base class COMASuplSmsListenerBase
    
    /**
     * Associates the socket with a SMS address family 
     * and binds the socket to listen on a WDP port
     *
     * @since S60 v3.1
     */
    virtual void InitializeL();

    /**
     * Extracts the payload from the received SMS message
     *
     * @since S60 v3.1
     */	
    virtual void ProcessMessageL();
    
private:

    /**
     * Handle to COMASuplIMSIObserver class
     */ 
    COMASuplIMSIObserver*       iSuplImsiObserver;

	/**
	* Handle to COMASuplBackupListener class used to observe restore operation
	*
	*/	
    //COMASuplBackupListener*      iBackupListener;
    COMASuplProductConfig*         iProductConfig;
    };

#endif  // C_COMASUPLGSMSMSLISTENER_H
