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
* Description:  Definition of CListener class. Observers RProperty changes.
*
*/


#ifndef LISTENER_H
#define LISTENER_H

// SYSTEM INCLUDES
#include <aknappui.h> 
#include <aknViewAppUi.h>
#include <e32property.h>
#include <e32def.h>

// FORWARD DECLARATIONS
class MKeyObserver;
class MKeyAndStatusObserver;

// CLASS DECLARATION
/**
 *  Property listener class.
 *  Observes property keys for key values and status changes.
 *
 *  @code
 *  // Create new listener.
 *	CListener* listener = CListener::NewL( 
 *			 aPropertyCategory,
 *			 aKey
 *			 ); 
 *	 
 *	listener->RegisterNewClientForKeyValueL( aKeyObserver );
 *  @endcode
 *
 *  @lib common.lib
 *  @since -
 */
NONSHARABLE_CLASS( CListener )  : public CActive
    {
public: 

    /**
     * Factory method NewL
     * @param aPropertyCategory The property's category.   
     * @param aKey       		The P&S key to be watched. 
     * @return           		The new watcher object. 
     */
    static CListener* NewL( const TUid aPropertyCategory,
    			const TUint aKey );
    
    /**
     * Factory method NewLC
     * @param aPropertyCategory The property's category.   
     * @param aKey       		The P&S key to be watched. 
     * @return           		The new watcher object on the cleanupstack. 
      */
     static CListener* NewLC( const TUid aPropertyCategory,
     			const TUint aKey );
     
    /**
     * Destructor. Make public to clean it via CPropertyWatcher. 
     */
    virtual ~CListener();
    
public: // New functions
	        
    /**
     * Returns observed key.
     * @return The key value.          		
     */
    TUint ObservedKey () const;
    
    /**
     * Returns observed category.
     * @return The category's uid.          		
     */
    TUid ObservedCategory () const;
    
    /**
     * Removes watcher client. 
     * @param aKeyObserver    Pointer to caller's adress.   
     */
    void RegisterNewClientForKeyValueL ( const MKeyObserver* aKeyObserver );
    
    /**
     * Adds new watcher client. Listens key and status changes.
     * @param aKeyAndStatusObserver    Pointer to caller's adress.   
     */
    void RegisterNewClientForKeyAndStatusL ( const MKeyAndStatusObserver* aKeyAndStatusObserver );
    
     /**
      * Removes watcher client. 
      * @param aKeyObserver    Pointer to caller's adress.   
      */
    void UnregisterKeyClient( const MKeyObserver* aKeyObserver );
    	
     /**
      * Removes watcher client. 
      * @param aKeyAndStatusObserver    Pointer to caller's adress.   
      */
   	void UnregisterKeyAndStatusClient( 
    			const MKeyAndStatusObserver* aKeyAndStatusObserver );
 
private: // Construction and destruction.	

    /**
     * Constructor.
     * @param aPropertyCategory The property's category.   
     * @param aKey       		The P&S key to be watched. 
	 */
   	CListener( const TUid aPropertyCategory,
    			const TUint aKey ); 
    	
   	/**
	 * Symbian 2nd phase constructor.
	 */
    void ConstructL();
	
private: // Functions from CActive
    /**
     *	Implements CActive
     */
    void RunL();
 
    /**
     *	Implements CActive
     *	@param aError the error returned
     *	@return error
     */
    TInt RunError( TInt aError );

    /**
     *	Implements CActive
     */
    void DoCancel();
            
private:
    	
    /**
     *	Starts active object - listen key changes.
     */
	void StartListening();
 
	/**
     *	Send notifications to registered clients.
     *	@param aKeyValue - observed key's value.
     *	@return aStatus - reference to iStatus.
     */
	void SendNotificationsL( const TUint aKeyValue,
			TRequestStatus& aStatus ) const; 
    
private: // Data
    
    /*iArrayForKeyValue - all registered clients for value
     * owned by this class.*/
    RPointerArray<MKeyObserver> iArrayForKeyValue;    

	/*iArrayForKeyAndStatus - all registered clients for status
     * owned by this class*/
    RPointerArray<MKeyAndStatusObserver> iArrayForKeyAndStatus;  
 
    /*iPropertyCategory; - property's gategory.
     * owned by this class*/
    TUid iPropertyCategory;
	
	/*iKey - observed key, owned by this class*/
    TUint iKey;
    
	/*iProperty - owned by this class*/
    RProperty iProperty;
    };

#endif // LISTENER_H

// End of File.
