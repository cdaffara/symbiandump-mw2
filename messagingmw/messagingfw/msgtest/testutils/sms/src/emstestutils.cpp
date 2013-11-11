// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include "emstestutils.h"
#include <emsinformationelement.h>
#include <gsmumsg.h>
#include <gsmubuf.h>
#include <emsformatie.h>
#include <emssoundie.h>
#include <emsanimationie.h>
#include <emspictureie.h>
#include <emsuserpromptie.h>
#include <fbs.h>

EXPORT_C TBool EmsTestUtils::CompareEmsIE(const CEmsInformationElement& left, const CEmsInformationElement& right)
	{
	TBool retVal = EFalse;

	// make sure the types, start position, and length are the same
	if ((left.Identifier() == right.Identifier()) &&
		(left.StartPosition() == right.StartPosition()) &&
		(left.Length() == right.Length()))
		{
		switch(left.Identifier())
			{
			case CSmsInformationElement::ESmsEnhancedTextFormatting:
				{
				const CEmsFormatIE& leftIE = STATIC_CAST(const CEmsFormatIE&, left);
				const CEmsFormatIE& rightIE = STATIC_CAST(const CEmsFormatIE&, right);

				retVal = (leftIE.FormatLength() == rightIE.FormatLength())   &&
						 (leftIE.Bold() == rightIE.Bold())                   &&
						 (leftIE.Italic() == rightIE.Italic())               &&
						 (leftIE.Underline() == rightIE.Underline())         &&
						 (leftIE.Strikethrough() == rightIE.Strikethrough()) &&
						 (leftIE.Alignment() == rightIE.Alignment())         &&
						 (leftIE.FontSize() == rightIE.FontSize());
				break;
				}

			case CSmsInformationElement::ESmsEnhancedPredefinedSound:
				{
				const CEmsPreDefSoundIE& leftIE = STATIC_CAST(const CEmsPreDefSoundIE&, left);
				const CEmsPreDefSoundIE& rightIE = STATIC_CAST(const CEmsPreDefSoundIE&, right);

				retVal = (leftIE.PredefinedSound() == rightIE.PredefinedSound());
				break;
				}

			case CSmsInformationElement::ESmsEnhancedUserDefinedSound:
				{
				const CEmsSoundIE& leftIE = STATIC_CAST(const CEmsSoundIE&, left);
				const CEmsSoundIE& rightIE = STATIC_CAST(const CEmsSoundIE&, right);

				// returns TRUE if the melodies are identical - ie if the
				// compare returns 0 and hence the NOT operator
				retVal = !(leftIE.Melody()->Compare(*rightIE.Melody()));
				break;
				}

			case CSmsInformationElement::ESmsEnhancedPredefinedAnimation:
				{
				const CEmsPreDefAnimationIE& leftIE = STATIC_CAST(const CEmsPreDefAnimationIE&, left);
				const CEmsPreDefAnimationIE& rightIE = STATIC_CAST(const CEmsPreDefAnimationIE&, right);

				retVal = (leftIE.Animation() == rightIE.Animation());
				break;
				}

			case CSmsInformationElement::ESmsEnhancedLargePicture:
			case CSmsInformationElement::ESmsEnhancedSmallPicture:
			case CSmsInformationElement::ESmsEnhancedVariablePicture:
				{
				// for pictures, we can only compare the size, not the actual
				// content
				const CEmsPictureIE& leftIE = STATIC_CAST(const CEmsPictureIE&, left);
				const CEmsPictureIE& rightIE = STATIC_CAST(const CEmsPictureIE&, right);

				retVal = (leftIE.SizeInPixels() == rightIE.SizeInPixels());
				break;
				}

			case CSmsInformationElement::ESmsEnhancedUserPromptIndicator:
				{
				// for pictures, we can only compare the size, not the actual
				// content
				const CEmsUserPrompt& leftIE = STATIC_CAST(const CEmsUserPrompt&, left);
				const CEmsUserPrompt& rightIE = STATIC_CAST(const CEmsUserPrompt&, right);

				retVal = (leftIE.ObjectCount() == rightIE.ObjectCount());
				break;
				}

			case CSmsInformationElement::ESmsEnhancedLargeAnimation:
			case CSmsInformationElement::ESmsEnhancedSmallAnimation:
				// for animations, we can't really compare anything
				retVal = ETrue;
				break;

			default:
				// if we come across an unknown ID, we just return false
				retVal = EFalse;
				break;
			}
		}
	return retVal;
	}


EXPORT_C TBool EmsTestUtils::CompareEmsMsgL(const CSmsMessage& left, const CSmsMessage& right)
	{

	TBool retVal = ETrue;

	RSafeEmsArray leftAdjusted, rightAdjusted;
	CleanupClosePushL(leftAdjusted);
	CleanupClosePushL(rightAdjusted);

	// Produce the adjusted ems objects so we can also compare
	// segmented formatting objects
	AdjustedEmsObjectsL(left, leftAdjusted);
	AdjustedEmsObjectsL(right, rightAdjusted);

	// make sure the number of ems objects match
	if (leftAdjusted.Count() != rightAdjusted.Count())
		{
		retVal = EFalse;
		}
	else
		{
		// for each EMS element in left, make sure we can find another
		// element in right which matches completely
		for (int i = 0; i < leftAdjusted.Count(); ++i)
			{
			int j;
			for (j = 0; j < rightAdjusted.Count(); ++j)
				{
				if (CompareEmsIE(*leftAdjusted[i], *rightAdjusted[j]))
					{
					break;
					}
				}


			// if j == right.Count(), this means we went through the entire
			// array without finding a match. Hence we return FALSE
			if (j == rightAdjusted.Count())
				{
				retVal = EFalse;
				break;
				}
			}
		}

	// if EMS objects OK, then compare text as well
	if (retVal)
		{
		HBufC* leftText = ExtractSmsTextLC(left);
		HBufC* rightText = ExtractSmsTextLC(right);

		// do a text compare
		retVal = (TPtr(leftText->Des()) == TPtr(rightText->Des()));

		CleanupStack::PopAndDestroy(rightText);
		CleanupStack::PopAndDestroy(leftText);
		}

	// pop the adjusted arrays
	CleanupStack::PopAndDestroy(&rightAdjusted);
	CleanupStack::PopAndDestroy(&leftAdjusted);

	return retVal;
	}

EXPORT_C const CEmsInformationElement* EmsTestUtils::FindElementL(
		const CSmsMessage& msg, CSmsInformationElement::TSmsInformationElementIdentifier id)
	{
	const RPointerArray<const CEmsInformationElement>& arr = 
		msg.GetEMSInformationElementsL();

	TInt cnt = arr.Count();
	for (TInt i = 0; i < cnt; ++i)
		{
		if (arr[i]->Identifier() == id)
			return arr[i];
		}
	return NULL;
	}

EXPORT_C CEmsPictureIE* EmsTestUtils::CreatePictureL(TInt index)
	{
	// path of the pictures
	_LIT(KPicsMBM,"\\Ems\\pics.mbm");

	// Create the picture object
	CEmsPictureIE* picture = CreateEmsObjFromBitmapL<CEmsPictureIE>(KPicsMBM, index);
	return picture;
	}
	
EXPORT_C CEmsAnimationIE* EmsTestUtils::CreateAnimationL(TInt index)
	{
	// path of the animations
	_LIT(KAnimsMBM,"\\Ems\\anims.mbm");

	CEmsAnimationIE* anim = CreateEmsObjFromBitmapL<CEmsAnimationIE>(KAnimsMBM, index);
	return anim;
	}
			
HBufC* EmsTestUtils::ExtractSmsTextLC(const CSmsMessage& msg)
	{
	const CSmsBufferBase& buffer = msg.Buffer();
	TInt bufLen=buffer.Length();
	HBufC* textBuf=HBufC::NewL(bufLen);
	CleanupStack::PushL(textBuf);
	TPtr textPtr(textBuf->Des());
	buffer.Extract(textPtr,0,bufLen);
	return textBuf;
	}

void EmsTestUtils::AdjustedEmsObjectsL(
		const CSmsMessage& msg, RPointerArray<CEmsInformationElement>& adjusted)
	{
	// This will contain all the formatting objects of the message,
	// but consolidated
	RPointerArray<CEmsInformationElement> formatObjects(10);
	
	const RPointerArray<const CEmsInformationElement>& emsObjects = 
		msg.GetEMSInformationElementsL();

	// now go through the array in the message, and copy the objects
	// if they are not format objects. Format objects go in a separate
	// array for consolidation
	TInt i;	
	for (i = 0; i < emsObjects.Count(); ++i)
		{
		if (emsObjects[i]->Identifier() != CSmsInformationElement::ESmsEnhancedTextFormatting)
			{
			adjusted.Append(emsObjects[i]->DuplicateL());
			}
		else
			{
			// otherwise, go through the format list and see if can be appended
			// to an existing format object
			const CEmsFormatIE* formatIE = STATIC_CAST(const CEmsFormatIE*, emsObjects[i]);
			TInt j;
			for (j = 0; j < formatObjects.Count(); ++j)
				{
				// check if the start position of this element equals the
				// startPos + length of the other format object
				CEmsFormatIE* other = STATIC_CAST(CEmsFormatIE*, formatObjects[j]);
				if (formatIE->StartPosition() == other->StartPosition() + other->FormatLength() &&
					formatIE->Bold() == other->Bold() &&
					formatIE->Italic() == other->Italic() &&
					formatIE->Underline() == other->Underline() &&
					formatIE->Strikethrough() == other->Strikethrough() &&
					formatIE->Alignment() == other->Alignment() &&
					formatIE->FontSize() == other->FontSize())
					{
					// add the length
					other->SetFormatLength(other->FormatLength() + formatIE->FormatLength());
					break;
					}
				}

			// if we have gone to the end without appending to another element,
			// then clone the element and add it to the formatObjects array
			if (j == formatObjects.Count())
				{
				formatObjects.Append(formatIE->DuplicateL());
				}
			}
		}

	// Now, move all elements from the format objects into the adjusted array
	for (i = 0; i < formatObjects.Count(); ++i)
		{
		adjusted.Append(formatObjects[i]);
		}

	// make sure array memory has been deallocated
	formatObjects.Close();
	}

template <class T>	
T* EmsTestUtils::CreateEmsObjFromBitmapL(const TDesC& filename, TInt32 index)
	{
	_LIT(KTxtCDrive,"C:");
	_LIT(KTxtZDrive,"Z:");

	// startup bitmap server
	FbsStartup();
	RFbsSession::Connect();

	// Given a particular filename, assumed to be in C:\ or Z:\, we load
	// a bitmap from the given index. The bitmap is pushed onto the
	// cleanup stack
	CFbsBitmap* bmp;

	TParse mbfn;
	bmp = new (ELeave) CFbsBitmap();
	CleanupStack::PushL(bmp);

	// load the indexed bitmap, first in drive C, then drive Z
	mbfn.Set(filename, &KTxtCDrive, NULL);

	// did it fail? 
	if (bmp->Load(mbfn.FullName(), index))
		{
		// then try drive Z
		mbfn.Set(filename, &KTxtZDrive, NULL);
		User::LeaveIfError(bmp->Load(mbfn.FullName(), index));
		}

 	// if it succeeded, then create the appropriate type
	T* obj = T::NewL(*bmp);
	CleanupStack::PopAndDestroy(bmp);

	RFbsSession::Disconnect();

	return obj;
	}







