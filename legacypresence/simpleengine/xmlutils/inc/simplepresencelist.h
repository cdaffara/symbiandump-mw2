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
* Description:    SIMPLE engine presencelist
*
*/




#ifndef simplepresencelist_H
#define simplepresencelist_H

#include <e32std.h>
#include "simplecommon.h"
#include "msimplepresencelist.h"

// FORWARD DECLARATION
class CSimpleNamespace;
class CSimpleMeta;
class CBodyPart;

/**
 *  CSimplePresenceList
 *
 *  SIMPLE Engine presence list
 *
 *  @lib simplexmlutils
 *  @since S60 v3.1
 */

class CSimplePresenceList : public CBase, public MSimplePresenceList
    {

public:

    /**
     * Two-phased constructor.
     * @since S60 v3.1
     * @param aData MIME multipart data
     * @param aBoundary MIME multipart boundary
     * @param aStart MIME multipart start content-id
     * @return CSimplePresenceList
     */
    static CSimplePresenceList* NewL(
        const TDesC8& aData, const TDesC8& aBoundary, const TDesC8& aStart );

    /**
     * Destructor.
     */
    virtual ~CSimplePresenceList();


// from base class MSimplePresenceList


    /**
     * Destructor.
     * @since Series60 3.1
     */
    void Close();


    /**
     * Getter for the Metadata document.
     * @since Series60 3.1
     * @return metadata document.
     */
    MSimpleMeta* MetaData();
    
    /**
     * Getting the child elements of this element.
     *
     * @since Series60 3.1
     * @param aDocuments an array of presence entities. This is an empty array if there
     *         are no entities. OUT.
     */
    void GetDocuments( RPointerArray<MSimpleDocument>& aDocuments );

    /**
     * Getting the sub-lists
     *
     * @since Series60 3.1
     * @param aLists array of presence sub lists. OUT.
     */
    void GetSubLists( RPointerArray<MSimplePresenceList>& aLists );    


protected:


private:

	/**
	 * ClenupStack operation
	 * aPtrArray RPointerArray<CBodyPart>
	 */
	static void ResetAndDestroy( TAny* aPtrArray );		

    /**
     * DoConstructL
     * Handles XML parsing of MIME multiparts
     * @param aParts MIME multipart parts
     * @param aStart MIME multipart start content-id     
     */
    void DoConstructL( RPointerArray<CBodyPart>& aParts, const TDesC8& aStart );    

    /**
     * Two-phase constructor
     * @param aData MIME multipart data     
     * @param aBoundary MIME multipart boundary     
     * @param aStart MIME multipart start content-id     
     */
    void ConstructL(
        const TDesC8& aData, const TDesC8& aBoundary, const TDesC8& aStart );

    /**
     * constructor
     */
    CSimplePresenceList();

private:    // data

    /**
     * Meta data of RLMI
     * Own.
     */
    CSimpleMeta*                        iMeta;

    /**
     * Sub presence lists
     */
    RPointerArray<CSimplePresenceList>  iPresList;

    /**
     * Presence documents in root level
     */
    RPointerArray<CSimpleDocument>      iDocuments;
    };

#endif      // simplepresencelist_H

// End of File
