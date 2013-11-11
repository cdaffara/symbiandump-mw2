/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  
*
*/


// Includes
#include <e32base.h>
#include <CLFContentListing.hrh>
#include <mdeconstants.h>
#include "CLFConsts.h"
#include "CLFUtils.h"
#include "MGDebugPrint.h"

// ---------------------------------------------------------------------------
// Maps CLF mediatype to MdE object class
// ---------------------------------------------------------------------------
//
const TDesC& CLFUtils::MapClfType( const TInt aType )
    {
    const TDesC* ret = &KNullDesC;

    switch ( aType )
        {
        /// Music media type
        case ECLFMediaTypeMusic:
            {
            ret = &MdeConstants::Audio::KAudioObject;
            break;
            }
        /// Sound media type
        case ECLFMediaTypeSound:
            {
            ret = &MdeConstants::Audio::KAudioObject;
            break;
            }
        /// Image media type
        case ECLFMediaTypeImage:
            {
            ret = &MdeConstants::Image::KImageObject;
            break;
            }
        /// Video media type
        case ECLFMediaTypeVideo:
            {
            ret = &MdeConstants::Video::KVideoObject;
            break;
            }
        /// Map ECLFMediaTypeUnknown, ECLFMediaTypeStreamingURL, ECLFMediaTypePlaylist and
        /// ECLFMediaTypePresentations to KBaseObject.
        case ECLFMediaTypeUnknown:
        case ECLFMediaTypeStreamingURL:
        case ECLFMediaTypePlaylist:
        case ECLFMediaTypePresentations:
        default:
            {
            ret = &MdeConstants::Object::KBaseObject;
            break;
            }
        }
    return *ret;
    }

//  End of File
