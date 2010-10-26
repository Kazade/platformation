#include <GL/gl.h>
#include <cassert>

#include "tileset.h"
#include "tile_instance.h"

/** @brief TileInstance
  *
  * @todo: document this function
  * @todo: Spawn a ClutterTexture
  */
TileInstance::TileInstance(Tileset* tileset, int tile_id):
tile_(NULL)
{
    tile_ = tileset->get_tile_by_id(tile_id);
    assert(tile_);
}


/** @brief render_geometry
  *
  * @todo: document this function
  */
void TileInstance::render_geometry()
{
    assert(tile_);

    //1 world unit = 64 pixels. THis should be render from the editor view
    float scale = 1.0f / 64.0f;

    float w = float(tile_->get_width()) * scale;
    float h = float(tile_->get_height()) * scale;

    float hw = w / 2.0f;
    float hh = h / 2.0f;

    kmVec2 pos = get_position();

    glPushMatrix();
    glTranslatef(pos.x, pos.y, 0.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(-hw, -hh);
        glTexCoord2f(1, 0); glVertex2f( hw, -hh);
        glTexCoord2f(1, 1); glVertex2f( hw,  hh);
        glTexCoord2f(0, 1); glVertex2f(-hw,  hh);
    glEnd();
    glPopMatrix();
}

