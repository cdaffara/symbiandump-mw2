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
* Description:    RLS and Presence XDM, This class represents Presence Transform
*
*/




#ifndef __PRESENCETRANSFORMXDM_H__
#define __PRESENCETRANSFORMXDM_H__

// Includes
#include <e32std.h>
#include <e32base.h>
#include <rlspresxdmconsts.h> 

// Forward declarations
class CXdmDocumentNode;
class CPresenceXDM;

// Class declaration
/** 
* This class should be used in conjunction with an already created CPresenceXDM.
*  The CPresenceXDM deals with the Presence rules as a whole and has communication
*  with the server.
*  TPresenceTransformXDM is used to manipulate the Transformation part of a rule. 
*  Remember to call Init function after constructing this object.
*
*  Here the Presence transforms are classified into three categories according to
*  their xml data types. These three categories are: complex transforms types, 
*  boolean transform types and other transform types.
*  Refer to RLSPresXDMConsts.h to find transforms and their categories.
*  Constants from rlspresxdmconsts.h may be used for transform names.
*  
*  @lib rlspresxdm.lib
*  @since S60 3.2
*/
class TPresenceTransformXDM
    {
    public:
    
        /**
        * C++ Constructor, After contructing Init must be called before using
        * this object.
        */                                        
        IMPORT_C TPresenceTransformXDM();
        
        /**
        * Initializes the object with the given pointer to CPresenceXDM.
        * This function must be called before using other functions.
        *
        * @param CPresenceXDM* pointer to created CPresenceXDM
        */
        IMPORT_C void Init(CPresenceXDM* const aPresXDM);
        
        /**
        * Adds transform's complex child element to a rule
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aComplexChild, The childtype of transform. Currently there are 
        *  following complex transforms possible: provide-persons, provide-devices,
        *  provide-services. Use the constants from rlspresxdmconsts.h
        * @param RArray<TTransDataCompo>& aTypeAndValues Type and values for complex child.
        *  The all-devices, all-persons and all-services can not be used in this 
        *  function, use AddOrRepProvideAllTransComplex for those. Remember that this
        *  function will delete above provide-all nodes if they found in this child.
        *  Constants from rlspresxdmconsts.h may be used for transform names.
        * @return TInt KErrNone, KErrNotFound if rule doesnt found,
        *   KErrArgument if invalid child element or its arguments
        */
        IMPORT_C TInt AddTransComplexL(const TDesC& aRuleID, 
                               const TDesC& aComplexChild, 
                               const RArray<TTransDataCompo>& aTypeAndValues);
                               
        /**
        * Adds or replace transform's complex child provide-all element to a rule
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aComplexChild, The childtype of transform. Currently there are 
        *  following complex transforms possible: provide-persons, provide-devices,
        *  provide-services. Additionally this function can also be used for 
        *  provide-all-attributes. 
        *  Depending on the complex child this function  will 
        *  replace the existing complex child with all-devices, all-persons or 
        *  all-services. Constants from rlspresxdmconsts.h may be used for 
        *  transform names.
        * @return TInt KErrNone, KErrNotFound if rule doesnt found,
        *   KErrArgument if invalid child element
        */
        IMPORT_C TInt AddOrRepProvideAllTransComplexL(const TDesC& aRuleID, 
                                                const TDesC& aComplexChild);

        /**
        * Gets transform's complex child element from a rule
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aComplexChild, The childtype of transform. Currently there are 
        *  following complex transforms possible: provide-persons, provide-devices, 
        *  provide-services. Use the constants from rlspresxdmconsts.h
        * @param RArray<TTransDataCompo>& aTypeAndValues Type and values for complex child.
        *  Constants from rlspresxdmconsts.h may be used for transform names.
        * @return TInt KErrNone, KErrNotFound if rule or transform not found, 
        *   KErrArgument if invalid child element.
        */
        IMPORT_C TInt GetTransComplexL(const TDesC& aRuleID, 
                               const TDesC& aComplexChild, 
                               RArray<TTransDataCompo>& aTypeAndValues);
                               
        /**
        * Adds or replace transform's boolean child element to a rule
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aBoolChild, The childtype of transform. Refer to rlspresxdmconsts.h 
        *  for a list of supported boolean transforms. Constants from rlspresxdmconsts.h
        *  may be used for transform names.
        * @param TBool aValue value to add or replace.
        * @return TInt KErrNone, KErrNotFound if rule not found,
        *   KErrArgument if invalid child element
        */
        IMPORT_C TInt AddOrRepTransKnownBoolL(const TDesC& aRuleID, 
                                const TDesC& aBoolChild, TBool aValue);
                                                   
        /**
        * Gets transform's boolean child element from a rule
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aBoolChild, The childtype of transform. Refer to rlspresxdmconsts.h 
        *  for a list of supported boolean transforms. Constants from rlspresxdmconsts.h
        *  may be used for transform names.
        * @param TBool aValue returned value.
        * @return TInt KErrNone, KErrNotFound if rule or child doesnt found,
        *   KErrArgument if invalid child element
        */
        IMPORT_C TInt GetTransKnownBoolL(const TDesC& aRuleID, 
                                    const TDesC& aBoolChild, TBool& aValue);
                                                                   
        /**
        * Adds or replace transform's unknown boolean element to a rule
        *
        * @param TDesC& aRuleID rule ID
        * @param const TDesC& aName, Unknown boolean permission name
        * @param TBool aValue value to add or replace.
        * @return TInt KErrNone, KErrNotFound if rule doesnt found
        */
        IMPORT_C TInt AddOrRepTransUnknownBoolL(const TDesC& aRuleID, 
                                        const TDesC& aName, TBool aValue);
                                        
        /**
        * Gets transform's unknown boolean child element from a rule
        *
        * @param TDesC& aRuleID rule ID
        * @param const TDesC& aName, Unknown boolean permission name
        * @param TBool aValue returned value.
        * @return TInt KErrNone, KErrNotFound if rule or child element 
        *   doesnt found
        */
        IMPORT_C TInt GetTransUnknownBoolL(const TDesC& aRuleID, 
                                            const TDesC& aName, TBool& aValue);
                                            
        /**
        * Removes transform's child element from a given rule. If Child Element
        * is NullDesC, whole transform with all childrens is removed from rule.
        * This method is applicable to all transform types.
        *
        * @param TDesC& aRuleID rule ID 
        * @param TDesC& aChildElement, The childtype of transform. Refer to rlspresxdmconsts.h 
        *  for a list of supported transforms. Constants from rlspresxdmconsts.h
        *  may be used for transform names.
        *
        * @return TInt KErrNone, KErrNotFound if rule or child doesnt found,
        *   KErrArgument if invalid child element.
        */
        IMPORT_C TInt RemoveTransformL(const TDesC& aRuleID, 
                                                const TDesC& aChildElement);
        
        /**
        * Finds whether given transform with childelement exists.
        * This method is applicable for all transform types.
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aChildElement, The childtype of transform. Refer to rlspresxdmconsts.h 
        *  for a list of supported transforms. Constants from rlspresxdmconsts.h
        *  may be used for transform names.
        * @return TBool ETrue if transform exist
        */
        IMPORT_C TBool IsTransformExistL(const TDesC& aRuleID, 
                                                const TDesC& aChildElement);
        
    private:

        /**
        * Gets the transform's child node
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aTransformChild The childtype of transform.
        * @param TBool aCreate, if ETrue creates the child in case it doesnt exists
        * @return CXdmDocumentNode* transfrom's child node or NULL if not found
        */
        CXdmDocumentNode* GetTransformChildNodeL(const TDesC& aRuleID,
                            const TDesC& aTransformChild, TBool aCreate);
        
        /**
        * Find whether the given transform child and its childrens are valid
        *
        * @param TDesC& aComplexChild, complex child of the transform
        * @param RArray<TTransDataCompo>& aTypeAndValues, children types and their
        *  contents
        * @return ETrue if valid
        */
        TBool AreNodeNamesValid(const TDesC& aComplexChild, 
                            const RArray<TTransDataCompo>& aTypeAndValues);

        /**
        * Find whether the provide-all node exists in given complex child.
        * the practicality of this is that if provide-all exists then it
        * should be the only child of given complex child of transform.
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aComplexChild, complex child of the transform
        * @return ETrue if provide-all node found
        */
        TBool IsProvideAllExistL(const TDesC& aRuleID,const TDesC& aComplexChild);
        
        /**
        * Find whether the given boolean child is supported or not.
        *
        * @param TDesC& aBoolChild
        * @return ETrue if supported 
        */
        TBool IsBoolChildSupported(const TDesC& aBoolChild);
        
        /**
        * Add or replace the transform's boolean child
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aBoolChild, boolean child of the transform, either known or unknown
        * @param TBool aValue, value of the boolean child
        * @return KErrNotFound if rule not found.
        */
        TInt AddOrRepTransBoolL(const TDesC& aRuleID, const TDesC& aBoolChild, 
                                                            TBool aValue);

        /**
        * Gets the transform's boolean child
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aBoolChild, boolean child of the transform, either known or unknown
        * @param TBool aValue, returned value of the boolean child
        * @return KErrNotFound if rule or booleanchild not found.
        */
        TInt GetTransBoolL(const TDesC& aRuleID, const TDesC& aBoolChild,
                                                                   TBool& aValue);
                            
    private: // data
        
        // Pointer to CPresenceXDM - not owned
        CPresenceXDM*           iPresXDM;

    };
    
 #endif //__PRESENCETRANSFORMXDM_H__
