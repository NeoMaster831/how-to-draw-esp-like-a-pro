#include "Esper.h"

vector<uintptr_t> Esper::getEntityPawns() {

    vector<uintptr_t> ret;
    auto entList = this->def.getEntityList();

    for (int i = 0; i < 64; i++) {
        try {
            auto entIdxList = this->def.RPM<uintptr_t>(entList + 16);
            if (!entIdxList) continue;
            auto entIdxController = this->def.RPM<uintptr_t>(120 * i + entIdxList);
            if (!entIdxController) continue;
            auto entPawnHandle = this->def.getPlayerPawn(entIdxController);
            if (!entPawnHandle) continue;
            entIdxList = this->def.RPM<uintptr_t>(entList + (8 * ((entPawnHandle & 0x7fff) >> 9)) + 16);
            if (!entIdxList) continue;
            auto entPawn = this->def.RPM<uintptr_t>(entIdxList + 120 * (entPawnHandle & 0x1ff));
            if (!entPawn) continue;
            ret.push_back(entPawn);
        }
        catch (int expn) {
            if (expn == 0x3d000005) continue;
            else ExitProcess(0x3d000006);
        }
    }

    return ret;
}

Vector3 Esper::worldToScreen(Vector3& location, Matrix4x4& viewMat, pair<int, int>& resolution) {
    auto m = viewMat.matrix;
    auto x = location.x, y = location.y, z = location.z;
    float _x = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3];
    float _y = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3];
    float w = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3];
    
    if (w < 0.01f) throw 0x3d000007;
    float inv_w = 1.f / w;
    _x *= inv_w;
    _y *= inv_w;

    x = resolution.first / 2.f;
    y = resolution.second / 2.f;
    
    x += 0.5f * _x * resolution.first + 0.5f;
    y -= 0.5f * _y * resolution.second + 0.5f;
    
    return { x, y, w };
}