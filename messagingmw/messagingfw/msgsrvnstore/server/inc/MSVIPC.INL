// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// TMsvPackedChangeNotification
// 
//

inline void TMsvPackedChangeNotification::Pack(TMsvServerChangeNotificationType aChangeType, const CMsvEntrySelection& aSelection, TInt aParameter1, TInt aParameter2)
//
// Packs all of the selection into the buffer
//
	{
	Pack(aChangeType, aSelection, aParameter1, aParameter2, 0, aSelection.Count()-1);
	}
