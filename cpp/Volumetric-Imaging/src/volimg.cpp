#include "volimg.h"

using namespace std;

abrsas::VolImage::VolImage()
{
    height = 0;
    width = 0;
}

abrsas::VolImage::~VolImage()
{
    for (int i = 0; i < numImages; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
        	// deletes row
            delete[] slices[i][j];
        }
        // deletes 'row container'
        delete[] slices[i];
    }
}

// populate the object with images in stack and
// set member variables
bool abrsas::VolImage::readImages(std::string baseName)
{
    // defining the steams
    stringstream toInt;
    ifstream in;

    // checking header file and reading it
    in.open(baseName + ".data");
    if (!in)
    {
        cout << "failed to open data file" << endl;
        return false;
    }

    string line;
    if (!getline(in, line))
    {
        cout << "failed to readline in data file" << endl;
        return false;
    }

    // splits the line up
    stringstream splitter(line);
    string strValue;
    // set height
    getline(splitter, strValue, ' ');
    toInt.str(strValue);
    toInt >> width;
    // set width
    getline(splitter, strValue, ' ');
    toInt.clear();
    toInt.str(strValue);
    toInt >> height;
    // get num images
    getline(splitter, strValue, ' ');
    toInt.clear();
    toInt.str(strValue);
    toInt >> numImages;

    // initialize slices as 3D array
    slices.reserve(numImages);
    for (int i = 0; i < numImages; i++)
    {
        // creates the new inner array
        slices[i] = new unsigned char *[height];

        // opening the file reader
        string path = baseName + to_string(i) + ".raw";
        ifstream inn;
        inn.open(path, std::ios::in | std::ios::binary);

        if (!inn)
        {
            cout << "failed to open raw file" << endl;
            return false;
        }

        for (int j = 0; j < height; j++)
        {
            // creating the final inner array and reading values into it
            slices[i][j] = new unsigned char[width];
            inn.read((char *)slices[i][j], width);
        }
        inn.close();
    }
    in.close();
    return true;
}

// compute difference map and write out;
void abrsas::VolImage::diffmap(int sliceI, int sliceJ, std::string output_prefix)
{
    ofstream out;
    out.open("../output/diffmap" + output_prefix, ios::binary | ios::out);

    auto **diffmap = new unsigned char* [height];

    for (int h = 0; h < height; ++h)
    {
        diffmap[h] = new unsigned char [width];
        for (int w = 0; w < width; ++w)
        {
            auto u_char =
                    (unsigned char) (abs((float) slices[sliceI][h][w] - (float) slices[sliceJ][h][w]) / 2);
            diffmap[h][w] = u_char;
        }
    }

    // Slightly more loops doing it this way, but much less iostreams are used.
    for (int i = 0; i < height; ++i)
    {
        out.write((char *)diffmap[i], width);
        delete[] diffmap[i];
    }
    delete[] diffmap;

    out.close();
}

// extract slice sliceId and write to output - define in .cpp
void abrsas::VolImage::extract(int sliceId, std::string output_prefix)
{
    /*
     * Create the output slice
     */
    ofstream out;
    out.open("../output/extract_slice" + to_string(sliceId) + output_prefix, ios::binary | ios::out);
    unsigned char **slice = slices[sliceId];

    for (int h = 0; h < height; ++h)
    {
        out.write((char *)slice[h], width);
    }

    out.close();

    /*
     * Create the header file
     */
    ofstream header("../output/output.dat", ios::out);
    header << width << " " << height << " 1";
    header.close();
}

// number of bytes uses to store image data bytes
// and pointers (ignore vector<> container, dims etc)
int abrsas::VolImage::volImageSize(void)
{
	// could also be width * height * numImages?
    int sum = 0;
    for (int i = 0; i < numImages; ++i)
    {
    	for (int j = 0; j < height; ++j)
    	{
    		sum += sizeof(slices[i][j]);
    	}
    	sum += sizeof(slices[i]);
    }
    return sum;
}

void abrsas::VolImage::extractRow(int rowId)
{
    /*
     * Create the output slice
     */
    ofstream out;
    out.open("../output/extract_row" + to_string(rowId) + ".raw", ios::binary | ios::out);

    for (int i = 0; i < numImages; ++i)
    {
        out.write((char *) slices[i][rowId], width);
    }

    out.close();
}