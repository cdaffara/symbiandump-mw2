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

#ifndef TEST_SCRIPTS_INL_
#define TEST_SCRIPTS_INL_

inline CScriptSection& CScriptFile::SectionL(const TDesC& aSectionName)
	{
	CScriptSection* section = Section(aSectionName);

	if (section == NULL)
		User::Leave(KErrNotFound);

	return *section;
	}

inline const TDesC& CScriptSection::SectionName() const
	{
	return *iSectionName;
	}

inline const TDesC& CScriptSectionItem::Item() const
	{
	return *iItem;
	}

inline void CScriptSection::SetDefaultsL(CScriptSection& aDefaults)
	{
	CScriptSection* def = aDefaults.CopyLC();
	CleanupStack::Pop(def);
	delete iDefaults;
	iDefaults = def;
	}

inline CScriptSectionItem* CScriptSectionItem::CopyLC()
	{
	return NewLC(iParent, *iItem, *iValue);
	}



#endif
