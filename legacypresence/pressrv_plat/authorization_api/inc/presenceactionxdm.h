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
* Description:    RLS and Presence XDM, This class represents Presence Action
*
*/



#ifndef __PRESENCEACTIONXDM_H__
#define __PRESENCEACTIONXDM_H__

// Includes
#include <e32base.h>
#include <rlspresxdmconsts.h>

//Forward declarations
class CXdmDocumentNode;
class CPresenceXDM;


// Class declaration
/** 
* This class should be used in conjunction with an already created CPresenceXDM.
*  The CPresenceXDM deals with the Presence rules as a whole and has communication
*  with the server.
*  TPresenceActionXDM is used to manipulate the Action part of a rule. 
*  Remember to call Init function after constructing this object.
*  
*  @lib rlspresxdm.lib
*  @since S60 3.2
*/
class TPresenceActionXDM
    {
    public:
    
        /**
        * C++ Constructor, After contructing Init must be called before using
        * this object.
        */                                        
        IMPORT_C TPresenceActionXDM();

        /**
        * Initializes the object with the given pointer to CPresenceXDM.
        * This function must be called before using other functions.
        *
        * @param CPresenceXDM* pointer to created CPresenceXDM
        * @return none
        */
        IMPORT_C void Init(CPresenceXDM* const aPresXDM);
        

        /**
        * Adds or replace action contents.
        *
        * @param TDesC& aRuleID rule ID 
        * @param TDesC& aAction it can be block,confirm, polite-block or allow,
        *   use constants from rlspresxdmconsts.h
        * @return TInt KErrNone, KErrNotFound if rule doesnt found,
        *   KErrArgument if invalid aAction
        */
        IMPORT_C TInt AddOrReplaceActionL(const TDesC& aRuleID, 
                                                        const TDesC& aAction);
                                                                
        /**
        * Removes action from a rule.
        *
        * @param TDesC& aRuleID rule ID 
        * @return TInt KErrNone, KErrNotFound if rule or action doesnt found
        */
        IMPORT_C TInt RemoveActionL(const TDesC& aRuleID);
        
        /**
        * Gets Action contents from a rule.
        *
        * @param TDesC& aRuleID rule ID
        * @param TPtrC& aAction action which can be block,confirm, polite-block or allow
        * @return TInt KErrNotFound if rule or action doesnt found.
        */
        IMPORT_C TInt GetActionL(const TDesC& aRuleID, TDes& aAction);
        
    private:
    
        /**
        * Gets the SubHandling node pointer in a rule
        *
        * @param TDesC& aRuleID rule ID.
        * @param TBool aCreate if ETrue, creates SubHandling node if not exist
        * @return CXdmDocumentNode* pointer to SubHandling node or NULL
        */
        CXdmDocumentNode* GetSubHandlingNodeL(const TDesC& aRuleID,  
                                                        TBool aCreate);
                                                        
        /**
        * Finds whether the provided action is valid or not
        *
        * @param TDesC& aAction, as a discriptor
        * @return ETrue of action is valid
        */
        TBool IsValidAction(const TDesC& aAction);

                                                        
    private: // data
        
        // Pointer to CPresenceXDM - not owned
        CPresenceXDM*           iPresXDM;      // not owned


    };
    
#endif //__PRESENCEACTIONXDM_H__