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
* Description:    RLS and Presence XDM, This is class for RLS XDM
*
*/



#ifndef __RLSXDM_H__
#define __RLSXDM_H__

// INCLUDE FILES
#include <e32base.h>
#include <badesca.h>
#include <rlspresxdmconsts.h>

// Forward declarations
class CXdmProtocolInfo;
class CXdmEngine;
class CXdmDocument;
class MRLSPresXDMAsyncHandler;
class CXdmDocumentNode;


// Class declaration
/** 
* This class is used for RLS XDM. Refer to OMA 'Resource List Server (RLS) XDM
*  Specification'. The class maintains a so called current path information, 
*  which can be manipulated using specific functions. The elements adding, 
*  removing, getting etc are performed on current path. However, 'service' 
*  uri nodes related functions are always treated on document root. The class
*  also applies restrictions on adding elements on certain paths, to comply with
*  OMA specifications. Some servers does not allow empty 'list' or 'service' uri
*  nodes, the class provides functions to delete those from the whole document.
*  
*  @lib rlspresxdm.lib
*  @since S60 3.2
*/
class CRLSXDM : public CActive
    {
    public:
        
        /**
        * Creates CRLSXDM
        *
        * @param TInt XDMSettingsList ID
        * @return Pointer to created CRLSXDM
        */
        IMPORT_C static CRLSXDM* NewL( const TInt aSettingId );
        
        /**
        * Create CRLSXDM, leaves pointer to stack
        *
        * @param TInt XDMSettingsList ID
        * @return Pointer to created CRLSXDM
        */
        IMPORT_C static CRLSXDM* NewLC( const TInt aSettingId );
        
        
        /**
        * Updates information (document) to server. This results in
        * HandleRLSUpdateToServerL on completion.
        *
        * @param aHandler, pointer to MRLSPresXDMAsyncHandler callback handler
        * @return Error code, KErrAlreadyExist if some async already going on
        */
        IMPORT_C TInt UpdateToServerL(MRLSPresXDMAsyncHandler* const aHandler);
        
        /**
        * Updates all information (document) from server. This results in
        * HandleRLSUpdateFromServerL on completion.
        *
        * @param aHandler, pointer to MRLSPresXDMAsyncHandler callback handler
        * @return Error code, KErrAlreadyExist if some async already going on
        */
        IMPORT_C TInt UpdateAllFromServerL(MRLSPresXDMAsyncHandler* const aHandler);


        /**
        * Cancels document update to/from server if going on. This results in
        * HandleRLSUpdateCancelL on completion.
        */
        IMPORT_C void CancelUpdateL();
     
        // Service URIs, will always be treated at the root level
        
        /**
        * Delete all data by deleting all service uris. It also resets the path.
        */
        IMPORT_C void DeleteAll();
        
        /**
        * Add a new service URI
        *
        * @param TDesC& aServiceURI
        * @return Error code, KErrNone, KErrAlreadyExist
        */
        IMPORT_C TInt AddServiceURIL(const TDesC& aServiceURI);

        /**
        * Removes a service URI (and all lists/Entries it holds). Current path
        * will be reset if this service exists in current path
        *
        * @param TDesC& aServiceURI to be removed
        * @return Error code, KErrNone or KErrNotFound
        */
        IMPORT_C TInt RemoveServiceURIL(const TDesC& aServiceURI);
        
        /**
        * Return ETrue if service URI is exist with given name
        *
        * @param TDesC& aServiceURI to be searched
        * @return ETrue if found
        */
        IMPORT_C TBool IsExistServiceURI(const TDesC& aServiceURI);
        
        /**
        * Get all Service Uri(s) in current document
        *
        * @param CDesCArray& array of Service Uri in current document
        */
        IMPORT_C void GetServiceUrisL(CDesCArray& aServiceUris);
        
        /**
        * Deletes all service-uris in the document which doesnt have any lists. 
        * This function is very useful with the servers which doesnt accept 
        * empty service uris. Note that as a side effect of this call, the path
        * information is deleted. If emtpy lists also needed to be deleted,
        * DeleteAllEmptyListsL must be called before calling this function.
        */
        IMPORT_C void DeleteAllEmptyServiceUrisL();


        // Methods that apply to current path
        
        /**
        * Renames a 'list' element
        *
        * @param TDesC& aListName list's current name
        * @param TDesC& aListNewName list's new name
        * @return Error code, KErrNone or KErrNotFound
        */
        IMPORT_C TInt RenameListL(const TDesC& aListName, const TDesC& aListNewName);

        /**
        * Async. function. Deletes all lists in the document which doesnt have 
        * any lists or entries. 
        * This function is very useful with the servers which doesnt accept 
        * empty lists. Note that as a side effect of this call, the path
        * information is deleted. Results in HandleRLSDeleteAllEmptyListsL upon
        * completion. If empty service-uris also needed to be deleted, 
        * DeleteAllEmptyServiceUrisL must be called after calling this function.
        *
        * @param TInt KErrAlreadyExist if some async call already exist
        */
        IMPORT_C TInt DeleteAllEmptyLists(MRLSPresXDMAsyncHandler* const aHandler);

        /**
        * Adds an element to current path, only 'list' and 'resource-list' 
        * elements can be added to the root of ServiceURI, to add any of other 
        * elements current path should be inside an existing list. aDisplayName
        * is optional and KNullDesC can be provided if display name is not desired.
        * Note that leaf element type can not have display-name so in that case 
        * display-name is always discarded.
        *
        * @param aElementType, an element type to be added, this can be list,
        *  resource-list, entry, external, entry ref. Use constants from 
        *  RLSPresXDMConsts.h.
        * @param aData name in case of list, value of element's attribute for 
        *  other elements. In case of resource-list this is the link.
        * @param aDisplayName display name of the element, use KNullDesC if
        *  display-name is not required.
        * @return Error code, KErrNone, KErrAlreadyExist or KErrPermissionDenied
        */
        IMPORT_C TInt AddElementL(const TDesC& aElementType,
                                const TDesC& aData, const TDesC& aDisplayName);
   
        /**
        * Gets display name of an element. Only applicable to those elements
        * which can have a display name.
        *
        * @param aElementType, this can be list, entry, external and entry ref. 
        *  Use constants from RLSPresXDMConsts.h.
        * @param aData name in case of list, value of element's attribute for 
        *  other elements.
        * @param aDisplayName display name of the element
        * @return Error code. KErrArgument, KErrNone, KErrNotFound
        *  or KErrPermissionDenied
        */
        IMPORT_C TInt GetDisplayName(const TDesC& aElementType,
                                const TDesC& aData, TPtrC8& aDisplayName);

        /**
        * Updates display name of an element.
        *
        * @param aElementType, an element type, this can be list,
        *  entry, external, entry ref. Use constants from RLSPresXDMConsts.h.
        * @param aData name in case of list, value of element's attribute for 
        *  other elements.
        * @param aDisplayName display name of the element
        * @return Error code. KErrArgument, KErrNone, KErrNotFound 
        *  or KErrPermissionDenied
        */
        IMPORT_C TInt UpdateDisplayNameL(const TDesC& aElementType,  
                                const TDesC& aData, const TDesC& aDisplayName);
        
        /**
        * Get all elements of same type at given node path
        *
        * @param aElementType, an element type, this can be list, resource-list
        *  entry, external, or entry ref. Use constants from RLSPresXDMConsts.h.
        * @param aValues array of data values. 'name' in case of list or value of 
        *  element's attribute for other element types.
        */
        IMPORT_C void GetElementsL(const TDesC& aElementType, CDesCArray& aValues);
                    
        /**
        * Removes the given element from the current path
        *
        * @param aElementType, an element type, this can be list, resource-list
        *  entry, external, or entry ref. Use constants from RLSPresXDMConsts.h.
        * @param aData name in case of list, value of element's attribute for 
        *  other elements.
        * @return Error code. KErrArgument, KErrNone, KErrNotFound 
        *  or KErrPermissionDenied
        */
        IMPORT_C TInt RemoveElementL(const TDesC& aElementType, const TDesC& aData);
        
        /**
        * Is element exist on the current path
        *
        * @param aElementType, an element type, this can be list, resource-list
        *  entry, external, or entry ref. Use constants from RLSPresXDMConsts.h.
        * @param aData name in case of list, value of element's attribute for 
        *  other elements.
        * @return ETrue if element exist.
        */
        IMPORT_C TBool IsElementExist(const TDesC& aElementType, const TDesC& aData);
        

        // Methods related to changing the current path

        /**
        * Switch into given service URI. After this the given ServiceURI 
        * will become currently pointed URI, and all operations will be 
        * performed on this URI. This corresponds to entering into a disk 
        * in DOS
        *
        * @param TDesC& name of the ServiceURI
        * @return Error code, KErrNone or KErrNotFound
        */
        IMPORT_C TInt SwitchToServiceURIL(const TDesC& aServiceURI);

        /**
        * Switch into given list. After this the given list will become
        * currently pointed list (path), and all operations will be performed 
        * in this path. This corresponds to entering into a directory in DOS
        *
        * @param TDesC& name of the list
        * @return Error code, KErrNone or KErrNotFound
        */
        IMPORT_C TInt SwitchToListL(const TDesC& aListName);
        
        /**
        * Switch out from current list, if path is already service-uri level
        * nothing will happed. This corresponds to gettting out from a directory.
        * The Current path changes if successful
        */
        IMPORT_C void SwitchOutFromList();
        
        /**
        * This gives complete hierarchy of the current path. The first element
        * of the array will be ServiceURI, and after that will be child lists, 
        * The current list name is the last element of the array.
        * 
        * @param CDesC16Array& array of service-uri and list names ordered from 
        *   root to the list's own name.
        */
        IMPORT_C void GetCurrentPathL(CDesCArray& aPath);
        
        /**
        * Set the current path according to given hierarchy. The first element
        * of the array must be the ServiceURI, with child lists to follow. 
        * The target list name is the last element of the array.
        * If the list is not found according to the given heirarchy, the current 
        * path will not be changed and KErrNotFound will be returned. An empty
        * array will cause the path to be completely empty.
        * 
        * @param MDesCArray& array of service-uri and list names ordered from 
        *   root to the list's own name.
        * @return Error code, KErrNone or KErrNotFound
        */
        IMPORT_C TInt SetCurrentpathL(const MDesCArray& aListsNames);
        
        /**
        * Accessor for the last negotiated RLS service URI.
        * URI is Zero length if the last HTTP PUT did not 
        * receive HTTP 409 error or if there was no <alt-value> value
        * in the response body.         
        * 
        * @return last negotiated RLS service URI. 
        *  
        */
        IMPORT_C TPtrC NegotiatedServiceUri(); 
                
        /**
        * Destructor
        *
        */
        ~CRLSXDM();
        
    private:
        
        /**
        * Second phase constructor
        *
        * @param TInt XDMSettingsList ID
        * @return none
        */
        void ConstructL(const TInt aSettingId );
        
        /**
        * C++ constructor
        *
        * @param none
        * @return none
        */
        CRLSXDM();
        
        
        /**
        * Return pointer to serviceUri and its attribute value
        *
        * @param CXdmNodeAttribute attribute value of serviceUri
        * @param CXdmDocumentNode pointer to root node       
        * @return CXdmDocumentNode* if found, otherwise NULL
        */
        CXdmDocumentNode* IsServiceUriExist(const TDesC& aUriAttr,
                                            CXdmDocumentNode* aRootNode);
                                            
        /**
        * Get list's non-leaf child. These includes getting of list, entry
        * external and entry ref.
        *
        * @param aChildName, name of non-leaf child
        * @param aAttrValue value of attribute to match with
        * @param aTargetNode pointer to target node
        * @return CXdmDocumentNode* if found, otherwise NULL
        */
        CXdmDocumentNode* GetNonLeafChild(const TDesC& aChildName,
                                        const TDesC& aAttrValue,
                                        CXdmDocumentNode* const aTargetNode);
        
        /**
        * Get list's leaf child. These includes getting of resource-list and
        * packages
        *
        * @param aChildName, name of non-leaf child 
        * @param aValue value to match with
        * @param aTargetNode pointer to target node
        * @return CXdmDocumentNode* if found, otherwise NULL
        */
        CXdmDocumentNode* GetLeafChild(const TDesC& aChildName,
            const TDesC& aValue, CXdmDocumentNode* const aTargetNode);
            
        /**
        * Return the attribute name used for the element.
        * Only valid for non-leaf elements.
        *
        * @param aElementName, name of non-leaf element
        * @param aAttrName name of the attribute returns here
        */
        void GetNonLeafElementAttrName(const TDesC& aElementName,
                                                           TDes& aAttrName);
            
        /**
        * Validates the given non-leaf element.
        *
        * @param aElementName, name of non-leaf element
        * @return ETrue if valid non-leaf element
        */
        TBool IsNonLeafElementValid(const TDesC& aElementName);
        
        
        /**
        * Adds a non leaf element to current path, only 'list' element can be 
        * added to the root of ServiceURI, to add any of other possible elements 
        * current path should be inside an existing list. This functions doesnt
        * care whether element already exist or not.
        *
        * @param aElementType, an element type to be added, this can be list,
        *  entry, external, entry ref. Use constants from RLSPresXDMConsts.h.
        * @param aData name in case of list, value of element's attribute for 
        *  other elements.
        * @param aDisplayName display name of the element
        * @param aTargetNode pointer to target node
        */
        void AddNonLeafElementL(const TDesC& aElementType, 
                                const TDesC& aData, const TDesC& aDisplayName,
                                        CXdmDocumentNode* const aTargetNode);
   
        /**
        * Adds a leaf element to current path, only 'resource-list' element can be 
        * added to the root of ServiceURI. This functions doesnt care whether 
        * element already exist or not.
        *
        * @param aElementType, an element type to be added, currently this can be
        *  only resource-list. Use constants from RLSPresXDMConsts.h.
        * @param aData value of the element
        * @param aTargetNode pointer to target node
        */
        void AddLeafElementL(const TDesC& aElementType, const TDesC& aData,
                                            CXdmDocumentNode* const aTargetNode);
        
        /**
        * Gets display name of a non leaf element.
        *
        * @param aElementType, an element type, this can be list,
        *  entry, external, entry ref. Use constants from RLSPresXDMConsts.h.
        * @param aData name in case of list, value of element's attribute for 
        *  other elements.
        * @param aDisplayName display name of the element
        * @param aTargetNode pointer to target node
        * @return Error code. KErrArgument, KErrNone, KErrAlreadyExist 
        *  or KErrPermissionDenied
        */
        TInt GetDisplayNameInt(const TDesC& aElementType,
                                const TDesC& aData, TPtrC8& aDisplayName,
                                        CXdmDocumentNode* const aTargetNode);

        /**
        * Updates display name of an element.
        *
        * @param aElementType, an element type, this can be list,
        *  entry, external, entry ref. Use constants from RLSPresXDMConsts.h.
        * @param aData name in case of list, value of element's attribute for 
        *  other elements.
        * @param aDisplayName display name of the element
        * @param aTargetNode pointer to target node
        * @return Error code. KErrArgument, KErrNone, KErrAlreadyExist 
        *  or KErrPermissionDenied
        */
        TInt UpdateDisplayNameIntL(const TDesC& aElementType, 
                                const TDesC& aData, const TDesC& aDisplayName,
                                        CXdmDocumentNode* const aTargetNode);
        
        /**
        * Get all elements of same type at given node path
        *
        * @param aElementType, an element type, this can be list,
        *  entry, external, or entry ref. Use constants from 
        *  RLSPresXDMConsts.h.
        * @param aValues array of data values. 'name' in case of list or value of 
        *  element's attribute for other element types.
        * @param aTargetNode pointer to target node
        */
        void GetNonLeafElementsL(const TDesC& aElementType,
                    CDesCArray& aValues, CXdmDocumentNode* const aTargetNode);
                    
         /**
        * Get all elements of same type at given node path
        *
        * @param aElementType, this can be resource-list
        * @param aValues array of data values. 'name' in case of list or value of 
        *  element's attribute for other element types.
        * @param aTargetNode pointer to target node
        * @return none
        */
        void GetLeafElementsL(const TDesC& aElementType, CDesCArray& aValues, 
                                        CXdmDocumentNode* const aTargetNode);
                    
        /**
        * Internal function for deleting empty lists, keep called by RunL
        * until all lists are deleted.
        */
        void DeleteEmptyListsL();
                                        
        /**
        * Adds the compulsory package node to current service uri
        * in case no current uri found, does nothing. This function needed
        * because some servers wants package as the last child node of
        * service uri node.
        */
        void AddPackageL();
        
        /**
        * Removes the compulsory package node to current service uri
        * in case no current uri found, does nothing. This function needed
        * because some servers wants package as the last child node of
        * service uri node.
        */
        void RemovePackageL();
                                        
        /**
        * Creates the document root if needed
        */
        void CreateRootIfNeededL();
        
        /**
         * Parse Serice URI negotiation response body and save it into
         * iNegotiatedServiceUri.
         * @param aErrorBody HTTP 409 error body
         */
        void DoParseNegotiatedServiceUriL( CXdmDocumentNode* aErrorBody );

        
        /**
        * From CActive
        *
        */
        void DoCancel();
        
        /**
        * From CActive
        *
        */
        void RunL();
        
        /**
        * From CActive
        *
        */
        TInt RunError(TInt aError);
        
    private: // Data
    
        CXdmProtocolInfo*    iXDMProtocolInfo;
        CXdmEngine*          iXDMEngine;
        CXdmDocument*        iRLSDocument;
        
        TInt   iAsyncReq;
        MRLSPresXDMAsyncHandler* iAsyncHandler;
        
        CDesCArray*                     iListPath;
        RPointerArray<CXdmDocumentNode> iListPointerPath;
        
        /**
         * Negotiated RLS service URI
         * Own 
         */
        HBufC* iNegotiatedServiceUri;
       
       
    };
    
#endif //__RLSXDM_H__
        
        
