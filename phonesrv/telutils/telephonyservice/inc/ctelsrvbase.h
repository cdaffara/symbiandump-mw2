/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides base class for telephonyservice API implementation classes.
*
*/

#ifndef CTELSRVBASE_H
#define CTELSRVBASE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include "cmediatorservice.h"

// CLASS DECLARATION

/**
 *  CTelSrvBase
 * 
 */
NONSHARABLE_CLASS( CTelSrvBase ) : public CBase,
                                   public MMediatorServiceObserver 
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CTelSrvBase();    
    
// from base class MMediatorServiceObserver
        
    /**
     * From MMediatorEventObserver.     
     * Template method for handling a response to a Mediator Service command.
     *
     * @since S60 v5.1
     * @param aData      The parameters of the response.
     */
    void CommandResponseL( const TDesC8& aData );      
          
    /**
     * From MMediatorEventObserver.
     * Template method for handling a Mediator Service event.     
     *
     * @since S60 v5.1     
     * @param aData      The parameters of the event.
     * @return None.
     */
    virtual void MediatorEventL( const TDesC8& aData );
    
protected:
    
    /**
     * Constructor for performing 1st stage construction
     */
    CTelSrvBase();
    
    /**
     * EPOC default constructor for performing 2nd stage construction
     */    
    void BaseConstructL( CMediatorService* aMediatorService );
    
    /**
     * Get the current info through Mediator. 
     * Takes care about the situation that Mediator command must be re-send.
     *
     * @since S60 v5.1      
     */
    void Initialize();  
    
    /**
     * Abstract method to be implemented by concrete subclass. 
     * 
     *
     * @since S60 v5.1      
     */
    virtual void DoHandleCommandResponseL( const TDesC8& aData ) = 0;
    
    /**
     * Abstract method to be implemented by concrete subclass. 
     * 
     *
     * @since S60 v5.1      
     */
    virtual void DoHandleMediatorEventL( const TDesC8& aData ) = 0;
    
    /**
     * Starts re-sending mediator command.
     *
     * @since S60 v5.1       
     */
    static TInt DoReSendCommand(TAny* aPtr);
    
    /**
     * Subscribe to Mediator Event.
     *
     * @since S60 v5.1       
     */
    virtual void SubscribeMediatorEventL(); 

protected: //data
    
    // Ownded: Mediator service proxy, must be created by by concrete sublass. 
    CMediatorService* iMediatorService;
    
private: //data    
    /**
     * To synchronize construction.
     * Own.
     */
    CActiveSchedulerWait* iWaitForCommandResponse;    
     
    /**
     * idle object handling of re-sending mediator command.
     * Own.
     */
    CIdle*            iCommandBuffer; 

    };

#endif // CTELSRVBASE_H
