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
* Description:  Provides implementation class for CCallInformation -class.
*
*/


#ifndef CCALLINFORMATIONIMPL_H_
#define CCALLINFORMATIONIMPL_H_

#include <e32base.h>

#include "ctelsrvbase.h"
#include "mcall.h"

// forward declarations 
class MCallInformationObserver;
class CCallInfoIter;
class CCallInfos;


/**
 *  This class provides implementation for CCallInformation -class.  
 * 
 *  @lib telephonyservice.lib
 *  @since S60 v5.1
 */
NONSHARABLE_CLASS( CCallInformationImpl ) : public CTelSrvBase                                            
    {
public:
    
    /**
    * Two-phased constructor.         
    */
    static CCallInformationImpl* NewL();
    
    /**
     * Two-phased constructor for testing purposes.         
     */
    static CCallInformationImpl* NewL(CMediatorService* aMediatorService);    
    

    /**
    * Destructor.
    */
    virtual ~CCallInformationImpl();

    /**
    * Sets the observer for changes in current calls.
    *
    * @since S60 v5.1 
    * @param aObserver Observer for notifications.
    */
    void NotifyCallInformationChanges( MCallInformationObserver& aObserver );
    
    /**
    * Used to cancel previously placed NotifyCallInformationChanges call.
    *
    * @since S60 v5.1      
    */
    void CancelNotification();
     
    /**
    * Gets the snapshot of ongoing calls.
    *
    * @since S60 v5.1      
    * @return Ongoing calls.
    */
    CCallInfoIter& GetCallsL(); 
    
    /**
     * Returns information whether call in given state exist.
     * 
     * @since S60 v5.2
     * @param aState Specifies a call state that is used as a search key.
     * @return ETrue if call in given state exist.
     */
    TBool IsCallInState( CCPCall::TCallState aState ) const;
     
// from base class CTelSrvBase
    
    /**
     * From CTelSrvBase.
     * A response to a Mediator Service command.
     *
     * @since S60 v5.1
     * @param aData      The parameters of the response.
     */
    void DoHandleCommandResponseL( const TDesC8& aData );      
      
    
    /**
     * From CTelSrvBase.
     * A Mediator Service event.
     *
     * @since S60 v5.1     
     * @param aData      The parameters of the event.
     * @return None.
     */
    void DoHandleMediatorEventL( const TDesC8& aData );                         
       
protected:
    
    CCallInformationImpl();

    void ConstructL(CMediatorService* aMediatorService);
    
protected: //data    
        
    /**
     * Observer to be notified when call information changes
     * Not own.  
     */
    MCallInformationObserver* iObserver;   
private: // data    
      
    CCallInfos*       iInfos;    
    CCallInfos*       iInfosInState;
    CCallInfoIter*    iCallIter;    
    CCallInfoIter*    iCallStateIter;
    };


#endif /*CCALLINFORMATIONIMPL_H_*/

