/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Audio Play Controller Recognizer
*
*/


#include	<f32file.h> // for RDebug
#include   	<implementationproxy.h>
#include	"HXAudioPlayControllerRecognizer.h"

#ifdef RD_RA_SUPPORT_FOR_MUSIC_PLAYER
#include    <hxmetadatakeys.h>
#include    <hxmetadatautil.h>
_LIT(KRAVideoType, "video/"); //Moved inside Flag for build warining
#endif

const TUid KUidMimeRARecognizer = {0x10207B62};

_LIT8(KRAMimeType, "audio/x-pn-realaudio");
_LIT(KRAMimeType1, "audio/x-pn-realaudio");
_LIT(KRAMimeType2, "audio/x-realaudio");
_LIT(KRAMimeType3, "audio/vnd.rn-realaudio");


_LIT(KASFMimeType1, "audio/x-hx-wma");
_LIT(KASFMimeType2, "video/x-hx-wmv");

_LIT8(KASFAudioMimeType, "audio/x-ms-wma");
_LIT8(KASFVideoMimeType, "video/x-ms-wmv");

_LIT(KDotASF, ".asf");
_LIT(KDotWMA, ".wma");
_LIT(KDotWMV, ".wmv");
_LIT8(KRMFileHeader, ".RMF");

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CHXAudioPlayControllerRecognizer::CHXAudioPlayControllerRecognizer()
// C++ default constructor can NOT contain any code, that
// might leave.
// Call base constructor with the recognizer's UID and confidence level
// -----------------------------------------------------------------------------
//
CHXAudioPlayControllerRecognizer::CHXAudioPlayControllerRecognizer()
	:	CApaDataRecognizerType(KUidMimeRARecognizer, CApaDataRecognizerType::EHigh)
	{
	// Set the number of data (MIME) types supported by this recognizer.
	iCountDataTypes = 1;
	}

// -----------------------------------------------------------------------------
// CHXAudioPlayControllerRecognizer::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------

void CHXAudioPlayControllerRecognizer::ConstructL()
	{
	}

// -----------------------------------------------------------------------------
// CHXAudioPlayControllerRecognizer::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
CHXAudioPlayControllerRecognizer* CHXAudioPlayControllerRecognizer::NewL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("CHXAudioPlayControllerRecognizer::NewL"));
#endif
	CHXAudioPlayControllerRecognizer* self = new (ELeave) CHXAudioPlayControllerRecognizer();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// Destructor
CHXAudioPlayControllerRecognizer::~CHXAudioPlayControllerRecognizer()
	{
	}

// -----------------------------------------------------------------------------
// CHXAudioPlayControllerRecognizer::SupportedDataTypeL
// Return the MimeType of the DataType depending on
// mime type of the file
// -----------------------------------------------------------------------------

TDataType CHXAudioPlayControllerRecognizer::SupportedDataTypeL(
	TInt aIndex ) const
	{
#ifdef _DEBUG
	RDebug::Print(_L("CHXAudioPlayControllerRecognizer::SupportedDataTypeL [%d]"), aIndex);
#endif
	TDataType dataType;
	switch ( aIndex )
		{
		case 0:
			dataType = TDataType(KRAMimeType);
			break;
		default:
			User::Leave(KErrArgument);
			break;
		}
	return dataType;
	}

// -----------------------------------------------------------------------------
// CHXAudioPlayControllerRecognizer::PreferredBufSize()
// Return the supposed minimum buffer size we need to
// successfully recognize the data
// -----------------------------------------------------------------------------

TUint CHXAudioPlayControllerRecognizer::PreferredBufSize()
	{
	return 0;
	}

// -----------------------------------------------------------------------------
// CHXAudioPlayControllerRecognizer::DoRecognizeL
// Attempt to recognize the data
// this recognizer only attempts to match the data and/or file suffix
//
// NB if the file is not recognized, this function should NOT leave :
// it should instead set iConfidence = ENotRecognized and return
// the function should only leave if there is an out-of-memory condition
// -----------------------------------------------------------------------------

void CHXAudioPlayControllerRecognizer::DoRecognizeL(
	const TDesC& aName,
	const TDesC8& aBuffer )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CHXAudioPlayControllerRecognizer::DoRecognizeL: "));
	RDebug::RawPrint(aName);
	RDebug::Print(_L(" "));
#endif
	// assume match will fail
	iConfidence = CApaDataRecognizerType::ENotRecognized;

    // Checks for only RA files with .RMF header and files with .ASF extension

    if (( aBuffer.Length() >= KRMFileHeader().Length()
        && (aBuffer.Left( KRMFileHeader().Length()).CompareF(KRMFileHeader()) == 0 ))
        || aName.Right( KDotASF.iTypeLength ).CompareF(KDotASF) == 0
        || aName.Right( KDotWMV.iTypeLength ).CompareF(KDotWMV) == 0
        || aName.Right( KDotWMA.iTypeLength ).CompareF(KDotWMA) == 0)
        {
#ifdef RD_RA_SUPPORT_FOR_MUSIC_PLAYER

        CHXMetaDataUtility *putil;
        putil = CHXMetaDataUtility::NewL();
        CleanupStack::PushL(putil);
        RFile *pHandle = NULL;
        TRAPD(err, pHandle = FilePassedByHandleL());
        if(pHandle)
            {
            TRAP(err,putil->OpenFileL((RFile &)*pHandle));
            }
        else
            {
            TRAP(err,putil->OpenFileL((TDesC &)aName));
            }

        if(err != KErrNone)
        	{
	        CleanupStack::Pop(putil);
	        putil->ResetL();
	        delete putil;
			return;
        	}

        TUint count = 0;
        putil->GetMetaDataCount(count);

        TUint i;
        HXMetaDataKeys::EHXMetaDataId  id;

        for (i=0; i<count; i++)
            {
            HBufC* pDes = NULL;

            putil->GetMetaDataAt(i, id, pDes);

            if(id == HXMetaDataKeys::EHXMimeType)
                {

                TPtr des = pDes->Des();
                TInt res = des.Find(KRAVideoType());
                if(res == KErrNotFound)
                    {
                    if(IsRealMimeTypeSupported(*pDes))
                        {
                        iDataType = TDataType(KRAMimeType);
                        iConfidence = ECertain;
                        }
                    else if(IsASFTypeSupported(*pDes))
                        {
                        iDataType = TDataType(KASFAudioMimeType);
                        iConfidence = ECertain;
                        }
                    }
                else
                    {
                    if(IsASFTypeSupported(*pDes))
                        {
                        iDataType = TDataType(KASFVideoMimeType);
                        iConfidence = ECertain;
                        }
                    else
                        {
                        iDataType = TDataType(KNullUid);
                        iConfidence = CApaDataRecognizerType::ENotRecognized;
                        }
                    break;
                    }
                }

            }

        CleanupStack::Pop(putil);

        putil->ResetL();
        delete putil;

#endif // RD_RA_SUPPORT_FOR_MUSIC_PLAYER
        }
  	}

TBool CHXAudioPlayControllerRecognizer::IsRealMimeTypeSupported(const TDesC& aMimeType)
    {
    TBool match = false;

    if(!aMimeType.Compare(KRAMimeType1()))
        {
            match = true;
            return match;
        }
    if(!aMimeType.Compare(KRAMimeType2()))
        {
            match = true;
            return match;
        }
    if(!aMimeType.Compare(KRAMimeType3()))
        {
            match = true;
            return match;
        }

    return match;
    }


TBool CHXAudioPlayControllerRecognizer::IsASFTypeSupported(const TDesC& aMimeType)
    {
    TBool match = false;

    if((aMimeType.Compare(KASFMimeType1()) == 0) || (aMimeType.Compare(KASFMimeType2()) == 0))
        {
        match = true;
        return match;
        }
    return match;
    }


// -----------------------------------------------------------------------------
// CApaDataRecognizerType* CreateRecognizerL()
// Used to create an instance of a CApaDataRecognizerType-derived class
// -----------------------------------------------------------------------------
CApaDataRecognizerType* CHXAudioPlayControllerRecognizer::CreateRecognizerL()
	{
	return new (ELeave) CHXAudioPlayControllerRecognizer ();
	}

const TImplementationProxy ImplementationTable[] =
	{
	IMPLEMENTATION_PROXY_ENTRY(0x10207B63, CHXAudioPlayControllerRecognizer::CreateRecognizerL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }


// End of file
