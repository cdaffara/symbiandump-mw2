// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TBULKCOMMITSERVERMTMPROGRESS_H__
#define __TBULKCOMMITSERVERMTMPROGRESS_H__

#include <e32base.h>

enum TBulkCommitTestOperation
	{
	ECreateBulk,
	ENoOperation
	};

class TBulkCommitServerMtmProgress 
	{
	public:
		inline TBulkCommitServerMtmProgress();

		inline TBulkCommitTestOperation Command() const;
		inline void SetCommand(TInt aCommand);

	public:
		TInt iError;
		TTime iTime;

	private:
		TBulkCommitTestOperation iCommand; 
	};

typedef TPckgBuf<TBulkCommitServerMtmProgress> TBulkCommitServerMtmProgressBuf;

inline TBulkCommitServerMtmProgress::TBulkCommitServerMtmProgress()
: iError(KErrNone), iCommand(ENoOperation)
	{
	iTime.HomeTime();
	}

inline TBulkCommitTestOperation TBulkCommitServerMtmProgress::Command() const
	{
	return iCommand;
	}

inline void TBulkCommitServerMtmProgress::SetCommand(TInt aCommand)
	{
	iCommand = (TBulkCommitTestOperation) aCommand;
	}

#endif	// __TBULKCOMMITSERVERMTMPROGRESS_H__
