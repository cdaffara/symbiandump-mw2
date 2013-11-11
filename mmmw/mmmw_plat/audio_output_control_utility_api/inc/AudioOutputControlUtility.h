/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Audio Output Control Utility
*
*/


#ifndef AUDIOOUTPUTCONTROLUTILITY_H
#define AUDIOOUTPUTCONTROLUTILITY_H

#include <e32base.h>

class CAudioOutputControlUtilityImpl;
class CMMFDevSound;
class CDRMConfigIntfc;
class MDataSource;
class CMultimediaDataSource;

class CAudioOutputControlUtility : public CBase
    {
public:
		IMPORT_C ~CAudioOutputControlUtility();
		IMPORT_C TInt Configure(CMMFDevSound& aDevSound);
		IMPORT_C TInt SetDRMConfigIntfc(CDRMConfigIntfc*& aDRMConfigIntfc);
		IMPORT_C TInt SetDataSource(MDataSource* aDataSource);
		IMPORT_C TInt SetDataSource(CMultimediaDataSource* aDataSource);
protected:
    IMPORT_C CAudioOutputControlUtility();
    IMPORT_C void ConstructL(CAudioOutputControlUtilityImpl* aImpl);

private:
		CAudioOutputControlUtilityImpl* iImpl;
  };

#endif // AUDIOOUTPUTCONTROLUTILITY_H

// End of file
