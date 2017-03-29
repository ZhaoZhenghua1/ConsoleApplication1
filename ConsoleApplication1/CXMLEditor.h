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
		CanNotFind,  //无法获取节点
		MultiNodes,  //获取了多个节点
		Exception,   //解析时引发异常
	};
	CXMLEditor();
	~CXMLEditor();

	//打开文件
	bool Open(const char_t* szFile);

	//定位到路径szRelativePath
	EError GoTo(const char_t* szRelativePath);

	//像XPath路径中设置值
	template<class Type>
	EError Write(const char_t* szPath, Type value);

	//像XPath路径中添加节点，并设置节点值，返回插入后的节点XPath路径
	template<class Type>
	string_t AppendNode(const char_t* szPath, const char_t* szNodeName, Type value);

	//像XPath路径中添加节点，不设置值，返回节点XPath路径
	string_t AppendNode(const char_t* szPath, const char_t* szNodeName);

	//像XPath路径中添加属性，并设置属性值
	template<class Type>
	EError AppendAttribute(const char_t* szPath, const char_t* szAttrName, Type value);

	//从XPath路径中读取值,如果读取失败，则返回默认值
	const char_t* ReadAsString(const char_t* szPath, const char_t* def = PUGIXML_TEXT("")) const;
	int ReadAsInt(const char_t* szPath, int def = 0) const;
	unsigned int ReadAsUint(const char_t* szPath, unsigned int def = 0) const;
	double ReadAsDouble(const char_t* szPath, double def = 0) const;
	float ReadAsFloat(const char_t* szPath, float def = 0) const;
	long long ReadAsLlong(const char_t* szPath, long long def = 0) const;
	unsigned long long ReadAsUllong(const char_t* szPath, unsigned long long def = 0) const;
	bool ReadAsBool(const char_t* szPath, bool def = false) const;

	//获取节点，如果失败返回失败类型，如果路径具有歧义性，包含多个节点，则返回MultiNodes的错误
	EError GetNode(const char_t* szPath, pugi::xpath_node& retNode = pugi::xpath_node())const;
private:
	//传入0地址时置为空串
	void SafeString(const char_t* & szStr)const;
private:
	pugi::xml_document m_doc;
	string_t m_strPath;
	string_t m_strFileName;
};

//像XPath路径中设置值
template<class Type>
CXMLEditor::EError CXMLEditor::Write(const char_t* szPath, Type value)
{
	SafeString(szPath);
	try
	{
		//获取路径节点，并判断路径是否有效
		pugi::xpath_node pathNode;
		EError error = GetNode(szPath, pathNode);
		if (error != NoError)
		{
			return error;
		}
		//如果路径是节点
		if (pugi::xml_node node = pathNode.node())
		{
			node.text() = value;
		}
		//如果路径是属性
		else if (pugi::xml_attribute attr = pathNode.attribute())
		{
			attr.set_value(value);
		}
		//无效的节点
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

//像XPath路径中添加节点，并设置节点值，返回插入后的节点XPath路径
template<class Type>
string_t CXMLEditor::AppendNode(const char_t* szPath, const char_t* szNodeName, Type value)
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
		//设置节点值
		nodeChild.text() = value;
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

//像XPath路径中添加属性，并设置属性值
template<class Type>
CXMLEditor::EError CXMLEditor::AppendAttribute(const char_t* szPath, const char_t* szAttrName, Type value)
{
	SafeString(szPath);
	SafeString(szAttrName);

	//获取路径节点，并判断路径是否有效
	pugi::xpath_node pathNode;
	EError error = GetNode(szPath, pathNode);
	if (error != NoError)
	{
		return error;
	}
	//必须是一个有效节点
	if (pugi::xml_node node = pathNode.node())
	{
		//添加属性并设置值
		node.append_attribute(szAttrName) = value;
	}
	else
	{
		return CanNotFind;
	}
	return NoError;
}



#endif // _XML_EDITOR_H_


