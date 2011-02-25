#ifndef TRANSPARENT_COLOUR_DETECTOR_H_INCLUDED
#define TRANSPARENT_COLOUR_DETECTOR_H_INCLUDED

#include "transparent_colour.h"

/**
    Find up to num_samples images in the path. Total up counts for each pixel colour.
    The colour with the most counts wins, return that colour.

    Yes, it'll only work some of the time, it's just a guess.
*/
class TransparentColourDetector {
public:
    TransparentColourDetector(boost::filesystem::path p, uint32_t num_samples);

    TransparentColour detect() {
        std::map<TransparentColour, int32_t> colour_counts_;

        for(uint32_t i = 0; i < num_samples_; ++i) {
            std::string image_path = find_random_file();

            //TODO: Load the image, go through each pixel and create a TransparentColour instance
            //If it exists in colour_counts_ then increment the count, otherwise set it to 1
        }

        //TODO: Go through colour_counts_, find the highest count and return that colour
    }

private:
    std::string find_random_file();

    boost::filesystem::path path_;
    uint32_t num_samples_;
};

#endif // TRANSPARENT_COLOUR_DETECTOR_H_INCLUDED
