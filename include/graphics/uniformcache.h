#ifndef UNIFORMCACHE_H_INCLUDED
#define UNIFORMCACHE_H_INCLUDED

///TODO : Add Uniform caching to models
namespace solstice {
template<class T>
class UCache {
    T data;
    int loc;
    UCache() {
        loc = -1;
    }
};
typedef UCache<float> Ucachef;
typedef UCache<int> Ucachei;
typedef UCache<double> Ucached;
typedef UCache<unsigned int> Ucacheu;
typedef UCache<bool> Ucacheb;

}

#endif // UNIFORMCACHE_H_INCLUDED
