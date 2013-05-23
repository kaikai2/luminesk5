#ifndef SOUNDLIST_PARSER_H
#define SOUNDLIST_PARSER_H

#import <vector>
#import <string>

using namespace std;

namespace Lumines_SoundList
{
enum SoundEffectId
{
    SE_LR = 0,  // 左右移动
    SE_Down, // 按下下落
    SE_L_Rot,   // 逆时针旋转
    SE_R_Rot,   // 顺时针旋转
    SE_On,  // 
    SE_Dash, // 快速移动
    SE_Fall1,
    SE_Fall2,
    SE_Fall3,
    SE_Fall4,
    SE_Fall5,
    SE_Lvup,    // 升级
    SE_Bonus,
    SE_dec_1,   // 联体1
    SE_dec_2,   // 联体2
    SE_dec_st,  // 宝石联体
    SE_dec_lp,  // 节奏线开始扫可消方块
    SE_rap,
    SE_Count,
};
struct SoundTag
{
    string filename;
};
struct SoundEffectTag : public SoundTag
{
    int priority;
    /*
    int bread;
    int volume;
    int exclude;
    reserved
    string command;
    */
};
struct TrackTag : public SoundTag
{
    int id;      // 排序
    int mesures; // 单个音乐占多少小节
    int loop;    // 一次连续播放多少小节
    /*
    bread
    volume
    command
    */
    bool operator < (const TrackTag &o) const
    {
        return (id) < o.id;
    }
};
struct Command
{
    enum CmdId
    {
        CMD_back,
        CMD_eff_on,
        CMD_eff_off,
    };
    CmdId cmdid; //
    string params;
};
struct TrackSeqNode
{
    int id;
    vector< int > trackTagIndex;
    vector< Command > cmds;
    bool operator < (const TrackSeqNode &o) const
    {
        return (id) < o.id;
    }
};
struct SoundListData
{
    int bpm;        // beats per minute
    int rhythm[2];  // [0]BeatsPerMeasure [1]GridsPerBeat
    int measure;    //  
    SoundEffectTag soundeffect[SE_Count];
    vector<TrackTag> track;
    vector<TrackSeqNode> trackSequence;
    void clear()
    {
        bpm = 0;
        rhythm[0] = rhythm[1] = 0;
        measure = 0;
        for (int i = 0; i < SE_Count; i++)
        {
            soundeffect[i].filename = "";
            soundeffect[i].priority = 0;
        }
        track.clear();
        trackSequence.clear();
    }
    bool checkDataValid()
    {
        return true;
    }
};

class SoundListParser
{
public:
    bool Load(const char *filename, SoundListData &data);
protected:
    const char* token(const string &s, const char delim)
    {
        lastEnd = 0;
        tokenString = s;
        return nextToken(delim);
    }
    const char* nextToken(const char delim)
    {
        if (lastEnd == string::npos)
            return NULL;
        const char *p = tokenString.c_str() + lastEnd;
        if ((lastEnd = tokenString.find(lastEnd, delim)) != string::npos)
            lastEnd++;
        return p;
    }
    string tokenString;
    string::size_type lastEnd;
};

}

#endif//SOUNDLIST_PARSER_H