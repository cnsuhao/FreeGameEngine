//
//  progress_bar.h
//  orange
//
//  Created by Kevin on 14-2-21.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef __liborange_progress_bar__
#define __liborange_progress_bar__

#include "billboard.h"

namespace ora{
	
	extern float progressReferenceToken;

    struct VertexXYZUV;
    typedef VertexXYZUV VertexFormat;
    typedef std::vector<VertexFormat> ProgressBarVertices;
    typedef std::vector<uint16> ProgressBarIndices;
	static ProgressBarIndices indices;

	class ProgressReference : public BillBoard{
	public:
		ProgressReference();
		virtual ~ProgressReference();

		virtual void draw();

		void setMaterial(const std::string &);
		MaterialPtr getMaterial() const;

		void setRange(float range) { range_ = range; }
		float getRange() const { return range_; }
		
    protected:
        bool genVertices(ProgressBarVertices& vertices);
        bool genMesh(ProgressBarVertices vertices);

	private:
		float		range_;
		MaterialPtr	mtl_;
	
	};

}; // end of namespace ora

#endif // __liborange_progress_bar__