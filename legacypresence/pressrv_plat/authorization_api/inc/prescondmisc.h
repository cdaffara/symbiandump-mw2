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
* Description:    RLS and Presence XDM, This class represents those parts of 
*  presence identity which are not present in any other class.
*
*/



#ifndef __PRESCONDMISC_H__
#define __PRESCONDMISC_H__

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
*  TPresCondMisc is used to manipulate the those Presence rule's conditions
*  which are not present in any other class. This class supports following
*  conditions: external-list, anonymous-request and other-identity.
*  
*  @lib rlspresxdm.lib
*  @since S60 3.2
*/
class TPresCondMisc
    {
    public:
    
        /**
        * C++ Constructor, After contructing Init must be called before using
        * this object.
        */                                        
        IMPORT_C TPresCondMisc();
        
        /**
        * Initializes the object with the given pointer to CPresenceXDM.
        * This function must be called before using other functions.
        *
        * @param CPresenceXDM* pointer to created CPresenceXDM
        */
        IMPORT_C void Init(CPresenceXDM* const aPresXDM);
        
        /**
        * Adds condition's other-identity element to a rule. This functions doesnt give
        * error if other-identity element already exists.
        *
        * @param TDesC& aRuleId rule ID  
        * @return TInt KErrNone, KErrNotFound if rule doesnt found
        */
        IMPORT_C TInt AddOtherIdentityL(const TDesC& aRuleId);
        
        /**
        * Finds if other-identity element exist in a rule
        *
        * @param TDesC& aRuleId rule ID
        * @return TBool ETrue if other-identity element found
        */
        IMPORT_C TBool IsOtherIdentityExistL(const TDesC& aRuleId);
        
        /**
        * Removes other-identity element from a rule
        *
        * @param TDesC& aRuleId rule ID 
        * @return KErrNotFound if rule or other-identity element not found.
        */
        IMPORT_C TInt RemoveOtherIdentityL(const TDesC& aRuleId);
        
        /**
        * Adds condition's anonymous-request element to a rule. This functions doesnt give
        * error if anonymous-request element already exists.
        *
        * @param TDesC& aRuleId rule ID  
        * @return TInt KErrNone, KErrNotFound if rule doesnt found
        */
        IMPORT_C TInt AddAnonymousRequestL(const TDesC& aRuleId);
        
        /**
        * Finds if anonymous-request element exist in a rule
        *
        * @param TDesC& aRuleId rule ID
        * @return TBool ETrue if anonymous-request element found
        */
        IMPORT_C TBool IsAnonymousRequestExistL(const TDesC& aRuleId);
        
        /**
        * Removes anonymous-request element from a rule
        *
        * @param TDesC& aRuleId rule ID 
        * @return KErrNotFound if rule or anonymous-request element not found.
        */
        IMPORT_C TInt RemoveAnonymousRequestL(const TDesC& aRuleId);
        
        /**
        * Adds a condition's external-list with 'anc' attribute containing a Uri
        *
        * @param TDesC& aRuleId rule ID
        * @param aUri an external list to be added
        * @return TInt KErrNone, KErrNotFound if rule doesnt found or
        *   KErrAlreadyExists in case already exists
        */
        IMPORT_C TInt AddExternListL(const TDesC& aRuleId, const TDesC& aUri);
        
        /**
        * Finds if an external list is exist in a rule
        *
        * @param TDesC& aRuleId rule ID
        * @param aUri an external list to be find
        * @return TBool ETrue if given external list is exist in the rule
        */
        IMPORT_C TBool IsExternListExistL(const TDesC& aRuleId, const TDesC& aUri);
                                                        
        /**
        * Gets condition's external-lists from a rule
        *
        * @param TDesC& aRuleId rule ID 
        * @param aUris contains returned array of external Uris
        */
        IMPORT_C void GetExternalListsL(const TDesC& aRuleId, 
                                                    CDesCArray& aUris);
                                                    
        /**
        * Delete specific external list if found.
        *
        * @param TDesC& aRuleId rule ID 
        * @param aUri an external list to be removed
        * @return KErrNotFound if rule or external list not found. 
        */
        IMPORT_C TInt DeleteExternalListL(const TDesC& aRuleId, 
                                                            const TDesC& aUri);
                                                            
        /**
        * Deletes specific external lists if they found.
        *
        * @param TDesC& aRuleId rule ID 
        * @param aUris contains array of external lists to be deleted.
        */
        IMPORT_C void DeleteExternalListsL(const TDesC& aRuleId,
                                                const MDesCArray& aUris);
                                                    
        /**
        * Deletes all external lists from a rule if found
        *
        * @param TDesC& aRuleId rule ID
        */
        IMPORT_C void DeleteAllExternalListsL(const TDesC& aRuleId);

    private: // data
        
        // Pointer to CPresenceXDM - not owned
        CPresenceXDM*           iPresXDM;
        
    };
    
   
#endif //__PRESCONDMISC_H__
