/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Recognizer for 3GP files.
*
*/


// INCLUDES
#include    <f32file.h>
#include    <barsread.h>
#include    <mp4lib.h>
#include    <implementationproxy.h>
#include    "3GPAudioPlayControllerRecognizer.h"
#include    "DebugMacros.h"

// LOCAL CONSTANTS AND MACROS
const TUid KUidMime3gpRecognizer = {0x101FAFAD};
const TInt KExtLength = 4;
_LIT(KDot, ".");
_LIT(KDot3gp, ".3gp");
_LIT(KDot3ga, ".3ga");
_LIT(KDot3gpp, ".3gpp");
_LIT8(K3gpAudioMimeType, "audio/3gpp");
_LIT8(K3gpVideoMimeType, "video/3gpp");

_LIT(KDotMp4, ".mp4");
_LIT(KDotM4a, ".m4a");
_LIT8(KMp4AudioMimeType, "audio/mp4");
_LIT8(KMp4VideoMimeType, "video/mp4");

_LIT(KDot3g2, ".3g2");
_LIT8(K3g2AudioMimeType, "audio/3gpp2");
_LIT8(K3g2VideoMimeType, "video/3gpp2");

// Needed for looking at headers for 3gp files w/ no extensions
const TInt KPreHeaderLength = 4;
const TInt KHeaderLen = 8;

_LIT8(K3gpHeader ,"ftyp3gp");
_LIT8(KMp4Header ,"ftypmp4");
_LIT8(KMmp4Header ,"ftypmmp4");
_LIT8(KM4aHeader, "ftypM4A");
_LIT8(K3g2Header, "ftyp3g2");
_LIT8(K3gp6Header, "ftyp3gp6"); // 3GPP Release 6 basic Profile
_LIT8(K3gr6Header, "ftyp3gr6"); // 3GPP Rel6 Progressive Download ProfileHeader
_LIT8(K3gs6Header, "ftyp3gs6"); // 3GPP Release 6 streaming-server Profile

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerRecognizer::C3GPAudioPlayControllerRecognizer()
// Call base constructor with the recognizer's UID and confidence level
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
C3GPAudioPlayControllerRecognizer::C3GPAudioPlayControllerRecognizer()
    :CApaDataRecognizerType(KUidMime3gpRecognizer, CApaDataRecognizerType::EHigh)
    {
    // Set the number of data (MIME) types supported by this recognizer.
    iCountDataTypes = 6;
    }

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerRecognizer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void C3GPAudioPlayControllerRecognizer::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerRecognizer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
C3GPAudioPlayControllerRecognizer* C3GPAudioPlayControllerRecognizer::NewL()
    {
    C3GPAudioPlayControllerRecognizer* self = new(ELeave) C3GPAudioPlayControllerRecognizer();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
C3GPAudioPlayControllerRecognizer::~C3GPAudioPlayControllerRecognizer()
    {
    }

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerRecognizer::SupportedDataTypeL
// Return the supposed datatype depending on the
// MimeType of the file
// -----------------------------------------------------------------------------
//
TDataType C3GPAudioPlayControllerRecognizer::SupportedDataTypeL(
        TInt aIndex) const
    {
    DP1(_L("C3GPAudioPlayControllerRecognizer::SupportedDataTypeL [%d]"), 
            aIndex);
    TDataType dataType;
    switch(aIndex)
        {
        case 0:
            dataType = TDataType(K3gpAudioMimeType);
            break;
        case 1:
            dataType = TDataType(K3gpVideoMimeType);
            break;
        case 2:
            dataType = TDataType(KMp4AudioMimeType);
            break;
        case 3:
            dataType = TDataType(KMp4VideoMimeType);
            break;
        case 4:
            dataType = TDataType(K3g2AudioMimeType);
            break;
        case 5:
            dataType = TDataType(K3g2VideoMimeType);
            break;
        default:
            User::Leave(KErrArgument);
            break;
        }
    return dataType;
    }

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerRecognizer::PreferredBufSize()
// Return the supposed minimum buffer size we need to
// successfully recognize the data
// -----------------------------------------------------------------------------
//
TUint C3GPAudioPlayControllerRecognizer::PreferredBufSize()
    {
    return 0;
    }

// -----------------------------------------------------------------------------
// C3GPAudioPlayControllerRecognizer::DoRecognizeL
// Attempt to recognize the data
// this recognizer only attempts to match the data and/or file suffix
//
// NB if the file is not recognized, this function should NOT leave :
// it should instead set iConfidence = ENotRecognized and return
// the function should only leave if there is an out-of-memory condition
// -----------------------------------------------------------------------------
//
void C3GPAudioPlayControllerRecognizer::DoRecognizeL(
        const TDesC& aName,
        const TDesC8& aBuffer)
    {
    DP0(_L("C3GPAudioPlayControllerRecognizer::DoRecognizeL"));
#ifdef _DEBUG
    RDebug::RawPrint(aName);
#endif
    DP0(_L(" "));
    TPtrC8 header;
    TInt headerMatch = 0;

    // assume Match will fail :
    iConfidence = CApaDataRecognizerType::ENotRecognized;
    if (aName.Find(KDot) == KErrNotFound) // No extension
        {
        DP0(_L("C3GPAudioPlayControllerRecognizer::DoRecognizeL - Ext not present"));

        if (aBuffer.Length() < (KPreHeaderLength+KHeaderLen))
            {
            return;
            }
        else
            {
            // Check headers for 3gp (allowed to play w/out ext):
            header.Set(aBuffer.Mid(KPreHeaderLength, KHeaderLen));
            headerMatch = ( (header.FindF(K3gpHeader)  == 0) ||
                            (header.FindF(KMp4Header)  == 0) ||
                            (header.FindF(KMmp4Header) == 0) ||
                            (header.FindF(KM4aHeader)  == 0) ||
                            (header.FindF(K3g2Header)  == 0) ||
                            (header.FindF(K3gp6Header) == 0) ||
                            (header.FindF(K3gr6Header) == 0) ||
                            (header.FindF(K3gs6Header) == 0) );

            DP1(_L("C3GPAudioPlayControllerRecognizer::DoRecognizeL: headerMatch = %d"), headerMatch);
            if (!headerMatch)
                {
                return;
                }
            }
        }

   // TPtrC ext(aName.Right(KExtLength));
   TInt num=aName.LocateReverse('.');
   if(num < 0)
   {
	num = 0;
   }
    TPtrC ext(aName.Right(aName.Length()-num));

    DP0(_L("C3GPAudioPlayControllerRecognizer::DoRecognizeL: Extension:"));
#ifdef _DEBUG
    RDebug::RawPrint(ext);
#endif
    DP0(_L(" "));

    if ( (ext.CompareF(KDot3gp) == 0) || (ext.CompareF(KDotMp4) == 0) ||
         (ext.CompareF(KDotM4a) == 0) || (ext.CompareF(KDot3g2) == 0) || 
         (ext.CompareF(KDot3ga) == 0) || (ext.CompareF(KDot3gpp) == 0) || headerMatch)
        {
        RFile* fileHandle= NULL;
        TRAPD(handleErr, fileHandle = FilePassedByHandleL());

        if (handleErr != KErrNone)
            {
            DP1(_L("C3GPAudioPlayControllerRecognizer::DoRecognizeL - Can't get FH [%d]"), handleErr);
            return;
            }

        MP4Handle mp4Handle;    // 3GP library handle
        MP4Err err;
        // Check if file handle exists first. If not, try to open with file name.
        if (fileHandle)
            {
            err = MP4ParseOpenFileHandle(&mp4Handle, fileHandle);
            DP1(_L("C3GPAudioPlayControllerRecognizer::DoRecognizeL FileHandle, err[%d]"), err);
            }
        else
            { // file handle not present
            RBuf buf;
            TUint nameSize = aName.Length() + 1; // Add 1 extra for "zero" termination
            buf.CreateL(nameSize); // Create the RBuf 
            buf = aName; // Copy "aName" using the assignment operation
            err = MP4ParseOpen(&mp4Handle, (MP4FileName) buf.PtrZ());  
            buf.Close(); // Close Buf to free the memory
            DP1(_L("C3GPAudioPlayControllerRecognizer::DoRecognizeL FileName, err[%d]"), err);
            if (err != MP4_OK)
                { // cannot find the file by name but the header content or extension was ok
                err = MP4ParseOpen(&mp4Handle, NULL);
                DP1(_L("C3GPAudioPlayControllerRecognizer::DoRecognizeL databuf, err[%d]"), err);
                if (err == MP4_OK)
                    {
                    TUint8* p = (TUint8*)(aBuffer.Ptr());
                    TInt l = aBuffer.Length();
                    DP2(_L("C3GPAudioPlayControllerRecognizer::DoRecognizeL databuf *p[0x%x] l[%d]"), p[0], l);
                    err = MP4ParseWriteData(mp4Handle, p, l);
                    DP1(_L("C3GPAudioPlayControllerRecognizer::DoRecognizeL databuf write, err[%d]"), err);
                    }
                }
            }

        if (err == MP4_OK)
            {
            mp4_u32 videoLength, videoType, videoWidth, videoHeight, timeScale;
            mp4_double frameRate;

            err = MP4ParseRequestVideoDescription(mp4Handle, &videoLength, &frameRate,
                    &videoType, &videoWidth, &videoHeight, &timeScale);
            if (err == MP4_NO_VIDEO)
                {
                mp4_u32 audioLength, audioType, timeScale, averateBitRate;
                mp4_u8 framesPerSample;

                err = MP4ParseRequestAudioDescription(mp4Handle, &audioLength, &audioType,
                        &framesPerSample, &timeScale, &averateBitRate);

                if (err == MP4_OK)
                    {
                    if ( (header.FindF(K3gpHeader)  == 0) ||
                         (header.FindF(K3gp6Header) == 0) ||
                         (header.FindF(K3gr6Header) == 0) ||
                         (header.FindF(K3gs6Header) == 0) ||
                         (header.FindF(KMmp4Header) == 0) ||
                         (ext.CompareF(KDot3gp)     == 0) ||
                         (ext.CompareF(KDot3ga)     == 0)||(ext.CompareF(KDot3gpp) == 0) )
                        {
                        iDataType = TDataType(K3gpAudioMimeType);
                        iConfidence = ECertain;
                        DP0(_L("C3GPAudioPlayControllerRecognizer::DoRecognizeL [3gp audio]"));
                        }
                    else if ( (header.FindF(KM4aHeader) == 0) ||
                              (ext.CompareF(KDotM4a)    == 0) ||
                              (header.FindF(KMp4Header) == 0) ||
                              (ext.CompareF(KDotMp4)    == 0) )
                        {
                        iDataType = TDataType(KMp4AudioMimeType);
                        iConfidence = ECertain;
                        DP0(_L("C3GPAudioPlayControllerRecognizer::DoRecognizeL [Mp4 audio]"));
                        }
                    else if ( (header.FindF(K3g2Header) == 0) ||
                              (ext.CompareF(KDot3g2)    == 0) )
                        {
                        iDataType = TDataType(K3g2AudioMimeType);
                        iConfidence = ECertain;
                        DP0(_L("C3GPAudioPlayControllerRecognizer::DoRecognizeL [3g2 audio]"));
                        }
                    } // End if (err == MP4_OK)
                } // End If (err == MP4_NO_VIDEO) i.e. Audio
            else if (err == MP4_OK)
                {
                if ( (header.FindF(K3gpHeader)  == 0) ||
                     (header.FindF(K3gp6Header) == 0) ||
                     (header.FindF(K3gr6Header) == 0) ||
                     (header.FindF(K3gs6Header) == 0) ||
                     (header.FindF(KMmp4Header) == 0) ||
                     (ext.CompareF(KDot3gp)     == 0) || (ext.CompareF(KDot3gpp) == 0))
                    {
                    iDataType = TDataType(K3gpVideoMimeType);
                    iConfidence = ECertain;
                    DP0(_L("C3GPAudioPlayControllerRecognizer::DoRecognizeL [3gp video]"));
                    }
                else if ( (header.FindF(KMp4Header) == 0) ||
                          (ext.CompareF(KDotMp4)    == 0) )
                    {
                    iDataType = TDataType(KMp4VideoMimeType);
                    iConfidence = ECertain;
                    DP0(_L("C3GPAudioPlayControllerRecognizer::DoRecognizeL [Mp4 video]"));
                    }
                else if ( (header.FindF(K3g2Header) == 0) ||
                          (ext.CompareF(KDot3g2)    == 0) )
                    {
                    iDataType = TDataType(K3g2VideoMimeType);
                    iConfidence = ECertain;
                    DP0(_L("C3GPAudioPlayControllerRecognizer::DoRecognizeL [3g2 video]"));
                    }
                }// end of else if (err == MP4_OK)
            }// End FileHandle or FileName Open OK
        if (mp4Handle)
            {
            MP4ParseClose(mp4Handle);
            }
        } // End of Extension Comparision If statement
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// CApaDataRecognizerType* CreateRecognizerL()
// Used to create an instance of a CApaDataRecognizerType-derived class
// -----------------------------------------------------------------------------
//
CApaDataRecognizerType* C3GPAudioPlayControllerRecognizer::CreateRecognizerL()
    {
	return new(ELeave) C3GPAudioPlayControllerRecognizer();
	}

const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(0x101FAFD6, C3GPAudioPlayControllerRecognizer::CreateRecognizerL)
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }

// End of file
