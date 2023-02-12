#include "vec3.h"
#include "color.h"
#include "ray.h"

#include <iostream>

using namespace std;

const int img_width = 256;
const int img_height = 256;

int main() {
    cout << "P3" << endl << img_width << ' ' << img_height << endl << "255" << endl;

    for (int j = img_height - 1; j >= 0; j--)
    {
        cerr << "\rLines remaining: " << j << ' ' << flush;

        for (int i = 0; i < img_width; i++)
        {
            Color pixel = Color(
                (double) i / (img_width - 1),
                (double) j / (img_height - 1),
                0.25
            );

            write_color(std::cout, pixel);
        }
    }
    cerr << endl << "Done!" << endl;

    return 0;
}

