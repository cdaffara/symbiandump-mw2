// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @internalComponent
 @released
*/


#ifndef __TAGS_H__
#define __TAGS_H__

namespace ReferenceTestAgent
	{
	// XML tags
	_LIT8(KContainerTag, "container");
	_LIT8(KContentTag, "content");
	_LIT8(KUniqueIdTag, "uniqueid");
	_LIT8(KNameTag, "name");
	_LIT8(KAttributeTag, "attribute");
	_LIT8(KStringAttributeTag, "stringattribute");
	_LIT8(KFileNameTag, "filename");
	_LIT8(KCidTag, "cid");
	_LIT8(KDrmFileTag, "drmfiles");
//	_LIT8(KDrmRightsTag, "drmrights");
	_LIT8(KRightsTag, "rights");
	_LIT8(KIntentTag, "intent");
	_LIT8(KCountTag, "count");
	_LIT8(KPendingTag, "pending");
	_LIT8(KForwardLockTag, "forwardlock");
	_LIT8(KTypeTag, "type");
	_LIT8(KExpiryTag, "expiry");
	_LIT8(KDayTag, "day");
	_LIT8(KMonthTag, "month");	
	_LIT8(KYearTag, "year");
	
	//Rights Object tags	
	_LIT8(KDrmRightsTag, "o-ex:rights");
	_LIT8(KContexttag, "o-ex:context");
	_LIT8(KAgreementTag, "o-ex:agreement");
	_LIT8(KAssetTag, "o-ex:asset");
	_LIT8(KPermissionTag, "o-ex:permission");
	_LIT8(KPlayTag, "o-dd:play");
	_LIT8(KDisplaytag, "o-dd:display");
	_LIT8(KExecutetag, "o-dd:execute");
	_LIT8(KPrinttag, "o-dd:print");
	_LIT8(KConstrainttag, "o-ex:constraint");
	_LIT8(KCounttag, "o-dd:count");
	_LIT8(KDateTimetag, "o-dd:datetime");
	_LIT8(KStarttag, "o-dd:start");
	_LIT8(KEndtag, "o-dd:end");
	_LIT8(KIntervaltag, "o-dd:interval");
	_LIT8(KVersion, "o-dd:version");
	_LIT8(KUid, "o-dd:uid");
	_LIT8(KKeyInfo, "ds:KeyInfo");
	_LIT8(KKeyValue, "ds:KeyValue");
	
	//ROAP Response Tags
	_LIT8(KRoapResponse,"roResponse");
	_LIT8(KDeviceId, "deviceId");
	_LIT8(KriID, "riID");
	_LIT8(KNonce, "nonce");
	_LIT8(KprotectedRO, "protectedRO");
	_LIT8(KSignature, "signature");
	_LIT8(KRightsObject, "ro");
	_LIT8(KKeyIdentifier, "keyIdentifier");
	_LIT8(KHash, "hash");
	
	//ROAP Trigger tags
	_LIT8(KRoapTrigger, "roapTrigger");
	_LIT8(KRegRequest, "registrationRequest");
	_LIT8(KROAquisation, "roAcquisition");
	_LIT8(KJoinDomain, "joinDomain");
	_LIT8(KLeaveDomain, "leaveDomain");
	_LIT8(KRiAlias, "riAlias");
	_LIT8(KRoapURL, "roapURL");
	_LIT8(KDomainID, "domainID");
	_LIT8(KRoID, "roID");
	_LIT8(KContentID, "contentID");
	}
#endif
