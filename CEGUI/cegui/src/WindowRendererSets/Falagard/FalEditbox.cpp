/***********************************************************************
    filename:   FalEditbox.cpp
    created:    Sat Jun 25 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "FalEditbox.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUICoordConverter.h"
#include "CEGUIFont.h"
#include "CEGUIBiDiVisualMapping.h"

#include <stdio.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
FalagardEditboxProperties::BlinkCaret FalagardEditbox::d_blinkCaretProperty;
FalagardEditboxProperties::BlinkCaretTimeout FalagardEditbox::d_blinkCaretTimeoutProperty;
FalagardEditboxProperties::TextFormatting FalagardEditbox::d_textFormattingProperty;

//----------------------------------------------------------------------------//
const utf8 FalagardEditbox::TypeName[] = "Falagard/Editbox";
const String FalagardEditbox::UnselectedTextColourPropertyName("NormalTextColour");
const String FalagardEditbox::SelectedTextColourPropertyName("SelectedTextColour");
const float FalagardEditbox::DefaultCaretBlinkTimeout(0.66f);

//----------------------------------------------------------------------------//
FalagardEditbox::FalagardEditbox(const String& type) :
    EditboxWindowRenderer(type),
    d_lastTextOffset(0),
    d_blinkCaret(false),
    d_caretBlinkTimeout(DefaultCaretBlinkTimeout),
    d_caretBlinkElapsed(0.0f),
    d_showCaret(true),
    d_textFormatting(HTF_LEFT_ALIGNED)
{
    registerProperty(&d_blinkCaretProperty);
    registerProperty(&d_blinkCaretTimeoutProperty);
    registerProperty(&d_textFormattingProperty);
}

//----------------------------------------------------------------------------//
void FalagardEditbox::render()
{
    const WidgetLookFeel& wlf = getLookNFeel();

    renderBaseImagery(wlf);

    // no font == no more rendering
    Font* const font = d_window->getFont();
    if (!font)
        return;

    String visual_text;
    setupVisualString(visual_text);

    const ImagerySection& caret_imagery = wlf.getImagerySection("Carat");

    // get destination area for text
    const Rect text_area(wlf.getNamedArea("TextArea").getArea().getPixelRect(*d_window));

    const size_t caret_index = getCaretIndex(visual_text);
    const float extent_to_caret = font->getTextExtent(visual_text.substr(0, caret_index));
    const float caret_width = caret_imagery.getBoundingRect(*d_window, text_area).getWidth();
    const float text_extent = font->getTextExtent(visual_text);
    const float text_offset = calculateTextOffset(text_area, text_extent, caret_width, extent_to_caret);

#ifdef CEGUI_BIDI_SUPPORT
    renderTextBidi(wlf, visual_text, text_area, text_offset);
#else
    renderTextNoBidi(wlf, visual_text, text_area, text_offset);
#endif

    // remember this for next time.
    d_lastTextOffset = text_offset;

    renderCaret(caret_imagery, text_area, text_offset, extent_to_caret);
}

//----------------------------------------------------------------------------//
void FalagardEditbox::renderBaseImagery(const WidgetLookFeel& wlf) const
{
    Editbox* w = static_cast<Editbox*>(d_window);

    const StateImagery* imagery = &wlf.getStateImagery(
        w->isDisabled() ? "Disabled" : (w->isReadOnly() ? "ReadOnly" : "Enabled"));

    imagery->render(*w);
}

//----------------------------------------------------------------------------//
void FalagardEditbox::setupVisualString(String& visual) const
{
    Editbox* w = static_cast<Editbox*>(d_window);

    if (w->isTextMasked())
        visual.assign(w->getText().length(), w->getMaskCodePoint());
    else
        visual.assign(w->getTextVisual());
}

//----------------------------------------------------------------------------//
size_t FalagardEditbox::getCaretIndex(const String& visual_text) const
{
    Editbox* w = static_cast<Editbox*>(d_window);

    size_t caretIndex = w->getCaratIndex();

#ifdef CEGUI_BIDI_SUPPORT
    // the char before the caret bidi type
    bool currCharIsRtl = false;
    if ((visual_text.size() > 0) && (caretIndex > 0))
    {
        size_t curCaretIndex = w->getCaratIndex();
        BidiCharType charBeforeCaretType = w->getBiDiVisualMapping()->
            getBidiCharType(visual_text[curCaretIndex - 1]);
        // for neutral chars you decide by the char after
        for (; BCT_NEUTRAL == charBeforeCaretType &&
               (visual_text.size() > curCaretIndex); curCaretIndex++)
        {
            charBeforeCaretType = w->getBiDiVisualMapping()->
                getBidiCharType(visual_text[curCaretIndex - 1]);
        }

        currCharIsRtl  = (BCT_RIGHT_TO_LEFT == charBeforeCaretType);
    }

    const bool isFirstChar = caretIndex == 0;

    // the pos is by the char before
    if (!isFirstChar)
        caretIndex--;

    // we need to find the caret pos by the logical to visual map
    if (w->getBiDiVisualMapping()->getV2lMapping().size() > caretIndex)
        caretIndex = w->getBiDiVisualMapping()->getL2vMapping()[caretIndex];

    // for non RTL char - the caret pos is after the char
    if (!currCharIsRtl)
        caretIndex++;

    // if first char is not rtl - we need to stand at the start of the line
    if (isFirstChar)
    {
        bool firstCharRtl =
            (visual_text.size() > 0) &&
            (BCT_RIGHT_TO_LEFT == w->getBiDiVisualMapping()->
                getBidiCharType(visual_text[0]));

        if (!firstCharRtl)
            caretIndex--;
    }
#endif

    return caretIndex;
}

//----------------------------------------------------------------------------//
float FalagardEditbox::calculateTextOffset(const Rect& text_area,
                                           const float text_extent,
                                           const float caret_width,
                                           const float extent_to_caret)
{
    // if carat is to the left of the box
    if ((d_lastTextOffset + extent_to_caret) < 0)
        return -extent_to_caret;

    // if carat is off to the right.
    if ((d_lastTextOffset + extent_to_caret) >= (text_area.getWidth() - caret_width))
        return text_area.getWidth() - extent_to_caret - caret_width;

    // handle formatting of text when it's shorter than the available space
    if (text_extent < text_area.getWidth())
    {
        if (d_textFormatting == HTF_CENTRE_ALIGNED)
            return (text_area.getWidth() - text_extent) / 2;

        if (d_textFormatting == HTF_RIGHT_ALIGNED)
            return text_area.getWidth() - text_extent;
    }

    // no change to text position; re-use last offset value.
    return d_lastTextOffset;
}

//----------------------------------------------------------------------------//
void FalagardEditbox::renderTextNoBidi(const WidgetLookFeel& wlf,
                                       const String& text,
                                       const Rect& text_area,
                                       float text_offset)
{
    Font* const font = d_window->getFont();

    // setup initial rect for text formatting
    Rect text_part_rect(text_area);
    // allow for scroll position
    text_part_rect.d_left += text_offset;
    // centre text vertically within the defined text area
    text_part_rect.d_top += (text_area.getHeight() - font->getFontHeight()) * 0.5f;

    ColourRect colours;
    const float alpha_comp = d_window->getEffectiveAlpha();
    // get unhighlighted text colour (saves accessing property twice)
    const colour unselectedColour(getUnselectedTextColour());
    // see if the editbox is active or inactive.
    Editbox* const w = static_cast<Editbox*>(d_window);
    const bool active = editboxIsFocussed();

    if (w->getSelectionLength() != 0)
    {
        // calculate required start and end offsets of selection imagery.
        float selStartOffset =
            font->getTextExtent(text.substr(0, w->getSelectionStartIndex()));
        float selEndOffset =
            font->getTextExtent(text.substr(0, w->getSelectionEndIndex()));

        // calculate area for selection imagery.
        Rect hlarea(text_area);
        hlarea.d_left += text_offset + selStartOffset;
        hlarea.d_right = hlarea.d_left + (selEndOffset - selStartOffset);

        // render the selection imagery.
        wlf.getStateImagery(active ? "ActiveSelection" :
                                     "InactiveSelection").
            render(*w, hlarea, 0, &text_area);
    }

    // draw pre-highlight text
    String sect = text.substr(0, w->getSelectionStartIndex());
    colours.setColours(unselectedColour);
    colours.modulateAlpha(alpha_comp);
    font->drawText(w->getGeometryBuffer(), sect, text_part_rect.getPosition(),
                   &text_area, colours);

    // adjust rect for next section
    text_part_rect.d_left += font->getTextExtent(sect);

    // draw highlight text
    sect = text.substr(w->getSelectionStartIndex(), w->getSelectionLength());
    colours.setColours(getSelectedTextColour());
    colours.modulateAlpha(alpha_comp);
    font->drawText(w->getGeometryBuffer(), sect, text_part_rect.getPosition(),
                   &text_area, colours);

    // adjust rect for next section
    text_part_rect.d_left += font->getTextExtent(sect);

    // draw post-highlight text
    sect = text.substr(w->getSelectionEndIndex());
    colours.setColours(unselectedColour);
    colours.modulateAlpha(alpha_comp);
    font->drawText(w->getGeometryBuffer(), sect, text_part_rect.getPosition(),
                   &text_area, colours);
}

//----------------------------------------------------------------------------//
void FalagardEditbox::renderTextBidi(const WidgetLookFeel& wlf,
                                     const String& text,
                                     const Rect& text_area,
                                     float text_offset)
{
#ifdef CEGUI_BIDI_SUPPORT
    Font* const font = d_window->getFont();

    // setup initial rect for text formatting
    Rect text_part_rect(text_area);
    // allow for scroll position
    text_part_rect.d_left += text_offset;
    // centre text vertically within the defined text area
    text_part_rect.d_top += (text_area.getHeight() - font->getFontHeight()) * 0.5f;

    ColourRect colours;
    const float alpha_comp = d_window->getEffectiveAlpha();
    // get unhighlighted text colour (saves accessing property twice)
    const colour unselectedColour(getUnselectedTextColour());
    // see if the editbox is active or inactive.
    Editbox* const w = static_cast<Editbox*>(d_window);
    const bool active = editboxIsFocussed();

    if (w->getSelectionLength() == 0)
    {
        // no highlighted text - we can draw the whole thing
        colours.setColours(unselectedColour);
        colours.modulateAlpha(alpha_comp);
        font->drawText(w->getGeometryBuffer(), text,
                       text_part_rect.getPosition(), &text_area, colours);

        // adjust rect for next section
        text_part_rect.d_left += font->getTextExtent(text);

    }
    else
    {
        // there is highlighted text - because of the BiDi support - the
        // highlighted area can be in some cases nonconsecutive.
        // So - we need to draw it char by char (I guess we can optimize it more
        // but this is not that big performance hit because it only happens if
        // we have highlighted text - not that common...)
        for (size_t i = 0 ; i < text.size() ; i++)
        {
            // get the char
            String currChar = text.substr(i, 1);
            size_t realPos = 0;

            // get he visual pos of the char
            if (w->getBiDiVisualMapping()->getV2lMapping().size() > i)
            {
                realPos = w->getBiDiVisualMapping()->getV2lMapping()[i];
            }

            // check if it is in the highlighted region
            bool highlighted =
                realPos >= w->getSelectionStartIndex() &&
                realPos < w->getSelectionStartIndex() + w->getSelectionLength();

            float charAdvance = font->getGlyphData(currChar[0])->getAdvance(1.0f);

            if (highlighted)
            {
                colours.setColours(getSelectedTextColour());
                colours.modulateAlpha(alpha_comp);

                {

                    // calculate area for selection imagery.
                    Rect hlarea(text_area);
                    hlarea.d_left = text_part_rect.d_left ;
                    hlarea.d_right = text_part_rect.d_left + charAdvance ;

                    // render the selection imagery.
                    wlf.getStateImagery(active ? "ActiveSelection" :
                                                 "InactiveSelection").
                        render(*w, hlarea, 0, &text_area);
                }

            }
            else
            {
                colours.setColours(unselectedColour);
                colours.modulateAlpha(alpha_comp);
            }
            font->drawText(w->getGeometryBuffer(), currChar,
                           text_part_rect.getPosition(), &text_area, colours);

            // adjust rect for next section
            text_part_rect.d_left += charAdvance;

        }
    }
#endif
}

//----------------------------------------------------------------------------//
bool FalagardEditbox::editboxIsFocussed() const
{
    Editbox* const w = static_cast<Editbox*>(d_window);
    return (!w->isReadOnly()) && w->hasInputFocus();
}

//----------------------------------------------------------------------------//
void FalagardEditbox::renderCaret(const ImagerySection& imagery,
                                  const Rect& text_area,
                                  const float text_offset,
                                  const float extent_to_caret) const
{
    if (editboxIsFocussed() && (!d_blinkCaret || d_showCaret))
    {
        Rect caratRect(text_area);
        caratRect.d_left += extent_to_caret + text_offset;

        imagery.render(*d_window, caratRect, 0, &text_area);
    }
}

//----------------------------------------------------------------------------//
size_t FalagardEditbox::getTextIndexFromPosition(const Point& pt) const
{
    Editbox* w = static_cast<Editbox*>(d_window);

    // calculate final window position to be checked
    float wndx = CoordConverter::screenToWindowX(*w, pt.d_x);

    wndx -= d_lastTextOffset;

    // Return the proper index
    if (w->isTextMasked())
        return w->getFont()->getCharAtPixel(
                String(w->getTextVisual().length(), w->getMaskCodePoint()),
                wndx);
    else
        return w->getFont()->getCharAtPixel(w->getTextVisual(), wndx);
}

//----------------------------------------------------------------------------//
colour FalagardEditbox::getOptionalPropertyColour(
    const String& propertyName) const
{
    if (d_window->isPropertyPresent(propertyName))
        return PropertyHelper::stringToColour(
            d_window->getProperty(propertyName));
    else
        return colour(0, 0, 0);
}

//----------------------------------------------------------------------------//
colour FalagardEditbox::getUnselectedTextColour() const
{
    return getOptionalPropertyColour(UnselectedTextColourPropertyName);
}

//----------------------------------------------------------------------------//
colour FalagardEditbox::getSelectedTextColour() const
{
    return getOptionalPropertyColour(SelectedTextColourPropertyName);
}

//----------------------------------------------------------------------------//
void FalagardEditbox::update(float elapsed)
{
    // do base class stuff
    WindowRenderer::update(elapsed);

    // only do the update if we absolutely have to
    if (d_blinkCaret &&
        !static_cast<Editbox*>(d_window)->isReadOnly() &&
        static_cast<Editbox*>(d_window)->hasInputFocus())
    {
        d_caretBlinkElapsed += elapsed;

        if (d_caretBlinkElapsed > d_caretBlinkTimeout)
        {
            d_caretBlinkElapsed = 0.0f;
            d_showCaret ^= true;
            // state changed, so need a redraw
            d_window->invalidate();
        }
    }
}

//----------------------------------------------------------------------------//
bool FalagardEditbox::isCaretBlinkEnabled() const
{
    return d_blinkCaret;
}

//----------------------------------------------------------------------------//
float FalagardEditbox::getCaretBlinkTimeout() const
{
    return d_caretBlinkTimeout;
}

//----------------------------------------------------------------------------//
void FalagardEditbox::setCaretBlinkEnabled(bool enable)
{
    d_blinkCaret = enable;
}

//----------------------------------------------------------------------------//
void FalagardEditbox::setCaretBlinkTimeout(float seconds)
{
    d_caretBlinkTimeout = seconds;
}

//----------------------------------------------------------------------------//
void FalagardEditbox::setTextFormatting(const HorizontalTextFormatting format)
{
    if (isUnsupportedFormat(format))
        CEGUI_THROW(InvalidRequestException(
            "FalagardEditbox::setTextFormatting: currently only "
            "HTF_LEFT_ALIGNED, HTF_RIGHT_ALIGNED and HTF_CENTRE_ALIGNED "
            "are accepted for Editbox formatting"));

    d_textFormatting = format;
    d_window->invalidate();
}

//----------------------------------------------------------------------------//
bool FalagardEditbox::isUnsupportedFormat(const HorizontalTextFormatting format)
{
    return !(format == HTF_LEFT_ALIGNED ||
             format == HTF_RIGHT_ALIGNED ||
             format == HTF_CENTRE_ALIGNED);
}

//----------------------------------------------------------------------------//
HorizontalTextFormatting FalagardEditbox::getTextFormatting() const
{
    return d_textFormatting;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
