/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of container class for a landmark.
*
*
*/


// INCLUDE FILES
#include "EPos_CPosLandmark.h"
#include "EPos_LandmarksErrors.h"
#include <e32math.h>
#include <lbsposition.h>

// CONSTANTS
const TInt KPosFieldsGranularity = 8;

// Constants used for setting the timestamp
_LIT( KYearFormat, "%04d" );
_LIT( KFormatMicroSeconds, "%06d" );
_LIT( KFormat, "%02d" );
_LIT( KTimeMicroSecondSeparator, "." );
_LIT( KDateTimeSeparator,   ":" );
// constants used for the place id & timestamp fields which internally use the 
// TPositionFieldId for storing the values.
const TPositionFieldId KPlaceIdPositionField = EPositionFieldProprietaryFieldsBegin;
const TPositionFieldId KTimeStampPositionField = EPositionFieldProprietaryFieldsBegin+1;

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLandmark::CPosLandmark()
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLandmark::CPosLandmark()
:   iId(KPosLmNullItemId),
    iIsPartial(EFalse),
    iLandmarkName(NULL),
    iPosition(NULL),
    iCategoryArray(),
    iPositionFieldIds(KPosFieldsGranularity),
    iPositionFieldStrings(NULL),
    iIconFileName(NULL),
    iIconIndex(KErrNotFound),
    iIconMaskIndex(KPosLmIconMaskNotUsed),
    iLandmarkDescription(NULL)
    {
    TRealX nan;
    nan.SetNaN();

    iCoverageRadius = nan;
    }

// -----------------------------------------------------------------------------
// CPosLandmark::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLandmark::ConstructL()
    {
    iPositionFieldStrings = new (ELeave) CDesCArrayFlat(KPosFieldsGranularity);
    }

// -----------------------------------------------------------------------------
// CPosLandmark::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLandmark::ConstructL(const CPosLandmark& aLandmark)
    {
    iId = aLandmark.iId;
    iIsPartial = aLandmark.iIsPartial;
    iCoverageRadius = aLandmark.iCoverageRadius;
    iIconIndex = aLandmark.iIconIndex;
    iIconMaskIndex = aLandmark.iIconMaskIndex;

    if (aLandmark.iLandmarkName)
        {
        iLandmarkName = aLandmark.iLandmarkName->AllocL();
        }

    if (aLandmark.iPosition)
        {
        iPosition = new (ELeave) TLocality(*aLandmark.iPosition);
        }

    TInt i;
    for (i = 0; i < aLandmark.iCategoryArray.Count(); i++)
        {
        User::LeaveIfError(iCategoryArray.Append(aLandmark.iCategoryArray[i]));
        }

    for (i = 0; i < aLandmark.iPositionFieldIds.Count(); i++)
        {
        User::LeaveIfError(
            iPositionFieldIds.Append(aLandmark.iPositionFieldIds[i]));
        }

    iPositionFieldStrings = new (ELeave) CDesCArrayFlat(KPosFieldsGranularity);
    for (i = 0; i < aLandmark.iPositionFieldStrings->MdcaCount(); i++)
        {
        iPositionFieldStrings->AppendL(
            aLandmark.iPositionFieldStrings->MdcaPoint(i));
        }

    if (aLandmark.iIconFileName)
        {
        iIconFileName = aLandmark.iIconFileName->AllocL();
        }

    if (aLandmark.iLandmarkDescription)
        {
        iLandmarkDescription = aLandmark.iLandmarkDescription->AllocL();
        }
    }

// -----------------------------------------------------------------------------
// CPosLandmark::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmark* CPosLandmark::NewLC()
    {
    CPosLandmark* self = new (ELeave) CPosLandmark;
    CleanupStack::PushL(self);
    self->ConstructL();

    return self;
    }

// -----------------------------------------------------------------------------
// CPosLandmark::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmark* CPosLandmark::NewL()
    {
    CPosLandmark* self = CPosLandmark::NewLC();
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CPosLandmark::NewLC
// Two-phased copy constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmark* CPosLandmark::NewLC(
    const CPosLandmark& aLandmark)
    {
    CPosLandmark* self = new (ELeave) CPosLandmark;
    CleanupStack::PushL(self);
    self->ConstructL(aLandmark);

    return self;
    }

// -----------------------------------------------------------------------------
// CPosLandmark::NewL
// Two-phased copy constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmark* CPosLandmark::NewL(
    const CPosLandmark& aLandmark)
    {
    CPosLandmark* self = CPosLandmark::NewLC(aLandmark);
    CleanupStack::Pop();

    return self;
    }

// Destructor
CPosLandmark::~CPosLandmark()
    {
    delete iLandmarkName;
    delete iPosition;
    iCategoryArray.Close();
    iPositionFieldIds.Close();
    delete iPositionFieldStrings;
    delete iIconFileName;
    delete iLandmarkDescription;
    }

// ---------------------------------------------------------
// CPosLandmark::LandmarkId
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TPosLmItemId CPosLandmark::LandmarkId() const
    {
    return iId;
    }

// ---------------------------------------------------------
// CPosLandmark::IsPartial
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TBool CPosLandmark::IsPartial() const
    {
    return iIsPartial;
    }

// ---------------------------------------------------------
// CPosLandmark::GetLandmarkName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CPosLandmark::GetLandmarkName(
    TPtrC& aLandmarkName) const
    {
    if (!iLandmarkName)
        {
        return KErrNotFound;
        }

    aLandmarkName.Set(*iLandmarkName);
    return KErrNone;
    }

// ---------------------------------------------------------
// CPosLandmark::SetLandmarkNameL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLandmark::SetLandmarkNameL(
    const TDesC& aLandmarkName)
    {
    if (aLandmarkName.Length() > KPosLmMaxTextFieldLength)
        {
        User::Leave(KErrArgument);
        }

    HBufC* previousName = iLandmarkName;
    iLandmarkName = aLandmarkName.AllocL();
    delete previousName;
    }

// ---------------------------------------------------------
// CPosLandmark::GetPosition
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CPosLandmark::GetPosition(
    TLocality& aPosition) const
    {
    if (!iPosition)
        {
        return KErrNotFound;
        }

    aPosition = *iPosition;
    return KErrNone;
    }

// ---------------------------------------------------------
// CPosLandmark::SetPositionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLandmark::SetPositionL(
    const TLocality& aPosition)
    {
    if (Math::IsNaN(aPosition.Latitude()) ||
        Math::IsNaN(aPosition.Longitude())  ||
        aPosition.Datum() != KPositionDatumWgs84)
        {
        User::Leave(KErrArgument);
        }

    TLocality* previousPosition = iPosition;
    iPosition = new (ELeave) TLocality(aPosition);
    delete previousPosition;
    }

// ---------------------------------------------------------
// CPosLandmark::GetCoverageRadius
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CPosLandmark::GetCoverageRadius(
    TReal32& aCoverageRadius) const
    {
    if (Math::IsNaN(TReal(iCoverageRadius)))
        {
        return KErrNotFound;
        }

    aCoverageRadius = iCoverageRadius;
    return KErrNone;
    }

// ---------------------------------------------------------
// CPosLandmark::SetCoverageRadius
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLandmark::SetCoverageRadius(
    TReal32 aCoverageRadius)
    {
    if (!Math::IsNaN(TReal(aCoverageRadius)) &&
        aCoverageRadius < 0)
        {
        Panic(KPosLandmarksClientPanic, EPosNegativeValue);
        }

    iCoverageRadius = aCoverageRadius;
    }

// ---------------------------------------------------------
// CPosLandmark::AddCategoryL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLandmark::AddCategoryL(
    TPosLmItemId aCategoryId)
    {
    if (aCategoryId == KPosLmNullItemId)
        {
        User::Leave(KErrArgument);
        }

    TInt index = iCategoryArray.Find(aCategoryId);
    if (index == KErrNotFound)
        {
        User::LeaveIfError(iCategoryArray.Append(aCategoryId));
        }
    }

// ---------------------------------------------------------
// CPosLandmark::RemoveCategory
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLandmark::RemoveCategory(
    TPosLmItemId aCategoryId)
    {
    TInt index = iCategoryArray.Find(aCategoryId);
    if (index != KErrNotFound)
        {
        iCategoryArray.Remove(index);
        }
    }

// ---------------------------------------------------------
// CPosLandmark::GetCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLandmark::GetCategoriesL(
    RArray<TPosLmItemId>& aCategoryIdArray) const
    {
    aCategoryIdArray.Reset();
    for (TInt i = 0; i < iCategoryArray.Count(); i++)
        {
        User::LeaveIfError(aCategoryIdArray.Append(iCategoryArray[i]));
        }
    }

// ---------------------------------------------------------
// CPosLandmark::IsPositionFieldAvailable
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TBool CPosLandmark::IsPositionFieldAvailable(
    TPositionFieldId aFieldId) const
    {
    TInt index = iPositionFieldIds.Find(aFieldId);
    if (index == KErrNotFound)
        {
        return EFalse;
        }
    else
        {
        return ETrue;
        }
    }

// ---------------------------------------------------------
// CPosLandmark::NumOfAvailablePositionFields
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TUint CPosLandmark::NumOfAvailablePositionFields() const
    {
    return iPositionFieldIds.Count();
    }

// ---------------------------------------------------------
// CPosLandmark::FirstPositionFieldId
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TPositionFieldId CPosLandmark::FirstPositionFieldId() const
    {
    if (iPositionFieldIds.Count() == 0)
        {
        return EPositionFieldNone;
        }

    return static_cast<TPositionFieldId>(iPositionFieldIds[0]);
    }

// ---------------------------------------------------------
// CPosLandmark::NextPositionFieldId
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TPositionFieldId CPosLandmark::NextPositionFieldId(
    TPositionFieldId aFieldId) const
    {
    TInt index = iPositionFieldIds.Find(aFieldId);
    if (index == KErrNotFound)
        {
        Panic(KPosLandmarksClientPanic, EPosInvalidPositionFieldId);
        }

    if (++index == iPositionFieldIds.Count())
        {
        return EPositionFieldNone;
        }

    return static_cast<TPositionFieldId>(iPositionFieldIds[index]);
    }

// ---------------------------------------------------------
// CPosLandmark::GetPositionField
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CPosLandmark::GetPositionField(
    TPositionFieldId aFieldId,
    TPtrC& aFieldValue) const
    {
    TInt index = iPositionFieldIds.Find(aFieldId);
    if (index == KErrNotFound)
        {
        return KErrNotFound;
        }

    aFieldValue.Set(iPositionFieldStrings->MdcaPoint(index));
    return KErrNone;
    }

// ---------------------------------------------------------
// CPosLandmark::SetPositionFieldL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLandmark::SetPositionFieldL(
    TPositionFieldId aFieldId,
    const TDesC& aFieldValue)
    {
    if (aFieldValue.Length() > KPosLmMaxTextFieldLength)
        {
        User::Leave(KErrArgument);
        }

    if (aFieldId == EPositionFieldNone)
        {
        User::Leave(KErrArgument);
        }

    // Does the field already exist?
    TInt index = iPositionFieldIds.Find(aFieldId);
    if (index == KErrNotFound)
        {
        // Create the field
        iPositionFieldStrings->AppendL(aFieldValue);
        TInt appendResult = iPositionFieldIds.Append(aFieldId);

        if (appendResult != KErrNone)
            {
            // iPositionFieldStrings and iPositionFieldIds must contain
            // matching string/id pairs, thus remove the last string in
            // iPositionFieldStrings.
            iPositionFieldStrings->Delete(iPositionFieldStrings->Count() - 1);
            User::Leave(appendResult);
            }
        }
    else
        {
        // Update the field by inserting a new field and removing the old one.
        iPositionFieldStrings->InsertL(index, aFieldValue);
        iPositionFieldStrings->Delete(index + 1);
        }
    }

// ---------------------------------------------------------
// CPosLandmark::RemovePositionField
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLandmark::RemovePositionField(
    TPositionFieldId aFieldId)
    {
    TInt index = iPositionFieldIds.Find(aFieldId);
    if (index != KErrNotFound)
        {
        iPositionFieldIds.Remove(index);
        iPositionFieldStrings->Delete(index);
        }
    }

// ---------------------------------------------------------
// CPosLandmark::SetIconL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLandmark::SetIconL(
    const TDesC& aIconFileName,
    TInt aIconIndex,
    TInt aIconMaskIndex)
    {
    if (aIconFileName.Length() > KMaxFileName)
        {
        User::Leave(KErrArgument);
        }
    if (aIconIndex < 0 || aIconMaskIndex < KPosLmIconMaskNotUsed)
        {
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument);
        }

    HBufC* previousFileName = iIconFileName;
    iIconFileName = aIconFileName.AllocL();
    delete previousFileName;

    iIconIndex = aIconIndex;
    iIconMaskIndex = aIconMaskIndex;
    }

// ---------------------------------------------------------
// CPosLandmark::GetIcon
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CPosLandmark::GetIcon(
    TPtrC& aIconFileName,
    TInt& aIconIndex,
    TInt& aIconMaskIndex) const
    {
    if (!iIconFileName)
        {
        return KErrNotFound;
        }

    aIconIndex = iIconIndex;
    aIconMaskIndex = iIconMaskIndex;
    aIconFileName.Set(*iIconFileName);
    return KErrNone;
    }

// ---------------------------------------------------------
// CPosLandmark::GetLandmarkDescription
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CPosLandmark::GetLandmarkDescription(
    TPtrC& aLandmarkDescription) const
    {
    if (!iLandmarkDescription)
        {
        return KErrNotFound;
        }

    aLandmarkDescription.Set(*iLandmarkDescription);
    return KErrNone;
    }

// ---------------------------------------------------------
// CPosLandmark::SetLandmarkDescriptionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLandmark::SetLandmarkDescriptionL(
    const TDesC& aLandmarkDescription)
    {
    if (aLandmarkDescription.Length() > KPosLmMaxDescriptionLength)
        {
        User::Leave(KErrArgument);
        }

    HBufC* previousDescription = iLandmarkDescription;
    iLandmarkDescription = aLandmarkDescription.AllocL();
    delete previousDescription;
    }

// ---------------------------------------------------------
// CPosLandmark::RemoveLandmarkAttributes
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLandmark::RemoveLandmarkAttributes(
    TAttributes aAttributes)
    {
    if (aAttributes & ELandmarkName)
        {
        delete iLandmarkName;
        iLandmarkName = NULL;
        }

    if (aAttributes & EPosition)
        {
        delete iPosition;
        iPosition = NULL;
        }

    if (aAttributes & ECoverageRadius)
        {
        TRealX nan;
        nan.SetNaN();

        iCoverageRadius = nan;
        }

    if (aAttributes & ECategoryInfo)
        {
        iCategoryArray.Reset();
        }

    if (aAttributes & EIcon)
        {
        delete iIconFileName;
        iIconFileName = NULL;
        }

    if (aAttributes & EDescription)
        {
        delete iLandmarkDescription;
        iLandmarkDescription = NULL;
        }
    
    if ( aAttributes & EPlaceId )
        {
        RemovePositionField( KPlaceIdPositionField );
        }
    
    if ( aAttributes & ETimeStamp )
        {
        RemovePositionField( KTimeStampPositionField );        
        }
    }

// ---------------------------------------------------------
// CPosLandmark::SetPartialL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLandmark::SetPartialL(
    TBool aPartial)
    {
    iIsPartial = aPartial;
    }

// ---------------------------------------------------------
// CPosLandmark::SetLandmarkIdL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLandmark::SetLandmarkIdL(
    TPosLmItemId aId)
    {
    iId = aId;
    }

// ---------------------------------------------------------
// CPosLandmark::SetPlaceIdL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLandmark::SetPlaceIdL( const TDesC& aPId )
    {
    // using the TPositionFieldId::EPositionFieldProprietaryFieldsBegin
    // for storing the placeid value
    SetPositionFieldL( KPlaceIdPositionField, aPId );
    }

// ---------------------------------------------------------
// CPosLandmark::GetPlaceIdL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CPosLandmark::GetPlaceId( TPtrC& aPId ) const
    {
    // Get the value from the TPositionFieldId::EPositionFieldProprietaryFieldsBegin
    return GetPositionField( KPlaceIdPositionField, aPId );
            
    }

// ---------------------------------------------------------
// CPosLandmark::SetTimeStampL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLandmark::SetTimeStampL( const TTime aTimeStamp )
    {
    // Convert TTime to descriptor to store it as position field
    TBuf<KPosLmMaxTextFieldLength> timeField;
    // Get the date time info from TTime 
    TDateTime dateTime = aTimeStamp.DateTime();
    
    // check that the year & day do not have zero values,if yes then 
    // leave with KErrArgument as the date is mandatory
    if( !dateTime.Year() || !dateTime.Day() )
        {
        User::Leave( KErrArgument );
        }
    
    // Format the year field according to the number of digits
    timeField.AppendFormat( KYearFormat, dateTime.Year() );
    
    // Format month field according to number of digits
    timeField.AppendFormat( KFormat, dateTime.Month() );
    
       
    // Format month field according to number of digits
    timeField.AppendFormat( KFormat, dateTime.Day() );
      
    // Append the date time separator
    timeField.Append( KDateTimeSeparator );
    
    // Format month field according to number of digits
    timeField.AppendFormat( KFormat, dateTime.Hour() );
   

    // Format month field according to number of digits
    timeField.AppendFormat( KFormat, dateTime.Minute() );
      
     // Format month field according to number of digits
    timeField.AppendFormat( KFormat, dateTime.Second() );
       
    // Append the time microseconds separator
    timeField.Append( KTimeMicroSecondSeparator );
    
    // Format the microseconds field according to number of digits
    timeField.AppendFormat( KFormatMicroSeconds, dateTime.MicroSecond() );
    
    
    // using the TPositionFieldId::EPositionFieldProprietaryFieldsBegin+1
    // for storing the timestamp value
    SetPositionFieldL( KTimeStampPositionField, timeField );    
    
    }

// ---------------------------------------------------------
// CPosLandmark::GetTimeStamp
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CPosLandmark::GetTimeStamp( TTime& aTimeStamp )const
    {
    TPtrC timeField;
    // Get the value from the TPositionFieldId::EPositionFieldProprietaryFieldsBegin+1
    TInt error = GetPositionField( KTimeStampPositionField, timeField );
    
    if ( !error  )
        {
        // if this field id is specified then convert it to TTime and set aTimeStamp
        TTime time( timeField );
        aTimeStamp = time;
        }
    return error;
    }
//  End of File
