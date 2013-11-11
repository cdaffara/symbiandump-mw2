// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
// tmsvbioinfo.h
//
/**
 * @file 
 * Base classes to store additional BIO message-specific information. 
 *
 * @internalTechnology
 * @released
 */

#if !defined(__TMSVBIOINFO_H__)
#define __TMSVBIOINFO_H__

// Symbols:
/** Space character. 
 * @internalTechnology
 * @released
 */
#define KCharSpace          ' '
/** Tab character. 
 * @internalTechnology
 * @released
 */
#define KCharTab            '\t'
/** Line feed character. 
 * @internalTechnology
 * @released
 */
#define KCharLineFeed       '\n'

// Define some generic error codes:
/** BIO error code base
 * @internalTechnology
 * @released
 */
const TInt KBspBaseError					= (-500);

/** Invalid BIO message error code.
 * @internalTechnology
 * @released
 */
const TInt KBspInvalidMessage				= (KBspBaseError);

/** Invalid smart message token error code.
 * @internalTechnology
 * @released
 */
const TInt KBspSmartMessageInvalidToken		= (KBspBaseError-1);

/** No smart message parser defined error code.
 * @internalTechnology
 * @released
 */
const TInt KBspSmartMessageNoParserDefined	= (KBspBaseError-2);

/**
* BIO client MTM UID. 
* @internalTechnology
* @released
*/
const TUid KUidBIOMessageClientMtmDll	= {0x1000125A}; //previously NBSC, now BIOC
/**
* BIO server MTM UID. 
* @internalTechnology
* @released
*/
const TUid KUidBIOMessageServerMtmDll	= {0x1000125C}; //previously NBSS, now BIOS
/**
* BIO UI MTM UID. 
* @internalTechnology
* @released
*/
const TUid KUidBIOMessageUiLayerMtmDll	= {0x10001265}; //previously NBUM, now BIUM
/**
* BIO message viewer UID. 
* @internalTechnology
* @released
*/
const TUid KUidBIOMessageViewerApp		= {0x10001264}; //previously NBED,now BIED

/** BIO data location flag values.
 * @internalTechnology
 * @released
 * @see TMsvBIOEntry 
 */
enum TMsvBIODataLocation
	{
	/** Unknown. */
	EUnknown,
	/** Parser wrote data into the body text. */
	EBodyText,		// parser wrote data back into richText
	/** Parser wrote data into the parsed fields data stream. */
	EBIODataStream,	// parser wrote data into KUIDMsvBioStream
	/** Parser wrote data into an attachment file. */
	EFile			// parser wrote data into attachment file
	};


/** Bearer Independent Object entry.
 * Specialises the TMsvEntry message entry class to store additional 
 * BIO message-specific information. 
 * @internalTechnology
 * @released
 */
class TMsvBIOEntry : public TMsvEntry
	{
public:
	/** Constructor. */
	TMsvBIOEntry() : TMsvEntry() {};
	void SetBIOParserUid(const TUid aId);
	const TUid BIOParserUid() const;
	void SetLocationOfData(const TMsvBIODataLocation aLocation);
	const TMsvBIODataLocation LocationOfData() const;

private:
	TMsvBIODataLocation	iLocationOfData;
	};

#include <bsp.inl>

#endif //__TMSVBIOINFO_H__
