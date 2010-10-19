#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

class Tile {
public:
    Tile(const std::string& path);

    ClutterTexture* get_texture() const;
    ClutterTexture* get_solidity_front() const;
    ClutterTexture* get_solidity_back() const;

private:
    std::string path_;
    ClutterTexture* texture_;
};


#endif // TILE_H_INCLUDED
