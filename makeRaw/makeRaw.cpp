//
// cool_random makeRaw
// This tool converts `cool_random.mmpld` to `cool_random.raw`
//

#include <mmpld.h>
#include <fstream>
#include <cassert>

#pragma pack(push, 1)
struct point {
	float x, y, z, s;
	unsigned char r, g, b;
};
#pragma pack(pop)

static_assert(sizeof(point) == 19, "point struct size wrong");

int main()
{
	mmpld::file<HANDLE> mmpld("cool_random.mmpld");

	auto frameHeader = mmpld.frame_header();
	assert(frameHeader.lists == 1);

	mmpld::list_header listHeader;
	mmpld.read_particles(false, listHeader, nullptr, 0);
	assert(listHeader.vertex_type == mmpld::vertex_type::float_xyzr);
	assert(listHeader.colour_type == mmpld::colour_type::rgb8);

	std::vector<point> points(static_cast<size_t>(listHeader.particles));
	mmpld.read_particles(listHeader, points.data(), points.size() * sizeof(point));

	std::ofstream raw;
	raw.open("cool_random.raw", std::ios::binary | std::ios::out);
	
	for (point const& pt : points)
		raw.write(reinterpret_cast<const char*>(&pt.x), sizeof(float) * 3);

	raw.close();
}
