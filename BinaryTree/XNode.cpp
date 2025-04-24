#include "XNode.h"

XNode::XNode()
{
}

XNode::XNode(const std::string& name):x_name(name)
{
}

XNode::~XNode()
{
}

void XNode::setXName(const std::string& xname)
{
	x_name = xname;
}

const std::string& XNode::getXName() const
{
	return x_name;
}

void XNode::addChild(std::shared_ptr<XNode> node)
{
	x_childs.push_back(node);
}

std::shared_ptr<XNode> XNode::getParent()
{
	return x_parent;
}
