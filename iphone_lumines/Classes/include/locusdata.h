/* LocusData.h
    
*/

#import "locus.h"
#import "bitstream.h"

namespace ogl2d
{
struct LocusDataHead
{
    char sig[4]; // 'LCSD'
    unsigned long firsttrunk;
};
struct LocusTrunk
{
    unsigned long nexttrunk;
    unsigned long datalenth;
    char name[32];
};
bool readLocusData(const char *filename, bitstream *out);
bool writeLocusData(const char *filename, bitstream *in);

};