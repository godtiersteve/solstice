#ifndef HEALTHBAR_H_INCLUDED
#define HEALTHBAR_H_INCLUDED
// TODO (Stephen#1#): Finish implementing HealthBar

namespace solstice {
    enum HealthBarStyle {HEALTH_VERTICAL, HEALTH_HORIZONTAL, HEALTH_RADIAL, HEALTH_LIVES};
    class HealthBar {
    protected:
        int mMaxHealth, mHealth;
        int mMinDamage;
        HealthBarStyle mStyle;
        bool mRegenerates;
        int mRegenerationRate;
        bool mDamagedOverTime;
        int mDotRate;
        int mCurFrame;
        //Bleed effect - HP will be reduced at once, but the HP bar will have a background bar that follows it down
        bool mBleeds;

        QuadData mQuadData;
    public:
        HealthBar();
        HealthBar(int maxHealth, HealthBarsStyle style = HEALTH_HORIZONTAL);

        void SetMaxHealth(int max);
        void SetHealth(int health);
        void SetMinDamage(int min);
        void SetStyle(HealthBarStyle style);
        void SetRegens(bool regens);
        void SetRegenRate(int regenrate);

        int GetMaxHealth();
        int GetHealth();
        int GetMinDamage();
        HealthBarStyle GetStyle();
        bool GetRegens();
        int GetRegenRate();

        void Hurt(int damage);
        void HurtDot(int damage, int frames);
        void Heal(int heal);
        void Update();

        QuadData& GetQuadData();
    };
}


#endif // HEALTHBAR_H_INCLUDED
