
#import <cstdio>
#import "locusdata.h"


namespace ogl2d
{

bool readLocusData(const char *filename, bitstream *out)
{
    assert(0 && "not completed yet");
    return false;
#if 0
    LocusDataHead head;
    FILE *fp = fopen(filename, "rb");
    fread(head, sizeof(head), 1, fp);
    if (*(unsigned long)head.sig != 'LCSD')
    {
        fclose(fp);
        return false;
    }
    unsigned long trunkpos = head.firsttrunk;
    while()
    out->write();
    fclose(fp);
#endif
}
bool writeLocusData(const char *filename, bitstream *in)
{
    assert(0 && "not completed yet");
    return false;
}


};