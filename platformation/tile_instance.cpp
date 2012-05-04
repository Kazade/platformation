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


#include <GL/gl.h>
#include <cassert>

#include "tileset.h"
#include "tile_instance.h"

/** @brief TileInstance
  *
  * @todo: document this function
  * @todo: Spawn a ClutterTexture
  */
TileInstance::TileInstance(Tileset::ptr tileset, int tile_id):
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

