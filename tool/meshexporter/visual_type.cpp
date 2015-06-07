

#include "pch.hpp"

#include "visual_type.h"

#include "utility.hpp"
#include "expsets.hpp"
#include "string_tool.h"

namespace MeshExporter
{
    Material::Material()
        : identifier( "empty" ),
        mapIdMeaning( MAP_NONE ),
        selfIllum( 0 ),
        inUse( false )
    {
    }

    bool Material::operator==( const Material& m ) const
    {
        return identifier == m.identifier &&
            mapIdMeaning == m.mapIdMeaning &&
            mapIdentifier == m.mapIdentifier &&
            selfIllum == m.selfIllum &&
            fxFile_ == m.fxFile_ &&
            textureOverrides_ == m.textureOverrides_ &&
            vectorOverrides_ == m.vectorOverrides_ &&
            boolOverrides_ == m.boolOverrides_ &&
            floatOverrides_ == m.floatOverrides_ &&
            intOverrides_ == m.intOverrides_;

    }

    //收集材质使用到的所有资源名
    /**
    *	Returns all resources used by the material.
    *
    *	@param	retResources	The vector to be filled with resources.
    */
    void Material::resources( std::vector<std::string>& retResources )
    {
        if (this->fxFile_.length())
        {
            retResources.push_back( unifySlashes( toLower( fxFile_ ) ) );
            for (uint32 i = 0; i < textureOverrides_.size(); i++)
            {
                std::string textureName = unifySlashes( toLower( textureOverrides_[i].second ) );
                retResources.push_back( textureName );
            }
        }
        else
        {
            if (mapIdMeaning == MAP_TEXTURE)
            {
                if (mapIdentifier.length())
                {
                    retResources.push_back( mapIdentifier );
                }
            }
            else if (mapIdMeaning == MAP_MFM)
            {
                if (mapIdentifier.length())
                {
                    retResources.push_back( mapIdentifier );
                }
            }
        }
    }

    //检查shader文件是否存在
    /**
    *	Checks that the effect file exists.
    *
    *	@param	fxName		The effect file name.
    *	@param	fxPostfix	The suffix being used.
    *	@return	Success or failure.
    */
    bool Material::checkFilename( std::string& fxName, const std::string& fxPostfix )
    {
        fxName = unifySlashes( this->fxFile_ );
        if (endsWith( fxName, ".fx" ))
        {
            std::string s = fxName.substr( 0, fxName.length() - 3 );


            std::string pre,post;

            bool skinnedFX=false;//是否带蒙皮
            uint skinPos = s.find("_skinned");
            if (skinPos != std::string::npos)
            {
                pre = s.substr(0, skinPos);
                if ( (skinPos+8) < s.size())
                    post = s.substr(skinPos+8, s.size());

                skinnedFX = true;
            }

            if (fileExists( pre + fxPostfix + post + ".fx" ))
            {
                fxName = pre + fxPostfix +  post + ".fx";
            }
            else if (skinnedFX)
                return false;
        }
        return true;
    }

    void Material::save(FileSreamPtr stream, uint32 pgid)
    {
        FileSreamPtr pMaterial = stream->newChild( "material" );
        if (!pMaterial) return;

        (*pMaterial) << pgid;	

        pMaterial->writeV( "identifier", identifier);
        pMaterial->writeV("fx", ExportSettings::instance().defaultShader());

        if (mapIdMeaning == MAP_TEXTURE)//纹理
        {
            if (mapIdentifier.length())
            {
                pMaterial->writeV( "texture", getFileName(mapIdentifier));
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////

    bool BloatVertex::operator == ( const BloatVertex& bv ) const 
    { 
        return  this->pos == bv.pos &&
            this->normal == bv.normal &&
            this->colour == bv.colour &&
            this->uv == bv.uv &&
            this->uv2 == bv.uv2 &&
            this->vertexIndex == bv.vertexIndex &&
            (this->smoothingGroup & bv.smoothingGroup) &&
            this->binormal == bv.binormal &&
            this->tangent == bv.tangent &&
            this->meshIndex == bv.meshIndex;
    };

    bool BloatVertex::operator < ( const BloatVertex& bv ) const 
    {
        if( pos < bv.pos )	return true;
        if( pos > bv.pos )	return false;

        if( normal < bv.normal )	return true;
        if( normal > bv.normal )	return false;

        if( colour < bv.colour )	return true;
        if( colour > bv.colour )	return false;

        if( uv < bv.uv )	return true;
        if( uv > bv.uv )	return false;

        if( uv2 < bv.uv2 )	return true;
        if( uv2 > bv.uv2 )	return false;

        if( vertexIndex < bv.vertexIndex )	return true;
        if( vertexIndex > bv.vertexIndex )	return false;

        if( binormal < bv.binormal )	return true;
        if( binormal > bv.binormal )	return false;

        if( tangent < bv.tangent )	return true;
        if( tangent > bv.tangent )	return false;

        if( meshIndex < bv.meshIndex )	return true;
        if( meshIndex > bv.meshIndex )	return false;

        return false; // enough
    };

}//end namespace MeshExporter