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

#ifndef AUDIOOUTPUTCONTROLUTILITYIMPL_H
#define AUDIOOUTPUTCONTROLUTILITYIMPL_H

//#include <DRMConfigIntfc.h>
#include "AudioOutputControlUtility.h"


/**
 * CAudioOutputControlUtilityImpl class documentation.
 *
 * @lib AudioOutputControlUtility.lib
 *
 * @since S60 5.0
 */
class CAudioOutputControlUtilityImpl
	{

public:

    inline virtual ~CAudioOutputControlUtilityImpl();

    /**
     * Determines which audio restriction interface to implements accordingly.
     *
     * @return KErrNone if successful.
     */
    inline virtual TInt Configure(CMMFDevSound& /*aDevSound*/);

    /**
     * Gives DRMConfigIntfc to Utility for ownership.
     *
     * @return KErrNone if successful.
     */
    inline virtual TInt SetDRMConfigIntfc(CDRMConfigIntfc* /*aDRMConfigIntfc*/);
    
    /**
     * Gives MDataSource to Utility for reference. NOT OWNED
     *
     * @return KErrNone if successful.
     */
    inline virtual TInt SetDataSource(MDataSource* /*aDataSource*/);
    
    /**
     * Gives CMultimediaDataSource to Utility for reference. NOT OWNED
     *
     * @return KErrNone if successful.
     */
    inline virtual TInt SetDataSource(CMultimediaDataSource* /*aDataSource*/);
	};

#include "AudioOutputControlUtilityImpl.inl"

#endif // AUDIOOUTPUTCONTROLUTILITYIMPL_H

// End of file
