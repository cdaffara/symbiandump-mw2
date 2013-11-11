/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class declaration of Psy change handler
*
*/


#ifndef C_LBTPSYCHANGELISTNER_H
#define C_LBTPSYCHANGELISTNER_H

#include <e32base.h>
#include <lbs.h>


class MLbtPsyChangeObserver;

/**
 *  This class listens to PSY change
 *  
 *  CLbtPsyChangeHandler registers for notification change in PSY
 *
 *  @since S60 v5.1
 */
class CLbtPsyChangeListner : public CActive
    {
public:
    /**
     * Factory function that instantiates an object of CLbtPsyChangeListner
     */
    static CLbtPsyChangeListner* NewL( MLbtPsyChangeObserver& aObserver );
    
    /**
     * Destructor.
     */
    virtual ~CLbtPsyChangeListner();

    /**
     * Starts listening to PSY change
     */
    void StartToListen( TPositionModuleId aPosID );

         
protected: // From CActive
    void RunL();
    void DoCancel();

private: 
    /**
     * Default constructor
     */
    CLbtPsyChangeListner( MLbtPsyChangeObserver& aObserver );
    
    /**
     * Symbian 2nd phase constructor
     */
    void ConstructL(); 

private://Data

    /**
     * Session to RPositionServer
     */
    RPositionServer iPosServer; 
    
    /**
     * PSY change observer
     */
    MLbtPsyChangeObserver& iObserver;
    
    /**
     * Module Id of the PSY for which change notification
     * is requested
     */
    TPositionModuleId iPosID;
    
    /**
     *
     */
    TPositionModuleStatusEvent iModuleStatusEvent;
    };


#endif // C_LBTPSYCHANGELISTNER_H
