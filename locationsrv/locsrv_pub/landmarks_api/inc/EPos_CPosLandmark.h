/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPosLandmark class
*
*/


#ifndef CPOSLANDMARK_H
#define CPOSLANDMARK_H

#include <e32base.h>
#include <lbsfields.h>
#include <badesca.h>

#include "EPos_Landmarks.h"

class TLocality;


/**
*  This is a container class for a landmark.
*
*  A landmark is principally a location with a name.
*
*  The landmark object can either be created by a client or retrieved from
*  @ref CPosLandmarkDatabase.
*
*  A landmark consists of a number of landmark attributes, e.g. landmark name,
*  landmark position, coverage area, etc.
*
*  A landmark may also contain generic position fields. These position fields
*  are defined in LbsFieldIds.h. Only text fields are supported.If the client 
*  wants to store a non-text field, the value must first be converted to a 
*  textual representation.
*  EPositionPlaceId position field added in extension of TPositionFieldId to support Place id
*  EPositionTimeStamp position field added in extension of TPositionFieldId to support TimeStamp
*
*  CPosLandmark contains functions for setting and getting landmark attributes
*  and position fields.
*
*  Note: CPosLandmark is only a local representation of the landmark. To
*  update the database, call @ref CPosLandmarkDatabase::UpdateLandmarkL (or
*  @ref CPosLandmarkDatabase::AddLandmarkL for a new landmark).
*
*  @lib eposlandmarks.lib
*  @since S60 3.0
*/
class CPosLandmark : public CBase
    {
    public:

        /**
        *  Bitmap for specifying a group of landmark attributes. Bit values are
        *  defined by @ref _TAttributes.
        */
        typedef TUint32 TAttributes;

        /**
        *  Specifies a landmark attribute such as landmark name or landmark
        *  position.
        */
        enum _TAttributes
            {
						/**
            * No attribute specified. 
						*/
            ENoAttribute        = 0x0000  ,
						/**
            * The name of the landmark.
            */
            ELandmarkName       = 0x0001  ,
						/**
            * The position of the landmark. 
						*/
            EPosition           = 0x0002  ,
						/**
            * The landmark coverage radius. 
						*/
            ECoverageRadius     = 0x0004  ,
            /**
            * The categories of the landmark. 
						*/
            ECategoryInfo       = 0x0008  ,
            /**
            * The icon that represents the landmark in a UI. 
            */
            EIcon               = 0x0010  ,
            /**
            * A description of the landmark. 
            */
            EDescription        = 0x0020  ,
            /**
            * PlaceId for the landmark
            */    
            EPlaceId						= 0x0040  ,
            /**
            * Timestamp associated with the landmark
            */
            ETimeStamp					= 0x0080  ,
            /*
            * All landmark attributes. 
            */
            EAllAttributes      = 0xFFFF  
            };

    public:

        /**
        * Two-phased constructor.
        *
        * @returns A new instance of this class.
        */
        IMPORT_C static CPosLandmark* NewLC();

        /**
        * Two-phased constructor.
        *
        * @returns A new instance of this class.
        */
        IMPORT_C static CPosLandmark* NewL();

        /**
        * Two-phased copy constructor.
        *
        * @param[in] aLandmark The landmark to copy.
        * @returns A copy of the specified landmark object.
        */
        IMPORT_C static CPosLandmark* NewLC(
               const CPosLandmark&  aLandmark
        );

        /**
        * Two-phased copy constructor.
        *
        * @param[in] aLandmark The landmark to copy.
        * @returns A copy of the specified landmark object.
        */
        IMPORT_C static CPosLandmark* NewL(
               const CPosLandmark&  aLandmark
        );

        /**
        * Destructor.
        */
        virtual ~CPosLandmark();

    public:

        /**
        * Reads the ID of the landmark entry in the database.
        *
        * @returns The ID of the landmark entry in the database, or
        *   @p KPosLmNullItemId if the landmark has not been added to the
        *   database yet.
        */
        IMPORT_C TPosLmItemId LandmarkId() const;

        /**
        * Checks if the landmark is partial.
        *
        * A client can read partial information about a landmark from the
        * database. This function checks if only partial information is
        * included in the landmark object. Partial landmark can not be used
        * with @ref CPosLandmarkDatabase::UpdateLandmarkL().
        *
        * For more about partial landmarks, see
        * @ref CPosLandmarkDatabase::ReadPartialLandmarkLC().
        *
        * @returns @p EFalse if the landmark contains all the landmark
        *   information, otherwise @p ETrue.
        */
        IMPORT_C TBool IsPartial() const;

        /**
        * Reads the name of the landmark.
        *
        * This function returns error code @p KErrNotFound if the landmark
        * name is not set in this landmark object. This will be the case if
        * the landmark is read from the database using partial read and
        * landmark name is excluded. Note that if a landmark is fully read
        * from the database, the landmark name will always be included. If no
        * name has been set for the landmark in the database, it will
        * have an empty name string "".
        *
        * @param[out] aLandmarkName On return contains the landmark name.
        * @returns @p KErrNone if successful, @p KErrNotFound if the landmark
        *   name is not specified.
        */
        IMPORT_C TInt GetLandmarkName(
               TPtrC&  aLandmarkName
        ) const;

        /**
        * Sets the name of the landmark.
        *
        * If no name is set for the landmark when it is written to the database,
        * the landmark in the database will have an empty name string "".
        *
        * @param[in] aLandmarkName The landmark name.
        *
        * @leave KErrArgument If the name of the landmark is longer than
        * @p KPosLmMaxTextFieldLength
        */
        IMPORT_C void SetLandmarkNameL(
               const TDesC&  aLandmarkName
        );

        /**
        * Reads the landmark position.
        *
        * @param[out] aPosition On return contains the landmark position.
        * @returns @p KErrNone if successful, @p KErrNotFound if the landmark
        *   position is not set.
        */
        IMPORT_C TInt GetPosition(
               TLocality&  aPosition
        ) const;

        /**
        * Sets the landmark position.
        *
        * Latitude and longitude must be set in the position.
        *
        * Only WGS 84 coordinates are allowed. @p KPositionDatumWgs84 must be set as datum.
        *
        * @param[in] aPosition The landmark position.
        *
        * @leave KErrArgument Latitude and/or longitude is not set or other datum 
        *   than @p KPositionDatumWgs84 is used in @p aPosition.
        */
        IMPORT_C void SetPositionL(
               const TLocality& aPosition
        );

        /**
        * Reads the landmark coverage radius.
        *
        * Coverage radius is set if the landmark is big, e.g. a city. It
        * defines the size of the area which the landmark represents.
        *
        * @param[out] aCoverageRadius On return contains the coverage radius.
        * @returns @p KErrNone if successful, @p KErrNotFound if the landmark
        *   coverage radius is not set.
        */
        IMPORT_C TInt GetCoverageRadius(
               TReal32&  aCoverageRadius
        ) const;

        /**
        * Sets the landmark coverage radius.
        *
        * Coverage radius is set if the landmark is big, e.g. a city. It
        * defines the size of the area which the landmark represents.
        *
        * If coverage radius is set to NaN, then the coverage radius will be
        * removed.
        *
        * @param aCoverageRadius The coverage radius.
        */
        IMPORT_C void SetCoverageRadius(
               TReal32  aCoverageRadius
        );

        /**
        * Adds a category to the landmark.
        *
        * If the specified category has already been added, nothing happens.
        *
        * @param aCategoryId The category to add.
        */
        IMPORT_C void AddCategoryL(
               TPosLmItemId  aCategoryId
        );

        /**
        * Removes a category from the landmark.
        *
        * If the specified category is not in the landmark, nothing happens.
        *
        * @param aCategoryId The category to remove
        */
        IMPORT_C void RemoveCategory(
               TPosLmItemId  aCategoryId
        );

        /**
        * Retrieves the database item IDs for the categories contained in this
        * landmark.
        *
        * @param[out] aCategoryIdArray On return contains the list of this 
        *   landmark's categories.
        */
        IMPORT_C void GetCategoriesL(
               RArray<TPosLmItemId>&  aCategoryIdArray
        ) const;

        /**
        * Checks if the landmark contains a specific position field.
        *
        * @param aFieldId The position field.
        * @returns @p ETrue if the landmark contains the field, otherwise
        *   @p EFalse.
        */
        IMPORT_C TBool IsPositionFieldAvailable(
               TPositionFieldId aFieldId
        ) const;

        /**
        * Returns the number of position fields set in the landmark.
        *
        * @returns The number of position fields set in the landmark.
        */
        IMPORT_C TUint NumOfAvailablePositionFields() const;

        /**
        * Returns the first position field contained in the landmark.
        *
        * This function is used to initiate iteration over the position fields.
        * @ref NextPositionFieldId() is called to continue the iteration.
        *
        * @returns The first position field contained by the landmark, or
        *   @p EPositionFieldNone if there are no position fields in the
        *   landmark.
        */
        IMPORT_C TPositionFieldId FirstPositionFieldId() const;

        /**
        * Returns the next position field contained in the landmark.
        *
        * This function is used to iterate the position fields in the landmark.
        * @ref FirstPositionFieldId() is called to get the first ID. This ID is
        * passed to @ref NextPositionFieldId() to obtain the second ID, etc.
        *
        * If the client specifies an ID which is not contained in the landmark,
        * this function will panic with code @p EPosInvalidPositionFieldId. It
        * is therefore important that the client does not remove the current
        * field while iterating. If the client still removes the current field,
        * the client must pass the previous field.
        *
        * @param aFieldId The last position field which was read.
        * @returns The next position field contained by the landmark, or
        *   @p EPositionFieldNone if there are no more position fields in the
        *   landmark.
        *
        * @panic "Landmarks Client"-EPosInvalidPositionFieldId Client specified a field ID,
        *   which is not contained in the landmark.
        */
        IMPORT_C TPositionFieldId NextPositionFieldId(
               TPositionFieldId aFieldId
        ) const;

        /**
        * Reads the value of a position field.
        *
        * @param[in] aFieldId The position field to read.
        * @param[out] aFieldValue On return contains the value of the position field.
        * @returns @p KErrNone if successful, @p KErrNotFound if the landmark
        *   does not contain the specified position field.
        */
        IMPORT_C TInt GetPositionField(
               TPositionFieldId aFieldId,
               TPtrC& aFieldValue
        ) const;

        /**
        * Sets a position field in the landmark.
        *
        * @param[in] aFieldId The position field to set.
        * @param[in] aFieldValue The new value for the position field.
        *
        * @leave KErrArgument If the position field text is longer than
        *   @p KPosLmMaxTextFieldLength.
        */
        IMPORT_C void SetPositionFieldL(
               TPositionFieldId aFieldId,
               const TDesC& aFieldValue
        );

        /**
        * Removes a position field from the landmark.
        *
        * If the specified position field is not contained in the landmark,
        * nothing will happen.
        *
        * @param aFieldId The position field to remove.
        */
        IMPORT_C void RemovePositionField(
               TPositionFieldId  aFieldId
        );

        /**
        * Associates the landmark with an icon.
        *
        * Icons are found in icon files. To set an icon, the client
        * must specify the name of the icon file and the index of the
        * icon within the file.
        *
        * The landmark is not affected if the icon file is changed or
        * removed. It only contains a link to the icon.
        *
        * @param[in] aIconFileName The full icon file name.
        * @param[in] aIconIndex The index of the icon within the icon file.
        * @param[in] aIconMaskIndex The index of the icon mask within the
        *   icon file.
        *
        * @leave KErrArgument The icon file name is longer than @p KMaxFileName.
        *
        * @panic "Landmarks Client"-EPosLmInvalidArgument The icon index is negative or
        *   the icon mask index is negative and not equal to @p KPosLmIconMaskNotUsed.
        */
        IMPORT_C void SetIconL(
               const TDesC&  aIconFileName,
               TInt  aIconIndex,
               TInt  aIconMaskIndex
        );

        /**
        * Returns the link to the icon associated with the landmark.
        *
        * Icons are found in icon files. It is referenced by the name of
        * the icon file and the index of the icon within the file.
        *
        * The landmark is not affected if the icon file is changed or
        * removed. It only contains a link to the icon. This means that the
        * link could be invalid.
        *
        * @param[out] aIconFileName The full icon file name.
        * @param[out] aIconIndex The index of the icon within the icon file.
        * @param[out] aIconMaskIndex The index of the icon mask within the
        *   icon file. If no icon mask index is defined @p KPosLmIconMaskNotUsed
        *   is returned.
        *
        * @returns @p KErrNone if successful, @p KErrNotFound if the icon is
        *   not set.
        */
        IMPORT_C TInt GetIcon(
               TPtrC& aIconFileName,
               TInt&  aIconIndex,
               TInt&  aIconMaskIndex
        ) const;

        /**
        * Reads the description of the landmark.
        *
        * This function returns error code @p KErrNotFound if the landmark
        * description is not set in this landmark object. This will be the case
        * if the landmark is read from the database using partial read and
        * landmark description is excluded. Note that if a landmark is fully
        * read from the database, the landmark description is always
        * included. If no description has been set for the landmark in
        * the database, it is set to an empty string "".
        *
        * @param[out] aLandmarkDescription On return contains the landmark description.
        * @returns @p KErrNone if successful, @p KErrNotFound if the landmark
        *   description is not specified.
        */
        IMPORT_C TInt GetLandmarkDescription(
               TPtrC&  aLandmarkDescription
        ) const;

        /**
        * Sets a description for the landmark.
        *
        * If no description is set for the landmark when it is written to the
        * database, the landmark in the database will have an empty description
        * string "".
        *
        * @param[in] aLandmarkDescription The landmark description.
        *
        * @leave KErrArgument The name of the landmark is longer than
        *   @p KPosLmMaxDescriptionLength.
        */
        IMPORT_C void SetLandmarkDescriptionL(
               const TDesC&  aLandmarkDescription
        );

        /**
        * Removes landmark attributes from the landmark.
        *
        * @param aAttributes A bitmap specifying which landmark attributes to
        *   remove.
        */
        IMPORT_C void RemoveLandmarkAttributes(
               TAttributes  aAttributes
        );

        /**
        * @internal */
        /*
        * Sets the partial update flag to the landmark.
        * This flag is used to indicate if only partial information is included
        * in the landmark object.
        *
        * @param aPartial @p EFalse if the landmark contains all the landmark
        *   information, otherwise @p ETrue.
        */
        void SetPartialL(
               TBool aPartial
        );

        /**
        * @internal */
        /*
        * Sets the landmark ID to the landmark.
        *
        * @param aId The landmark ID to set.
        */
        void SetLandmarkIdL(
               TPosLmItemId aId
        );
				
				/**
				* Sets the PlaceId for the landmark
				* @param[in]  aPId The place id of the landmark
				* @leave  Symbian error codes
				*/
				IMPORT_C void SetPlaceIdL( const TDesC& aPId );
				
				/**
				* Gets the PlaceId of the landmark
				* @param[out] aPId On return contains the place id of the landmark
				* @return KErrNone if successful , else KErrNotFound if PlaceId is 
				*					not specified.
				*/
				IMPORT_C TInt GetPlaceId( TPtrC& aPId ) const;
				
				/**
				* Sets the timestamp of the landmark.
				* @param[in] aTimeStamp Timestamp of the landmark(Full date & time)
				* @leave KErrArgument If the full date & time have not been specified.
				*/
				IMPORT_C void SetTimeStampL( const TTime aTimeStamp );
				
				/**
				* Gets the timestamp of the landmark
				* @param[out] aTimeStamp On return contains the timestamp of the landamrk
				* @return KErrNone if successful , else KErrNotFound if TimeStamp is 
				*					not specified.
				*/
				IMPORT_C TInt GetTimeStamp( TTime& aTimeStamp )const;

    private:

        // C++ constructor.
        CPosLandmark();

        // Prohibit copy constructor
        CPosLandmark(const CPosLandmark&);

        // Prohibit assigment operator
        CPosLandmark& operator= (const CPosLandmark&);

        void ConstructL();
        void ConstructL(const CPosLandmark& aLandmark);

    private:

        // Landmark ID
        TPosLmItemId iId;

        // Partial landmark flag
        TBool iIsPartial;

        // Landmark name
        HBufC* iLandmarkName;

        // Landmark position information
        TLocality* iPosition;

        // Landmark coverage radius
        TReal32 iCoverageRadius;

        // Landmark categories
        RArray<TPosLmItemId> iCategoryArray;

        // Landmark position field IDs
        RArray<TUint> iPositionFieldIds;

        // Landmark position field strings
        CDesCArray* iPositionFieldStrings;

        // Landmark icon filename
        HBufC* iIconFileName;

        // Landmark icon index
        TInt iIconIndex;

        // Landmark icon mask index
        TInt iIconMaskIndex;

        // Landmark description
        HBufC* iLandmarkDescription;
    };

#endif      // CPOSLANDMARK_H


