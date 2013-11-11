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
* Description:   CDMA SMS Listener class definition
*
*/



#ifndef C_COMASUPLCDMASMSLISTENER_H
#define C_COMASUPLCDMASMSLISTENER_H

#include "epos_comasuplsmslistenerbase.h"

class COMASuplIMSIObserver;

/**
 *  Class definition for OMA SUPL CDMA SMS Listener implementation
 *  @since S60 v3.1
 */
class COMASuplCdmaSmsListener : public COMASuplSmsListenerBase
    {

public:

    /**
     * Factory function that instantiates a new object of 
     * COMASuplCdmaSmsListener
     *
     */
	static COMASuplSmsListenerBase* NewL();
	
    /**
     * Destructor
     *
     */
	~COMASuplCdmaSmsListener();

private:

    /**
     * Default constructor
     *
     */
	COMASuplCdmaSmsListener();
	
    /**
     * 2nd phase constuctor for instantiating member variables
     *
     */
	void ConstructL();

    // from base class COMASuplSmsListenerBase
    
    /**
     * Associates the socket with a SMS address family 
     * and binds the socket to listen on a WDP port
     *
     * @since S60 v3.1
     */
    void InitializeL();

    /**
     * Extracts the payload from the received SMS message
     *
     * @since S60 v3.1
     */	
    void ProcessMessageL();

private:

    /**
     * Handle to COMASuplIMSIObserver class
     */ 
    COMASuplIMSIObserver*       iSuplImsiObserver;
    
    };


#endif  // C_COMASUPLCDMASMSLISTENER_H
