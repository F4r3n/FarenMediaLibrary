#include "inspector/Body3DInspector.h"
#include "Components/CBody3D.h"
#include <imgui/imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Body3D, fmc::CBody3D)

void Body3DInspector::init()
{

}

void Body3DInspector::draw(bool *)
{
	if (ImGui::CollapsingHeader("Body3D"))
	{
		
	}
}


