#include "CXMLEditor.h"

int main()
{
	CXMLEditor editor;
	editor.Open("ab.xml");

	//判断是否存在根节点project
	if (CXMLEditor::NoError != editor.GetNode("/project"))
	{
		//不存在，则创建根节点
		editor.AppendNode("/", "project");
	}
	
	//定位
	editor.GoTo("/project");

	//判断/project下是否存在属性num
	if (CXMLEditor::NoError != editor.GetNode("/@num"))
	{
		//不存在属性num，则添加属性
		editor.AppendAttribute("/.","num",15);

		//向有效路径的属性中写入值，如果属性不存在，则无法写入
		editor.Write("/@num", 16);
	}

	//向project节点中添加投影仪节点
	string_t path = editor.AppendNode("/.", "projector");

	//投影仪节点的index属性设置为0
	editor.AppendAttribute(path.c_str(), "index", 0);

	//投影仪节点添加orient节点，节点值设置为0.31
	editor.AppendNode(path.c_str(), "orient", 0.31);

	//找到/project节点下所有的projector节点，如果节点的index值为0，则设置子节点orient的值为0.5
	//如果有多个节点满足条件，则会失败
	editor.Write("/projector[@index='0']/orient", 0.5);

	//判断有没有projector的index属性为1的节点
	if (CXMLEditor::NoError != editor.GetNode("/projector[@index='1']"))
	{
		//如果没有则创建节点
		string_t nodepath = editor.AppendNode("/.", "projector");
		//添加属性并设置值
		editor.AppendAttribute(nodepath.c_str(), "index", 1);
		//添加子节点并设置值
		editor.AppendNode(nodepath.c_str(), "location", "103,568");
	}

	//路径/project/projector/location的节点值如果为103,568，则在父节点/project/projector/中添加节点opened,值设置为FALSE
	editor.AppendNode("/projector/location[.='103,568']/..", "opened", false);

	//路径/project/projector/location的节点值如果为103,568，则在父节点/project/projector/中节点opened值设置为true
	editor.Write("/projector/location[.='103,568']/../opened", true);

	//读取值
	bool ret = editor.ReadAsBool("/projector/location[.='103,568']/../opened", false);
	return 0;
}
