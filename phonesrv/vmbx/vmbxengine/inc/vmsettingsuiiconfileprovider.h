/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
 *
*/

#ifndef C_VMSETTINGSUIICONFILEPROVIDER
#define C_VMSETTINGSUIICONFILEPROVIDER

#include <e32base.h>
#include <AknIconUtils.h>

/**
 *  CVmSettingsUiIconFileProvider class
 *  Declarition of CVmSettingsUiIconFileProvider.
 *
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CVmSettingsUiIconFileProvider ) : public CBase,
                                                     public MAknIconFileProvider
    {
public:

    /**
     * Two-phased constructor.
     * @param in aFile for handle to file
     */
    static CVmSettingsUiIconFileProvider* NewL( RFile& aFile );

    /**
     * Destructor.
     */
    virtual ~CVmSettingsUiIconFileProvider();

public: // from base class MAknIconFileProvider

    /**
     * from MAknIconFileProvider.
     */
    void RetrieveIconFileHandleL( RFile& aFile, const TIconFileType aType );

    /**
     * from MAknIconFileProvider.
     */
    void Finished();

private:

    /**
     * C++ default constructor.
     */
    CVmSettingsUiIconFileProvider();

     /**
      * By default Symbian 2nd phase constructor is private.
      */
    void ConstructL( RFile& aFile );

private: // data

    // File handle. Own.
    RFile iFile;
    };

#endif // C_VMSETTINGSUIICONFILEPROVIDER
