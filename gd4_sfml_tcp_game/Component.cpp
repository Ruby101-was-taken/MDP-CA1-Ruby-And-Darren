#include "Component.hpp"

namespace gui
{

    Component::Component()
        : is_selected_(false)
        , is_active_(false)
    {
    }

    Component::~Component()
    {
    }

    bool Component::IsSelected() const
    {
        return is_selected_;
    }

    void Component::Select()
    {
        is_selected_ = true;
    }

    void Component::Deselect()
    {
        is_selected_ = false;
    }

    bool Component::IsActive() const
    {
        return is_active_;
    }

    void Component::Activate()
    {
        is_active_ = true;
    }

    void Component::Deactivate()
    {
        is_active_ = false;
    }
}