/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   SUPL Notifiers Base class definition
*
*/



#ifndef C_SUPLNOTIFIER_H
#define C_SUPLNOTIFIER_H

// System Includes
#include <eiknotapi.h>

// User Includes

// Forward Declarations
class CEikonEnv;
class CAknMessageQueryDialog;
class CLocSettingsUiClient;
class CAknNotifierAppServerAppUi;

/**
 * Base class definition for SUPL notifiers.
 *
 * This class provides a base class description for SUPL Notifiers. The
 * derived implementation need to extend this for a particular SUPL notifier.
 * The notifiers inheriting from this hierarchy only differ in the logical
 * strings which they display to the user.
 *
 * @since S60 v3.2
 */
class CSuplNotifier : public CActive,
                      public MEikSrvNotifierBase2
    {
private: // Constants and Enumerations
	enum TSuplEvent    
		{
		// No event
		ENone,
		
		// Display SUPL notifier
		EDisplayNotifier,
		
		// Dismiss the current SUPL notifier dialog
		
		EDismissNotifier,
		
		// Launch SUPL settings
		ESettingsLaunched,
		};
public:
    /**
     * Destructor.
     */
    virtual ~CSuplNotifier();
    
public: // Virtual function to be implemented by 
        // the child classes
    
    /**
     * Returns the query text for the SUPL notifier
     */
    virtual HBufC* QueryTextL() = 0;
    
    /**
     * Returns the SUPL query ID
     */
    virtual TInt SuplQueryId()  = 0;
    
public: // Inherited from MEikSrvNotifierBase2

    /**
     * Frees all the resources owned by the notifier
     */
    virtual void Release();

    /**
     * Gets the notifier parameters.
     */
    TNotifierInfo Info() const;
     
    /**
     * Starts the notifier.
     */
    virtual TPtrC8 StartL( const TDesC8&        aBuffer );
     
    /**
     * Starts the notifier. This is an asynchronous function. The completion
     * of the request can be communicated through the RMessagePtr2 object
     */
    virtual void StartL( const TDesC8&          aBuffer,
                               TInt             aReplySlot,
                         const RMessagePtr2&    aMessage );
     
    /**
     * Cancels an active notifier.
     */
    virtual void Cancel();
     
    /**
     * Updates a currently active notifier with new data.
     */
    virtual TPtrC8 UpdateL( const TDesC8&   aBuffer );
    
    /**
     * Updates a currently active notifier with new data.
     */    
    virtual void UpdateL( const TDesC8&         aBuffer, 
                                TInt            aReplySlot, 
                          const RMessagePtr2&   aMessage);
                          
	/**
	 * Hightlight click callback
	 */
	static TInt LinkClickedL( TAny* 	aMyClass = NULL );
	                
protected:
    /**
     * C++ constructor made protected for this class must not be instantiated
     * directly
     */
    CSuplNotifier();
    
    /**
     * Second phase of the two phase constructor
     */
    void ConstructL();
    
private:
	/**
	 * Completes any outstanding request with the requested error code.
	 */
	void CompleteMessage( TInt aError );
	    
protected: // Inherited from CActive
    virtual void RunL();
    
    virtual void DoCancel();
    
    virtual TInt RunError( TInt aError ); 
     
protected:    
    /**
     * Eikon Env pointer
     */
    CEikonEnv*                  iEikEnv;
        
    /**
     * Notifier information structure
     * Owns
     */
    TNotifierInfo               iInfo;
            
private:  // Data members

    /**
     * Handle to the current message
     */
    RMessagePtr2                iMessage;
    
    /**
     * Resource file handle
     */
    TInt                        iResourceFileFlag;
    
    /** 
     * Confirmation query dialog
     */
    CAknMessageQueryDialog*    	iSuplQuery;
    
    /**
	 * SUPL Query text. This query appends the actual message
	 * text with the link.
	 * Owns
	 */
	HBufC*						iQueryText;
	
	/**
	 * Current SUPL event
	 */
	TSuplEvent					iSuplEvent;
	
	/**
	 * Settings UI client handle
	 */
	CLocSettingsUiClient*       iSettingsUi;
	
	/**
	 * Boolean flag to indicate whether App switching has been
	 * enabled.
	 */
	TBool                       iAppSwitchSupress;
	
	CAknNotifierAppServerAppUi* iAppUi;
    };

#endif // C_SUPLNOTIFIER_H
