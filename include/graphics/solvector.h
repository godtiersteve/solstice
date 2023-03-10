#ifndef SOLVEC_H_INCLUDED
#define SOLVEC_H_INCLUDED

#include <vector>
#include <iostream>

#include "graphics/vector2.h"
#include "graphics/color.h"

using std::vector;
using std::cout;
using std::endl;

struct Color;

namespace solstice {
    class SolVector {
        private:
            vector<float> mElements;
        public:
            SolVector();
            SolVector(int size);
            SolVector(int size, float value);
            SolVector(vector<float> v);
            SolVector(const SolVector& v);
            SolVector(float* v, int size);
            SolVector(Vector2<float> v);
            SolVector& operator=(const SolVector& v);
            SolVector& operator+=(const SolVector& v);
            const SolVector operator*(float f);
            SolVector operator+(const SolVector& v);
            SolVector& operator-=(const SolVector& v);
            float operator*(const SolVector& v);
            SolVector CrossProduct(const SolVector& v);
            SolVector CrossProduct();
            float DotProduct(const SolVector& v);

            void Init(float size);
            void InitValues(float value);
            void Init(vector<float> values);
            void Init(float values[], int size);
            void Init(Vector2<float> v);
            void Clear();
            int Size() const;
            void Resize(unsigned int i);
            bool Empty();
            float GetElement(int i) const;
            void SetElement(int i, float value);
            void AddElement(float value);

            SolVector GetUnit();
            SolVector GetNormal();
            float GetMagnitude() const;

            void Print();

            float& operator[](int i);
            SolVector& operator*=(float f);
            SolVector& operator=(const vector<float>& v);
            SolVector operator-();
            SolVector& operator=(Color c);
            friend SolVector operator/(SolVector& vec, float f);
            friend SolVector operator/(float f, SolVector& vec);
    };
    SolVector operator-(const SolVector& v, const SolVector& v2);
};

#endif // SOLVEC_H_INCLUDED
