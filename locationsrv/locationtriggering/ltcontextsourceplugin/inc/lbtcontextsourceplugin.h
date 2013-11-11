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
* Description:  CLBTContextSourcePlugin DLL source
*
*/

#ifndef __LBTCONTEXTSOURCEPLUGIN_H__
#define __LBTCONTEXTSOURCEPLUGIN_H__

//  Include Files
#include <cfcontextsourceplugin.h>
#include <cfcontextsourceinterface.h>
#include <cfcontextsourcesetting.h>
#include <cfcontextsourcesettingarray.h>
#include <cfcontextobject.h>
#include <cfcontextsourceupgrade.h>
#include <cfcontextsourcecommand.h>


#include <e32capability.h>
#include <cfcontextinterface.h>

#include "lbttriggeradapter.h"
#include "lbttriggermanager.h"

_LIT_SECURITY_POLICY_C1( KLBTContextSec, ECapabilityLocalServices );

/*
 * This class is responsible for parsing the input settings file. The parsed setting information is
 * passed to the CLocationTriggerAdapter class to create and monitor these triggers.
 */
NONSHARABLE_CLASS( CLBTContextSourcePlugin ):
    public CCFContextSourcePlugIn,
    public MCFContextSourceUpgrade,
    private MLBTContextTriggerObserver
    {
    public:
    
        /*
         * Two phase constructor for CLBTContextSourcePlugin
         */
        static CLBTContextSourcePlugin* NewL( 
            TContextSourceInitParams* aParams );
        
        /*
         * Two phase constructor for CLBTContextSourcePlugin
         */
        static CLBTContextSourcePlugin* NewLC( 
            TContextSourceInitParams* aParams );
        
        /*
         * Create an instance of the CCFContextObject object here.
         * This object is used to define and publish contexts.
         */
        void ConstructL();
            
        /*
         * Default contructor for CLBTContextSourcePlugin.
         */
        CLBTContextSourcePlugin( TContextSourceInitParams* aParams );
        
        /*
         * Default destuctor for CLBTContextSourcePlugin class
         */
        ~CLBTContextSourcePlugin();
    
    public:
    
        /*
         * Derived from CCFContextSourcePlugIn
         * Context Framework parses the XML settings file and passes up the trigger data 
         */
        void HandleSettingL(
             CCFContextSourceSettingArray* aSettingList );
            
        /*
         * Derived from CCFContextSourcePlugIn
         * Called by Context Framework to define contexts. 
         * LBT Source plugin does not define contexts here, hence left empty.
         */
        void DefineContextsL();
        
        /*
         * Derived from CCFContextSourcePlugIn
         */
        void InitializeL();
        
        
        /*
         * @see MCFContextSourceUpgrade
         */
        TInt CheckValidity( RThread& aClientThread, 
                const CCFContextSourceSettingArray& aSettingList); 
        /*
         * @see MCFContextSourceUpgrade
         */
        void UpdateSettingsL( CCFContextSourceSettingArray* aSettingList );
        
        /*
         * @see CCFContextSourcePlugIn
         */
        TAny* Extension( const TUid& aExtensionUid )const;
        
        

        /*
         * Derived from MLBTContextTriggerObserver
         * Callback for fired trigger
         * 
         * @param[in] aTriggerId Trigger id of fired trigger
         */
        void ContextTriggerFiredL( TLbtTriggerId aTriggerId );
        
        /*
         * Derived from MLBTContextTriggerObserver
         * Callback for error in trigger creation.
         */
        void NotifyError( TInt aError );
        
        
    private: 
        
        /*
         * Parse TriggerLocation node and extract required trigger information.
         * Information required for trigger creation like latitude, longitude, trigger name, etc.
         * are parsed and stored here. The parsed information is stored into an array of type TTriggerNode.
         * 
         * param[in] aItem TriggerLocation node
         */
        void ParseTriggerInformationL( const CCFContextSourceSetting& aItem );
        
        /*
         * Contexts are defined here. The list of created triggers is retrieved from iTriggerList array.
         */
        void DefineTriggerContexts( );
        
        /*
         * Calls CreateTrigger() from CLocationTriggerAdapter class with items from iTriggerNodeList
         */
        void CreateTriggersL();
        
        /*
         * Derived from MLBTContextTriggerObserver
         * List of triggers created is maintained here. After trigger creation the trigger ID, 
         * name and context name are maintained in an array here. 
         */
        void TriggerCreatedL( TLbtTriggerId& aTrigId , TPtrC& aContextName, TPtrC& aTriggerName );
        
    private: 
        
        /**
         * Holds the list of parsed values from input settings file
         */
        CCFContextSourceSettingArray* iSettingList;
    
        /*
         * Pointer to instance of CLocationTriggerAdapter class
         */
        CLocationTriggerAdapter *iLocationTrigger;
        
        /*
         * Pointer to class monitoring LBT ManagementUI operations
         */
        CLocationTriggerManager* iLbtManagerObserver;
        
         /*
          *  List of triggers.
          *  Used for keeping track of triggers defined in user file.
          */
         RPointerArray<TTriggerNode> iTriggerNodeList;
         
         /*
          * List of { trigger ID, context name, trigger name}
          */
         RPointerArray<TriggerList> iTriggerList;
       
         /*
          *  Index value of next trigger to be created
          */
         TInt iCurrentTriggerIndex;      
         
         /*
          * Flag to check for completion of all trigger creation requests.
          */
         TBool iTriggersCreated;
         
    };

#endif  // __LBTCONTEXTSOURCEPLUGIN_H__

