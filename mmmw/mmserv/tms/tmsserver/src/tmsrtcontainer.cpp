/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Class representing ringing tone
 *
 */

// INCLUDE FILES
#include <apgcli.h>
#include <DRMHelper.h>
#include <bldvariant.hrh>
#include "tmsrtcontainer.h"

using namespace TMS;

// CONSTANTS
// Rich audio file MIME types
_LIT(KAac, "audio/aac");
_LIT(KMp3, "audio/mp3");
_LIT(KMpeg, "audio/mpeg");
_LIT(K3gpp, "audio/3gpp");
_LIT(KMp4, "audio/mp4");
_LIT(KAmrWb, "audio/amr-wb");
_LIT(KWavX, "audio/x-wav");
_LIT(KWav, "audio/wav");
// Rich video file MIME types
_LIT(KV3gpp, "video/3gpp");
_LIT(KVMp4, "video/mp4");
_LIT(KV3gpp2, "video/3gpp2");
// Used in CPhoneRingingTone
_LIT(KPhoneRingingToneDriveZ, "z:");
#ifdef RD_VIDEO_AS_RINGING_TONE
_LIT(KPhoneRingingToneVideoMime, "video/*");
_LIT(KPhoneRingingToneRealVideoMime, "*realmedia");
#endif

// -----------------------------------------------------------------------------
// TMSRtContainer::TMSRtContainer
// C++ default constructor
// -----------------------------------------------------------------------------
//
TMSRtContainer::TMSRtContainer(TBool aDrmInPlayback) :
    iDrmInPlayback(aDrmInPlayback)
    {
    }

// -----------------------------------------------------------------------------
// TMSRtContainer::ConstructL
// Symbian 2nd phase constructor
// -----------------------------------------------------------------------------
//
void TMSRtContainer::ConstructL(const TDesC& aFileName)
    {
    iFileName = aFileName.AllocL();
    }

// -----------------------------------------------------------------------------
// TMSRtContainer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
TMSRtContainer* TMSRtContainer::NewL(const TDesC& aFileName,
        TBool aDrmInPlayback)
    {
    TMSRtContainer* self = new (ELeave) TMSRtContainer(aDrmInPlayback);
    CleanupStack::PushL(self);
    self->ConstructL(aFileName);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
TMSRtContainer::~TMSRtContainer()
    {
    delete iFileName;
    delete iMimeType;
    }

// -----------------------------------------------------------------------------
// TMSRtContainer::SetFileName
// -----------------------------------------------------------------------------
//
void TMSRtContainer::SetFileName(const TDesC& aFileName)
    {
    delete iFileName;
    iFileName = aFileName.Alloc();
    }

// -----------------------------------------------------------------------------
// TMSRtContainer::FileName
// -----------------------------------------------------------------------------
//
const TDesC& TMSRtContainer::FileName() const
    {
    // iFileName is never NULL
    return *iFileName;
    }

// -----------------------------------------------------------------------------
// TMSRtContainer::MimeType
// -----------------------------------------------------------------------------
//
const TDesC& TMSRtContainer::MimeType() const
    {
    if (iMimeType)
        {
        return *iMimeType;
        }
    else
        {
        return KNullDesC;
        }
    }

// -----------------------------------------------------------------------------
// TMSRtContainer::IsVideoRingingTone
// -----------------------------------------------------------------------------
//
TBool TMSRtContainer::IsVideoRingingTone()
    {
#ifdef RD_VIDEO_AS_RINGING_TONE
    if (RefreshMime() != KErrNone)
        {
        // try to handle as audio
        return EFalse;
        }

    TBool isVideo(EFalse);

    if (iMimeType && iMimeType->MatchF(KPhoneRingingToneVideoMime)
            != KErrNotFound)
        {
        isVideo = ETrue;
        }
    else if (iMimeType && iMimeType->MatchF(KPhoneRingingToneRealVideoMime)
            != KErrNotFound)
        {
        isVideo = ETrue;
        }

    if (isVideo)
        {
        if (IsFileInRom() && !IsFileInVideoDirectory())
            {
            // For ROM files check also location, because
            // MIME check is not fully reliable.
            isVideo = EFalse;
            }
        }

    return isVideo;

#else
    // if extended security -> refresh MIME
    if (iDrmInPlayback)
        {
        RefreshMime();
        }

    return EFalse;
#endif
    }

// -----------------------------------------------------------------------------
// TMSRtContainer::IsFileDrmProtected
// -----------------------------------------------------------------------------
//
TBool TMSRtContainer::IsFileDrmProtected() const
    {
    const TDesC& type = MimeType();

    if (type == KAac || type == KMp3 || type == KMpeg || type == K3gpp ||
            type == KMp4 || type == KAmrWb || type == KWavX || type == KWav ||
            type == KV3gpp || type == KVMp4 || type == KV3gpp2)
        {
        ContentAccess::CContent* content = NULL;
        TRAPD(err, content = ContentAccess::CContent::NewL(*iFileName));
        if (err == KErrNone && content)
            {
            TInt drmProtected(0);
            content->GetAttribute(ContentAccess::EIsProtected, drmProtected);
            delete content;
            return drmProtected;
            }
        }

    return ETrue; // Other MIMEs can be played without DRM check.
    }

// -----------------------------------------------------------------------------
// TMSRtContainer::IsFileInRom
// -----------------------------------------------------------------------------
//
TBool TMSRtContainer::IsFileInRom() const
    {
    TParsePtrC parsedName(*iFileName);

    // Files on ROM don't need to be checked for DRM
    if (parsedName.Drive().CompareF(KPhoneRingingToneDriveZ) == 0)
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// TMSRtContainer::IsFileInVideoDirectory
// -----------------------------------------------------------------------------
//
TBool TMSRtContainer::IsFileInVideoDirectory() const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// TMSRtContainer::RefreshMime
// -----------------------------------------------------------------------------
//
TInt TMSRtContainer::RefreshMime()
    {
    TRAPD(err, RefreshMimeL());
    return err;
    }

// -----------------------------------------------------------------------------
// TMSRtContainer::RefreshMimeL
// -----------------------------------------------------------------------------
//
void TMSRtContainer::RefreshMimeL()
    {
    RApaLsSession apaLsSession;
    User::LeaveIfError(apaLsSession.Connect());
    CleanupClosePushL(apaLsSession);
    TUid dummyUid = {0};
    TDataType dataType(dummyUid);
    User::LeaveIfError(apaLsSession.AppForDocument(*iFileName, dummyUid,
            dataType));
    CleanupStack::PopAndDestroy(); // CleanupClosePushL
    delete iMimeType;
    iMimeType = NULL;
    iMimeType = dataType.Des().AllocL();
    }

//  End of File
