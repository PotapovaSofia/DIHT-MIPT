#include <math.h>
#include <assert.h>
#include <Magick++.h>
#include <vector>

using namespace Magick;

class ImageWrapper {

    Image original;
    Image tubnale;
    int width;
    int height;
    std::vector<int> rgb_pixels;

public:

    ImageWrapper(std::string file) {
        original.read(file);
        tubnale = original;
        tubnale.resize("20x20");
        width = tubnale.columns();
        height = tubnale.rows();
        int range = (1 << 8);
        PixelPacket *pixels = tubnale.getPixels(0, 0, width, height);
        for (int row = 0; row < height; row++) {
            for (int column = 0; column < width; column++) {
                Color color = pixels[width * row + column];
                rgb_pixels.push_back(color.redQuantum() / range);
                rgb_pixels.push_back(color.greenQuantum() / range);
                rgb_pixels.push_back(color.blueQuantum() / range);
                tubnale.syncPixels();
            }
        }
        rgb_pixels.resize(1200); 
    }

    std::vector<int> get_coord() {
        return rgb_pixels;
    }

    void write_image(std::string file) {
        original.write(file);
    }

    void operator=(const ImageWrapper& other) {
        original = other.original;
        tubnale = other.tubnale;
        width = other.width;
        height = other.height;
        rgb_pixels = other.rgb_pixels;

    }
};
