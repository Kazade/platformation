#include <SOIL/SOIL.h>
#include <GL/gl.h>
#include <boost/filesystem.hpp>
#include <tinyxml.h>

#include "tile.h"

namespace bfs = boost::filesystem;

/** @brief Tile
  *
  * @todo: document this function
  */
Tile::Tile(const std::string& path):
path_(path) {
    load_tile(path);
    id_ = next_id();
}

std::string Tile::get_name() const {
    return bfs::path(path_).leaf();
}

void Tile::load_tile(const std::string& path) {
    unsigned char* data = SOIL_load_image(path.c_str(), &width_, &height_, &channels_, SOIL_LOAD_AUTO);

    assert(data);

    data_ = std::vector<unsigned char>(data, data + (width_ * height_ * channels_));

    //SOIL loads images upside-down this loop will flip it the right way
    for(uint32_t j = 0; j * 2 < height_; ++j)
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
    bfs::path properties_file = bfs::path(path_).replace_extension(".properties");
    if(!bfs::exists(properties_file)) {
        //If there is no properties file, that's OK.
        return true;
    }

    TiXmlDocument doc(properties_file.string().c_str());
    if(!doc.LoadFile()) {
        throw std::runtime_error("Couldn't load the tile properties file: " + properties_file.string());
    }

    TiXmlElement* root = doc.FirstChildElement("tile");
    TiXmlElement* geom = root->FirstChildElement("geometry");

    for(TiXmlElement* element = geom->FirstChildElement("element"); element; element = element->NextSiblingElement()) {

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
        geom->LinkEndChild(geom_element);

        GeometryElement::PointArrayIteratorPair point_iters = (*iters.first)->get_point_iterators();

        TiXmlElement* vertices = new TiXmlElement("points");
        for(; point_iters.first != point_iters.second; ++point_iters.first) {
            TiXmlElement* vert_element = new TiXmlElement("point");
            vert_element->SetAttribute("x", (*point_iters.first).x);
            vert_element->SetAttribute("y", (*point_iters.first).y);

            vertices->LinkEndChild(vert_element);
        }

        geom_element->LinkEndChild(vertices);
    }

    doc.SaveFile(properties_file.string().c_str());

    return true;
}
