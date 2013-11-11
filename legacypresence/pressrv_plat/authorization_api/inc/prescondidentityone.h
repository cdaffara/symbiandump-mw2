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
* Description:    RLS and Presence XDM, This class represents 'one' identity part
*                 of a Presence Condition in a rule
*
*/



#ifndef __PRESCONDIDENTITYONE_H__
#define __PRESCONDIDENTITYONE_H__

// Includes
#include <e32std.h>
#include <e32base.h>
#include <badesca.h>
#include <rlspresxdmconsts.h>

// Forward declarations
class CPresenceXDM;
class CXdmDocumentNode;

// Class declaration
/** 
* This class should be used in conjunction with an already created CPresenceXDM.
*  The CPresenceXDM deals with the Presence rules as a whole and has communication
*  with the server.
*  TPresCondIdentityOne is used to manipulate the 'one' identity part of a Condition
*  in a rule. Remember to call Init function after constructing this object.
*  
*  @lib rlspresxdm.lib
*  @since S60 3.2
*/
class TPresCondIdentityOne
    {
    public:
    
        /**
        * C++ Constructor, After contructing Init must be called before using
        * this object.
        */                                        
        IMPORT_C TPresCondIdentityOne();
        
        /**
        * Initializes the object with the given pointer to CPresenceXDM.
        * This function must be called before using other functions.
        *
        * @param CPresenceXDM* pointer to created CPresenceXDM
        */
        IMPORT_C void Init(CPresenceXDM* const aPresXDM);
        
                                                
        /**
        * Adds condition identities to a rule. This functions doesnt give
        * error if any or all ids already exists, nor it overwrites them.
        *
        * @param TDesC& aRuleID rule ID  
        * @param aIds It contains array of ids.
        * @return TInt KErrNone, KErrNotFound if rule doesnt found
        */
        IMPORT_C TInt AddIdentitiesL(const TDesC& aRuleID,
                                                const MDesCArray& aIds);

        /**
        * Gets condition identities from a rule
        *
        * @param TDesC& aRuleID rule ID 
        * @param aIds contains returned array of ids.
        */
        IMPORT_C void GetIdentitiesL(const TDesC& aRuleID, 
                                                    CDesCArray& aIds);
                                                    
        /**
        * Deletes specific condition identities if they found.
        *
        * @param TDesC& aRuleID rule ID 
        * @param aIds contains array of ids to be deleted.
        */
        IMPORT_C void DeleteIdentitiesL(const TDesC& aRuleID,
                                                const MDesCArray& aIds);
                                                    
        /**
        * Deletes all conditions identities from a rule if found
        *
        * @param TDesC& aRuleID rule ID
        */
        IMPORT_C void DeleteAllIdentitiesL(const TDesC& aRuleID);

        /**
        * Adds a condition identity to a rule
        *
        * @param TDesC& aRuleID rule ID
        * @param aId an identity to be added
        * @return TInt KErrNone, KErrNotFound if rule doesnt found or
        *   KErrAlreadyExists in case already exists
        */
        IMPORT_C TInt AddIdentityL(const TDesC& aRuleID, 
                                                        const TDesC& aId);
                                                        
        /**
        * Deletes an identity from a condition
        *
        * @param TDesC& aRuleID rule ID 
        * @param TDesC& aId an identity to be removed
        * @return KErrNotFound if not found.
        */
        IMPORT_C TInt DeleteIdentityL(const TDesC& aRuleID, 
                                                        const TDesC& aId);
                                                        
        /**
        * Finds if a identity exist in a rule
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aId to be searched
        * @return TBool ETrue if entry found
        */
        IMPORT_C TBool IsIdentityExistL(const TDesC& aRuleID, 
                                                    const TDesC& aId);
                                                
    private:
    
        /**
        * Finds whether the given id exists in the node array
        *
        * @param RPointerArray<CXdmDocumentNode>& nodes array of nodes
        * @param TDesC& aId to find
        * @return ETrue of exists
        */
        TBool IsIdExistInNodeArray(const RPointerArray<CXdmDocumentNode>& nodes, 
                                                    const TDesC& aId);

    private: // data
        
        // Pointer to CPresenceXDM - not owned
        CPresenceXDM*           iPresXDM;      // not owned
        
    };
    
   
#endif //__PRESCONDIDENTITYONE_H__