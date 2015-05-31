#include "IwImage.h"
#include "SmartPointer.h"
#include "Iw2D.h"

#ifndef EN_HITIMAGE_H
#define EN_HITIMAGE_H

class EnHitImage{
private:
	CIwImage m_Hitmap;
	spt<CIwImage> _img;

public:

	EnHitImage(spt<CIwImage> img) : _img(img) {
		// todo: this image could be stored only once per each object =)

		m_Hitmap.SetFormat(CIwImage::A_8);
		m_Hitmap.SetWidth(img->GetWidth());
		m_Hitmap.SetHeight(img->GetHeight());
		img->ConvertToImage(&m_Hitmap);
	}

	bool HitTest(const CIwFVec2& testPos)
	{
		//Move the test position into "local" coordinate space
		CIwFVec2 localPos = testPos + CIwFVec2(m_Hitmap.GetWidth() / 2, m_Hitmap.GetHeight() / 2);
		//Test for location outside the image rectangle
		if (localPos.x < 0
			|| localPos.y < 0
			|| localPos.x >(float)m_Hitmap.GetWidth()
			|| localPos.y >(float)m_Hitmap.GetHeight())
			return false;
		//Return a hit if the specified local alpha value is greater than half
		return m_Hitmap.GetTexels()[(int)localPos.y * m_Hitmap.GetWidth() + (int)localPos.x] > 0x80;
	}
};

#endif