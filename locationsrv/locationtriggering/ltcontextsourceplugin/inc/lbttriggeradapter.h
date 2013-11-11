
/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Location Trigger Creation and monitoring
*
*/

#ifndef __LBTTRIGGERADAPTER_H__
#define __LBTTRIGGERADAPTER_H__


#include <lbt.h>
#include <lbtsessiontrigger.h>
#include <lbtgeocircle.h>
#include <lbttriggerfiringeventnotifier.h>
#include <lbttriggerinfo.h> 
#include <lbttriggerfilterbyattribute.h> 

#include "lbtcontexttriggerobserver.h"
#include "lbttriggernode.h"

/*
 * A handle to the trigger creation and handling mechanism.
 * This class is responsible for creating triggers and monotoring them.
 */

NONSHARABLE_CLASS( CLocationTriggerAdapter ) :
public CActive,
public MLbtTriggerFiringEventObserver
    {
       
    public:

        /*
         * Default constructor for CLocationTriggerAdapter class.
         * 
         * @param[in] aObserver Handle of the source plugin. The pointer is noted 
         * for calling the virtual functions implemented by the source plugin. 
         */
        CLocationTriggerAdapter( MLBTContextTriggerObserver& aObserver );
        
        /*
         * Default destructor.
         */
        ~CLocationTriggerAdapter();
        
        /*
         * Creates the trigger for the given input node.
         * 
         * @param[in] aTriggerNode Object of TTriggerNode type. 
         * Contains specifications of trigger to be created.
         */
        void CreateContextTriggerL(TTriggerNode& aTriggerNode);
        
        /*
         * Intialises the lbt server and opens sessions to it.
         * The observer for trigger changes is also started here.
         */
        void ConstructL();
        
        /*
         * Two phase constructor for CLocationTriggerAdapter
         */
        static CLocationTriggerAdapter* NewL( MLBTContextTriggerObserver& aObserver );

        /*
         * Two phase constructor for CLocationTriggerAdapter
         */
        static CLocationTriggerAdapter* NewLC( MLBTContextTriggerObserver& aObserver );
        
    public:
        
        /*
         * Derived from CActive
         */
        void RunL();
        
        /*
         * Derived from CActive
         */
        void DoCancel();
        
        /*
         * Derived from CActive
         */
        TInt RunError( TInt aError );
        
        /*
         * Derived from MLbtTriggerFiringEventObserver
         */
        void TriggerFiredL( const TLbtTriggerFireInfo& aFireInfo );
        
        /*
         * Delete all triggers owned by LBT Context Source Plugin here
         * This function is called when an update settings call occurs. 
         */
         void DeleteAllTriggersL();
        
        
    private: 

        /*
         * Pointer to Trigger Fire notifier
         */
        CLbtTriggerFiringEventNotifier* iNotifier;
        
        /*
         * Holds the trigger id,context name,trigger name
         */
        
        TriggerList iTriggerList;
        
        /*
         * Handle to LBT Server
         */
        RLbtServer iLbtServ;
        
        /*
         * Handle to LBT session for trigger creation
         */
        RLbt iLbt;
        
        /*
         * Handle to LBT session for trigger monitoring
         */
        RLbt iLbtNotifier;
        
        /*
         * Obsever for trigger fire changes
         */
        MLBTContextTriggerObserver& iObserver;
        
        
    };

#endif // __LBTTRIGGERADAPTER_H__
