// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
    
#include "cmultipartiteratorbase.h"


/**
 * CMultipartIteratorBase is a base utility class to crack out parts
 * of a multipart message.
 * @param aPushMessage: A message determined to be multipart. 
 */
CMultipartIteratorBase::CMultipartIteratorBase(CPushMessage& aPushMessage) : 
	iPushMessage(aPushMessage)
	{
	}

