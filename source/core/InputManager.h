#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Event.hpp>

#include <vector>
#include "math/Vector.h"
#include "render/RenderManager.h"

namespace epsilon
{
	class InputManager
	{
	private:
		typedef std::vector<sf::Keyboard::Key> InputKeyState;
		typedef std::vector<sf::Mouse::Button> InputButtonState;

		InputManager(void)
		{
			sf::Vector2i mp = sf::Mouse::getPosition();
			mousePosLast = Vector2(mp.x, mp.y);

			rm = &RenderManager::GetInstance();
		};
		InputManager(InputManager const&);
		void operator=(InputManager const&);
		
	public:
		static InputManager & GetInstance()
		{
			static InputManager instance;
			return instance;
		};
		~InputManager(void) {};

		void OnFrameStart()
		{
			// When frame starts reset key state

			memset(keyDown, false, sizeof(keyDown));
			memset(keyUp, false, sizeof(keyUp));

			memset(buttonDown, false, sizeof(buttonDown));
			memset(buttonUp, false, sizeof(buttonUp));

			// calculate window relative mouse position values
			sf::Vector2i mp = sf::Mouse::getPosition(*rm->GetWindow());

			mousePosRelative = Vector2(mp.x, mp.y) - mousePosLast;
			mousePosLast = Vector2(mp.x, mp.y);
		}

		void MouseVisible(bool visible)
		{
			rm->GetWindow()->setMouseCursorVisible(visible);
		}

		void MousePosition(Vector2 nMousePos)
		{
			sf::Mouse::setPosition(sf::Vector2i(nMousePos.x, nMousePos.y), *rm->GetWindow());
		}

		// Process Input events raised by the window
		void ProcessEvent(sf::Event &event)
		{
			switch(event.type)
			{
				case sf::Event::KeyPressed:
					keyDown[event.key.code] = true;
					break;
				case sf::Event::KeyReleased:
					keyUp[event.key.code] = true;
					break;

				case sf::Event::MouseButtonPressed:
					buttonDown[event.mouseButton.button] = true;
					break;
				
				case sf::Event::MouseButtonReleased:
					buttonUp[event.mouseButton.button] = true;
					break;
				default:
					break;
			}
		}

		// Keyboard
		static bool GetKey(sf::Keyboard::Key key)
		{
			// For this we just need to query SFML
			return sf::Keyboard::isKeyPressed(key);
		};

		static bool GetKeyDown(sf::Keyboard::Key key)
		{
			return InputManager::GetInstance().keyDown[key];
		};

		static bool GetKeyUp(sf::Keyboard::Key key)
		{
			return InputManager::GetInstance().keyUp[key];
		};

		// Mouse
		static bool GetMouseButton(sf::Mouse::Button button)
		{
			return sf::Mouse::isButtonPressed(button);
		};

		static bool GetMouseButtonDown(sf::Mouse::Button button)
		{
			return InputManager::GetInstance().buttonDown[button];
		};

		static bool GetMouseButtonUp(sf::Mouse::Button button)
		{
			return InputManager::GetInstance().buttonUp[button];
		};

		static Vector2 GetMousePosition()
		{
			return InputManager::GetInstance().mousePosLast;
		};

		static Vector2 GetMousePositionRelative()
		{
			return InputManager::GetInstance().mousePosRelative;
		};

		static void SetMouseVisible(bool visible)
		{
			InputManager::GetInstance().MouseVisible(visible);
		}

		static void SetMousePosition(Vector2 mousePos)
		{
			InputManager::GetInstance().MousePosition(mousePos);
		}

		// TODO: Implement AXIS handling for controllers

	private:
		bool keyDown[100];
		bool keyUp[100];

		bool buttonDown[6];
		bool buttonUp[6];

		Vector2 mousePosLast;
		Vector2 mousePosRelative;

		RenderManager * rm;
	};
}

