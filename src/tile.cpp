#include <SOIL/SOIL.h>
#include <GL/gl.h>

#include "tile.h"

/** @brief Tile
  *
  * @todo: document this function
  */
Tile::Tile(const std::string& path) {
    load_tile(path);
    id_ = next_id();
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
void Tile::add_geometry_element(GeometryElement element)
{
    geometry_.push_back(element);
}

