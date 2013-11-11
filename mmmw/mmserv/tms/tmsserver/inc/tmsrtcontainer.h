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

#ifndef TMSRTCONTAINER_H
#define TMSRTCONTAINER_H

//  INCLUDES
#include <e32base.h>

namespace TMS {

// CLASS DECLARATION

/**
 *  Ringtone wrapper for file name. In addtion, provides methods for ringtone
 *  type identification.
 */
class TMSRtContainer : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     * @param aFileName Ringing tone file name with path.
     * @param aDrmInPlayback ETrue to use IsFileDrmProtected().
     */
    static TMSRtContainer* NewL(const TDesC& aFileName,
            TBool aDrmInPlayback = EFalse);

    /**
     * Destructor.
     */
    virtual ~TMSRtContainer();

public:
    /**
     * Sets the file name including path.
     * @param aFileName file name plus path.
     */
    void SetFileName(const TDesC& aFileName);

    /**
     * Returns file name including path.
     * @return File name.
     */
    const TDesC& FileName() const;

    /**
     * Returns MIME type.
     * @return MIME type.
     */
    const TDesC& MimeType() const;

    /**
     * Checks if video ringing tone.
     * @return ETrue if video ringing tone.
     */
    TBool IsVideoRingingTone();

    /**
     * Checks if the file is a DCF file.
     * @return ETrue if files is DRM protected.
     */
    TBool IsFileDrmProtected() const;

    /**
     * Checks if the file is in ROM.
     */
    TBool IsFileInRom() const;

    /**
     * Refresh MIME type. This must be called
     * before using MimeType().
     */
    TInt RefreshMime();

private:

    /**
     * Refresh MIME type.
     */
    void RefreshMimeL();

    /**
     * Checks if file is located in video directory.
     */
    TBool IsFileInVideoDirectory() const;

private:

    /**
     * C++ default constructor.
     */
    TMSRtContainer(TBool aDrmInPlayback);

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL(const TDesC& aFileName);

private:
    // Data

    // File name with path
    HBufC* iFileName;

    // MIME type
    HBufC* iMimeType;

    // Extended security
    TBool iDrmInPlayback;
    };

} //namespace TMS {

#endif // TMSRTCONTAINER_H

// End of File
