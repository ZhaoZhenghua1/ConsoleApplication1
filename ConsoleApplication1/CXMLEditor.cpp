#include "CXMLEditor.h"


CXMLEditor::CXMLEditor()
{
}

CXMLEditor::~CXMLEditor()
{
	// add a custom declaration node
	pugi::xml_node decl = m_doc.prepend_child(pugi::node_declaration);
	decl.append_attribute("version") = "1.0";
	decl.append_attribute("encoding") = "UTF-16";

	m_doc.save_file(m_strFileName.c_str(), PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_utf16);
}

CXMLEditor::EError CXMLEditor::GoTo(const char_t* szRelativePath)
{
	SafeString(szRelativePath);
	//判断路径是否有效
	if (m_doc.select_node(szRelativePath))
	{
		//有效定位则记录路径
		m_strPath = szRelativePath;
		return NoError;
	}
	return CanNotFind;
}

//打开文件
bool CXMLEditor::Open(const char_t* szFile)
{
	SafeString(szFile);
	m_strFileName = szFile;
	return m_doc.load_file(szFile, pugi::parse_default, pugi::encoding_utf16);
}

//像XPath路径中添加节点，不设置值，返回节点XPath路径
string_t CXMLEditor::AppendNode(const char_t* szPath, const char_t* szNodeName)
{
	SafeString(szPath);
	SafeString(szNodeName);
	//获取路径节点，并判断路径是否有效
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return string_t();
	}
	//必须是一个有效节点
	if (pugi::xml_node node = pathNode.node())
	{
		//添加子节点
		pugi::xml_node nodeChild = node.append_child(szNodeName);
		//添加的节点相对于当前节点的路径
		string_t path = szNodeName;
		//添加在最后一个节点，获取添加节点的位置
		pugi::xpath_node_set nodeSet = node.select_nodes(path.c_str());
		//添加节点后生成节点的有效XPath路径，主要是添加节点索引[]
		return string_t(szPath) + PUGIXML_TEXT("/") + path + PUGIXML_TEXT("[") + std::to_string(nodeSet.size()) + PUGIXML_TEXT("]");
	}
	else
	{
		return string_t();
	}
	return string_t();
}

const char_t* CXMLEditor::ReadAsString(const char_t* szPath, const char_t* def /*= PUGIXML_TEXT("")*/) const
{
	SafeString(szPath);
	SafeString(def);
	//获取节点
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return def;
	}
	//如果是节点
	if (pugi::xml_node node = pathNode.node())
	{
		return node.text().as_string(def);
	}
	//如果是属性
	else if (pugi::xml_attribute attr = pathNode.attribute())
	{
		return attr.as_string(def);
	}
	return def;
}

int CXMLEditor::ReadAsInt(const char_t* szPath, int def /*= 0*/) const
{
	SafeString(szPath);
	//获取节点
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return def;
	}
	//如果是节点
	if (pugi::xml_node node = pathNode.node())
	{
		return node.text().as_int(def);
	}
	//如果是属性
	else if (pugi::xml_attribute attr = pathNode.attribute())
	{
		return attr.as_int(def);
	}
	return def;
}

unsigned int CXMLEditor::ReadAsUint(const char_t* szPath, unsigned int def /*= 0*/) const
{
	SafeString(szPath);
	//获取节点
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return def;
	}
	//如果是节点
	if (pugi::xml_node node = pathNode.node())
	{
		return node.text().as_uint(def);
	}
	//如果是属性
	else if (pugi::xml_attribute attr = pathNode.attribute())
	{
		return attr.as_uint(def);
	}
	return def;
}

float CXMLEditor::ReadAsFloat(const char_t* szPath, float def /*= 0*/) const
{
	SafeString(szPath);
	//获取节点
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return def;
	}
	//如果是节点
	if (pugi::xml_node node = pathNode.node())
	{
		return node.text().as_float(def);
	}
	//如果是属性
	else if (pugi::xml_attribute attr = pathNode.attribute())
	{
		return attr.as_float(def);
	}
	return def;
}

long long CXMLEditor::ReadAsLlong(const char_t* szPath, long long def /*= 0*/) const
{
	SafeString(szPath);
	//获取节点
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return def;
	}
	//如果是节点
	if (pugi::xml_node node = pathNode.node())
	{
		return node.text().as_llong(def);
	}
	//如果是属性
	else if (pugi::xml_attribute attr = pathNode.attribute())
	{
		return attr.as_llong(def);
	}
	return def;
}

unsigned long long CXMLEditor::ReadAsUllong(const char_t* szPath, unsigned long long def /*= 0*/) const
{
	SafeString(szPath);
	//获取节点
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return def;
	}
	//如果是节点
	if (pugi::xml_node node = pathNode.node())
	{
		return node.text().as_ullong(def);
	}
	//如果是属性
	else if (pugi::xml_attribute attr = pathNode.attribute())
	{
		return attr.as_ullong(def);
	}
	return def;
}

bool CXMLEditor::ReadAsBool(const char_t* szPath, bool def /*= false*/) const
{
	SafeString(szPath);
	//获取节点
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return def;
	}
	//如果是节点
	if (pugi::xml_node node = pathNode.node())
	{
		return node.text().as_bool(def);
	}
	//如果是属性
	else if (pugi::xml_attribute attr = pathNode.attribute())
	{
		return attr.as_bool(def);
	}
	return def;
}

CXMLEditor::EError CXMLEditor::GetNode(const char_t* szPath, pugi::xpath_node& retNode)const
{
	SafeString(szPath);
	try
	{
		string_t desPath = m_strPath + szPath;
		//选中节点
		pugi::xpath_node_set pathNodes = m_doc.select_nodes(desPath.c_str());
		//无效路径
		if (0 == pathNodes.size())
		{
			return CanNotFind;
		}
		//歧义路径
		else if (pathNodes.size() > 1)
		{
			return MultiNodes;
		}
		//正确路径
		retNode = pathNodes.first();
		return NoError;
	}
	catch (const pugi::xpath_exception& )
	{
		return Exception;
	}
}

void CXMLEditor::SafeString(const char_t* & szStr)const
{
	szStr = szStr ? szStr : PUGIXML_TEXT("");
}
