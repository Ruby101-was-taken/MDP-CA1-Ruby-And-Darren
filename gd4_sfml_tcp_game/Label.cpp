#include "Label.hpp"
#include "resource_holder.hpp"

gui::Label::Label(const std::string& text, const FontHolder& fonts)
    :text_(fonts.Get(Font::kMain))
{
    text_.setString(text);
    text_.setCharacterSize(16);
}

bool gui::Label::IsSelectable() const
{
    return false;
}

void gui::Label::SetText(const std::string& text)
{
    text_.setString(text);
}

void gui::Label::HandleEvent(const sf::Event& event)
{
}

void gui::Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(text_, states);
}
