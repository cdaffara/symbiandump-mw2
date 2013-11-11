// Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __REFTESTAGENTRIGHTSMANAGER_H__
#define __REFTESTAGENTRIGHTSMANAGER_H__

#include <caf/cafplatform.h>
#include <caf/agentinterface.h>
#include "rtarights.h"

using namespace ContentAccess;

/** A simple agent implementing a small part of OMA DRM v1.0

Several of the functions such as Copy, Rename and GetDir are not supported
by this agent.

 */
class CRefTestAgentRightsManager : public ContentAccess::CAgentRightsManager
		{
	public:
		static CRefTestAgentRightsManager* NewL();
		static CRefTestAgentRightsManager* NewLC();

		~CRefTestAgentRightsManager();

	public:
		// From CAgentRightsManager
		virtual void  ListAllRightsL (RStreamablePtrArray< CRightsInfo > &aArray) const; 
		virtual void  ListRightsL (RStreamablePtrArray< CRightsInfo > &aArray, const TDesC &aUri) const; 
		virtual void  ListRightsL (RStreamablePtrArray< CRightsInfo > &aArray, TVirtualPathPtr &aVirtualPath) const;
		virtual void  ListContentL (RStreamablePtrArray< CVirtualPath > &aArray, CRightsInfo &aRightsInfo) const;
		virtual MAgentRightsBase *  GetRightsDataL (const CRightsInfo &aRightsInfo) const;
		virtual TInt  DeleteRightsObject (const CRightsInfo &aRightsInfo);
		virtual TInt  DeleteAllRightsObjects (const TVirtualPathPtr &aVirtualPath);
		virtual TInt SetProperty(TAgentProperty aProperty, TInt aValue);
	private:
		CRefTestAgentRightsManager();
		};

#endif // __REFTESTAGENTMANAGER_H__
