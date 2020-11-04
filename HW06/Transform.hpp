#ifndef __HEADER_TRANSFORM
#define __HEADER_TRANSFORM
#include "Matrix.hpp"
#include <vector>

namespace Math {
    namespace Transform {
        using namespace Matrix;

        struct ITransform {};

        class Transform2D : public ITransform {
        private:
            std::vector<Matrix3x3> TopStack;

        public:
            /// Push and pop matrix
            void Push(Matrix3x3* m1) {
                Matrix3x3 new_top;
                Matrix3x3 top; GetTop(&top);
                Matrix3x3::Multiply(m1, &top, &new_top);

                TopStack.push_back(new_top);
            }
            void Pop() {
                TopStack.pop_back();
            }

            /// return top to out parameter
            void GetTop(Matrix3x3* output) {
                if (TopStack.empty()) {
                    Matrix3x3::BuildIdentity(output);
                    return;
                }
                *output = TopStack.back();
            }

            /// transform a point
            void TransformPoint(Vec3* point, Vec3* output) {
                Matrix3x3 top; GetTop(&top);
                Matrix3x3::Multiply(point, &top, output);
            }
            Vec3* TransformPoint(Vec3* point) {
                Vec3* output = new Vec3();
                TransformPoint(point, output);
                return output;
            }

            /// transform a vector
            void TransformVector(Vec3* point, Vec3* output) {
                Matrix3x3 Matrix; GetTop(&Matrix);
                float tx = Matrix.V02; Matrix.V02 = 0;
                float ty = Matrix.V12; Matrix.V12 = 0;
                Matrix3x3::Multiply(point, &Matrix, output);
                Matrix.V02 = tx; Matrix.V12 = ty;
            }
            Vec3* TransformVector(Vec3* point) {
                Vec3* output = new Vec3();
                TransformVector(point, output);
                return output;
            }
        };

        class Transform3D : public ITransform {
        private:
            std::vector<Matrix4x4> TopStack;

        public:
            /// Push and pop matrix
            void Push(Matrix4x4* m1) {
                Matrix4x4 new_top;
                Matrix4x4 top; GetTop(&top);
                Matrix4x4::Multiply(m1, &top, &new_top);

                TopStack.push_back(new_top);
            }
            void Pop() {
                TopStack.pop_back();
            }

            /// return top to out parameter
            void GetTop(Matrix4x4* output) {
                if (TopStack.empty()) {
                    Matrix4x4::BuildIdentity(output);
                    return;
                }
                *output = TopStack.back();
            }
            /// Clear all data and make top identity
            void Clear() {
                TopStack.clear();
            }

            /// transform a point
            void TransformPointSafe(Vec4* point, Vec4* output) {
                Matrix4x4 top; GetTop(&top);
                Matrix4x4::MultiplySafe(point, &top, output);
            }
            void TransformPoint(Vec4* point, Vec4* output) {
                Matrix4x4 top; GetTop(&top);
                Matrix4x4::Multiply(point, &top, output);
            }
            Vec4* TransformPoint(Vec4* point) {
                Vec4* output = new Vec4();
                TransformPoint(point, output);
                return output;
            }

            /// transform a vector
            void TransformVectorSafe(Vec4* point, Vec4* output) {
                Matrix4x4 Matrix; GetTop(&Matrix);
                float tx = Matrix.V03; Matrix.V03 = 0;
                float ty = Matrix.V13; Matrix.V13 = 0;
                float tz = Matrix.V23; Matrix.V23 = 0;
                Matrix4x4::MultiplySafe(point, &Matrix, output);
                Matrix.V03 = tx; Matrix.V13 = ty; Matrix.V23 = tz;
            }
            void TransformVector(Vec4* point, Vec4* output) {
                Matrix4x4 Matrix; GetTop(&Matrix);
                float tx = Matrix.V03; Matrix.V03 = 0;
                float ty = Matrix.V13; Matrix.V13 = 0;
                float tz = Matrix.V23; Matrix.V23 = 0;
                Matrix4x4::Multiply(point, &Matrix, output);
                Matrix.V03 = tx; Matrix.V13 = ty; Matrix.V23 = tz;
            }
            Vec4* TransformVector(Vec4* point) {
                Vec4* output = new Vec4();
                TransformVector(point, output);
                return output;
            }
        };
    }
}

#endif // __HEADER_TRANSFORM
