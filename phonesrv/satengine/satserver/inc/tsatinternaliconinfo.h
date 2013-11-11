/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Sub-session for SAT services.
*
*/

#ifndef TSATINTERNALICONINFO_H
#define TSATINTERNALICONINFO_H

/**
 * Class for transporting icon image data to SatEngine
 * Internal parameter class between SAT client and server.
 *
 * @code
 * TSatInternalIconInfo internalIconInfo;
 * TSatInternalIconInfoPckg internalIconInfoPckg
 * @endcode 
 *
 * @since 5.1
 */
NONSHARABLE_CLASS( TSatInternalIconInfo )
    {
    public:
    
        /**
         * Coding scheme of the icon.
         * The same with TSatIconInfo enum.
         * 
         * @since 5.1
         */
        enum TCodingScheme
            {
            EBasic = 0x11,  ///< Basic (Black and White) icon
            EColor = 0x21   ///< Color icon
            };    

        /**
         * Class constructor.
         */
        TSatInternalIconInfo() :
            iWidth( 0 ),
            iHeight( 0 ),
            iCodingScheme( EBasic ),
            iFileIdentifier( 0 ),
            iOffset( 0 ),
            iLength( 0 )
            {
            };

    public:

        /**
         * The icon width in pixels.
         */
        TInt iWidth;

        /**
         * The icon height in pixels
         */
        TInt iHeight;

        /**
         * Coding scheme of the icon.
         */
        TCodingScheme iCodingScheme;

        /**
         * File identifier of the icon
         */
        TUint16 iFileIdentifier;

        /**
         * Offset of the icon in EF.
         */
        TUint16 iOffset;

        /**
         * Length of the icon data.
         */
        TUint16 iLength;
    };
    
typedef TPckg<TSatInternalIconInfo> TSatInternalIconInfoPckg;

#endif // TSATINTERNALICONINFO_H

// End of File
