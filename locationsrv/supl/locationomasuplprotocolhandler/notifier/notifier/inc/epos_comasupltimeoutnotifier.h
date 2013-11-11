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
* Description:   SUPL Timeout Notifier class definition
*
*/



#ifndef C_SUPLTIMEOUTNOTIFIER_H
#define C_SUPLTIMEOUTNOTIFIER_H

// System Includes
#include <eiknotapi.h>

// User Includes

// Forward Declarations
class CEikonEnv;
class CAknQueryDialog;
class CAknNotifierAppServerAppUi;

/**
 * Class definition for SUPL Timeout notifier.
 *
 * This class extends MEikSrvNotifierBase2. This class provides a
 * description for SUPL Timeout Notifier. The notifier is invoked
 * when the SUPL connection  times out.
 *
 * @since S60 v5.0.1
 */
class CSuplTimeOutNotifier : public CActive,
                      public MEikSrvNotifierBase2
    {
private: // Constants and Enumerations
	enum TSuplEvent    
		{
		// No event
		ENone,
		
		// Display SUPL notifier
		EDisplayNotifier
		};
public:
    /**
     * Destructor.
     */
    virtual ~CSuplTimeOutNotifier();
    
    /**
     * Symbian two phased constructor
     */
    static CSuplTimeOutNotifier* NewL();
    
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
    	    
public: // Inherited from MEikSrvNotifierBase2

    /**
     * Registers the SUPL Timeout notifier
     */
    virtual TNotifierInfo RegisterL();
                                          
protected:
    /**
     * C++ constructor made protected for this class must not be instantiated
     * directly
     */
    CSuplTimeOutNotifier();
    
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
    CAknQueryDialog*    	iSuplQuery;
    
    /**
	 * SUPL Query text.
	 * Owns
	 */
	HBufC*						iQueryText;
	
	/**
	 * Current SUPL event
	 */
	TSuplEvent					iSuplEvent;
		
	/**
	 * Boolean flag to indicate whether App switching has been
	 * enabled.
	 */
	TBool                       iAppSwitchSupress;
	
	/**
	 * AppUi from Notifier Server
	 */
	CAknNotifierAppServerAppUi* iAppUi;
    };

#endif // C_SUPLTIMEOUTNOTIFIER_H
