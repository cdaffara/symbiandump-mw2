/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observes Profile state changes.
*
*/



#ifndef CSATPROFILECHANGEOBSERVER_H
#define CSATPROFILECHANGEOBSERVER_H

#include <e32base.h>
#include "MSatSystemStateObserver.h"

class MSatSystemStateChangeNotifier;
class MSatUtils;


/**
 *  Observes Profile status change and informs CSatCommandContainer
 *  of a possible change.
 *
 *  @lib SatEngine
 *  @since S60 3.0
 */
class CSatProfileChangeObserver : public CBase,
                                  public MSatSystemStateObserver
    {
public:  

    /**
     * Two-phased constructor.
     * @param aUtils Utils inteface
     */
    static CSatProfileChangeObserver* NewL( MSatUtils& aUtils );

    /**
     * Destructor.
     */
    virtual ~CSatProfileChangeObserver();


    /**
     * From MSatSystemStateObserver Notifies that system state changed.
     *
     * @since S60 v3.0
     * @param aValue The new value that has changed.
     */
    void StateChanged( const TInt aValue );

private:

    /**
     * C++ default constructor.
     * @param aUtils Utils inteface
     */
    CSatProfileChangeObserver( MSatUtils& aUtils );

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

private:  // Data

    /**
     * Reference to command container.
     */
    MSatUtils& iUtils;    

    /**
     * System state notifier.
     */
    MSatSystemStateChangeNotifier* iStateNotifier;


    /**
     * Stores the previous profile state.
     */
    TBool iPreviousProfile;     
    };
    
#endif // CSATPROFILECHANGEOBSERVER_H
