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
* Description:   Class definition of COMASuplIMSIObserver
*
*/



#ifndef C_COMASUPLIMSIOBSERVER_H
#define C_COMASUPLIMSIOBSERVER_H

#include <e32property.h>
#include <epos_csuplsettings.h>
#include "epos_csuplsettingsinternal.h"
#include "epos_msuplimsiobserver.h"

#include "epos_comasuplsmslistenerbase.h"


/**
*  Class definition for OMA SUPL GSM SMS Listener implementation
*  @since S60 v3.1
*/
class COMASuplIMSIObserver : public CActive
    {

public:

    /**
     * Factory function that instantiates a new object of 
     * COMASuplGsmSmsListener
     *
     */
	static COMASuplIMSIObserver* NewL(MHandleImsiObserver& aObserver);
	
    /**
     * Returns current state of the observer
     *
     */
	TInt State();
	
    /**
     * Destructor
     *
     */
	~COMASuplIMSIObserver();

private:

    /**
     * Default constructor
     *
     */
	COMASuplIMSIObserver(MHandleImsiObserver& aObserver);
	
    /**
     * 2nd phase constuctor for instantiating member variables
     *
     */
	void ConstructL();

    /**
     * Checks the current system state to initiate further processing
     *
     */	
    void InitializeL();
    
    /**
     * Initializes the instance of CSuplSettings
     *
     */	
    void InitializeSettings();
    
    /**
     * Handles the event of a SIM change
     *
     */	
    virtual void HandleIMSIChangeL();
    
    /**
     * Request completion event handler
     *
     */	
    void RunL();
    
    /**
     * Cancels any outstanding request
     *
     */	
    void DoCancel();
    
    /**
     * IsCenRepEmptyL... checks CenRep is empty or not...
     *
     */	
    TBool IsCenRepEmptyL();

    /**
     * CheckForBootCompletionL... Check Bootcompletion done...if yes update settings
     *
     */	
	void CheckForBootCompletionL();
    
	/**
	 * Wrapper function used for tracing
	 *
	 */
	void Trace( const TDesC& aMessage, const TDesC& aFileName, TInt line );
    
private:

    /**
     * Current state of COMASuplIMSIObserver
     */	
    COMASuplSmsListenerBase::TState iState;
    
    /**
     * Handle to the startup property object
     *
     */	
    RProperty                       iStartupObserver;
    
    /**
     * Handle to CSuplSettings class used to update SUPL settings
     *
     */	
    CSuplSettingsInternal*                  iSettings;
    
    MHandleImsiObserver&           iImsiObserver;
    
    };

#endif  // C_COMASUPLIMSIOBSERVER_H
