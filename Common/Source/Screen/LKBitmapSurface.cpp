/*
 * LK8000 Tactical Flight Computer -  WWW.LK8000.IT
 * Released under GNU/GPL License v.2
 * See CREDITS.TXT file for authors and copyrights
 *
 * File:   LKBitmapSurface.cpp
 * Author: Bruno de Lacheisserie
 * 
 * Created on 16 octobre 2014, 21:49
 */

#include "LKBitmapSurface.h"
#include <cassert>

#ifndef WIN32
#include "Screen/VirtualCanvas.hpp"
#endif

LKBitmapSurface::LKBitmapSurface() : LKSurface()
#ifdef WIN32
    , _hBitmap(), _oldBitmap() 
#endif
{
}

LKBitmapSurface::~LKBitmapSurface() {
    Release();
}

LKBitmapSurface::LKBitmapSurface(LKSurface& Surface, unsigned width, unsigned height) : LKSurface() 
#ifdef WIN32
    , _hBitmap(), _oldBitmap() 
#endif
{
    Create(Surface, width, height);
}

void LKBitmapSurface::Create(const LKSurface& Surface, unsigned width, unsigned height) {
#ifdef WIN32
    Attach(::CreateCompatibleDC(Surface.GetAttribDC()));
    SetAttribDC(Surface.GetAttribDC());

    _hBitmap = LKBitmap (::CreateCompatibleBitmap(GetAttribDC(), width, height));
    _oldBitmap = LKBitmap((HBITMAP)::SelectObject(_OutputDC, _hBitmap));
#else
    _pCanvas = new VirtualCanvas(Surface, {width, height});
#endif    
}

void LKBitmapSurface::Resize(unsigned width, unsigned height) {
#ifdef WIN32
    if (_oldBitmap) {
        ::SelectObject(_OutputDC, _oldBitmap);
        _oldBitmap.Release();
    }
    if (_hBitmap) {
        _hBitmap.Release();
    }

    _hBitmap = LKBitmap (::CreateCompatibleBitmap(GetAttribDC(), width, height));
    _oldBitmap = LKBitmap((HBITMAP)::SelectObject(_OutputDC, _hBitmap));
#else
    if(_pCanvas) {
        static_cast<VirtualCanvas*>(_pCanvas)->Resize({width, height});
    }
#endif
}

void LKBitmapSurface::Release() {
#ifdef WIN32
    if (_oldBitmap) {
        ::SelectObject(_OutputDC, _oldBitmap);
        _oldBitmap.Release();
    }
    if (_hBitmap) {
        _hBitmap.Release();
    }
#else
    if(_pCanvas) {
        static_cast<VirtualCanvas*>(_pCanvas)->Destroy();
    }
    delete _pCanvas;
    _pCanvas = nullptr;
#endif
    LKSurface::Release();
}


void LKMaskBitmapSurface::Create(const LKSurface& Surface, unsigned width, unsigned height) {
#ifdef WIN32
    Attach(::CreateCompatibleDC(Surface.GetAttribDC()));
    SetAttribDC(Surface.GetAttribDC());

    _hBitmap = LKBitmap(::CreateBitmap(width, height, 1, 1, NULL));
    _oldBitmap = LKBitmap((HBITMAP)::SelectObject(_OutputDC, _hBitmap));
#endif
}

void LKMaskBitmapSurface::Resize(unsigned width, unsigned height) {
#ifdef WIN32
    if (_oldBitmap) {
        ::SelectObject(_OutputDC, _oldBitmap);
        _oldBitmap.Release();
    }
    if (_hBitmap) {
        _hBitmap.Release();
    }

    _hBitmap = LKBitmap(::CreateBitmap(width, height, 1, 1, NULL));
    _oldBitmap = LKBitmap((HBITMAP)::SelectObject(_OutputDC, _hBitmap));
#endif
}