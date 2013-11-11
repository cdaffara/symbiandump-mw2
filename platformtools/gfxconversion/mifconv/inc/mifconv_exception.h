/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Mifconv exception class.
*
*/


#ifndef __MIFCONVEXCEPTION_H__
#define __MIFCONVEXCEPTION_H__

#include "mifconv.h"

/**
 * MifConvException class
 */
class MifConvException
{
public:

    /**
     * Default constructor
     */
	MifConvException();

    /**
     * Constructor
     */
	MifConvException(const MifConvString& str, const MifConvString& file = MifConvString(), int line = 0);

    /**
     * Copy constructor
     */
    MifConvException(const MifConvException& rhs);

    /**
     * Destructor
     */
	virtual ~MifConvException();

    /**
     * Sets the error string to be shown to user.
     * @param str Error string
     */
	void SetString(const MifConvString& str);

    /**
     * Returns the error string
     * @return Error string
     */
	const MifConvString& String() const;

    /**
     * Returns the error string
     * @return Error string
     */
	MifConvString& String();

    /**
     * Sets the file name
     * @param filename Source code file name
     */
    void SetFile(const MifConvString& filename);

    /**
     * Sets the line in the file
     * @param line Line number in the source code file
     */
    void SetLine(int line);

    /**
     * Returns the filename
     * @return Source code filename
     */
    const MifConvString& File() const;

    /**
     * Returns the line number
     * @return Line number in the source code file
     */
    int Line() const;

protected:

	MifConvString iString;
    MifConvString iFile;
    int iLine;
};

inline void THROW_ERROR_COMMON(const MifConvString& errorMsg, const MifConvString file = MifConvString(), int line = 0 )
{    
    throw MifConvException( MifConvString("ERROR: " + errorMsg + "\n"), file, line );
}

#endif
