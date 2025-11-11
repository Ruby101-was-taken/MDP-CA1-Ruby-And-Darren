#include "settings_state.hpp"
#include "resource_holder.hpp"
#include "utility.hpp"

SettingsState::SettingsState(StateStack& stack, Context context)
	: State(stack, context)
	, m_gui_container()
	, m_background_sprite(context.textures->Get(TextureID::kTitleScreen))
{
	//m_background_sprite.setTexture();

	//Build key binding buttons and labels
	AddButtonLabel(Action::kMoveUp, 150.f, "Move Up", context);
	AddButtonLabel(Action::kMoveDown, 200.f, "Move Down", context);
	AddButtonLabel(Action::kMoveRight, 250.f, "Move Right", context);
	AddButtonLabel(Action::kMoveLeft, 300.f, "Move Left", context);
	AddButtonLabel(Action::kBulletFire, 350.f, "Fire", context);
	AddButtonLabel(Action::kMissileFire, 400.f, "Missile Fire", context);

	UpdateLabels();

	auto back_button = std::make_shared<gui::Button>(context);
	back_button->setPosition({ 80.f, 475.f });
	back_button->SetText("Back");
	back_button->SetCallback(std::bind(&SettingsState::RequestStackPop, this));
	m_gui_container.Pack(back_button);
}

void SettingsState::Draw()
{
	sf::RenderWindow& window = *GetContext().window;
	window.draw(m_background_sprite);
	window.draw(m_gui_container);
}

bool SettingsState::Update(sf::Time dt)
{
	return true;
}

bool SettingsState::HandleEvent(const sf::Event& event)
{
	bool is_key_binding = false;

	//Iterate through all of the key binding buttons to see if they are being presssed, waiting for the user to enter a key
	for (std::size_t action = 0; action < static_cast<int>(Action::kActionCount); ++action)
	{
		if (binding_buttons_[action]->IsActive())
		{
			is_key_binding = true;
			if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
			{
				GetContext().player->AssignKey(static_cast<Action>(action), keyReleased->code);
				
				binding_buttons_[action]->Deactivate();
			}
			break;
		}
	}

	//If pressed button changed key bindings, then update the labels
	if (is_key_binding)
	{
		UpdateLabels();
	}
	else
	{
		m_gui_container.HandleEvent(event);
	}
	return false;
}

void SettingsState::UpdateLabels()
{
	Player& player = *GetContext().player;
	for (std::size_t i = 0; i < static_cast<int>(Action::kActionCount); ++i)
	{
		sf::Keyboard::Key key = player.GetAssignedKey(static_cast<Action>(i));
		binding_labels_[i]->SetText(Utility::toString(key));
	}
}

void SettingsState::AddButtonLabel(Action action, float y, const std::string& text, Context context)
{
	binding_buttons_[static_cast<int>(action)] = std::make_shared<gui::Button>(context);
	binding_buttons_[static_cast<int>(action)]->setPosition({ 80.f, y });
	binding_buttons_[static_cast<int>(action)]->SetText(text);
	binding_buttons_[static_cast<int>(action)]->SetToggle(true);

	binding_labels_[static_cast<int>(action)] = std::make_shared<gui::Label>("", *context.fonts);
	binding_labels_[static_cast<int>(action)]->setPosition({ 300.f, y + 15.f });

	m_gui_container.Pack(binding_buttons_[static_cast<int>(action)]);
	m_gui_container.Pack(binding_labels_[static_cast<int>(action)]);
}
