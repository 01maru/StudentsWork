#include "Matrix.h"
#include "Window.h"
#include <cmath>

#define MAX_ERR 1e-10

MyMath::Matrix::Matrix()
{
	Identity();
}

void MyMath::Matrix::Identity()
{
	for (size_t x = 0; x < 4; x++)
	{
		for (size_t y = 0; y < 4; y++)
		{
			if (x == y) m[y][x] = 1.0f;
			else		m[y][x] = 0.0f;
		}
	}
}

MyMath::Matrix& MyMath::Matrix::operator*=(const Matrix& m2)
{
	Matrix prev(*this);

	for (size_t y = 0; y < 4; y++)
	{
		for (size_t x = 0; x < 4; x++)
		{
			m[y][x] = prev.m[y][0] * m2.m[0][x] + prev.m[y][1] * m2.m[1][x] + prev.m[y][2] * m2.m[2][x] + prev.m[y][3] * m2.m[3][x];
		}
	}

	return *this;
}

const MyMath::Matrix MyMath::operator*(const Matrix& m1, const Matrix& m2)
{
    Matrix temp(m1);
    return temp *= m2;
}

bool MyMath::InverseMatrix(const Matrix& mat, Matrix& invMat)
{
    float sweep[4][8];

    for (size_t y = 0; y < 4; y++) {
        for (size_t x = 0; x < 4; x++) {
            // sweepの左側に逆行列を求める行列をセット
            sweep[y][x] = mat.m[y][x];

            // sweepの右側に単位行列をセット
            if (y == x) {
                sweep[y][4 + x] = 1;
            }
            else {
                sweep[y][4 + x] = 0;
            }
        }
    }


    /* 全ての列の対角成分に対する繰り返し */
    for (size_t k = 0; k < 4; k++) {

        /* 最大の絶対値を注目対角成分の絶対値と仮定 */
        double max = fabs(sweep[k][k]);
        size_t max_i = k;

        /* k列目が最大の絶対値となる行を探す */
        for (size_t y = k + 1; y < 4; y++) {
            if (fabs(sweep[y][k]) > max) {
                max = fabs(sweep[y][k]);
                max_i = y;
            }
        }

        if (fabs(sweep[max_i][k]) <= MAX_ERR) {
            /* 逆行列は求められない */
            return false;
        }

        /* 操作（１）：k行目とmax_i行目を入れ替える */
        if (k != max_i) {
            for (size_t x = 0; x < 8; x++) {
                double tmp = sweep[max_i][x];
                sweep[max_i][x] = sweep[k][x];
                sweep[k][x] = (float)tmp;
            }
        }

        /* sweep[k][k]に掛けると1になる値を求める */
        float a = 1 / sweep[k][k];

        /* 操作（２）：k行目をa倍する */
        for (size_t x = 0; x < 8; x++) {
            /* これによりsweep[k][k]が1になる */
            sweep[k][x] *= a;
        }

        /* 操作（３）によりk行目以外の行のk列目を0にする */
        for (size_t y = 0; y < 4; y++) {
            if (y == k) {
                /* k行目はそのまま */
                continue;
            }

            /* k行目に掛ける値を求める */
            a = -sweep[y][k];

            for (size_t x = 0; x < 8; x++) {
                /* i行目にk行目をa倍した行を足す */
                /* これによりsweep[i][k]が0になる */
                sweep[y][x] += sweep[k][x] * a;
            }
        }
    }

    /* sweepの右半分がmatの逆行列 */
    for (size_t y = 0; y < 4; y++) {
        for (size_t x = 0; x < 4; x++) {
            invMat.m[y][x] = sweep[y][4 + x];
        }
    }

    return true;
}

MyMath::Matrix MyMath::Create2DTransformMatrix()
{
    Matrix ans;

    ans.m[0][0] = 2.0f / MNE::Window::sWIN_WIDTH;
    ans.m[1][1] = -2.0f / MNE::Window::sWIN_HEIGHT;
    ans.m[3][0] = -1.0f;
    ans.m[3][1] = 1.0f;

    return ans;
}
