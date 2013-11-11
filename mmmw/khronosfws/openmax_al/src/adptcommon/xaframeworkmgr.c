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
 * Description: Framework Manager. Parses the CFG file
 *
 */

#include <stdio.h>
#include <string.h>
#include "xaframeworkmgr.h"
#include "xadebug.h"

/* Default line width permitted in the cfg file + 2 to hold "\r\n"*/
#define LINEWIDTH 82

typedef enum
    {
    FWMgrTagNone,
    FWMgrTagURIScheme,
    FWMgrTagFileExt
    } FWMgrTagType;

/* Config file location */
const char configFileLocationZ[] = "z:/openmaxal/openmaxal.cfg";
const char configFileLocationC[] = "c:/openmaxal/openmaxal.cfg";

/* Tags used for parsing */
const char mediaPlayerBeginTag[] = "<mediaplayer>";
const char mediaPlayerEndTag[] = "</mediaplayer>";
const char mediaRecorderBeginTag[] = "<mediarecorder>";
const char mediaRecorderEndTag[] = "</mediarecorder>";
const char mediaFrameworkMmfBeginTag[] = "<mmf>";
const char mediaFrameworkMmfEndTag[] = "</mmf>";
const char mediaFrameworkGstBeginTag[] = "<gst>";
const char uriSchemeBeginTag[] = "<urischeme>";
const char uriSchemeEndTag[] = "</urischeme>";
const char mediaFrameworkGstEndTag[] = "</gst>";
const char fileExtBeginTag[] = "<fileext>";
const char fileExtEndTag[] = "</fileext>";

/* Local function definitions */
/* returns FWMgrTrue if processed successfully */
static FWMgrBool processConfigEntry(const char* buffer,
        FWMgrMOType *mediaType, FWMgrFwType *frameworkType,
        FWMgrTagType *tagType, FWMgrBool *newNode, FrameworkMap **node);

/* returns FWMgrTrue if processed successfully */
static FWMgrBool processTagType(const char* buffer,
        FWMgrFwType *frameworkType, FWMgrTagType *tagType,
        FrameworkMap **node);

/* returns FWMgrTrue if processed successfully */
static FWMgrBool tokenizeTag(FWMgrTagType tagType, const char* buffer,
        FrameworkMap **node);

/* Global functions from header file */

/* FrameworkMap* XAFrameworkMgr_CreateFrameworkMap
 * Description: Creates a list of framework and use-case map.
 */
FrameworkMap* XAFrameworkMgr_CreateFrameworkMap()
    {
    char buffer[LINEWIDTH];
    int readSize;
    int lineNumber = 0;
    FWMgrBool processedEntry = FWMgrTrue;
    FWMgrMOType currentMediaType = FWMgrMOUnknown;
    FWMgrFwType currentFrameworkType = FWMgrFWUknown;
    FWMgrTagType currentTagType = FWMgrTagNone;
    FrameworkMap *curNode = NULL;
    FWMgrBool newNode;
    FrameworkMap *frameworkMap = NULL;
    FILE* fp = NULL;

    DEBUG_API("->XAFrameworkMgr_CreateFrameworkMap");
    /* Open file on C drive */
    DEBUG_INFO("Opening configuration file from C drive...");
    fp = fopen(configFileLocationC, "r");
    if (fp == NULL)
        {
        DEBUG_INFO("Opening configuration file from Z drive...");
        fp = fopen(configFileLocationZ, "r");
        }

    if (fp == NULL)
        {
        DEBUG_ERR("ERROR: Unable to open configuration file!");
        DEBUG_API("<-XAFrameworkMgr_CreateFrameworkMap");
        return NULL;
        }
    while ((fgets(buffer, LINEWIDTH, fp) != NULL) && processedEntry)
        {
        /* keep looping until NULL pointer OR error... */
        lineNumber++;
        readSize = strlen(buffer);
        /* Ignore comments line */
        if (buffer[0] == '#')
            continue;

        /* Ignore replace "\r\n" with '\0' */
        if ((readSize >= 2) && (buffer[readSize - 2] == '\r')
                && (buffer[readSize - 1] == '\n'))
            buffer[readSize - 2] = '\0';

        /* Ignore new line... */
        if (readSize == 2)
            continue;

        processedEntry = processConfigEntry(buffer, &currentMediaType,
                &currentFrameworkType, &currentTagType, &newNode,
                &curNode);
        if (newNode)
            {
            /*Just link to the last element in the chain*/
            if (!frameworkMap)
                {
                frameworkMap = curNode;
                }
            else
                {
                FrameworkMap *lastNode = frameworkMap;
                while (lastNode->next)
                    {
                    lastNode = lastNode->next;
                    }
                lastNode->next = curNode;
                }
            }
        }
    fclose(fp);
    DEBUG_API("<-XAFrameworkMgr_CreateFrameworkMap");
    return frameworkMap;
    }

#ifdef _DEBUG
/* void XAFrameworkMgr_DumpFrameworkMap
 * Description: Prints map to std console.
 */
void XAFrameworkMgr_DumpFrameworkMap(FrameworkMap *map)
    {
    FrameworkMap *node = map;
    int i;
    int loopIndex = 0;

    DEBUG_API("->XAFrameworkMgr_DumpFrameworkMap");
    while (node)
        {
        loopIndex++;
        DEBUG_INFO_A1("%d>", loopIndex);
        if (node->moType == FWMgrMOPlayer)
            {
            DEBUG_INFO("MediaPlayer-");
            }
        else if (node->moType == FWMgrMORecorder)
            {
            DEBUG_INFO("MediaRecrdr-");
            }
        else
            {
            DEBUG_INFO("UKNOWN-");
            }
        if (node->fwType == FWMgrFWMMF)
            {
            DEBUG_INFO("MMF-");
            }
        else if (node->fwType == FWMgrFWGST)
            {
            DEBUG_INFO("GST-");
            }
        else
            {
            DEBUG_INFO("UKNOWN-");
            }
        DEBUG_INFO("Scheme[");
        for (i = 0; i < node->uriSchemeCount; i++)
            {
            DEBUG_INFO_A1_STR(" %s", node->uriSchemes[i]);
            }
        printf("]FileExt[");
        for (i = 0; i < node->fileExtCount; i++)
            {
            DEBUG_INFO_A1_STR(" %s", node->fileExts[i]);
            }
        DEBUG_INFO("]\n");
        node = node->next;
        }
    DEBUG_API("<-XAFrameworkMgr_DumpFrameworkMap");
    }
#endif

/* void XAFrameworkMgr_DeleteFrameworkMap
 * Description: Deletes the list of framework and use-case map.
 */
void XAFrameworkMgr_DeleteFrameworkMap(FrameworkMap **map)
    {
    FrameworkMap *node = *map;
    FrameworkMap *nextNode = NULL;
    int i;
    DEBUG_API("->XAFrameworkMgr_DeleteFrameworkMap");
    while (node)
        {
        for (i = 0; i < node->uriSchemeCount; i++)
            free(node->uriSchemes[i]);
        free(node->uriSchemes);

        for (i = 0; i < node->fileExtCount; i++)
            free(node->fileExts[i]);
        free(node->fileExts);

        nextNode = node->next;
        free(node);
        node = nextNode;
        }
    *map = NULL;
    DEBUG_API("<-XAFrameworkMgr_DeleteFrameworkMap");
    }

/* FWMgrFwType XAFrameworkMgr_GetFramework
 * Description: Returns the framework enum that handles uri.
 */
FWMgrFwType XAFrameworkMgr_GetFramework(FrameworkMap *map, const char *uri,
        FWMgrMOType mediaObject)
    {
    FWMgrFwType retVal = FWMgrFWUknown;
    char fileScheme[] = "file";
    char *uriScheme = NULL;
    char *fileExt = NULL;
    FrameworkMap *node = map;
    FWMgrBool uriMatchFound = FWMgrFalse;
    FWMgrBool fileExtMatchFound = FWMgrFalse;
    int i = 0;
    int copyLen = 0;

    DEBUG_API("->XAFrameworkMgr_GetFramework");
    if (!map || !uri)
        {
        DEBUG_ERR("NULL map/uri");
        DEBUG_API("<-XAFrameworkMgr_GetFramework");
        return retVal;
        }

    DEBUG_INFO_A1_STR("uri[%s]", uri);
    /* Get uri scheme */
    uriScheme = strchr(uri, ':');
    if (uriScheme == NULL)
        {
        DEBUG_ERR("malformed URI. Could not find [:]");
        DEBUG_API("<-XAFrameworkMgr_GetFramework");
        return retVal;
        }

    copyLen = (uriScheme - uri);
    uriScheme = (char*) calloc(copyLen + 1, sizeof(char));
    strncpy(uriScheme, uri, copyLen);
    uriScheme[copyLen] = '\0'; /*Null terminate it*/

    if (strcasecmp(uriScheme, fileScheme) == 0)
        {
        /* Get uri extension */
        char* dotLoc = strrchr(uri, '.');
        if (dotLoc == NULL)
            {
            DEBUG_ERR("malformed URI. Could not find extension char[.]");
            DEBUG_API("<-XAFrameworkMgr_GetFramework");
            free(uriScheme);
            return retVal;
            }
        /* We need to add 1 to exclude '.'*/
        copyLen = strlen(uri) - (dotLoc + 1 - uri);
        fileExt = (char*) calloc(copyLen + 1, sizeof(char));
        strncpy(fileExt, dotLoc + 1, copyLen);
        fileExt[copyLen] = '\0'; /*Null terminate it*/
        }

    while (node)
        {
        if (mediaObject == node->moType)
            {
            uriMatchFound = FWMgrFalse;
            fileExtMatchFound = FWMgrFalse;
            /* Match for uri*/
            for (i = 0; i < node->uriSchemeCount; i++)
                {
                if (strcasecmp(uriScheme, node->uriSchemes[i]) == 0)
                    {                    
                    uriMatchFound = FWMgrTrue;
                    DEBUG_INFO_A1_STR("Found macthing framework for scheme[%s]", uriScheme);
                    break;
                    }
                }
            /* if uri scheme is not file, we only need to check for uri */
            if (!fileExt)
                {
                fileExtMatchFound = FWMgrTrue;
                }
            else
                {
                for (i = 0; i < node->fileExtCount; i++)
                    {
                    if (strcasecmp(fileExt, node->fileExts[i]) == 0)
                        {
                        fileExtMatchFound = FWMgrTrue;
                        DEBUG_INFO_A1_STR("Found macthing framework for fileext[%s]", fileExt);
                        break;
                        }
                    }
                }

            if ((uriMatchFound == FWMgrTrue) &&
                    (fileExtMatchFound == FWMgrTrue))
                {
                retVal = node->fwType;
                DEBUG_INFO_A1("Framework is[%d]", (int)retVal);
                break;
                }
            }
        node = node->next;
        }
    free(uriScheme);
    free(fileExt);
    DEBUG_API("<-XAFrameworkMgr_GetFramework");
    return retVal;
    }

/* Local functions */

/* FWMgrBool processConfigEntry
 * Description: Processes a single line entry from the config file.
 */
FWMgrBool processConfigEntry(const char* buffer, FWMgrMOType *mediaType,
        FWMgrFwType *frameworkType, FWMgrTagType *tagType,
        FWMgrBool *newNode, FrameworkMap **node)
    {
    FWMgrBool processedSuccessfully = FWMgrTrue;

    if (!buffer || !mediaType || !frameworkType || !tagType || !newNode)
        {
        processedSuccessfully = FWMgrFalse;
        return processedSuccessfully;
        }

	*newNode = FWMgrFalse;
    switch (*mediaType)
        {
        case FWMgrMOUnknown:
            {
            if (strcmp(buffer, mediaPlayerBeginTag) == 0)
                {
                *mediaType = FWMgrMOPlayer;
                *frameworkType = FWMgrFWUknown;
                *tagType = FWMgrTagNone;
                *node = NULL;
                }
            else if (strcmp(buffer, mediaRecorderBeginTag) == 0)
                {
                *mediaType = FWMgrMORecorder;
                *frameworkType = FWMgrFWUknown;
                *tagType = FWMgrTagNone;
                *node = NULL;
                }
            }
            break;
        case FWMgrMOPlayer:
        case FWMgrMORecorder:
            {
            switch (*frameworkType)
                {
                case FWMgrFWUknown:
                    {
                    if ((*mediaType == FWMgrMOPlayer) && (strcmp(buffer,
                            mediaPlayerEndTag) == 0))
                        *mediaType = FWMgrMOUnknown;
                    else if ((*mediaType == FWMgrMORecorder) && (strcmp(
                            buffer, mediaRecorderEndTag) == 0))
                        *mediaType = FWMgrMOUnknown;
                    else if ((strcmp(buffer, mediaFrameworkMmfBeginTag) == 0)
                            || (strcmp(buffer, mediaFrameworkGstBeginTag)
                                    == 0))
                        {

                        *frameworkType = FWMgrFWMMF;
                        if (strcmp(buffer, mediaFrameworkGstBeginTag) == 0)
                            *frameworkType = FWMgrFWGST;
                        if (*node)
                            {
                            printf(
                                    "Fatal error error. Entry already exists and creating another one!!!");
                            processedSuccessfully = FWMgrFalse;
                            }
                        else
                            {
                            *node = (FrameworkMap*) calloc(1,
                                    sizeof(FrameworkMap));
                            if (!(*node))
                                {
                                printf(
                                        "Fatal error. No memory to create an Entry!!!");
                                processedSuccessfully = FWMgrFalse;
                                }
                            else
                                {
                                *newNode = FWMgrTrue;
                                (*node)->moType = *mediaType;
                                (*node)->fwType = *frameworkType;
                                }
                            }
                        }
                    }
                    break;
                case FWMgrFWMMF:
                    {
                    processedSuccessfully = processTagType(buffer,
                            frameworkType, tagType, node);
                    }
                    break;
                case FWMgrFWGST:
                    {
                    processedSuccessfully = processTagType(buffer,
                            frameworkType, tagType, node);
                    }
                    break;
                default:
                    processedSuccessfully = FWMgrFalse;
                    break;
                };
            }
            break;
        default:
            processedSuccessfully = FWMgrFalse;
            break;
        };
    return processedSuccessfully;
    }

/* FWMgrBool processTagType
 * Description: Processes a framework type, uri, file tags entry from 
 * the config file.
 */
FWMgrBool processTagType(const char* buffer, FWMgrFwType *frameworkType,
        FWMgrTagType *tagType, FrameworkMap **node)
    {
    FWMgrBool processedSuccessfully = FWMgrTrue;

    if (!buffer || !frameworkType || !tagType || (*node == NULL))
        {
        processedSuccessfully = FWMgrFalse;
        return processedSuccessfully;
        }

    switch (*tagType)
        {
        case FWMgrTagNone:
            {
            if (((*frameworkType == FWMgrFWMMF) && (strcmp(buffer,
                    mediaFrameworkMmfEndTag) == 0)) || ((*frameworkType
                    == FWMgrFWGST)
                    && (strcmp(buffer, mediaFrameworkGstEndTag) == 0)))
                {
                *node = NULL;
                *frameworkType = FWMgrFWUknown;
                }
            else if (strcmp(buffer, uriSchemeBeginTag) == 0)
                *tagType = FWMgrTagURIScheme;
            else if (strcmp(buffer, fileExtBeginTag) == 0)
                *tagType = FWMgrTagFileExt;
            }
            break;
        case FWMgrTagURIScheme:
            {
            if (strcmp(buffer, uriSchemeEndTag) == 0)
                *tagType = FWMgrTagNone;
            else
                {
                processedSuccessfully = FWMgrFalse;
                if (*node)
                    processedSuccessfully = tokenizeTag(FWMgrTagURIScheme,
                            buffer, node);
                }
            }
            break;
        case FWMgrTagFileExt:
            {
            if (strcmp(buffer, fileExtEndTag) == 0)
                *tagType = FWMgrTagNone;
            else
                {
                processedSuccessfully = FWMgrFalse;
                if (*node)
                    processedSuccessfully = tokenizeTag(FWMgrTagFileExt,
                            buffer, node);
                }
            }
            break;
        default:
            processedSuccessfully = FWMgrFalse;
            break;
        };
    return processedSuccessfully;
    }

/* FWMgrBool processTagType
 * Description: Processes a framework type, uri, file tags entry from 
 * the config file.
 */
FWMgrBool tokenizeTag(FWMgrTagType tagType, const char* buffer,
        FrameworkMap **node)
    {
    char* tempStartPtr;
    char* tempEndPtr;
    int index = 0;
    int strLen = 0;

    if (!buffer || (*node == NULL))
        {
        return FWMgrFalse;
        }

    tempStartPtr = /*const_cast<char*>*/(char*) (buffer);
    tempEndPtr = /*const_cast<char*>*/(char*) (buffer);

    if (tagType == FWMgrTagURIScheme)
        {
        (*node)->uriSchemeCount = atoi(buffer);
        (*node)->uriSchemes = (char**) calloc((*node)->uriSchemeCount,
                sizeof(*((*node)->uriSchemes)));
        if (!(*node)->uriSchemes)
            {
            printf("Fatal error. No memory to create an Entry!!!");
            return FWMgrFalse;
            }
        }
    else if (tagType == FWMgrTagFileExt)
        {
        (*node)->fileExtCount = atoi(buffer);
        (*node)->fileExts = (char**) calloc((*node)->fileExtCount,
                sizeof(*((*node)->fileExts)));
        if (!(*node)->fileExts)
            {
            printf("Fatal error. No memory to create an Entry!!!");
            return FWMgrFalse;
            }
        }
    else
        return FWMgrFalse;

    /*Find the index of :*/
    tempStartPtr = strchr(tempStartPtr, ',');
    index = 0;
    while (tempStartPtr && (strlen(tempStartPtr) > 1))
        {
        tempStartPtr++; /* Ignore separator ','*/
        tempEndPtr = strchr(tempStartPtr, ',');
        strLen = (tempEndPtr - tempStartPtr) + 1; /* To hold null terminator */
        if (strLen > 0)
            {
            if (tagType == FWMgrTagURIScheme)
                {
                (*node)->uriSchemes[index] = (char*) calloc(strLen,
                        sizeof(char));
                strncpy((*node)->uriSchemes[index], tempStartPtr, strLen);
                (*node)->uriSchemes[index][strLen - 1] = '\0'; /*Null terminate it*/
                index++;
                }
            else if (tagType == FWMgrTagFileExt)
                {
                (*node)->fileExts[index] = (char*) calloc(strLen,
                        sizeof(char));
                strncpy((*node)->fileExts[index], tempStartPtr, strLen);
                (*node)->fileExts[index][strLen - 1] = '\0'; /*Null terminate it*/
                index++;
                }
            }
        tempStartPtr = tempEndPtr;
        }
    return FWMgrTrue;
    }
	
