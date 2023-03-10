#include "solvector.h"
namespace solstice {
    SolVector::SolVector() {
    }
    SolVector::SolVector(int size) {
        for(int i = 0; i < size; i++) {
            mElements.push_back(0);
        }
    }
    SolVector::SolVector(int size, float value) {
        for(int i = 0; i < size; i++) {
            mElements.push_back(value);
        }
    }
    SolVector::SolVector(vector<float> v) {
        Init(v);
    }
    SolVector::SolVector(float* v, int size) {
        Init(v, size);
    }
    SolVector::SolVector(Vector2<float> v) {
        Init(v);
    }
    SolVector::SolVector(const SolVector& v) {
        *this = v;
    }
    SolVector& SolVector::operator=(Color c) {
        Clear();
        mElements.push_back(c.red);
        mElements.push_back(c.green);
        mElements.push_back(c.blue);
        mElements.push_back(c.alpha);
        return *this;
    }
    SolVector& SolVector::operator=(const SolVector& v) {
        if(this != &v) {
            Clear();
            for(int i = 0; i < v.Size(); i++) {
                mElements.push_back(v.GetElement(i));
            }
        }
        return *this;
    }
    void SolVector::Init(vector<float> values) {
        mElements.clear();
        for(int i = 0; i < values.size(); i++) {
            mElements.push_back(values[i]);
        }
    }
    void SolVector::Init(float values[], int size) {
        mElements.clear();
        for(int i = 0; i < size; i++) {
            mElements.push_back(values[i]);
        }
    }
    void SolVector::Init(float size) {
        mElements.clear();
        for(int i = 0; i < size; i++) {
            mElements.push_back(0);
        }
    }
    void SolVector::InitValues(float value) {
        for(int i = 0; i < Size(); i++) {
            mElements[i] = value;
        }
    }
    void SolVector::Init(Vector2<float> v) {
        mElements.clear();
        mElements.push_back(v.x);
        mElements.push_back(v.y);
    }
    void    SolVector::SolVector::Clear()                   {mElements.clear();}
    int     SolVector::SolVector::Size() const              {return mElements.size();}
    void    SolVector::Resize(unsigned int i)               {mElements.resize(i);}
    bool    SolVector::Empty()                              {return mElements.empty();}
    float&  SolVector::operator[](int i)                    {return mElements[i];}
    float SolVector::GetElement(int i) const                {return mElements[i];}
    SolVector& SolVector::operator*=(float f){
        for(int i = 0; i < mElements.size(); i++) {
            mElements[i] = mElements[i] * f;
        }
        return *this;
    }
    const SolVector SolVector::operator*(float f) {
        for(int i = 0; i < mElements.size(); i++) {
            mElements[i]*= f;
        }
        return *this;
    }

    float SolVector::operator*(const SolVector& v) {
        if(Size() != v.Size()) {
            return 0;
        }
        else {
            float total = 0;
            for(int i = 0; i < v.Size(); i++) {
                total+= v.GetElement(i) * mElements[i];
            }
            return total;
        }
    }
    float SolVector::DotProduct(const SolVector& v) {
        SolVector v2 = *this;
        return v2 * v;
    }
    SolVector SolVector::CrossProduct(const SolVector& v)
    {
        if(v.Size() != Size() || Size() != 3) {
            return 0;
        }
        vector<float> ret;
        ret.push_back(mElements[1] * v.GetElement(2) - mElements[2] * v.GetElement(1));
        ret.push_back(mElements[2] * v.GetElement(0) - mElements[0] * v.GetElement(2));
        ret.push_back(mElements[0] * v.GetElement(1) - mElements[1] * v.GetElement(0));
        return ret;
    }
    SolVector SolVector::CrossProduct() {
        if(Size() != 2) {
            return 0;
        }
        vector<float> ret;
        ret.push_back(-mElements[1]);
        ret.push_back(mElements[0]);
        return ret;
    }
    SolVector& SolVector::operator=(const vector<float>& v) {
        Clear();
        Init(v);
        return *this;
    }
    SolVector SolVector::operator-() {
        SolVector v = *this;
        for(int i = 0; i < v.Size(); i++) {
            v[i] = -v[i];
        }
        return v;
    }
    SolVector SolVector::operator+(const SolVector& v) {
        if(v.Size() != Size()) {
            return SolVector();
        }
        else {
            vector<float> vec;
            for(int i = 0; i < Size(); i++) {
                vec.push_back(v.GetElement(i) + mElements[i]);
            }
            return vec;
        }
    }
    SolVector& SolVector::operator+=(const SolVector& v) {
        *this = *this + v;
        return *this;
    }
    SolVector operator-(const SolVector& v, const SolVector& v2) {
        if(v.Size() != v2.Size()) {
            return SolVector();
        }
        else {
            SolVector vec(v2.Size());
            for(size_t i = 0; i < vec.Size(); i++) {
                vec[i] = v.GetElement(i) - v2.GetElement(i);
            }
            return vec;
        }
    }
    SolVector& SolVector::operator-=(const SolVector& v){
       *this = *this - v;
        return *this;
    }
    SolVector operator/(SolVector& vec, float f) {
        SolVector v = vec;
        for(int i = 0; i < v.Size(); i++) {
            v[i]/= f;
        }
        return v;
    }
    SolVector operator/(float f, SolVector& vec) {
        SolVector v = vec;
        for(int i = 0; i < v.Size(); i++) {
            v[i]/= f;
        }
        return v;
    }
    void SolVector::SetElement(int i, float value) {
        if(i < 0 || i > mElements.size()) {
            return;
        }
        if(mElements.empty()) {
            return;
        }
        mElements[i] = value;
    }
    void SolVector::AddElement(float value) {
        mElements.push_back(value);
    }
    void SolVector::Print() {
        for(int i = 0; i < Size(); i++) {
            cout << "[" << mElements[i] << "]\n";
        }
    }
    SolVector SolVector::GetUnit() {
        SolVector v;
        v = *this;
        float length = GetMagnitude() != 0 ? GetMagnitude() : 1;
        v = v / length;
        return v;
    }
    SolVector SolVector::GetNormal() {
        SolVector v;
        v = *this;
        if(mElements.size() == 2) {
            float x = v[0];
            float y = v[1];
            v[0] = -y;
            v[1] = x;
        }
        return v;
    }
    float SolVector::GetMagnitude() const {
        float total = 0;
        for(int i = 0; i < mElements.size(); i++) {
           total+= mElements[i] * mElements[i];
        }
        return sqrt(total);
    }
};
