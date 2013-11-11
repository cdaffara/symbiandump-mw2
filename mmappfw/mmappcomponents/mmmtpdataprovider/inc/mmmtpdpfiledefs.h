/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Definition for MTP File, helper for Mimetype and extension
*
*/


#ifndef MMMTPDPFILEDEFS_H
#define MMMTPDPFILEDEFS_H

// constants
const TInt KMimeTypeMaxLength = 76; // according to RFC 2045 spec

_LIT( KTxtBackSlash, "\\" );

//--------------------------------------------------------------------------------------------
// extention
//--------------------------------------------------------------------------------------------

_LIT( KTxtExtension3GP, ".3gp" );
_LIT( KTxtExtension3G2, ".3g2" );
_LIT( KTxtExtensionAAC, ".aac" );
_LIT( KTxtExtensionASF, ".asf" );
_LIT( KTxtExtensionM4A, ".m4a" );
_LIT( KTxtExtensionMP3, ".mp3" );
_LIT( KTxtExtensionMP4, ".mp4" );
_LIT( KTxtExtensionWAV, ".wav" );
_LIT( KTxtExtensionWMA, ".wma" );
_LIT( KTxtExtensionWMV, ".wmv" );
_LIT( KTxtExtensionO4A, ".o4a" );
_LIT( KTxtExtensionO4V, ".o4v" );
_LIT( KTxtExtensionODF, ".odf" );

_LIT( KTxtExtensionALB, ".alb" );
_LIT( KTxtExtensionM3U, ".m3u" );
_LIT( KTxtExtensionPLA, ".pla" );
_LIT( KTxtExtensionVIR, ".vir" ); // virtual playlist from MPX

_LIT(KFormatExtensionMP3, "0x3009:mp3::3");
_LIT(KFormatExtensionWMA, "0xb901:wma::3");
_LIT(KFormatExtensionMP4, "0xb982:mp4::3");
_LIT(KFormatExtension3GP, "0xb984:3gp::3");
_LIT(KFormatExtension3G2, "0xb984:3g2::3");
_LIT(KFormatExtensionAAC, "0xb903:aac::3");
_LIT(KFormatExtensionWAV, "0x3008:wav::3");
_LIT(KFormatExtensionWMV, "0xb981:wmv::3");
_LIT(KFormatExtensionASF, "0x300c:asf::3");

//ODF container
_LIT(KFormatExtensionODFAudio3GPP, "0xB984:ODF:audio/3gpp:3");
_LIT(KFormatExtensionODFVideo3GPP, "0xB984:ODF:video/3gpp:3");

_LIT(KFormatExtensionM4A, "0xB982:m4a::3");
_LIT(KFormatExtensionO4A, "0xB984:o4a::3");
_LIT(KFormatExtensionO4V, "0xB984:o4v::3");

//Abstract Album
_LIT(KFormatExtensionALB, "0xBA03:alb::3");

//Playlists
_LIT(KFormatExtensionM3U, "0xba11:m3u::3");
_LIT(KFormatExtensionPLA, "0xBA05:pla");
_LIT(KFormatExtensionVIR, "0xBA05:vir"); //abstract audio & video playlist 0xBA05
//--------------------------------------------------------------------------------------------
// end extention
//--------------------------------------------------------------------------------------------

// file standard definition like extension
_LIT8( KMimeTypeAudio3gpp, "audio/3gpp" );
_LIT8( KMimeTypeVideo3gpp, "video/3gpp" );
_LIT8( KMimeTypeAudioMp4, "audio/mp4" );
_LIT8( KMimeTypeVideoMp4, "video/mp4" );
_LIT8( KMimeTypeAudioWm, "audio/x-ms-wma" );
_LIT8( KMimeTypeVideoWm, "video/x-ms-wmv" );

// generic DRM status
enum TMmMtpDrmStatus
    {
    EMTPDrmStatusNotProtected,
    EMTPDrmStatusProtected,
    EMTPDrmStatusUnknown
    };

// generic audio vs video vs unknown
enum TMmMtpSubFormatCode
    {
    EMTPSubFormatCodeUnknown,
    EMTPSubFormatCodeAudio,
    EMTPSubFormatCodeVideo,
    EMTPSubFormatCodeUndefined
    };

#endif // MMMTPDPFILEDEFS_H
