#include <string>
#include "glCurvedani.h"
#include "caInterpolaters.h"
#include "caFileStream.h"

int main(int argc, char *argv[])
{	
    if (argc == 1)
    {
        printf("Usage:\n");
        printf("converter <in> [out]\n");
        printf("if <in> is .xml then converter will try to parse it and convert it to binary animation file <out>,\n");
        printf("otherwise, the converter will treat it as a binary animation file and convert it to a .xml file.\n");
        return 0;
    }

    std::string out;
    if (argc == 2)
    {
        out = argv[1];
        out += ".xml";
    }
    else
    {
        out = argv[2];
    }

    glCurvedAniSystem system; // NOTE: this system will release before hge's shutdown

    char *s2 = _strdup(argv[1]);
    if (!s2)
        return 0;

    cAni::iAnimResManager &manager = *iSystem::GetInstance()->createAnimResManager();
    const cAni::AnimData *pAnimData = manager.getAnimData(argv[1]);
    if (!pAnimData)
    {
        iSystem::GetInstance()->release(&manager);
        fprintf(stderr, "failed to load from <in>.\n");
        return -1;
    }
    std::vector<const char *> pathTokens;
    for (const char *pathToken = strtok(s2, "/\\"); pathToken; pathToken = strtok(0, "/\\"))
    {
        pathTokens.push_back(pathToken);
    }
    const char *cdot = strrchr(pathTokens.back(), '.');
    bool bXmlFile = false;
    if (cdot && 0 == strcmp(cdot, ".xml"))
        bXmlFile = true;
    free(s2);

    if (bXmlFile)
    {    
        cAni::FileStream stream;
        if (!stream.openFile(out.c_str(), "wb"))
        {
            iSystem::GetInstance()->release(&manager);
            fprintf(stderr, "failed to open output file for writing.\n");
            return -1;
        }
        if (!manager.saveToByteStream(pAnimData, &stream))
        {
            iSystem::GetInstance()->release(&manager);
            fprintf(stderr, "failed to save to output file.\n");
            return -1;
        }
    }
    else
    {
        if (!manager.saveToXml(pAnimData, out.c_str()))
        {
            iSystem::GetInstance()->release(&manager);
            fprintf(stderr, "failed to save to output file.\n");
            return -1;
        }
    }
    iSystem::GetInstance()->release(&manager);
    return 0;
}
