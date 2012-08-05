/***********************************************************************
	filename: 	CEGUIResourceProvider.h
	created:	8/7/2004
	author:		James '_mental_' O'Sullivan
	
	purpose:	Defines abstract base class for loading DataContainer objects
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#ifndef _CEGUIResourceProvider_h_
#define _CEGUIResourceProvider_h_

#include "CEGUIBase.h"
#include "CEGUIDataContainer.h"
#include "CEGUIString.h"
#include <vector>
#include <map>


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Abstract class that defines the required interface for all resource provider sub-classes.

	A ResourceProvider is used to load both XML and binary data from an external source.  This could be from a filesystem or the resource manager of a specific renderer.
*/
class CEGUIEXPORT ResourceProvider
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
    /*!
    \brief
        Constructor for the ResourceProvider class
    */
	ResourceProvider() { }

    /*!
    \brief
        Destructor for the ResourceProvider class
    */
	virtual ~ResourceProvider(void) { }

    /*************************************************************************
        Accessor functions
    *************************************************************************/
    /*!
    \brief
        Return the current default resource group identifier.

    \return
        String object containing the currently set default resource group identifier.
    */
    const String&   getDefaultResourceGroup(void) const     { return d_defaultResourceGroup; }
    
    /*!
    \brief
        Set the default resource group identifier.

    \param resourceGroup
        String object containing the default resource group identifier to be used.

    \return
        Nothing.
    */
    void    setDefaultResourceGroup(const String& resourceGroup)    { d_defaultResourceGroup = resourceGroup; }

	    /*!
    \brief
        Set the directory associated with a given resource group identifier.

    \param resourceGroup
        The resource group identifier whose directory is to be set.

    \param directory
        The directory to be associated with resource group identifier
        \a resourceGroup

    \return
        Nothing.
    */
    void setResourceGroupDirectory(const String& resourceGroup, const String& directory);

    /*!
    \brief
        Return the directory associated with the specified resource group
        identifier.

    \param resourceGroup
        The resource group identifier for which the associated directory is to
        be returned.

    \return
        String object describing the directory currently associated with resource
        group identifier \a resourceGroup.

    \note
        This member is not defined as being const because it may cause
        creation of an 'empty' directory specification for the resourceGroup
        if the resourceGroup has not previously been accessed.
    */
    const String& getResourceGroupDirectory(const String& resourceGroup);

    /*!
    \brief
        clears any currently set directory for the specified resource group
        identifier.

    \param resourceGroup
        The resource group identifier for which the associated directory is to
        be cleared.
    */
    void clearResourceGroupDirectory(const String& resourceGroup);

    void loadRawDataContainer(const String& filename, RawDataContainer& output, const String& resourceGroup);
    void unloadRawDataContainer(RawDataContainer& data);
    size_t getResourceGroupFileNames(std::vector<String>& out_vec,
                                     const String& file_pattern,
                                     const String& resource_group);

protected:
    /*!
    \brief
        Return the final path and filename, taking into account the given
        resource group identifier that should be used when attempting to
        load the data.
    */
    String getFinalFilename(const String& filename, const String& resourceGroup) const;

    typedef std::map<String, String, String::FastLessCompare> ResourceGroupMap;
    ResourceGroupMap    d_resourceGroups;
    String  d_defaultResourceGroup;     //!< Default resource group identifier.
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIResourceProvider_h_
