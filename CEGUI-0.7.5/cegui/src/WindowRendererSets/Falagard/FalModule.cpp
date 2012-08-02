/***********************************************************************
    filename:   FalModule.cpp
    created:    Wed Jun 22 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "falagard/FalModule.h"
#include "CEGUITplWRFactoryRegisterer.h"

// includes for types we create
#include "falagard/FalButton.h"
#include "falagard/FalDefault.h"
#include "falagard/FalEditbox.h"
#include "falagard/FalFrameWindow.h"
#include "falagard/FalItemEntry.h"
#include "falagard/FalListHeader.h"
#include "falagard/FalListHeaderSegment.h"
#include "falagard/FalListbox.h"
#include "falagard/FalMenubar.h"
#include "falagard/FalMenuItem.h"
#include "falagard/FalMultiColumnList.h"
#include "falagard/FalMultiLineEditbox.h"
#include "falagard/FalPopupMenu.h"
#include "falagard/FalProgressBar.h"
#include "falagard/FalScrollablePane.h"
#include "falagard/FalScrollbar.h"
#include "falagard/FalSlider.h"
#include "falagard/FalStatic.h"
#include "falagard/FalStaticImage.h"
#include "falagard/FalStaticText.h"
#include "falagard/FalSystemButton.h"
#include "falagard/FalTabButton.h"
#include "falagard/FalTabControl.h"
#include "falagard/FalTitlebar.h"
#include "falagard/FalToggleButton.h"
#include "falagard/FalTooltip.h"
#include "falagard/FalItemListbox.h"
#include "falagard/FalTree.h"

//----------------------------------------------------------------------------//
extern "C"
CEGUI::WindowRendererModule& getWindowRendererModule()
{
    static CEGUI::FalagardWRModule mod;
    return mod;
}

namespace CEGUI
{
//----------------------------------------------------------------------------//
FalagardWRModule::FalagardWRModule()
{
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardButton>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardDefault>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardEditbox>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardFrameWindow>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardItemEntry>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardListHeader>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardListHeaderSegment>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardListbox>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardMenubar>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardMenuItem>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardMultiColumnList>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardMultiLineEditbox>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardPopupMenu>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardProgressBar>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardScrollablePane>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardScrollbar>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardSlider>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardStatic>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardStaticImage>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardStaticText>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardSystemButton>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardTabButton>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardTabControl>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardTitlebar>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardToggleButton>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardTooltip>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardItemListbox>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardTree>);
}

//----------------------------------------------------------------------------//
FalagardWRModule::~FalagardWRModule()
{
    FactoryRegistry::iterator i = d_registry.begin();
    for ( ; i != d_registry.end(); ++i)
        delete (*i);
}

//----------------------------------------------------------------------------//

}
