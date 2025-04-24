#pragma once

#include <string>
#include <list>
#include <memory>

class XNode
{
public:
	enum class NodeType
	{

	};

	XNode();
	XNode(const std::string& name);
	virtual ~XNode();
	void setXName(const std::string& xname);
	const std::string& getXName()const;
	void addChild(std::shared_ptr<XNode> node);
	std::shared_ptr<XNode> getParent();
	bool isNeedInput() { return need_input; }
	void setInput(bool n) { need_input = n; }
	bool has_child() { return x_childs.size() > 0; }
	std::string& get_name() { return x_name; }
	std::list<std::shared_ptr<XNode>>& get_child_list() { return x_childs; }

	virtual void on_update(float delat) = 0;
	virtual void on_render() = 0;

protected:
	std::string x_name;
	std::list<std::shared_ptr<XNode>> x_childs;
	std::shared_ptr<XNode> x_parent;
	bool need_input = false;
};

