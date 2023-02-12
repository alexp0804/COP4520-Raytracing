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
            auto r = double(i) / (img_width - 1);
            auto g = double(j) / (img_height - 1);
            auto b = 0.25;

            int ir = (int) (255.999 * r);
            int ig = (int) (255.999 * g);
            int ib = (int) (255.999 * b);

            cout << ir << ' ' << ig << ' ' << ib << endl;
        }
    }
    cerr << endl << "Done!" << endl;

    return 0;
}

