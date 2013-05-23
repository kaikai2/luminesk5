#include <map>
#include <string>
#include "curvedani.h"
#include "caAnimation.h"
#include "caAnimResManager.h"
#include "tinyxml/tinyxml.h"
using std::map;
using std::string;

//#pragma init_seg(lib)

namespace cAni
{
    typedef map<string, iTexture*> TextureMap;
    typedef map<string, iRenderer*> RendererMap;
    iSystem *iSystem::s_Instance;
    iTexture::iTexture() : internalData(0)
    {
    }
    iTexture::~iTexture()
    {
        if (internalData)
        {
            string *s = (string *)internalData;
            delete s;
        }
    }
    const char *iTexture::getFilename() const
    {
        if (internalData)
        {
            return ((string *)internalData)->c_str();
        }
        return 0;
    }
    struct iCurveDataSetInternal
    {
        string name;
    };
    iCurveDataSet::iCurveDataSet() : internalData(0)
    {
    }
    iCurveDataSet::~iCurveDataSet()
    {
        if (internalData)
        {
            iCurveDataSetInternal *i = (iCurveDataSetInternal *)internalData;
            delete i;
        }
    }
    void iCurveDataSet::setDataSetName(const char *name)
    {
        iCurveDataSetInternal *i = (iCurveDataSetInternal *)internalData;
        if (!i)
        {
            i = new iCurveDataSetInternal;
            i->name = name;
            internalData = (void *)i;
        }
        else
        {
            i->name = name;
        }
    }
    const char *iCurveDataSet::getDataSetName() const
    {
        if (internalData)
        {
            return ((iCurveDataSetInternal *)internalData)->name.c_str();
        }
        return 0;
    }

    bool iCurveDataSet::loadFromByteStream(iByteStream *bs)
    {
        unsigned short size;
        if (!bs->read(size))
            return false;
        resize(size);
        size_t dataSize = getDataSize();
        char *buf = new char[dataSize];
        for (size_t i = 0; i < size; i++)
        {
            int time;
            if (!bs->read(time) || !bs->read(buf, dataSize))
            {
                delete [] buf;
                resize(0);
                return false;
            }
            setData(i, buf, time);
        }
        delete [] buf;
        return true;
    }

    bool iCurveDataSet::saveToByteStream(iByteStream *bs) const
    {
        unsigned short size = (unsigned short)getDataCount();
        if (!bs->write(size))
            return false;
        size_t dataSize = getDataSize();
        char *buf = new char[dataSize];
        for (size_t i = 0; i < size; i++)
        {
            int time = getTime(i);
            if (!bs->write(time) || !bs->write(getData(i), dataSize))
            {
                delete [] buf;
                return false;
            }
        }
        delete [] buf;
        return true;
    }

    bool iCurveDataSet::loadFromXml(const TiXmlElement *elem)
    {
        assert(elem);
        //int numData;
        const char *name = elem->Attribute("name");
        if (!name)
            return false;
        setDataSetName(name);
        const char *type = elem->Attribute("type");
        if (strcmp(type, getTypeName()) != 0)
            return false;

        //if (!elem->Attribute("numData", &numData) || numData < 0 || numData > 65535)
        //    return false;

        //resize((size_t)numData);

        vector<std::pair<int, string> > vecData;

        size_t i = 0;
        for (const TiXmlElement *child = (const TiXmlElement *)elem->FirstChild("Data"); child;// && i < (size_t)numData;
            child = (const TiXmlElement *)child->NextSibling("Data"), i++)
        {
            int time;
            if (!child->Attribute("time", &time))
            {
                resize(0);
                return false;
            }

            //this->setTime(i, time);
            //this->setDataString(i, child->Attribute("value"));
            vecData.push_back(std::make_pair(time, child->Attribute("value")));
        }

        resize(vecData.size());
        for (i = 0; i < vecData.size(); i++)
        {
            this->setTime(i, vecData[i].first);
            this->setDataString(i, vecData[i].second.c_str());
        }
        //if (i < (size_t)numData)
        //{
        //    resize(0);
        //    return false;
        //}

        return true;
    }

    bool iCurveDataSet::saveToXml(TiXmlElement *elem) const
    {
        assert(elem);
        const char *dsn = getDataSetName();
        if (!dsn)
            return false;
        elem->SetAttribute("name", dsn);
        const char *type = getTypeName();
        if (!dsn)
            return false;
        elem->SetAttribute("type", type);
        size_t size = getDataCount();
        //elem->SetAttribute("numData", (int)size);
        for (size_t i = 0; i < size; i++)
        {
            TiXmlElement dataElem("Data");
            dataElem.SetAttribute("time", getTime(i));
            dataElem.SetAttribute("value", getDataString(i));
            elem->InsertEndChild(dataElem);
        }
        return true;
    }
    struct iSystemData
    {
        TextureMap texMap;
        RendererMap rendererMap;

        ~iSystemData()
        {

        }
        static iSystemData &GetInstance();
    };
    static iSystemData s_SystemData;
    iSystemData &iSystemData::GetInstance()
    {
        return s_SystemData;
    }

    /// @fn getRenderer
    /// @brief get the specific renderer
    /// @param name identification of the renderer
    iRenderer *iSystem::getRenderer(const char *name) const
    {
        assert(name);
        if (!name)
            return 0;

        RendererMap &rendererMap = iSystemData::GetInstance().rendererMap;
        RendererMap::iterator ir = rendererMap.find(name);
        if (ir == rendererMap.end())
        {
            assert(0 && "renderer not found.");
            return 0;
        }
        return ir->second;
    }
    void iSystem::attach(iRenderer *r)
    {
        assert(r);
        if (!r)
            return;

        RendererMap &rendererMap = iSystemData::GetInstance().rendererMap;
        string name = r->getName();
        RendererMap::iterator ir = rendererMap.find(name);
        if (ir == rendererMap.end())
        {
            rendererMap.insert(RendererMap::value_type(name, r));
        }
        else
        {
            if (ir->second == r)
            {
                assert(0 && "renderer already attached.");
            }
            else
            {
                assert(0 && "duplicated renderer name.");
            }
        }
    }
    void iSystem::dettach(iRenderer *r)
    {
        assert(r);
        if (!r)
            return;

        RendererMap &rendererMap = iSystemData::GetInstance().rendererMap;
        string name = r->getName();
        RendererMap::iterator ir = rendererMap.find(name);
        if (ir == rendererMap.end())
        {
            assert(0 && "renderer not found.");
        }
        else
        {
            rendererMap.erase(ir);
        }
    }
    void iSystem::release()
    {
        TextureMap &texMap = iSystemData::GetInstance().texMap;
        for (TextureMap::iterator r = texMap.begin(); r != texMap.end(); ++r)
        {
            release(r->second);
        }
    }

    iTexture *iSystem::getTexture(const char *name)
    {
        TextureMap &texMap = iSystemData::GetInstance().texMap;
        TextureMap::iterator r = texMap.find(name);
        if (r == texMap.end())
        {
            iTexture *tex = allocTexture();
            if (tex)
            {
                texMap.insert(r, TextureMap::value_type(name, tex));
                tex->internalData = new string(name);
            }
            return tex;
        }
        return r->second;
    }
    iAnimation *iSystem::createAnimation(size_t animCount)
    {
        return new Animation(animCount);
    }
    iAnimResManager *iSystem::createAnimResManager()
    {
        return new AnimResManager;
    }
    void iSystem::release(iAnimation *a)
    {
        delete a;
    }
    void iSystem::release(iAnimResManager *arm)
    {
        delete arm;
    }

    void *iClipState::_getState(StateId sid)
    {
        return const_cast<void *>(getState(sid));
    }

};