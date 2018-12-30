#ifndef VOLIMG
#define VOLIMG

#include <sstream>  // Needed for file stream objects
#include <iostream> // Needed for console stream objects
#include <string>   // Needed for strings
#include <vector>   // Needed for the vector container
#include <fstream>

namespace abrsas
{
	class VolImage
	{
		private:
			int width, height;
			// width and height of image stack
			std::vector<unsigned char**> slices;
			// data for each slice, in order
		public:
			int numImages;
			VolImage();
			// default constructor
			~VolImage();
			// destructor - define in
			// populate the object with images in stack and
			//set member variables
			bool readImages(std::string baseName);
			// compute difference map and write out;
			void diffmap(int sliceI, int sliceJ, std::string output_prefix);
			// extract slice sliceId and write to output - define in .cpp
			void extract(int sliceId, std::string output_prefix);
			// number of bytes uses to store image data bytes
			//and pointers (ignore vector<> container, dims etc)
			int volImageSize(void);

			void extractRow(int rowID);

	};
}

#endif
