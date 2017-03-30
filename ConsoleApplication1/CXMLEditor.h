#ifndef _XML_EDITOR_H_
#define _XML_EDITOR_H_

#include "pugixml.hpp"

using pugi::char_t;
using pugi::string_t;

class CXMLEditor
{
public:
	enum EError
	{
		NoError,
		CanNotFind,  //�޷���ȡ�ڵ�
		MultiNodes,  //��ȡ�˶���ڵ�
		Exception,   //����ʱ�����쳣
	};
	CXMLEditor();
	~CXMLEditor();

	//���ļ�
	bool Open(const char_t* szFile);

	//��λ��·��szRelativePath
	EError GoTo(const char_t* szRelativePath);

	//��XPath·��������ֵ
	template<class Type>
	EError Write(const char_t* szPath, Type value);

	//��XPath·������ӽڵ㣬�����ýڵ�ֵ�����ز����Ľڵ�XPath·��
	template<class Type>
	string_t AppendNode(const char_t* szPath, const char_t* szNodeName, Type value);

	//��XPath·������ӽڵ㣬������ֵ�����ؽڵ�XPath·��
	string_t AppendNode(const char_t* szPath, const char_t* szNodeName);

	//��XPath·����������ԣ�����������ֵ
	template<class Type>
	EError AppendAttribute(const char_t* szPath, const char_t* szAttrName, Type value);

	//��XPath·���ж�ȡֵ,�����ȡʧ�ܣ��򷵻�Ĭ��ֵ
	const char_t* ReadAsString(const char_t* szPath, const char_t* def = PUGIXML_TEXT("")) const;
	int ReadAsInt(const char_t* szPath, int def = 0) const;
	unsigned int ReadAsUint(const char_t* szPath, unsigned int def = 0) const;
	double ReadAsDouble(const char_t* szPath, double def = 0) const;
	float ReadAsFloat(const char_t* szPath, float def = 0) const;
	long long ReadAsLlong(const char_t* szPath, long long def = 0) const;
	unsigned long long ReadAsUllong(const char_t* szPath, unsigned long long def = 0) const;
	bool ReadAsBool(const char_t* szPath, bool def = false) const;

	//��ȡ�ڵ㣬���ʧ�ܷ���ʧ�����ͣ����·�����������ԣ���������ڵ㣬�򷵻�MultiNodes�Ĵ���
	EError GetNode(const char_t* szPath, pugi::xpath_node& retNode = pugi::xpath_node())const;
private:
	//����0��ַʱ��Ϊ�մ�
	void SafeString(const char_t* & szStr)const;
private:
	pugi::xml_document m_doc;
	string_t m_strPath;
	string_t m_strFileName;
};

//��XPath·��������ֵ
template<class Type>
CXMLEditor::EError CXMLEditor::Write(const char_t* szPath, Type value)
{
	SafeString(szPath);
	try
	{
		//��ȡ·���ڵ㣬���ж�·���Ƿ���Ч
		pugi::xpath_node pathNode;
		EError error = GetNode(szPath, pathNode);
		if (error != NoError)
		{
			return error;
		}
		//���·���ǽڵ�
		if (pugi::xml_node node = pathNode.node())
		{
			node.text() = value;
		}
		//���·��������
		else if (pugi::xml_attribute attr = pathNode.attribute())
		{
			attr.set_value(value);
		}
		//��Ч�Ľڵ�
		else
		{
			return CanNotFind;
		}
	}
	catch (const pugi::xpath_exception&)
	{
		return Exception;
	}
	return NoError;
}

//��XPath·������ӽڵ㣬�����ýڵ�ֵ�����ز����Ľڵ�XPath·��
template<class Type>
string_t CXMLEditor::AppendNode(const char_t* szPath, const char_t* szNodeName, Type value)
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
		//���ýڵ�ֵ
		nodeChild.text() = value;
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

//��XPath·����������ԣ�����������ֵ
template<class Type>
CXMLEditor::EError CXMLEditor::AppendAttribute(const char_t* szPath, const char_t* szAttrName, Type value)
{
	SafeString(szPath);
	SafeString(szAttrName);

	//��ȡ·���ڵ㣬���ж�·���Ƿ���Ч
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return error;
	}
	//������һ����Ч�ڵ�
	if (pugi::xml_node node = pathNode.node())
	{
		//������Բ�����ֵ
		node.append_attribute(szAttrName) = value;
	}
	else
	{
		return CanNotFind;
	}
	return NoError;
}



#endif // _XML_EDITOR_H_


