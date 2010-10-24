#include "opengl_tile_selector.h"
#include "tileset.h"

float tile_size = 1.0f;
float tile_spacing = 0.25f;

OpenGLTileSelector::OpenGLTileSelector(Gtk::DrawingArea* canvas):
OpenGLWidget(canvas),
tileset_(NULL) {

}

/** @brief do_resize
  *
  * @todo: document this function
  */
void OpenGLTileSelector::do_resize(int width, int height)
{
    glClearColor(0.8f, 0.8f, 1.0f, 1.0f);

    float scroll = 0.0f;
    float num_tiles_visible = 5.0f;

    glMatrixMode(GL_PROJECTION);
    float ortho_height = num_tiles_visible * (tile_size + tile_spacing);

    glOrtho(-1.0f, 1.0f, scroll + ortho_height, scroll, -1.0f, 1.0f);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
}

/** @brief do_render
  *
  * @todo: document this function
  */
void OpenGLTileSelector::do_render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if(!tileset_) {
        return;
    }

    glTranslatef(0.0f, tile_spacing, 0.0f);
    glEnable(GL_TEXTURE_2D);

    for(int i = 0; i < tileset_->get_tile_count(); ++i) {
        Tile* t = tileset_->get_tile_at(i);

        int width = t->get_width();
        int height = t->get_height();

        float ratio = float(height) / float(width);

        float hdw = tile_size / 2.0f; //Half display width
        float hdh = (tile_size * ratio) / 2.0f;

        glBindTexture(GL_TEXTURE_2D, get_texture_for_tile(t));

        glPushMatrix();

        glBegin(GL_QUADS);
            glVertex2f(-hdw, hdw);
            glVertex2f( hdw, hdw);
            glVertex2f( hdw,-hdw);
            glVertex2f(-hdw,-hdw);
        glEnd();

        glPopMatrix();
        glTranslatef(0.0f, tile_spacing + tile_size, 0.0f);
    }
}

/** @brief initialize
  *
  * @todo: document this function
  */
bool OpenGLTileSelector::initialize()
{

}

/** @brief generate_texture
  *
  * @todo: document this function
  */
GLuint OpenGLTileSelector::generate_texture(int width, int height, int channels, unsigned char* data)
{
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, (channels == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    return tex;
}


/** @brief get_texture_for_tile
  *
  * @todo: document this function
  */
GLuint OpenGLTileSelector::get_texture_for_tile(Tile* tile)
{
    if(tile_textures_.find(tile->get_id()) != tile_textures_.end()) {
        return tile_textures_[tile->get_id()];
    }

    GLuint tex_id = generate_texture(tile->get_width(), tile->get_height(), tile->get_channels(), tile->get_data());
    tile_textures_[tile->get_id()] = tex_id;
    return tex_id;
}


