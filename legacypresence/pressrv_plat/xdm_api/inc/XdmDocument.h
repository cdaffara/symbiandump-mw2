/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   XDM Document
*
*/




#ifndef __XDMDOCUMENT__
#define __XDMDOCUMENT__

#include <e32cmn.h>
#include <e32base.h>
#include <XdmNamespaceContainer.h>

//The supported document types = application usages
enum TXdmDocType
    {
    EXdmDocGeneral = 0,
    EXdmTestDocType,
    EXdmPoCGroup,
    EXdmPoCUserAccessPolicy,
    EXdmDirectory,
    EXdmCapabilities,
    EXdmSharedXdm,
    EXdmRlsServices,
    EXdmResourceLists,
    EXdmIetfCommonPolicy,
    EXdmOmaCommonPolicy,
    EXdmIetfPresenceRules,
    EXdmOmaPresenceRules
    };

//Helper class for searching documents; 16-bit 
struct SXdmAttribute16
    {
    TPtrC iName;
    TPtrC iValue;
    };

//Helper class for searching documents; 8-bit
struct SXdmAttribute8
    {
    TPtrC8 iName;
    TPtrC8 iValue;
    };

//Constants
const TInt KXdmPathSeparatorChar                       = 47;

//Forward declarations    
class CXdmEngine;
class MXdmOperation;
class CXdmDocumentNode;
class CXdmNodeAttribute;
class CXdmOperationFactory;

// CLASS DECLARATION
class CXdmDocument : public CActive,
                     public MXdmNamespaceContainer
    {
    public:  // Constructors and destructor
    
        /**
        * Returns the name of this document
		* @return TPtrC The name of the document this object models 
        */
        IMPORT_C TPtrC Name() const;
        
        /**
        * Fetch data from the document this object models beginning from the 
        * element the parameter points to. If the parameter is not specified,
        * the operation applies to the whole of the document.
        *
        * @param CXdmDocumentNode* The element (including its descendants) to fetch.
		* @return void
        */
        IMPORT_C void FetchDataL( CXdmDocumentNode* aDocNode = NULL );
        
        /**
        * Replace the document this object models
        * 
        * NOTE: The name of the document MUST remain the same. This is because
        *       the operation first needs to delete the old document and then
        *       store the new one. The risk of creating conflicting document
        *       names is far too great in case this rule is not imposed.
        *
		* @return void 
        */
        IMPORT_C void ReplaceL();

        /**
        * Replace a named element in the document this object models
        *
        * NOTE: Both elements MUST have a common parent element,
        *       which, in effect, must be the element this document
        *       model points to. Also note that the operation will
        *       fail in case the "aOldNode" does not exist in the document.
        *
        * @param CXdmDocumentNode* The element to replace
        * @param CXdmDocumentNode* The replacing element
		* @return void 
        */
        IMPORT_C void ReplaceL( CXdmDocumentNode* aOldNode,
                                CXdmDocumentNode* aNewNode );

        /**
        * Insert the node identified by the parameter. Note that this
        * method cannot be applied to complete documents. Thus the method
        * will fail in case the parameter is NULL. If the client needs
        * to a) add b) replace a document, it needs to do this through
        * the AppendL() or the ReplaceL() method.
        *
        * NOTE: The operation will fail in case the element identified
        *       by the aDocNode parameter already exists in the document.
        *       XDM Engine does not make assumptions in this regard; it
        *       does not, for instance, change some aspects of the 
        *       request in order to turn it into a valid insert operation.
        *       Instead, it will re-read the whole document and retry the
        *       insert operation. If the operation still fails, the engine
        *       reports the failure that occurred while performing the
        *       operation. In the latter case, the client can get a hold
        *       of the error report document, provided that the used
        *       protocol supports the feature and that the server included
        *       the document in the response, by fetching the root node of
        *       the error document through ErrorRoot() method.
        *       
        * @param CXdmDocumentNode* The element (including its descendants) to insert
		* @return void 
        */
        IMPORT_C void InsertL( CXdmDocumentNode* aDocNode );

        /**
        * Append data to the document this object models beginning from the 
        * element the parameter points to. If the parameter is not specified,
        * the operation applies to the whole of the document.
        *
        * NOTE: This is a "blind insert"; no guarantees can be given
        *       as to the success or failure of the operation, since
        *       it includes no checking for existence of the resource
        *       that is to be appended/inserted. It is also worth noticing
        *       that this method may effectively turn into a replace
        *       operation in case the method is invoked on a document that
        *       already exists in the storage endpoint.
        *
        * @param CXdmDocumentNode* The element (including its descendants) to append.
		* @return void
        */
        IMPORT_C void AppendL( CXdmDocumentNode* aDocNode = NULL );

        /**
        * Delete data from the document this object models beginning from the 
        * element the parameter points to. If the parameter is not specified,
        * the operation applies to the whole of the document.
        *
        * @param CXdmDocumentNode* The element (including its descendants) to delete.
		* @return void 
        */
        IMPORT_C void DeleteDataL( CXdmDocumentNode* aDocNode = NULL );
        
        /**
        * Search for nodes which match the specified name & attributes
        *
        * @param CXdmDocumentNode& The node to search for
        * @param RPointerArray& On completion, contains the nodes
        *                       that were found, if any.
		* @return TInt KErrNotFound if no matching nodes are found.
		*              Otherwise indicates the number of nodes found
        */
        IMPORT_C TInt Find( const CXdmDocumentNode& aTargetNode,
                            RPointerArray<CXdmDocumentNode>& aResultArray ) const;
        
        /**
        * Search for nodes which match the specified name & attributes
        *
        * @param TDesC& The name of the node to search for
        * @param RPointerArray& On completion, contains the nodes
        *                       that were found, if any.
		* @return TInt KErrNotFound if no matching nodes are found.
		*              Otherwise indicates the number of nodes found
        */
        IMPORT_C TInt Find( const TDesC& aNodeName,
                            RPointerArray<CXdmDocumentNode>& aResultArray ) const;
        
        /**
        * Search for attributes which match the specified name
        *
        * @param TDesC& The name of the attribute to search for
        * @param RPointerArray& On completion, contains the nodes
        *                       that were found, if any.
		* @return TInt KErrNotFound if no matching nodes are found.
		*              Otherwise indicates the number of nodes found
        */
        IMPORT_C TInt Find( const TDesC& aAttributeName,
                            RPointerArray<CXdmNodeAttribute>& aResultArray ) const;
                          
        /**
        * Search for nodes which match the specified name & attributes
        *
        * @param TDesC& The name of the node to search for
        * @param RPointerArray& On completion, contains the nodes
        *                       that were found, if any.
        * @param RPointerArray& The attribute names & values that a
                                matching node must have.
		* @return TInt KErrNotFound if no matching nodes are found.
		*              Otherwise indicates the number of nodes found
        */
        IMPORT_C TInt Find( const TDesC& aNodeName,
                            RPointerArray<CXdmDocumentNode>& aResultArray,
                            const RPointerArray<SXdmAttribute16>& aAttributeArray ) const;
                            
        /**
        * Comparison operator
        *
        * @param CXdmDocument& The document to compare to
		* @return TBool Are the contents of the two identical
        */
        IMPORT_C TBool operator==( CXdmDocument& aDocument ) const;
        
        /**
        * Remove an element from the document model
        *
        * NOTE: This method cannot be used with document models that
        *       only target a fragment of the document; in practise
        *       this means that if the client has constructed a node
        *       using DocumentSubsetL(), this method leaves with KErrGeneral.
        *       A document subset cannot be treated as an actual document
        *       since only a fraction of the elements and their dependencies
        *       in the document is known. The client must have the whole 
        *       document at its disposal in order to be properly able to
        *       make use of this method.
        *
        * @param CXdmDocumentNode* The node to be removed
		* @return void 
        */
        IMPORT_C void RemoveFromModelL( CXdmDocumentNode* aChileNode );
        
        /**
        * Append an element to the document model
        *
        * NOTE: This method cannot be used with document models that
        *       only target a fragment of the document; in practise
        *       this means that if the client has constructed a node
        *       using DocumentSubsetL(), this method leaves with KErrGeneral.
        *       A document subset cannot be treated as an actual document
        *       since only a fraction of the elements and their dependencies
        *       in the document is known. The client must have the whole 
        *       document at its disposal in order to be properly able to
        *       make use of this method.
        *
        * @param The next node in the list
		* @return void 
        */
        IMPORT_C void AppendToModelL( CXdmDocumentNode* aNewNode,
                                      CXdmDocumentNode* aParentNode );
        
        /**
        * Replace an element in the document model
        *
        * NOTE: This method cannot be used with document models that
        *       only target a fragment of the document; in practise
        *       this means that if the client has constructed a node
        *       using DocumentSubsetL(), this method leaves with KErrGeneral.
        *       A document subset cannot be treated as an actual document
        *       since only a fraction of the elements and their dependencies
        *       in the document is known. The client must have the whole 
        *       document at its disposal in order to be properly able to
        *       make use of this method.
        *
        * @param CXdmDocumentNode The new node
        * @param CXdmDocumentNode The old (target) node
		* @return void 
        */
        IMPORT_C void ReplaceInModelL( CXdmDocumentNode* aNewNode,
                                       CXdmDocumentNode* aTargetNode );
                                         
        /**
        * Return a subset of the document this object models. 
        * For instance, if the whole (physical) document is
        * the following:
        *
        * <root>
        *   <element1>
        *     <child1>
        *       <leaf1>Foo</leaf1>
        *     </child1>
        *   </element1>
        * </root>
        *
        * calling this method with the parameter "root/element1/child1"
        * would make this document model model the subset
        * 
        * <child1>
        *   <leaf1>Foo</leaf1>
        * </child1>
        *
        * It is important to understand that, after calling this method,
        * the document model only models the document starting from the 
        * node the method returns. The client may traverse the document
        * all the way to the root node and back, but the way from the root
        * node to the node this method returns should be regarded merely
        * as a "path" of elements that has no real content.
        *
        * A prime use case for this method is one in which the structure
        * of the document the client application wants to operate on is
        * well-known; a typical example would be an XDM directory document.
        * The schema for this application usage is very simple: the root
        * element is <xcap-directory> that has a collection of <folder>
        * elements as its children. Each <folder> element has a mandatory
        * "auid" parameter that identifies the type of documents that
        * particular folder contains. Therefore, a client application has
        * all the necessary information it needs to have in order to be
        * able to carry out a fetch for a document subset (one folder):
        *
        * _LIT( KNodePath, "xcap-directory/folder" );
        * CXdmDocumentNode* node = iDirectoryDoc->DocumentSubsetL( KNodePath );
        * CleanupStack::PushL( node );
        * CXdmNodeAttribute* auid = node->CreateAttributeL( _L( "auid" ) );
        * CleanupStack::PushL( auid );
        * auid->SetAttributeValueL( _L( "My document type" ) );
        * iDirectoryDoc->FetchDataL( node );
        * CleanupStack::Pop( 2 );  //auid, node
        *
        * In this particular case, even after the fetch operation is
        * complete, the <xcap-directory> element in this document model
        * would not be a real element, a genuine part of the document, but,
        * rather, a "virtual" node on the way to the element that is of
        * real interest to the client application.
        *
        * Also note that the instance of CXdmDocument on which this subset
        * has been created, cannot be used as a container for a whole
        * document until ResetSubset() has been called.
        *
        * The incoming parameter MUST separate each step to the
        * target element with a slash character ('/'). 
        *
        * @param TDesC8& The path to the target element
		* @return CXdmDocumentNode* 
        */
        IMPORT_C CXdmDocumentNode* DocumentSubsetL( const TDesC& aNodePath );
        
        /*
        * Reset the document subset.
        * 
        * See the comments to DocumentSubsetL method for details.
        * @return void
        */
        IMPORT_C virtual void ResetSubset();
        
        /**
        * Destructor.
        */
        IMPORT_C ~CXdmDocument();

    public: // New functions
        
        /**
        * Return the type of this document
        *
        * @return TXdmDocType The type of the document
        */
        virtual TXdmDocType DocumentType() const = 0;
        
        /**
        * Construct an "anonymous" entry point for this document.
        * This node effectively becomes the root of this document.
        *
        * @return CXdmDocumentNode* Document root
        */
        virtual CXdmDocumentNode* CreateRootL() = 0;

        /**
        * Return the root element of this document
        *
        * NOTE: The returning node may or may not contain any data,
        *       since, in the case that this document model does not
        *       have content yet, the element is a simple "entry point"
        *       to the whole document. Thus, for instance, if the client
        *       application requests the XDM Engine to fetch a particular
        *       document, the root element points to nothing until the 
        *       request has been completed.
        *       
        *       Also note that in case the client has created a document
        *       subset using the DocumentSubsetL() method, this method 
        *       will return NULL, since the document is not traversable 
        *       from the root; it, by definition, represents only a subset
        *       of this document. In case of a document subset, the path from
        *       the root node to the target node should be considered a simple
        *       linked list that does not necessarily contain all children
        *       for any of the nodes in between.
		*
        * @return CXdmDocumentNode* The root element of the document   
        */
        virtual CXdmDocumentNode* DocumentRoot() const = 0;
        
        /*
        * Reset the contents of this document model.
        * 
        * This method comes into play when client wants to reuse an instance
        * of this class for, for instance, construction of different content.
        * Other properties, such as name and the type of the document
        * do not change.
        *
        * @return void
        */
        virtual void ResetContents() = 0;
                
        /**
        * Return the currently last node in the path. 
        * NOTE: Only works with document subsets!
        *
        * @return CXdmDocumentNode*
        */
        virtual CXdmDocumentNode* CurrentExtent() const = 0;
        
        /**
        * Return the time stamp for this document
        *
		* @return TTime Time stamp
        */
        virtual TTime TimeStamp() const = 0;
        
        /**
        * Return an error document for a failed update
        * 
        * NOTE: The returning object will be zero in case
        *       the used protocol does not support this feature.
        *
		* @return CXdmDocument* The error document
        */
        virtual CXdmDocumentNode* ErrorRoot() = 0;
        
    protected:
    
        /**
        * Append an element to the path identifying the 
        * the current document subset
        *
        * @param TDesC& The path part to append
        * @return void
        */
        virtual void AppendPathPartL( const TDesC& aString ) = 0;
    
    private:
        
        /**
        * Empty the operation queue
        */
        void EmptyOperationQueue();
        
        /**
        * Find attributes by name
        *
        * @param CXdmDocumentNode* The node to search from
        * @param TDesC& The name of the attributes to search for
        * @param RPointerArray& On completion, contains the results
		* @return TInt The number of attributes found 
        */
        TInt DoFindAttributes( CXdmDocumentNode* aNode,
							   const TDesC& aAttributeName,
                               RPointerArray<CXdmNodeAttribute>& aResultArray ) const;

    private:  //Methods which need to be accessible from the CXdmEngine
              //but NOT from the client application => friend class
        
        /**
        * Start updating the document
        * @param TRequestStatus& The status of the request
		* @return void 
        */
        virtual void StartUpdateL() = 0;

        /**
        * Cancel a document update
        * @param TRequestStatus& The status of the request
		* @return void 
        */
        virtual void CancelUpdate() = 0;
        
        /**
        * Save the client's request status
        * @param TRequestStatus& The status of the request
		* @return void 
        */
        virtual void SaveClientStatus( TRequestStatus& aClientStatus ) = 0;
    
    protected:
        
        /**
        * By default Symbian OS constructor is private.
        * @return void
        */
        IMPORT_C void BaseConstructL( TInt aOperationFactoryUid,
                                      const TDesC& aDocumentName );
        
        /**
        * By default Symbian OS constructor is private.
        * @return void
        */
        IMPORT_C void BaseConstructL( TInt aOperationFactoryUid,
                                      const TDesC8& aDocumentName );
        
        /**
        * C++ constructor is private.
        */
		IMPORT_C CXdmDocument( CXdmEngine& aXdmEngine );

    protected:   //Data
        
        TBool                                   iDocSubset;
        HBufC*                                  iDocumentName;
        TRequestStatus*                         iClientStatus;
        TTime                                   iTimeStamp;
        CXdmEngine&                             iXdmEngine;
        friend class                            CXdmEngine;
        RPointerArray<MXdmOperation>            iChangeRequests;
        CXdmOperationFactory*                   iOperationFactory;

    private:    // Data
    };

#endif      //__XDMDOCUMENT__
            
// End of File
