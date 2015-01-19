#include "script/python/UIModule.h"
#include "ui/UIWindow.h"
#include "render/Colour.h"

bool imgui_begin(std::string title, WindowOptions options = WindowOptions())
{
	const ImGuiWindowFlags layout_flags = (!options.titlebar ? ImGuiWindowFlags_NoTitleBar : 0) |
										  (options.border ? ImGuiWindowFlags_ShowBorders : 0) |
										  (!options.resize ? ImGuiWindowFlags_NoResize : 0) |
										  (!options.move ? ImGuiWindowFlags_NoMove : 0) |
										  (!options.scrollbar ? ImGuiWindowFlags_NoScrollbar : 0);

	return ImGui::Begin(title.c_str(),
						&options.is_open,
						ImVec2(options.position.x, options.position.y),
						options.fill_alpha,
						layout_flags);
}

bool imgui_treenode(std::string text)
{
	return ImGui::TreeNode(text.c_str());
}

void imgui_bullet_text(std::string text)
{
	ImGui::BulletText(text.c_str());
}

void imgui_text(std::string text)
{
	ImGui::Text(text.c_str());
}

void imgui_text_wrapped(std::string text)
{
	ImGui::TextWrapped(text.c_str());
}

void imgui_text_coloured(Colour colour, std::string text)
{
	ImVec4 col;
	col.x = colour.r;
	col.y = colour.g;
	col.z = colour.b;
	col.w = colour.a;

	ImGui::TextColored(col, text.c_str());
}

bool imgui_checkbox(std::string text, bool value)
{
	ImGui::Checkbox(text.c_str(), &value);
	return value;
}

float imgui_slider_float(std::string text, float value, float min, float max)
{
	ImGui::SliderFloat(text.c_str(), &value, min, max);
	return value;
}

int imgui_slider_int(std::string text, int value, int min, int max)
{
	ImGui::SliderInt(text.c_str(), &value, min, max);
	return value;
}

float imgui_slider_angle(std::string text, float value, float min, float max)
{
	ImGui::SliderAngle(text.c_str(), &value, min, max);
	return value;
}

bool imgui_button(std::string text, Vector2 size = Vector2(0.f, 0.f), bool repeat = false)
{
	ImVec2 s;
	s.x = size.x;
	s.y = size.y;
	return ImGui::Button(text.c_str(), s, repeat);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(imgui_button_overloads, imgui_button, 1, 3);
BOOST_PYTHON_FUNCTION_OVERLOADS(imgui_sameline_overloads, ImGui::SameLine, 0, 2);

int imgui_radio_button(std::string text, int selected, int button)
{
	ImGui::RadioButton(text.c_str(), &selected, button);
	return selected;
}

float imgui_input_float(std::string text, float value, float step = 0.1f, float step_fast = 1.0f, int decimal_precision = 3)
{
	ImGuiInputTextFlags extra_flags = 0;
	ImGui::InputFloat(text.c_str(), &value, step, step_fast, decimal_precision, extra_flags);
	return value;
}
BOOST_PYTHON_FUNCTION_OVERLOADS(imgui_input_float_overloads, imgui_input_float, 2, 5);

Vector2 imgui_input_float2(std::string text, Vector2 value, int decimal_precision = 3)
{
	float valuef[2] = { value.x, value.y };

	if (ImGui::InputFloat2(text.c_str(), valuef, decimal_precision))
	{
		value.x = valuef[0];
		value.y = valuef[1];
	}

	return value;
}
BOOST_PYTHON_FUNCTION_OVERLOADS(imgui_input_float2_overloads, imgui_input_float2, 2, 3);

Vector3 imgui_input_float3(std::string text, Vector3 value, int decimal_precision = 3)
{
	float valuef[3] = { value.x, value.y, value.z };

	if (ImGui::InputFloat3(text.c_str(), valuef, decimal_precision))
	{
		value.x = valuef[0];
		value.y = valuef[1];
		value.z = valuef[2];
	}

	return value;
}
BOOST_PYTHON_FUNCTION_OVERLOADS(imgui_input_float3_overloads, imgui_input_float3, 2, 3);

Vector4 imgui_input_float4(std::string text, Vector4 value, int decimal_precision = 3)
{
	float valuef[4] = { value.x, value.y, value.z, value.w };

	if (ImGui::InputFloat4(text.c_str(), valuef, decimal_precision))
	{
		value.x = valuef[0];
		value.y = valuef[1];
		value.z = valuef[2];
		value.w = valuef[3];
	}

	return value;
}
BOOST_PYTHON_FUNCTION_OVERLOADS(imgui_input_float4_overloads, imgui_input_float4, 2, 3);

Colour imgui_colour_edit4(std::string text, Colour colour, bool alpha = true)
{
	float col[4] = { colour.r, colour.g, colour.b, colour.a };
	if (ImGui::ColorEdit4(text.c_str(), col, alpha))
	{
		colour.r = col[0];
		colour.g = col[1];
		colour.b = col[2];
		colour.a = col[3];
	}
	return colour;
}
BOOST_PYTHON_FUNCTION_OVERLOADS(imgui_colour_edit4_overloads, imgui_colour_edit4, 2, 3);

void imgui_tooltip(std::string text)
{
	ImGui::SetTooltip(text.c_str());
}

void initUI()
{
	using namespace epsilon;

	object uiModule(handle<>(borrowed(PyImport_AddModule("epsilon.ui"))));
	scope().attr("ui") = uiModule;
	scope uiScope = uiModule;

	// Expose the UIWindow Class
	class_<UIWindow, UIWindow::Ptr, boost::noncopyable>("UIWindow", no_init)
		.def("title", &UIWindow::GetTitle)
		.add_property("position", &UIWindow::GetPosition, &UIWindow::SetPosition)
		.add_property("open", &UIWindow::IsOpen, &UIWindow::OpenWindow)
	;

	class_<WindowOptions>("WindowOptions")
		.def_readwrite("is_open", &WindowOptions::is_open)
		.def_readwrite("title", &WindowOptions::titlebar)
		.def_readwrite("border", &WindowOptions::border)
		.def_readwrite("resize", &WindowOptions::resize)
		.def_readwrite("move", &WindowOptions::move)
		.def_readwrite("scrollbar", &WindowOptions::scrollbar)
		.def_readwrite("fill_alpha", &WindowOptions::fill_alpha)
		.def_readwrite("position", &WindowOptions::position)
	;

	// Expose IMGUI Class
	object imguiModule(handle<>(borrowed(PyImport_AddModule("epsilon.ui.IMGUI"))));
	uiScope.attr("IMGUI") = imguiModule;
	scope imguiScope = imguiModule;

	def("begin", imgui_begin);
	def("end", ImGui::End);

	def("help", ImGui::ShowUserGuide);

	// Layout
	def("spacing", ImGui::Spacing);
	def("separator", ImGui::Separator);

	def("same_line", ImGui::SameLine, imgui_sameline_overloads());

	def("collapsing_header", ImGui::CollapsingHeader);

	def("tree_node", imgui_treenode);
	def("tree_pop", ImGui::TreePop);

	// Controls
	def("text", imgui_text);
	def("text_wrapped", imgui_text_wrapped);
	def("text_coloured", imgui_text_coloured);
	def("bullet_text", imgui_bullet_text);
	
	// Control Query
	def("is_item_hovered", ImGui::IsItemHovered);
	def("is_item_focused", ImGui::IsItemFocused);
	def("set_tooltip", imgui_tooltip);

	// Input
	def("button", imgui_button, imgui_button_overloads());
	def("small_button", ImGui::SmallButton);
	def("radio_button", imgui_radio_button);
	
	def("checkbox", imgui_checkbox);
	def("slider_float", imgui_slider_float);
	def("slider_int", imgui_slider_int);
	def("slider_angle", imgui_slider_angle);

	def("input_float", imgui_input_float, imgui_input_float_overloads());
	def("input_float2", imgui_input_float2, imgui_input_float2_overloads());
	def("input_float3", imgui_input_float3, imgui_input_float3_overloads());
	def("input_float4", imgui_input_float4, imgui_input_float4_overloads());

	def("colour_edit4", imgui_colour_edit4, imgui_colour_edit4_overloads());

}