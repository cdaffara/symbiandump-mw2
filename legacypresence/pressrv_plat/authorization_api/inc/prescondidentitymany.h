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
* Description:    RLS and Presence XDM, This class represents the 'many' element
*                 part of the Presence Condition
*
*/



#ifndef __PRESCONDIDENTITYMANY_H__
#define __PRESCONDIDENTITYMANY_H__

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
*  TPresCondIdentityMany is used to manipulate the 'many' element part of a 
*  Condition in a rule. Remember to call Init function after constructing this object.
*  
*  @lib rlspresxdm.lib
*  @since S60 3.2
*/
class TPresCondIdentityMany
    {
    public:
    
        /**
        * C++ Constructor, After contructing Init must be called before using
        * this object.
        */                                        
        IMPORT_C TPresCondIdentityMany();
        
        /**
        * Initializes the object with the given pointer to CPresenceXDM.
        * This function must be called before using other functions.
        *
        * @param CPresenceXDM* pointer to created CPresenceXDM
        */
        IMPORT_C void Init(CPresenceXDM* const aPresXDM);
        
                                                
        /**
        * Puts the 'many' element in identity, with domain name
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aDomain domain name, provide KNullDesC if not needed
        * @return KErrNotFound if rule not found, KErrAlreadyExists
        */
        IMPORT_C TInt AddManyIdentityL(const TDesC& aRuleID, 
                                                const TDesC& aDomain);
        
        /**
        * Finds whether 'many' element exists in identities or not, with 
        * optional domain name
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aDomain domain name, provide KNullDesC if not needed
        * @return ETrue if exists
        */
        IMPORT_C TBool IsManyExistsL(const TDesC& aRuleID, 
                                            const TDesC& aDomain);
        
        /**
        * Removes the 'many' element from identity, optional domain name
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aDomain domain name, provide KNullDesC if not needed
        * @return KErrNotFound if rule or many element not found
        */
        IMPORT_C TInt RemoveManyIdentityL(const TDesC& aRuleID, 
                                            const TDesC& aDomain);
                                            
        /**
        * Get the number of 'many' elements in identity node, those elements
        * which has domain name are returned in aDomains array
        *
        * @param TDesC& aRuleID rule ID
        * @param CDesCArray& aDomains domain names if exists
        * @return TInt number of total many elements.
        */
        IMPORT_C TInt GetManyIdentityElementsL(const TDesC& aRuleID, 
                                                    CDesCArray& aDomains);
        
        /**
        * Add identity to 'except' child of many element
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aEntity to be added
        * @param TDesC& aDomain domain name, provide KNullDesC if not needed
        * @return KErrNotFound if rule or many element not found
        */
        IMPORT_C TInt AddIdentityInManyExceptL(const TDesC& aRuleID,
                    const TDesC& aEntity, const TDesC& aDomain);
                    
        /**
        * Add identities to 'except' child of many element
        *
        * @param TDesC& aRuleID rule ID
        * @param CDesCArray& aEntities to be added
        * @param TDesC& aDomain domain name, provide KNullDesC if not needed
        * @return KErrNotFound if rule or many element not found
        */
        IMPORT_C TInt AddIdentitiesInManyExceptL(const TDesC& aRuleID,
                const CDesCArray& aEntities, const TDesC& aDomain);
                                        
        /**
        * Get identities from 'except' child of many element
        *
        * @param TDesC& aRuleID rule ID
        * @param CDesCArray& aEntities fethed identities
        * @param TDesC& aDomain domain name, provide KNullDesC if not needed
        * @return KErrNotFound if rule or many element not found
        */
        IMPORT_C TInt GetIdentitiesFromManyExceptL(const TDesC& aRuleID,
                    CDesCArray& aEntities, const TDesC& aDomain);
                    
        /**
        * Removes an identity from 'except' child of many element
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aEntity to be deleted
        * @param TDesC& aDomain domain name, provide KNullDesC if not needed
        * @return KErrNotFound if rule or many element or id not found
        */
        IMPORT_C TInt RemoveIdentityFromManyExceptL(const TDesC& aRuleID,
                        const TDesC& aEntity, const TDesC& aDomain);
                                        
        /**
        * Deletes all identities in the 'except' child of 'many'
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aDomain domain name, provide KNullDesC if not needed
        * @return KErrNotFound if rule or many element not found
        */
        IMPORT_C TInt DeleteAllIdentitiesFromManyExceptL(const TDesC& aRuleID,
                                               const TDesC& aDomain);
                                               
        /**
        * Is identity exists in the 'except' child of many element
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aEntity to be searched
        * @param TDesC& aDomain domain name, provide KNullDesC if not needed
        * @return ETrue if exists
        */
        IMPORT_C TBool IsIdentityExistsInManyExceptL(const TDesC& aRuleID, 
                                        const TDesC& aEntity, 
                                        const TDesC& aDomain);

    private:
    
        /**
        * Gets the 'many' node pointer in a rule
        *
        * @param TDesC& aRuleID rule ID.
        * @param TBool aCreate if ETrue, creates many node if not exist
        * @param TDesC& aDomain, domain name, can be KNullDesC
        * @return CXdmDocumentNode* pointer to 'many' node or NULL
        */
        CXdmDocumentNode* GetManyNodeL(const TDesC& aRuleID,  
                                    TBool aCreate, const TDesC& aDomain);
                                                            
        /**
        * Finds whether the given entity exists in the node array
        *
        * @param RPointerArray<CXdmDocumentNode>& nodes array of nodes
        * @param TDesC& aEntity to find
        * @return ETrue of exists
        */
        TBool IsEntityExistInNodeArray(const RPointerArray<CXdmDocumentNode>& nodes, 
                                                    const TDesC& aEntity);

    private: // data
        
        // Pointer to CPresenceXDM - not owned
        CPresenceXDM*           iPresXDM;      // not owned
        
    };
    
   
#endif //__PRESCONDIDENTITYMANY_H__
        
