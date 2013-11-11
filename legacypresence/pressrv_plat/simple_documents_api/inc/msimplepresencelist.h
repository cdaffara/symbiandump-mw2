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
* Description:    SIMPLE Engine presence list
*
*/




#ifndef M_simplepresencelist_H
#define M_simplepresencelist_H

#include <e32std.h>
#include <msimpledocument.h>

// FORWARD DECLARATION

class MSimpleMeta;

/**
 *  MSimplePresenceList
 *
 *  SIMPLE Engine presence list
 *
 *  @lib simplexmlutils
 *  @since S60 3.2
 */

class MSimplePresenceList
    {

public:

    /**
     * Destructor.
     * @since S60 3.2
     */
    virtual void Close() = 0;

    /**
     * Getting the child elements of this element.
     *
     * @since S60 3.2
     * @param aDocuments an array of presence entities. This is an empty array if there
     *         are no entities. OUT.
     *         Do NOT call ResetAndDestroy() for the array,
     *         you may call Reset() only. The array is valid only till
     *         the next call of this method in this entity.
     */
    virtual void GetDocuments( RPointerArray<MSimpleDocument>& aDocuments ) = 0;

    /**
     * Getter for the Metadata document.
     * @since S60 3.2
     * @return metadata document. Ownership is not transferred.
     */
    virtual MSimpleMeta* MetaData() = 0;

    /**
     * Getting the sub-lists
     *
     * @since S60 3.2
     * @param aLists array of presence sub lists. OUT.
     *         Do NOT call ResetAndDestroy() for the array,
     *         you may call Reset() only. The array is valid only till
     *         the next call of this method in this entity.
     */
    virtual void GetSubLists( RPointerArray<MSimplePresenceList>& aLists ) = 0;

    };

#endif

// End of File
