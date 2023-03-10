#ifndef WATERSHED_SETUP_H_INCLUDED
#define WATERSHED_SETUP_H_INCLUDED
#include <vector>
#include "animation/animation.h"
namespace solstice {
std::vector<Animation> GenIonaAnimations() {
    std::vector<Animation> animations;
	animations.push_back(solstice::Animation());
	animations[0].SetTexHW(2048, 2048);
	animations[0].AutoAddFramesSheet("ionatorso", 6, 128, 128, 128, 0);
	animations[0].SetRef("idle");
	animations[0].SetDuration(80);
	animations.back().SetLoop(-1);

	animations.push_back(solstice::Animation());
	animations[1].SetTexHW(2048, 2048);
	animations[1].AutoAddFramesSheet("ionatorso", 8, 128, 128, 256, 0);
	animations[1].SetRef("walking");
	animations[1].SetDuration(80);
	animations.back().SetLoop(-1);

	animations.push_back(solstice::Animation());
	animations[2].SetTexHW(2048, 2048);
	animations[2].AutoAddFramesSheet("ionatorso", 2, 128, 128, 384, 0);
	animations[2].SetRef("jumping");
	animations.back().SetDuration(80);
	animations.back().SetLoop(-1);

	animations.push_back(solstice::Animation());
	animations[3].SetTexHW(2048, 2048);
	animations[3].AutoAddFramesSheet("ionatorso", 6, 128, 128, 384, 2 * 128);
	animations[3].SetRef("falling");
	animations[3].SetLoopFrame(80);
	animations.back().SetDuration(80);
	animations.back().SetLoop(-1);

	animations.push_back(solstice::Animation());
	animations.back().SetTexHW(2048, 2048);
	animations.back().AutoAddFramesSheet("ionatorso", 6, 128, 128, 4 * 128, 0);
	animations.back().SetRef("swimIdle");
	animations.back().SetDuration(80);
	animations.back().SetLoop(-1);

	animations.push_back(solstice::Animation());
	animations.back().SetTexHW(2048, 2048);
	animations.back().AutoAddFramesSheet("ionatorso", 6, 128, 128, 5 * 128, 0);
	animations.back().SetLoop(-1);
	animations.back().SetDuration(80);
	//animations.back().GetFrame(0).SetDuration(10);
	animations.back().SetRef("swimming");

	animations.push_back(solstice::Animation());
	animations.back().SetTexHW(2048, 2048);
	animations.back().AutoAddFramesSheet("ionatorso", 4, 128, 128, 6 * 128, 0);
	animations.back().SetDuration(80);
	//animations.back().GetFrame(3).SetDuration(10);
	animations.back().SetLoop(0);
	animations.back().SetRef("flipping");

	animations.push_back(solstice::Animation());
	animations.back().SetTexHW(2048, 2048);
	//animations.back().AutoAddFramesSheet("ionatorso", 8, 128, 128, 256, 0);
	animations.back().AutoAddFramesSheet("ionatorso", 7, 128, 128, 6 * 128, 8 * 128);
	animations.back().SetDuration(80);
	animations.back().SetLoop(0);
	animations.back().SetRef("skidding");
	animations.back().Reverse();

    animations.push_back(solstice::Animation());
	animations.back().SetTexHW(2048, 2048);
	//animations.back().AddFrame(animations[animations.size() - 2].GetFrame(2));
	animations.back().AutoAddFramesSheet("ionatorso", 4, 128, 128, 7 * 128, 12 * 128);
	animations.back().SetLoop(0);
	animations.back().SetRef("swimstart");
	animations.back().SetDuration(80);

    animations.push_back(solstice::Animation());
	animations.back().SetTexHW(2048, 2048);
	animations.back().AutoAddFramesSheet("ionatorso", 2, 128, 128, 6 * 128, 13 * 128);
	animations.back().SetDuration(80);
	animations.back().SetLoop(0);
	animations.back().SetRef("skidrecov");
	animations.back().Reverse();

	animations.push_back(solstice::Animation());
	animations.back().SetTexHW(2048, 2048);
	animations.back().AutoAddFramesSheet("ionatorso", 5, 128, 128, 5 * 128, 8 * 128);
	animations.back().SetDuration(80);
	//animations.back().GetFrame(0).SetDuration(3);
	//animations.back().GetFrame(0).SetDuration(10);
	animations.back().SetLoop(0);
	animations.back().SetRef("turning");
	//animations.back().Reverse();

    animations.push_back(solstice::Animation());
	animations.back().SetTexHW(2048, 2048);
	animations.back().AutoAddFramesSheet("ionatorso", 2, 128, 128, 1 * 128, 9 * 128);
	animations.back().SetDuration(80);
	animations.back().SetLoop(-1);
	//animations.back().SetLoopFrame(1);
	animations.back().SetRef("nudge");

	return animations;
}
}
#endif // WATERSHED_SETUP_H_INCLUDED
