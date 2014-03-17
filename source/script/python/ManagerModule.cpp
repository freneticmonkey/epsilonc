#include "script/python/ManagerModule.h"
#include "events/EventManager.h"
#include "core/InputManager.h"
#include "render/RenderManager.h"
#include "render/gizmos/GizmoManager.h"
#include "scene/SceneManager.h"
#include "script/ScriptManager.h"
#include "script/Script.h"
#include "ui/UIManager.h"

void initManagers()
{
	// No namespace business here. These are going straight into 
	// the global (epsilon) namespace

	// Inject the singletons straight into the root namespace to remove get_instance calls

	object package = scope();
	
	class_<EventManager, boost::noncopyable>("EventManager", no_init)
		.def("get_instance", &EventManager::GetInstance, return_value_policy<reference_existing_object>())
		.staticmethod("get_instance")

		.def("add_listener", &EventManager::AddListener)
		.staticmethod("add_listener")

		.def("remove_listener", &EventManager::RemoveListener)
		.staticmethod("remove_listener")

		.def("fire_event", &EventManager::FireEvent)
		.staticmethod("fire_event")
	;
	
	object sceneManager = class_<SceneManager, boost::noncopyable>("SceneManager", no_init)
		.def("get_instance", &SceneManager::GetInstance, return_value_policy<reference_existing_object>())
		.staticmethod("get_instance")

		.def("set_scene", (void (SceneManager::*)(Scene::Ptr) )(&SceneManager::SetScene) )
		.def("set_scene", (void (SceneManager::*)(std::string) )(&SceneManager::SetScene) )

		.def_readonly("current_scene", &SceneManager::CurrentScene)
	;
	// Injecting the get instance result into the namespace
	object smGI = sceneManager.attr("get_instance");
	package.attr("SceneManager") = smGI();

	Script::Ptr (ScriptManager::*CreateScriptStandard)(void) = &ScriptManager::CreateScript;
	Script::Ptr (ScriptManager::*CreateScriptName)(std::string) = &ScriptManager::CreateScript;

	object scriptManager = class_<ScriptManager, boost::noncopyable>("ScriptManager", no_init)
		.def("get_instance", &ScriptManager::GetInstance, return_value_policy<reference_existing_object>())
		.staticmethod("get_instance")

		//.def("create_script", (Script::Ptr (ScriptManager::*)
		.def("create_script", CreateScriptStandard)
		.def("create_script", CreateScriptName )

		.def("create_behaviour", &ScriptManager::CreateBehaviour)

		.def("reload_script", &ScriptManager::ReloadScript)
	;
	// Injecting the get instance result into the namespace
	smGI = scriptManager.attr("get_instance");
	package.attr("ScriptManager") = smGI();

	object renderManager = class_<RenderManager, boost::noncopyable>("RenderManager", no_init)
		.def("get_instance", &RenderManager::GetInstance, return_value_policy<reference_existing_object>())
		.staticmethod("get_instance")

		.def("close_window", &RenderManager::CloseWindow)
	;
	// Injecting the get instance result into the namespace
	smGI = renderManager.attr("get_instance");
	package.attr("RenderManager") = smGI();

	object inputManager = class_<InputManager, boost::noncopyable>("Input", no_init)
		.def("get_instance", &InputManager::GetInstance, return_value_policy<reference_existing_object>())
		.staticmethod("get_instance")

		.def("key_down", &InputManager::GetKeyDown)
		.staticmethod("key_down")

		.def("key", &InputManager::GetKey)
		.staticmethod("key")

		.def("key_up", &InputManager::GetKeyUp)
		.staticmethod("key_up")

		.def("mouse_button", &InputManager::GetMouseButton)
		.staticmethod("mouse_button")

		.def("mouse_button_down", &InputManager::GetMouseButtonDown)
		.staticmethod("mouse_button_down")

		.def("mouse_button_up", &InputManager::GetMouseButtonUp)
		.staticmethod("mouse_button_up")

		.def("mouse_position", &InputManager::GetMousePosition)
		.staticmethod("mouse_position")

		.def("set_mouse_position", &InputManager::SetMousePosition)
		.staticmethod("set_mouse_position")

		.def("mouse_position_relative", &InputManager::GetMousePositionRelative)
		.staticmethod("mouse_position_relative")

		.def("mouse_visible", &InputManager::SetMouseVisible)
		.staticmethod("mouse_visible")
	;
	// Injecting the get instance result into the namespace
	smGI = inputManager.attr("get_instance");
	package.attr("Input") = smGI();

	// Scopoing the Key Values to the Input class

	scope input_scope = inputManager;

	enum_<sf::Keyboard::Key>("Key")
      .value("A"         , sf::Keyboard::Key::A)
      .value("B"         , sf::Keyboard::Key::B)
      .value("C"         , sf::Keyboard::Key::C)
      .value("D"         , sf::Keyboard::Key::D)
      .value("E"         , sf::Keyboard::Key::E)
      .value("F"         , sf::Keyboard::Key::F)
      .value("G"         , sf::Keyboard::Key::G)
      .value("H"         , sf::Keyboard::Key::H)
      .value("I"         , sf::Keyboard::Key::I)
      .value("J"         , sf::Keyboard::Key::J)
      .value("K"         , sf::Keyboard::Key::K)
      .value("L"         , sf::Keyboard::Key::L)
      .value("M"         , sf::Keyboard::Key::M)
      .value("N"         , sf::Keyboard::Key::N)
      .value("O"         , sf::Keyboard::Key::O)
      .value("P"         , sf::Keyboard::Key::P)
      .value("Q"         , sf::Keyboard::Key::Q)
      .value("R"         , sf::Keyboard::Key::R)
      .value("S"         , sf::Keyboard::Key::S)
      .value("T"         , sf::Keyboard::Key::T)
      .value("U"         , sf::Keyboard::Key::U)
      .value("V"         , sf::Keyboard::Key::V)
      .value("W"         , sf::Keyboard::Key::W)
      .value("X"         , sf::Keyboard::Key::X)
      .value("Y"         , sf::Keyboard::Key::Y)
      .value("Z"         , sf::Keyboard::Key::Z)
      .value("Num0"      , sf::Keyboard::Key::Num0)
      .value("Num1"      , sf::Keyboard::Key::Num1)
      .value("Num2"      , sf::Keyboard::Key::Num2)
      .value("Num3"      , sf::Keyboard::Key::Num3)
      .value("Num4"      , sf::Keyboard::Key::Num4)
      .value("Num5"      , sf::Keyboard::Key::Num5)
      .value("Num6"      , sf::Keyboard::Key::Num6)
      .value("Num7"      , sf::Keyboard::Key::Num7)
      .value("Num8"      , sf::Keyboard::Key::Num8)
      .value("Num9"      , sf::Keyboard::Key::Num9)
      .value("Escape"    , sf::Keyboard::Key::Escape)
      .value("LControl"  , sf::Keyboard::Key::LControl)
      .value("LShift"    , sf::Keyboard::Key::LShift)
      .value("LAlt"      , sf::Keyboard::Key::LAlt)
      .value("LSystem"   , sf::Keyboard::Key::LSystem)
      .value("RControl"  , sf::Keyboard::Key::RControl)
      .value("RShift"    , sf::Keyboard::Key::RShift)
      .value("RAlt"      , sf::Keyboard::Key::RAlt)
      .value("RSystem"   , sf::Keyboard::Key::RSystem)
      .value("Menu"      , sf::Keyboard::Key::Menu)
      .value("LBracket"  , sf::Keyboard::Key::LBracket)
      .value("RBracket"  , sf::Keyboard::Key::RBracket)
      .value("SemiColon" , sf::Keyboard::Key::SemiColon)
      .value("Comma"     , sf::Keyboard::Key::Comma)
      .value("Period"    , sf::Keyboard::Key::Period)
      .value("Quote"     , sf::Keyboard::Key::Quote)
      .value("Slash"     , sf::Keyboard::Key::Slash)
      .value("BackSlash" , sf::Keyboard::Key::BackSlash)
      .value("Tilde"     , sf::Keyboard::Key::Tilde)
      .value("Equal"     , sf::Keyboard::Key::Equal)
      .value("Dash"      , sf::Keyboard::Key::Dash)
      .value("Space"     , sf::Keyboard::Key::Space)
      .value("Return"    , sf::Keyboard::Key::Return)
      .value("BackSpace" , sf::Keyboard::Key::BackSpace)
      .value("Tab"       , sf::Keyboard::Key::Tab)
      .value("PageUp"    , sf::Keyboard::Key::PageUp)
      .value("PageDown"  , sf::Keyboard::Key::PageDown)
      .value("End"       , sf::Keyboard::Key::End)
      .value("Home"      , sf::Keyboard::Key::Home)
      .value("Insert"    , sf::Keyboard::Key::Insert)
      .value("Delete"    , sf::Keyboard::Key::Delete)
      .value("Add"       , sf::Keyboard::Key::Add)
      .value("Subtract"  , sf::Keyboard::Key::Subtract)
      .value("Multiply"  , sf::Keyboard::Key::Multiply)
      .value("Divide"    , sf::Keyboard::Key::Divide)
      .value("Left"      , sf::Keyboard::Key::Left)
      .value("Right"     , sf::Keyboard::Key::Right)
      .value("Up"        , sf::Keyboard::Key::Up)
      .value("Down"      , sf::Keyboard::Key::Down)
      .value("Numpad0"   , sf::Keyboard::Key::Numpad0)
      .value("Numpad1"   , sf::Keyboard::Key::Numpad1)
      .value("Numpad2"   , sf::Keyboard::Key::Numpad2)
      .value("Numpad3"   , sf::Keyboard::Key::Numpad3)
      .value("Numpad4"   , sf::Keyboard::Key::Numpad4)
      .value("Numpad5"   , sf::Keyboard::Key::Numpad5)
      .value("Numpad6"   , sf::Keyboard::Key::Numpad6)
      .value("Numpad7"   , sf::Keyboard::Key::Numpad7)
      .value("Numpad8"   , sf::Keyboard::Key::Numpad8)
      .value("Numpad9"   , sf::Keyboard::Key::Numpad9)
      .value("F1"        , sf::Keyboard::Key::F1)
      .value("F2"        , sf::Keyboard::Key::F2)
      .value("F3"        , sf::Keyboard::Key::F3)
      .value("F4"        , sf::Keyboard::Key::F4)
      .value("F5"        , sf::Keyboard::Key::F5)
      .value("F6"        , sf::Keyboard::Key::F6)
      .value("F7"        , sf::Keyboard::Key::F7)
      .value("F8"        , sf::Keyboard::Key::F8)
      .value("F9"        , sf::Keyboard::Key::F9)
      .value("F10"       , sf::Keyboard::Key::F10)
      .value("F11"       , sf::Keyboard::Key::F11)
      .value("F12"       , sf::Keyboard::Key::F12)
      .value("F13"       , sf::Keyboard::Key::F13)
      .value("F14"       , sf::Keyboard::Key::F14)
      .value("F15"       , sf::Keyboard::Key::F15)
      .value("Pause"     , sf::Keyboard::Key::Pause)
	;

	enum_<sf::Mouse::Button>("Button")
		.value("Left"     , sf::Mouse::Button::Left)
		.value("Right"    , sf::Mouse::Button::Right)
		.value("Middle"   , sf::Mouse::Button::Middle)
		.value("XButton1" , sf::Mouse::Button::XButton1)
		.value("XButton2" , sf::Mouse::Button::XButton2)
	;

	object uiManager = class_<UIManager, boost::noncopyable>("UIManager", no_init)
		.def("get_instance", &UIManager::GetInstance, return_value_policy<reference_existing_object>())
		.staticmethod("get_instance")

		.def("get_window", &UIManager::GetWindowByName)
		;

	// Injecting the get instance result into the namespace
	smGI = uiManager.attr("get_instance");
	package.attr("UIManager") = smGI();

	// Gizmos

	object gizmoManager = class_<GizmoManager, boost::noncopyable>("Gizmos", no_init)
		.def("get_instance", &GizmoManager::GetInstance, return_value_policy<reference_existing_object>())
		.staticmethod("get_instance")

		.def("colour", &GizmoManager::SetColour)
		.staticmethod("colour")

		.def("life", &GizmoManager::SetLife)
		.staticmethod("life")

		.def("draw_cube", &GizmoManager::DrawCube)
		.staticmethod("draw_cube")

		.def("draw_sphere", &GizmoManager::DrawSphere)
		.staticmethod("draw_sphere")
		;
	// Injecting Gizmo manager into the namespace
	smGI = gizmoManager.attr("get_instance");
	package.attr("Gizmos") = smGI();
}