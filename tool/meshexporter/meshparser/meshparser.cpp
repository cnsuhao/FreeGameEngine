// meshparser.cpp : 定义控制台应用程序的入口点。
//

#include "pch.hpp"

#include <iostream>

#include "../quick_file_reader.h"
#include "../primitive_file_structs.hpp"

#include "../file_stream.h"
#include "../vertex_formats.hpp"
#include "../mmath.h"

using namespace Moo;

using std::cout;
using std::endl;

typedef std::vector<Moo::PrimitiveGroup> PGVector;

const unsigned int MeshMagic = 'm' | 'e' << 8 | 's' << 16 | 'h' << 24;

struct Color3
{
    float r, g, b;
};


struct Material
{
    std::string name_;
    std::string shader_;
    std::string texture_;
};

std::ostream & operator << (std::ostream & out, const Color3 & c)
{
    out << "(" << c.r << ", " << c.g << ", " << c.b << ")";
    return out;
}

std::ostream & operator << (std::ostream & out, const Vector3 & c)
{
    out << "(" << c.x << ", " << c.y << ", " << c.z << ")";
    return out;
}


std::ostream & operator << (std::ostream & out, const Material & m)
{
    out << "material: \n"
        << "name: " << m.name_ << "\n"
        << "shader: " << m.shader_ << "\n"
        << "texture: " << m.texture_ ;
    return out;
}

std::ostream & operator << (std::ostream & out, const Moo::PrimitiveGroup & pg)
{
    cout<< "startIndex: " << pg.startIndex_ << "\n"
        << "nPrimitives: " << pg.nPrimitives_ << "\n"
        << "startVertex: " << pg.startIndex_ << "\n"
        << "nVertices: " << pg.nVertices_;
    return out;
}

std::string removeExt(const std::string & name)
{
    size_t pos = name.find_last_of('.');
    if(pos != name.npos)
    {
        return name.substr(0, pos);
    }

    return name;
}

void parse(const std::string & name)
{
    cout << "filename: " << name << "\n" << endl;

    FILE *pFile = fopen(name.c_str(), "rb");
    if(!pFile)
    {
        cout << "open file '" << name << "' failed!" << endl;
        return;
    }

    fseek(pFile, 0, SEEK_END);
    long flen = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    std::string buffer(flen, '\0');
    if(fread(&buffer[0], flen, 1, pFile) != 1)
    {
        cout << "read file failed!" << endl;
        fclose(pFile);
        return;
    }

    fclose(pFile);
    pFile = nullptr;

    QuickFileReader reader(buffer.data(), flen);

    unsigned int magic, version;
    reader >> magic >> version;
    cout << "magic: " << magic << "\n"
        << "version: " << version << "\n"
        << endl;
    
    if(magic != MeshMagic)
    {
        cout << name << " is not a valid mesh file!" << endl;
        return;
    }

    //索引数据
    Moo::IndexHeader ih;
    reader >> ih;
    {
        cout << "index format: " << ih.indexFormat_ << endl;
        cout << "index count: " << ih.nIndices_ << endl;
        cout << "index group: " << ih.nTriangleGroups_ << endl;
    }

    int indexSize = strcmp(ih.indexFormat_, "list") == 0 ? 2 : 4;
    if(indexSize == 2)
    {
        std::vector<unsigned short> indices;
        reader.readVector(indices, ih.nIndices_);
    }
    else
    {
        std::vector<unsigned int> indices(ih.nIndices_);
        reader.readVector(indices, ih.nIndices_);
    }

    //按材质分组
    PGVector primGroups;
    reader.readVector(primGroups, ih.nTriangleGroups_);

    //顶点数据
    Moo::VertexHeader vh;
    reader >> vh;
    {
        cout << "vertex format: " << vh.vertexFormat_ << endl;
        cout << "vertex count: " << vh.nVertices_ << endl;
    }

    std::vector<VertexXYZNUV> vertices;
    reader.readVector(vertices, vh.nVertices_);

    Vector3 boxMin, boxMax;
    reader >> boxMin >> boxMax;
    cout << "bounding box: \n"
        << "min: " << boxMin << "\n"
        << "max: " << boxMax << "\n"
        ;

    std::vector<Material> materials(ih.nTriangleGroups_);
    FileSreamPtr root = FileSream::openStream(removeExt(name) + ".mtl");
    if(!root)
    {
        cout << "Failed to open material!" << endl;
    }
    else
    {
        for (FileSream::Children::iterator it = root->begin();
            it != root->end(); ++it)
        {
            FileSreamPtr p = *it;
            if(p->getTag() == "material")
            {
                int pgid;
                (*p) >> pgid;

                p->readV("identifier", materials[pgid].name_);
                p->readV("fx", materials[pgid].shader_);
                p->readV("texture", materials[pgid].texture_);
            }
        }
    }

    for(size_t i = 0; i<primGroups.size(); ++i)
    {
        cout << "pg " << i <<endl;
        cout << primGroups[i] << endl;

        if(materials[i].name_.empty())
        {
            cout << "no material." << endl;
        }
        else
        {
            cout << "material name: " << materials[i].name_ << endl;
            cout << "material fx: " << materials[i].shader_ << endl;
            cout << "material texture: " << materials[i].texture_ << endl;
        }
    }


    cout << "reader remain bytes: " << reader.remain() << endl;
}

void testFileStreamWrite(const std::string & filename)
{
    FileSreamPtr root = new FileSream("root");

    root->writeV("position/x", 1.0f);

    FileSreamPtr child = root->newChild("group", "this is the group value");
    child->writeV("identify", "name");
    child->writeV("null", "");

    if(!FileSream::saveStream(root, filename))
    {
        cout << "Failed Save stream." << endl;
        return;
    }
    else
    {
        cout << "Succed save stream." << endl;
    }
}

void testFileStreamRead(const std::string & filename)
{
    FileSreamPtr root = FileSream::openStream(filename);
    if(!root)
    {
        cout << "Open stream failed!" << endl;
        return;
    }

    root->print(cout);
}

int _tmain(int argc, _TCHAR* argv[])
{
    if(argc < 2)
    {
        cout << "need one arguments." << endl;
    }
    else
    {
        parse(argv[1]);
    }

    //parse("a1.mesh");

    //testFileStreamRead("test.stream");
    //testFileStreamWrite("test2.stream");



    system("pause");
	return 0;
}
