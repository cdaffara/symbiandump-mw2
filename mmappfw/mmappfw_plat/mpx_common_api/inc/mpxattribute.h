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
* Description:  mpx attribute class
*
*/


#ifndef MPXATTRIBUTE_H
#define MPXATTRIBUTE_H

#include <s32strm.h>

enum TMPXAttributeType
    {
    EMPXTypeUnknown,
    EMPXTypeTInt,
    EMPXTypeText,
    EMPXTypeTObject,
    EMPXTypeCObject,
    EMPXTypeError
    };


/**
*  MPX Attribute data type class.
*
*  @lib mpxcommon.lib
*/
class TMPXAttributeData
    {
public:
    /**
     * Overloadded operator&.
     *
     * @since S60 3.2.3
     * @param aData TMPXAttributeData object
     */
    IMPORT_C TBool operator& (const TMPXAttributeData& aData) const;
    /**
     * Overloadded operator&.
     *
     * @since S60 3.2.3
     * @param aData attribute
     */
    IMPORT_C TBool operator& (TUint& aData) const;
    /**
     * Overloadded operator|.
     *
     * @since S60 3.2.3
     * @param aData TMPXAttributeData object
     */
    IMPORT_C TMPXAttributeData operator| (const TMPXAttributeData& aData) const;
public:
    TInt iContentId;
    TUint iAttributeId;
    };


/**
*  TMPXAttribute data type class.
*  It represents unique id of an item, e.g. a song in the device.
*
*  @lib mpxcommon.lib
*/
class TMPXAttribute
    {
public:     // Constructors and destructor
    /**
    * C++ default constructor.
    *
    * @since S60 3.2.3
    */
    IMPORT_C TMPXAttribute();

    /**
    * C++ constructor.
    *
    * @since S60 3.2.3
    * @param aContentId media content id
    * @param aAttributeId attribute id within the content
    */
    IMPORT_C TMPXAttribute(TInt aContentId, TUint aAttributeId);

    /**
    * Copy constructor.
    *
    * @since S60 3.2.3
    * @param aId source of attribute id
    */
    IMPORT_C TMPXAttribute(const TMPXAttribute& aId);

    /**
     * Copy constructor.
     *
     * @since S60 3.2.3
     * @param aData source of attribute data
     */
    IMPORT_C TMPXAttribute(const TMPXAttributeData& aData);

public:
     /**
    * Overloaded assignment operator.
    *
    * @since S60 3.2.3
    * @param aId source of attribute
    * @return reference to updated object
    */
    IMPORT_C TMPXAttribute& operator=(const TMPXAttribute& aId);

    /**
    * Overloaded equal operator.
    *
    * @since S60 3.2.3
    * @param aId id to be compared
    * @return ETrue if equal, otherwise, EFalse
    */
    IMPORT_C TBool operator==(const TMPXAttribute& aId) const;

public:
    /**
    * Externalize the object of this class.
    *
    * @since S60 3.2.3
    * @param aStream write stream
    */
    IMPORT_C void ExternalizeL(RWriteStream& aStream) const;

    /**
    * Internalize the object of this class.
    *
    * @since S60 3.2.3
    * @param aStream read stream
    */
    IMPORT_C void InternalizeL(RReadStream& aStream);

    /**
    * Get content id.
    *
    * @since S60 3.2.3
    * @return content id
    */
    IMPORT_C TInt ContentId() const;

    /**
    * Attribute id.
    *
    * @since S60 3.2.3
    * @return attribute db
    */
    IMPORT_C TUint AttributeId() const;

    /**
    * Helper function to do match in the RArray of the objects of this class.
    *
    * @since S60 3.2.3
    * @param aFirst first object
    * @param aSecond second object
    */
    IMPORT_C static TBool Match(const TMPXAttribute& aFirst,
                               const TMPXAttribute& aSecond);

    /**
    * Helper function to do match in the RArray of the objects of this class by
    * content id.
    *
    * @since S60 3.2.3
    * @param aFirst first object
    * @param aSecond second object
    */
    IMPORT_C static TBool MatchContentId(const TMPXAttribute& aFirst,
                                         const TMPXAttribute& aSecond);

private:     // Data
    TMPXAttributeData iData;
    };

// CONSTANTS NULL attribute not used for storing data
const TMPXAttributeData KMPXMediaNullAttribute={0x0,0x00};

#endif   // MPXATTRIBUTE_H

// End of File
