// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __UNKNOWNMIMEDEFS_H__
#define __UNKNOWNMIMEDEFS_H__

/** @file
@publishedPartner
@released
*/

/** generic filename for saving the Unknown Push Msg data */
_LIT(KTempUnknownFileName, "TempData");


/** Mime Types */

//text types
_LIT(KTextBasic, "text/*");						// ".txt"
_LIT(KTextPlain, "text/plain");					// ".txt"
_LIT(KTextRich, "text/rtf");					// ".rtf"
_LIT(KTextXML, "text/xml");						// ".xml"
_LIT(KTextHTML, "text/html");					// ".html"
_LIT(KTextWML, "text/vnd.wap.wml");				// ".wml"
_LIT(KTextWMLScript,"text/vnd.wap.wmlscript");	// ".wmls"  ?
_LIT(KTextMSWord,"application/msword");			// ".doc"  
_LIT(KTextMSWord95,"application/msword95");		// ".doc"  
_LIT(KTextMSWord97,"application/msword97");		// ".doc"  
_LIT(KTextMSWord00,"application/msword00");		// ".doc"  
_LIT(KTextMSWord2000,"application/msword2000");	// ".doc"  



// Image types
_LIT(KImageBasic, "image/*");					// Don't handle  or use ".bmp" ?
_LIT(KImageGIF, "image/gif");					// ".gif"
_LIT(KImageJPEG, "image/jpeg");					// ".jpg"
_LIT(KImageTIFF, "image/tiff");					// ".tif"
_LIT(KImageWBMP, "image/vnd.wap.wbmp");			// ".wbmp"

// Audio types
_LIT(KAudioBasic, "audio/basic");				// ".wav"  ??

//Video
_LIT(KVideoMpeg, "video/mpeg");					// ".mpg" ??

//Application types
_LIT(KAppXML, "application/xml");				// ".xml"
_LIT(KAppWMLC, "application/vnd.wap.wmlc");		// ".wmlc"
_LIT(KAppWBXML, "application/vnd.wap.wbxml");	// ".wmlc" ??
_LIT(KAppSIS, "application/vnd.symbian.install");	// ".sis"


// Extensions for the file holding the Unknown Push Msg data
_LIT(KStdUnknownExt, ".dat");
_LIT(KUnknownTextExt, ".txt");
_LIT(KUnknownDocExt, ".doc");
_LIT(KUnknownRTextExt, ".rtf");
_LIT(KUnknownXmlExt, ".xml");
_LIT(KUnknownHtmlExt, ".html");
_LIT(KUnknownWmlExt,  ".wml");
// _LIT(KUnknownWmlsExt, ".wmls");

_LIT(KUnknownGifExt, ".gif");
_LIT(KUnknownJpegExt, ".jpg");
_LIT(KUnknownTiffExt, ".tif");
_LIT(KUnknownWbmpExt, ".wbmp");

//_LIT(KUnknownBasicAudioExt, ".wav");
//_LIT(KUnknownVidMpegExt, ".mpg");

_LIT(KUnknownWmlcExt, ".wmlc");
_LIT(KUnknownSISExt, ".sis");










#endif