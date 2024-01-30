#pragma once
#include "Defines.h"
#include "GDI.h"
#include <vector>
#include <iostream>

#define Vector3 Defines::Vector3
#define Matrix4x4 Defines::Matrix4x4

typedef struct _EsperSettings {
    int thickness;
    float size;
    bool showName;
    bool showHP;
    bool showTeam;
    bool highlightTeam;
} EsperSettings;

class Esper
{
    // Esper - Main of Esper(ESP)

public:

    Defines def;
    GDI gdi;

    // You can set further down the *default* values
    
    // Static values (can't edit by users)
    bool running = true;
    bool useGUI = false;

    // Dynamic values
    EsperSettings settings = {
        2,
        1.0f,
        false, false, false, false
    };

    // End

    Esper(LPCWSTR proc, LPCWSTR mod, LPCWSTR window, WNDCLASSEX& wndClassEx) : def(proc, mod), gdi(window, &def, wndClassEx, this) {}
    vector<uintptr_t> getEntityPawns();
    Vector3 worldToScreen(Vector3& location, Matrix4x4& viewMat, pair<int, int>& resolution);
};

