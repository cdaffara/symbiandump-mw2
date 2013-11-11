/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the MCbsMcnObserver class.
*
*                MCbsMcnObserver is a mix-in class, which must be implemented
*                by a MCN client class. 
*
*                The sole method of this interface gets called when the MCN state
*                changes. This occurs when a MCN message is received from network
*                and the cell information relayed by the message differs from 
*                the previous cell information.
*
*/



#ifndef MCBSMCNOBSERVER_H
#define MCBSMCNOBSERVER_H

// FORWARD DECLARATIONS
class TCbsMcnMessage;

//  CLASS DECLARATION

/**
*   An interface class for MCN observers.
*
*   A class on the client side is required to derive from this class
*   and register itself to an instance of CMcn.
*/
class MCbsMcnObserver
    {
public:             // New functions

    /**
    *   This method is called when a message of a subscribed topic 
    *   is received from the network.
    *
    *   @param  aMcnMessage         MCN message received from the network.
    *   @return                     Result code
    */
    virtual TInt CellInfoChanged( const TCbsMcnMessage& aMcnMessage ) = 0;    
    
    /**
    *   Virtual destructor.
    */
    virtual ~MCbsMcnObserver() {};

    };

#endif      // __MCBSMCNOBSERVER_H
            
// End of File


