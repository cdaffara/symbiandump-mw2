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

#ifndef __REFTESTAGENTCONSTANTS_H__
#define __REFTESTAGENTCONSTANTS_H__

_LIT8(KRtaMimeContent, "application/x-rta.drm.content");
_LIT8(KRtaMimeContentRights, "application/x-rta.drm.contentrights");
_LIT8(KXmlRORights, "application/vnd.oma.drm.rights+xml");
_LIT8(KXmlTriggerRights, "application/vnd.oma.drm.roap-trigger+xml");

_LIT(KRtaExtensionContent, ".content");
_LIT(KRtaExtensionContentRights, ".contentrights");
_LIT(KRtaExtensionRights, ".rights");

_LIT(KRtaDefaultOutputDirectory, "c:\\");
_LIT(KRtaDefaultOutputFile, "output");

// combine KRtaDefaultOutputDirectory, KRtaDefaultOutputFile and
// KRtaExtensionContent
_LIT(KRtaDefaultOutputPath, "c:\\output.content");

#endif
