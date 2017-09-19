#ifndef _XML_LOADER_H_INCLUDED_
#define _XML_LOADER_H_INCLUDED_

//　Disable Warning C4996
#pragma warning(disable : 4996)

//　include
#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
using namespace boost::property_tree;

class Projector
{
public:
	//xmlfileからプロジェクタ情報を取得
	void set(char* filename)
	{
		ptree pt;
		read_xml(filename, pt);

		if (boost::optional<int> id = pt.get_optional<int>("root.pixel.<xmlattr>.height")) {
			pixel_h = id.get();
		}
		if (boost::optional<int> id = pt.get_optional<int>("root.pixel.<xmlattr>.width")) {
			pixel_w = id.get();
		}

		if (boost::optional<int> id = pt.get_optional<int>("root.angle.<xmlattr>.left")) {
			angle_l = id.get();
		}
		if (boost::optional<int> id = pt.get_optional<int>("root.angle.<xmlattr>.rigth")) {
			angle_r = id.get();
		}
		if (boost::optional<int> id = pt.get_optional<int>("root.angle.<xmlattr>.up")) {
			angle_u = id.get();
		}
		if (boost::optional<int> id = pt.get_optional<int>("root.angle.<xmlattr>.down")) {
			angle_d = id.get();
		}

		
		BOOST_FOREACH(const ptree::value_type& child, pt.get_child("root.position")) {
			const int value = boost::lexical_cast<float>(child.second.data());
			//std::cout << value << std::endl;
		}

		BOOST_FOREACH(const ptree::value_type& child, pt.get_child("root.direction")) {
			const int value = boost::lexical_cast<float>(child.second.data());
			//std::cout << value << std::endl;
		}
	}

	float pixel_w;
	float pixel_h;
	float angle_l;
	float angle_r;
	float angle_u;
	float angle_d;
	float center[2];
	float pos[3];
	float dir[3];
};
#endif	//　_XML_LOADER_H_INCLUDED_