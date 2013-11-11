/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  CSendUiAttachment.inl*
*/



// -----------------------------------------------------------------------------
// Path
// -----------------------------------------------------------------------------
//
inline const TDesC16* CSendUiAttachment::Path()
    {
    return iPath;
    }

// -----------------------------------------------------------------------------
// Handle
// -----------------------------------------------------------------------------
//
inline const RFile* CSendUiAttachment::Handle()
    {
    return iHandle;
    }

// -----------------------------------------------------------------------------
// SetMimeType
// -----------------------------------------------------------------------------
//
inline void CSendUiAttachment::SetMimeType( TDataType aMimeType )
    {
    iMimeType = aMimeType;
    }

// -----------------------------------------------------------------------------
// MimeType
// -----------------------------------------------------------------------------
//
inline const TPtrC8 CSendUiAttachment::MimeType()
    {
    return iMimeType.Des8();
    }

// -----------------------------------------------------------------------------
// Size
// -----------------------------------------------------------------------------
//
inline TInt CSendUiAttachment::Size()
    {
    return iSize;
    }

// -----------------------------------------------------------------------------
// Flag
// -----------------------------------------------------------------------------
//
inline TBool CSendUiAttachment::Flag( TInt aFlag )
    {
    return iFlags && aFlag;
    }

// -----------------------------------------------------------------------------
// SetFlags
// -----------------------------------------------------------------------------
//
inline void CSendUiAttachment::SetFlags( TInt aFlag )
    {
    iFlags |= aFlag;
    }

