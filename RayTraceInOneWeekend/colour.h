// -----------------------------------------------------------------------------
#ifndef COLOUR_H_
#define COLOUR_H_
// -----------------------------------------------------------------------------

//--INCLUDES--//
#include "vec3.h"

// -----------------------------------------------------------------------------

void writeColour(ostream& out, colour pixelColour)
{
	// write the translated [0, 255] value of each colour component
	out << static_cast<int>(255.999 * pixelColour.x()) << ' '
		<< static_cast<int>(255.999 * pixelColour.y()) << ' '
		<< static_cast<int>(255.999 * pixelColour.z()) << '\n';
}

// -----------------------------------------------------------------------------

void writeColour(std::ostream& pOut, colour pPixelColour, int pSamplePerPixel)
{
	auto r = pPixelColour.x();
	auto g = pPixelColour.y();
	auto b = pPixelColour.z();

	// divide the colour by the number of samples
	auto scale = 1.0 / pSamplePerPixel;
	r = sqrt(scale*r);
	g = sqrt(scale*g);
	b = sqrt(scale*b);

	// write othe translated [0, 255] value of each colour component
	pOut << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		 << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		 << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#endif // !COLOUR_H_