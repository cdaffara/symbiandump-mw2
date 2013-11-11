/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This class is responsible for communication towards Mediator.
*
*/


#ifndef CMEDIATORSERVICE_H
#define CMEDIATORSERVICE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include <MediatorCommandInitiator.h> 
#include <MediatorEventConsumer.h> 

// forward declarations 

// CLASS DECLARATION

/**
 *  An observer interface for receiving responses to Mediator Service commands.  
 * 
 *  @lib telephonyservice.lib
 *  @since S60 v5.1
 */
NONSHARABLE_CLASS( MMediatorServiceObserver ) 
    {
    public: // Receiving responses

        /**
        * A response to a Mediator Service command.
        *
        * @since S60 5.1       
        * @param aData      The parameters of the response.
        * @return None.
        */
        virtual void CommandResponseL( const TDesC8& aData ) = 0;
        
        /**
         * A Mediator Service event.
         *
         * @since S60 5.1       
         * @param aData      The parameters received with the event.
         * @return None.
         */
        virtual void MediatorEventL( const TDesC8& aData ) = 0;
        
    };


// CLASS DECLARATION

/**
 *  This class is responsible for communication towards Mediator.  
 * 
 *  @lib telephonyservice.lib
 *  @since S60 v5.1
 */
NONSHARABLE_CLASS( CMediatorService ) : public CBase,
                         public MMediatorCommandResponseObserver,
                         public MMediatorEventObserver
    {
public:
    
    /**
     * Enumerates service categories provided by CMediatorService.
     *
     * ECallInfo - CMediatorService is used for call information commands and events.
     * ECallRemotePartyInfo - CMediatorService is used for call remote party information
     *                        commands and events.
     */
    enum TServiceCategory
        {
        ECallInfo,  
        ECallRemotePartyInfo        
        };
    
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CMediatorService();

    /**
     * Two-phased constructor.
     */
    static CMediatorService* NewL( TServiceCategory aService );

    /**
     * Two-phased constructor.
     */
    static CMediatorService* NewLC( TServiceCategory aService );
    
    /**
     * Sets the observer.
     */
    void SetObserver( MMediatorServiceObserver* aObserver );    
    
    /**
     * Send mediator command.
     */
    virtual TInt SendCommand();
    
    /**
     * Subscribe mediator event.
     */
    virtual TInt SubscribeEvent();
    
// from base class MMediatorCommandResponseObserver
         
    /**
     * From MMediatorCommandResponseObserver.
     * A response to a Mediator Service command.
     *
     * @since S60 v3.1
     * @param aDomain    The identifier of the domain          
     * @param aCategory  The identifier of the category.
     * @param aCommandId The identifier of the command.
     * @param aStatus    Standard Symbian error code indicating the
     *                   success of the command.
     * @param aData      The parameters of the response.
     * @return None.
     */
    void CommandResponseL( TUid aDomain,
                           TUid aCategory, 
                           TInt aCommandId,
                           TInt aStatus, 
                           const TDesC8& aData );
    
// from base class MMediatorEventObserver      
          
    /**
     * From MMediatorEventObserver.
     * A Mediator Service event.
     *
     * @since S60 3.1        
     * @param aDomain    The identifier of the domain.
     * @param aCategory  The identifier of the category.
     * @param aEventId   The identifier of the event.
     * @param aData      The parameters of the event.
     * @return None.
     */
    void MediatorEventL( TUid aDomain,
                         TUid aCategory, 
                         TInt aEventId, 
                         const TDesC8& aData );    

protected:

    /**
     * Constructor for performing 1st stage construction
     */
    CMediatorService( TServiceCategory aService );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
private:
    
    /**
     * Sets the parameters used in Mediator commands and events
     */
    void SetMediatorParams();
    
private:  //data
    CMediatorCommandInitiator* iCommandInitiator;
    CMediatorEventConsumer*    iMediatorEvent;
    
    MMediatorServiceObserver*  iObserver;
    
    TServiceCategory    iServiceCategory;
    TUid        iDomain;
    TUid        iCategory; 
    TInt        iCommandId;
    TInt        iEventId;
    TVersion    iVersion;
    };

#endif // CMEDIATORSERVICE_H
