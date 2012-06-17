/***********************************************************************************
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published
*  by the Free Software Foundation; either version 3 of the License, or (at
*  your option) any later version.
*
*  This program is distributed in the hope that it will be useful, but
*  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
*  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
*  License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with this program; if not, see <http://www.gnu.org/copyleft/lesser.html>.
*
**********************************************************************************/


#include <SOIL/SOIL.h>
#include <GL/gl.h>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <tinyxml.h>

#include "kglt/shortcuts.h"
#include "kazbase/os/path.h"
#include "tile.h"

namespace bfs = boost::filesystem;

/** @brief Tile
  *
  * @todo: document this function
  */
Tile::Tile(kglt::WindowBase& window, const std::string& path, const TransparentColour& c):
    window_(window),
    path_(path) {

    load_tile(path, c);
    id_ = next_id();
}

std::string Tile::get_name() const {
	return os::path::split(path_).second;
}

void Tile::load_tile(const std::string& path, const TransparentColour& c) {
    texture_id_ = kglt::create_texture_from_file(window_, path);
	
	/*
    unsigned char* data = SOIL_load_image(path.c_str(), &width_, &height_, &channels_, SOIL_LOAD_AUTO);

    assert(data);
    assert(channels_ == 4 || channels_ == 3); //FIXME: Handle if this is not the case

    data_ = std::vector<uint8_t>(data, data + (width_ * height_ * channels_));

    //SOIL loads images upside-down this loop will flip it the right way
    for(int32_t j = 0; j * 2 < height_; ++j)
    {
        int index1 = j * width_ * channels_;
        int index2 = (height_ - 1 - j) * width_ * channels_;
        for(int i = width_ * channels_; i > 0; --i )
        {
            uint8_t temp = data_[index1];
            data_[index1] = data_[index2];
            data_[index2] = temp;
            ++index1;
            ++index2;
        }
    }

    SOIL_free_image_data(data);

    std::vector<uint8_t> new_data;

    //Now, we convert to 4 channels if there are 3 and set the alpha to the transparent colour
    for(int i = 0; i < width_ * height_; ++i) {
        uint8_t r = data_[i * channels_];
        uint8_t g = data_[(i * channels_) + 1];
        uint8_t b = data_[(i * channels_) + 2];
        uint8_t a = 255;
        if(r == c.r && g == c.g && b == c.b) {
            a = 0;
        }

        new_data.push_back(r);
        new_data.push_back(g);
        new_data.push_back(b);
        new_data.push_back(a);
    }
    data_ = new_data;
    channels_ = 4;*/
}

/** @brief get_height
  *
  * @todo: document this function
  */
int Tile::get_height() const
{
    return height_;
}

/** @brief get_width
  *
  * @todo: document this function
  */
int Tile::get_width() const
{
    return width_;
}

/** @brief get_channels
  *
  * @todo: document this function
  */
int Tile::get_channels() const
{
    return channels_;
}


 /** @brief get_rendered_dimensions
  *
  * @todo: document this function
  */

std::pair<float, float> Tile::get_rendered_dimensions() const
{
    float ratio = float(get_height()) / float(get_width());

    float w = 1.0f;
    float h = w * ratio;

    return std::make_pair(w, h);
}

void Tile::render_geometry() {
    std::pair<float, float> dim = get_rendered_dimensions();

    float w = dim.first;
    float h = dim.second;

    float hw = w / 2.0f;
    float hh = h / 2.0f;

    glPushMatrix();

    kmVec2 pos = get_position();

    glTranslatef(pos.x, pos.y, 0.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(-hw, -hh);
        glTexCoord2f(1, 0); glVertex2f( hw, -hh);
        glTexCoord2f(1, 1); glVertex2f( hw,  hh);
        glTexCoord2f(0, 1); glVertex2f(-hw,  hh);
    glEnd();
    glPopMatrix();
}

    /** @brief add_geometry_element
  *
  * @todo: document this function
  */
void Tile::add_geometry_element(GeometryElement::ptr element)
{

    //Clamp the vertices before adding

    GeometryElement::PointArrayIteratorPair iters = element->get_point_iterators();
    for(; iters.first != iters.second; ++iters.first) {
        kmVec2* v = &(*iters.first);
        if(v->x < 0.0f) v->x = 0.0f;
        if(v->x > width_) v->x = (float) width_;

        if(v->y < 0.0f) v->y = 0.0f;
        if(v->y > width_) v->y = (float) height_;
    }


    geometry_.push_back(element);
}

/** @brief delete_geometry_element
  *
  * @todo: document this function
  */
void Tile::delete_geometry_element(GeometryElement* element)
{
    GeometryArray::iterator it = geometry_.begin();
    for(; it != geometry_.end(); ++it) {
        if((*it).get() == element) {
            break;
        }
    }

    if(it == geometry_.end()) {
        return;
    }

    geometry_.erase(it);
}

bool Tile::load() {
    geometry_.clear();

    bfs::path properties_file = bfs::path(path_).replace_extension(".properties");
    if(!bfs::exists(properties_file)) {
        //If there is no properties file, that's OK.
        return true;
    }

    TiXmlDocument doc(properties_file.string().c_str());
    if(!doc.LoadFile()) {
        //If a file exists, but it's not valid XML, that's not OK
        throw std::runtime_error("Couldn't load the tile properties file: " + properties_file.string());
    }

    std::cout << "Loading XML for tile from: " <<  properties_file.string() << std::endl;

    TiXmlElement* root = doc.FirstChildElement("tile");
    assert(root);

    TiXmlElement* geom = root->FirstChildElement("geometry");
    assert(geom);

    for(TiXmlElement* element = geom->FirstChildElement("element"); element; element = element->NextSiblingElement()) {
        std::vector<kmVec2> points;
        for(TiXmlElement* xml_point = element->FirstChildElement("point"); xml_point; xml_point = xml_point->NextSiblingElement()) {
            kmVec2 np;
            np.x = boost::lexical_cast<float>(xml_point->Attribute("x"));
            np.y = boost::lexical_cast<float>(xml_point->Attribute("y"));

            points.push_back(np);
        }

        if(!points.empty()) {
            GeometryElement::ptr new_elem(new GeometryElement(points));
            GeometryLayer layer = (GeometryLayer) boost::lexical_cast<int>(element->Attribute("layer"));
            new_elem->set_layer(layer);
            add_geometry_element(new_elem);
        }
    }

    return true;
}

bool Tile::save() {
    bfs::path properties_file = bfs::path(path_).replace_extension(".properties");

    if(geometry_.empty()) {
        //If there is no geometry, but the properties file exists, then remove it
        if(bfs::exists(properties_file)) {
            bfs::remove(properties_file);
        }
        return true;
    }

    TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild(decl);

	TiXmlElement* element = new TiXmlElement("tile");
	doc.LinkEndChild(element);

	TiXmlElement* geom = new TiXmlElement("geometry");
	element->LinkEndChild(geom);

    GeometryIteratorPair iters = get_geometry_iterators();
    for(; iters.first != iters.second; ++iters.first) {
        //TODO: set the layer

        TiXmlElement* geom_element = new TiXmlElement("element");
        geom_element->SetAttribute("layer", boost::lexical_cast<std::string>((*iters.first)->get_layer()));

        geom->LinkEndChild(geom_element);

        GeometryElement::PointArrayIteratorPair point_iters = (*iters.first)->get_point_iterators();

        for(; point_iters.first != point_iters.second; ++point_iters.first) {
            TiXmlElement* vert_element = new TiXmlElement("point");
            vert_element->SetAttribute("x", (*point_iters.first).x);
            vert_element->SetAttribute("y", (*point_iters.first).y);

            geom_element->LinkEndChild(vert_element);
        }
    }

    doc.SaveFile(properties_file.string().c_str());

    return true;
}
