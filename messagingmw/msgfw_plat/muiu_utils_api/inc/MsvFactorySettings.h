/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: MsvFactorySettings  declaration
*
*/



#ifndef MSVFACTORYSETTINGS_H__
#define MSVFACTORYSETTINGS_H__

//  INCLUDES
#include "MtmExtendedCapabilities.hrh"

// CLASS DECLARATION

/**
*  MsvFactorySettings
*  static class for RestoryFactorySettings module.
*  Mtm ui module should implement function
*  CMsvOperation* CBaseMtmUi::InvokeSyncFunctionL( TInt aFunctionId, const CMsvEntrySelection& aSelection, TDes8& aParameter )
*  with parameters:
*  aFunctionId: KMtmUiFunctionRestoreFactorySettings
*  aSelection can be ignored
*  aParameter: EMsvFactorySettingsLevelNormal needs to be implemented,
*              EMsvFactorySettingsLevelDeep is not used.
*/
class MsvFactorySettings
    {
    public:
        /**
         * Goes through every mtm ui and calls
         * CMsvOperation* CBaseMtmUi::InvokeSyncFunctionL(TInt aFunctionId, const CMsvEntrySelection& aSelection, TDes8& aParameter)
         * @param aLevel: EMsvFactorySettingsLevelNormal (or EMsvFactorySettingsLevelDeep)
         */
        IMPORT_C static void ResetL( TMsvFactorySettingsLevel aLevel );
    };

#endif  // MSVFACTORYSETTINGS_H__

// End of File
