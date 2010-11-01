#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

#include <utility>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <string>

#include "object.h"
#include "geometry_element.h"

class Tile : public Object {
public:
    typedef boost::shared_ptr<Tile> ptr;
    typedef int id_type;

    Tile(const std::string& path);

    id_type get_id() const { return id_; }

    //Probably should be in a separate place than here
    int get_width() const;
    int get_height() const;
    unsigned char* get_data() { return &data_[0]; }
    int get_channels() const;

    void render_geometry();

    std::pair<float, float> get_rendered_dimensions() const;

    void add_geometry_element(GeometryElement element);
    int get_geometry_element_count() const { return (int) geometry_.size(); }
    GeometryElement* get_geometry_element_at(int i) { return &geometry_[i]; }

private:
    std::string path_;

    int width_;
    int height_;
    int channels_;
    std::vector<unsigned char> data_;

    void load_tile(const std::string& path);

    Tile::id_type id_;

    static Tile::id_type next_id() {
        static Tile::id_type id = 0;
        return id++;
    }

    std::vector<GeometryElement> geometry_;
};


#endif // TILE_H_INCLUDED
