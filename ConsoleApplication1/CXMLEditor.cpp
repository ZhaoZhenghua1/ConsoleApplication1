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
	//�ж�·���Ƿ���Ч
	if (m_doc.select_node(szRelativePath))
	{
		//��Ч��λ���¼·��
		m_strPath = szRelativePath;
		return NoError;
	}
	return CanNotFind;
}

//���ļ�
bool CXMLEditor::Open(const char_t* szFile)
{
	SafeString(szFile);
	m_strFileName = szFile;
	return m_doc.load_file(szFile, pugi::parse_default, pugi::encoding_utf16);
}

//��XPath·������ӽڵ㣬������ֵ�����ؽڵ�XPath·��
string_t CXMLEditor::AppendNode(const char_t* szPath, const char_t* szNodeName)
{
	SafeString(szPath);
	SafeString(szNodeName);
	//��ȡ·���ڵ㣬���ж�·���Ƿ���Ч
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return string_t();
	}
	//������һ����Ч�ڵ�
	if (pugi::xml_node node = pathNode.node())
	{
		//����ӽڵ�
		pugi::xml_node nodeChild = node.append_child(szNodeName);
		//��ӵĽڵ�����ڵ�ǰ�ڵ��·��
		string_t path = szNodeName;
		//��������һ���ڵ㣬��ȡ��ӽڵ��λ��
		pugi::xpath_node_set nodeSet = node.select_nodes(path.c_str());
		//��ӽڵ�����ɽڵ����ЧXPath·������Ҫ����ӽڵ�����[]
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
	//��ȡ�ڵ�
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return def;
	}
	//����ǽڵ�
	if (pugi::xml_node node = pathNode.node())
	{
		return node.text().as_string(def);
	}
	//���������
	else if (pugi::xml_attribute attr = pathNode.attribute())
	{
		return attr.as_string(def);
	}
	return def;
}

int CXMLEditor::ReadAsInt(const char_t* szPath, int def /*= 0*/) const
{
	SafeString(szPath);
	//��ȡ�ڵ�
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return def;
	}
	//����ǽڵ�
	if (pugi::xml_node node = pathNode.node())
	{
		return node.text().as_int(def);
	}
	//���������
	else if (pugi::xml_attribute attr = pathNode.attribute())
	{
		return attr.as_int(def);
	}
	return def;
}

unsigned int CXMLEditor::ReadAsUint(const char_t* szPath, unsigned int def /*= 0*/) const
{
	SafeString(szPath);
	//��ȡ�ڵ�
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return def;
	}
	//����ǽڵ�
	if (pugi::xml_node node = pathNode.node())
	{
		return node.text().as_uint(def);
	}
	//���������
	else if (pugi::xml_attribute attr = pathNode.attribute())
	{
		return attr.as_uint(def);
	}
	return def;
}

float CXMLEditor::ReadAsFloat(const char_t* szPath, float def /*= 0*/) const
{
	SafeString(szPath);
	//��ȡ�ڵ�
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return def;
	}
	//����ǽڵ�
	if (pugi::xml_node node = pathNode.node())
	{
		return node.text().as_float(def);
	}
	//���������
	else if (pugi::xml_attribute attr = pathNode.attribute())
	{
		return attr.as_float(def);
	}
	return def;
}

long long CXMLEditor::ReadAsLlong(const char_t* szPath, long long def /*= 0*/) const
{
	SafeString(szPath);
	//��ȡ�ڵ�
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return def;
	}
	//����ǽڵ�
	if (pugi::xml_node node = pathNode.node())
	{
		return node.text().as_llong(def);
	}
	//���������
	else if (pugi::xml_attribute attr = pathNode.attribute())
	{
		return attr.as_llong(def);
	}
	return def;
}

unsigned long long CXMLEditor::ReadAsUllong(const char_t* szPath, unsigned long long def /*= 0*/) const
{
	SafeString(szPath);
	//��ȡ�ڵ�
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return def;
	}
	//����ǽڵ�
	if (pugi::xml_node node = pathNode.node())
	{
		return node.text().as_ullong(def);
	}
	//���������
	else if (pugi::xml_attribute attr = pathNode.attribute())
	{
		return attr.as_ullong(def);
	}
	return def;
}

bool CXMLEditor::ReadAsBool(const char_t* szPath, bool def /*= false*/) const
{
	SafeString(szPath);
	//��ȡ�ڵ�
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return def;
	}
	//����ǽڵ�
	if (pugi::xml_node node = pathNode.node())
	{
		return node.text().as_bool(def);
	}
	//���������
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
		//ѡ�нڵ�
		pugi::xpath_node_set pathNodes = m_doc.select_nodes(desPath.c_str());
		//��Ч·��
		if (0 == pathNodes.size())
		{
			return CanNotFind;
		}
		//����·��
		else if (pathNodes.size() > 1)
		{
			return MultiNodes;
		}
		//��ȷ·��
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
