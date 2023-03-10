#ifndef SHADERPARTICLE_H_INCLUDED
#define SHADERPARTICLE_H_INCLUDED

class ShaderParticle {
    std::vector<std::vector<float> > mVelocities;
    std::vector<std::vector<float> > mPositions;
    std::vector<std::vector<float> > mStartTimes;
};


#endif // SHADERPARTICLE_H_INCLUDED
