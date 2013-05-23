#ifndef LUMINES_SPLITTER_H
#define LUMINES_SPLITTER_H



class Player;
class Lumines;

/// LuSplitter
/// @note	��Ϸ����ָ��ߣ���ֹHandͨ��
class LuSplitter
{
public:
    LuSplitter();
    //virtual ~LuSplitter();

    void init(Lumines *_lumines, Player *leftHand, Player *rightHand, int width, int pos);
    
    void moveLeft();
    void moveRight();

    void render() const;

    void step();
protected:
    friend class Player;
    int getSplitPos(){return m_Pos;}

private:
    void updateHandLimit();
    int m_Pos;  // the split position
    int m_Width;  // total width of gamearea that contains the splitter
    Lumines * m_pLumines;
    Player *m_pLeftHand, *m_pRightHand; // ����

    int m_lastLeftThroughCount;
};


#endif//LUMINES_SPLITTER_H