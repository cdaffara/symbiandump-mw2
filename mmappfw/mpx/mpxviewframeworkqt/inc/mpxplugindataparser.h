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
* Description: 
*
*/

#ifndef QMPXPLUDINDATAPARSER_H
#define QMPXPLUDINDATAPARSER_H

class MpxPluginData;
class QString;
class QStringRef;

/**
 *  QMpxPluginDataParser
 * 
 */
class MpxPluginDataParser
    {
public:

    /**
     * Constructor
     */
    MpxPluginDataParser();
        
    /**
     * Destructor.
     */
    ~MpxPluginDataParser();
    
    const MpxPluginData* data() const;
    
    MpxPluginData* takeData();
    
    void parse( const QString& aData );
private:
    bool findNextNode( const QString& aSrc, int& aOffset, QStringRef& aTagName, QStringRef& aTagContent ) const;
    bool parseInt( int& aDst, const QString& aSrc ) const ;
    void parseSupportedId( const QString& aValue );
private:
    MpxPluginData* dataPtr;
    };

#endif // QMPXPLUDINDATAPARSER_H
