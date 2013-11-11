/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Factory class to construct collection helper classes
*
*/



#ifndef MMPXCOLLECTIONHELPERFACTORY_H
#define MMPXCOLLECTIONHELPERFACTORY_H

#include <bautils.h>
#include <mpxcollectionframeworkdefs.h>

// FORWARD DECLARATIONS
class CMPXMedia;
class CMPXCollectionPath;
class MMPXCollection;
class MMPXHarvesterUtility;
class MMPXCollectionHelper;
class MMPXCollectionUiHelper;
 
/**
 *  Factory class to construct collection helpers.
 *
 *  @lib mpxcollectionhelper.lib
 */
class CMPXCollectionHelperFactory : public CBase
    {

public:
 
    /**
    * Two phased constructor to create the collection UI helper.
	* @since S60 3.2.3
    * @param aModeId collection client context id to bind to.
    * @return New CMPXCollectionUIHelper implementing instance based on aModeID
    */
    IMPORT_C static MMPXCollectionUiHelper* NewCollectionUiHelperL(const TUid& aModeId = KMcModeDefault);

    /**
    * Two phased constructor to create the general collection helper.
	* @since S60 3.2.3
    * @return New CMPXCollectionHelper implementing instance
    */
    IMPORT_C static MMPXCollectionHelper* NewCollectionHelperL();
    
    /**
    * Two phased constructor to create the cached collection helper.
	* @since S60 3.2.3
    * @return New CMPXCollectionCachedHelper implementing instance
    */
    IMPORT_C static MMPXCollectionHelper* NewCollectionCachedHelperL();
    
    };
       
#endif // MMPXCOLLECTIONHELPERFACTORY_H
