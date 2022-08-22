#include "Test.h"
#include "imgui/imgui.h"

namespace test {

	test::TestMenu::TestMenu(Test*& CurrentTestPointer)
		:m_CurrentTest(CurrentTestPointer)
	{

	}

	void test::TestMenu::OnImGuiRender()
	{
		for (auto& test : m_Tests) {
			if (ImGui::Button(test.first.c_str()))
				m_CurrentTest = test.second();
		}
	}
}


