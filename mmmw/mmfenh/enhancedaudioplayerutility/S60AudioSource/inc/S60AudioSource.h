/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Base class definition of the S60 Audio Source
*
*/

#ifndef __CS60AUDIOSOURCE_H__
#define __CS60AUDIOSOURCE_H__

#include <e32std.h>
#include <e32base.h>
#include <mmf/common/mmfcontroller.h>

// CLASS DEFINITIONS

/**
*  This base class defines the common S60 audio source.
*
*  @lib S60AudioPlayer.lib
*  @since 3.1
*/
class CS60AudioBaseSource: public CBase
	{
	public: // Constructors and destructor

		/**
        *	Destructor
        */
		IMPORT_C virtual ~CS60AudioBaseSource();

    public: // New functions

		/**
        * Gets the Size of the Source
        * @return The Size of the Source
        * @since 3.1
        */
		virtual TUint GetSize() = 0;

		/**
        * Gets the Mimetype of the Source
        * @param aMimeType mimetype of the Source.
        * @since 3.1
        */
		virtual void GetMimeType(TDes8& aMimeType) = 0;

		/**
        * Gets the Position in the Source
        * @return The Position in the Source
        * @since 3.1
        */
		virtual TInt64 GetPosition() = 0;

		/**
        * Sets the Position in the Source
        * @param aPosition position to be set in the Source.
        * @return One of the Standard Return Error Codes
        * @since 3.1
        */
		virtual TInt SetPosition(TInt64 aPosition) = 0;

	protected:

		RMMFController iController;
		TUid iControllerUid;
		TMMFMessageDestination iSourceHandle;

	protected:    // Friend classes

		friend class CS60AudioPlayer;


	};

#endif // __CS60AUDIOSOURCE_H__

//  End of File
