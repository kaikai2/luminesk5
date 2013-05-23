//#import <cstdlib>
#import <cassert>
#import <vector>
#import <string>
//#import <iostream>
#import <fstream>
#import <algorithm>

#import "soundlist_parser.h"

namespace Lumines_SoundList
{
    using namespace std;

bool SoundListParser::Load(const char *filename, SoundListData &data)
{
    assert(filename != NULL);

    string linebuf;
    ifstream f(filename);

    data.clear();

    while(getline(f, linebuf))
    {
        if (linebuf[0] != '[') // ignore
            continue;

        string::size_type pos = linebuf.find(']');
        if (pos == string::npos) // invalid
            continue;

        string tag = linebuf.substr(1, pos);
        token(linebuf, ',');
        if (tag == "BPM")
        {
            data.bpm = atoi(nextToken(','));
        }
        else if (tag == "RHYTHM")
        {
            data.rhythm[0] = atoi(nextToken(','));
            data.rhythm[1] = atoi(nextToken(','));
        }
        else if (tag == "MEASURE")
        {
            data.measure = atoi(nextToken(','));
        }
        else if (tag.substr(0,3) == "SE_")
        {
            int id = atoi(tag.c_str() + 3);
            if (id > 0 && id <= SE_Count)
            {
                SoundEffectTag &se = data.soundeffect[id-1];
                se.filename = nextToken(',');
                se.priority = atoi(nextToken(','));
                /* se.bread = */ nextToken(',');
                /* se.volume = */ nextToken(',');
                /* se.exclude = */ nextToken(',');
                /* se.reserved = */ nextToken(',');
                /* se.cmd = */ nextToken(',');
            }
        }
        else if (tag.substr(0,3) == "TR_")
        {
            int id = atoi(tag.c_str() + 3);
            if (id > 0)
            {
                TrackTag tr;
                tr.id = id;
                tr.filename = nextToken(',');
                tr.mesures = atoi(nextToken(','));
                tr.loop = atoi(nextToken(','));
                /* tr.bread = */ nextToken(',');
                /* tr.volume = */ nextToken(',');
                /* tr.cmd = */ nextToken(',');
                data.track.push_back(tr);
            }
        }
        else if (tag.substr(0,4) == "SEQ_")
        {
            int id = atoi(tag.c_str() + 3);
            if (id > 0)
            {
                TrackSeqNode seq;
                seq.id = id;
                for (int i = 0; i < 5; i++)
                {
                    const char *n = nextToken(',');
                    if (n == NULL || *n < '0' && *n > '9')
                        break;
                    seq.trackTagIndex.push_back(atoi(n));
                }
            }
        }
        else
        {
            return false;
        }
    }
    sort(data.track.begin(), data.track.end());
    sort(data.trackSequence.begin(), data.trackSequence.end());
    // f.close();
    return data.checkDataValid();
}

}
