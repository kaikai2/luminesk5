#ifndef LUMINES_SPECTRUM_H
#define LUMINES_SPECTRUM_H

class Spectrum
{
public:
    Spectrum(unsigned int _size, unsigned int _height);
    virtual ~Spectrum();
    void step(std::vector<DWORD> &rVecCurrentChannel);
    void render();
private:
	Spectrum &operator = (const Spectrum &){return *this;}

    const unsigned int size;
    const unsigned int height;
    unsigned int *data;
};

#endif//LUMINES_SPECTRUM_H
