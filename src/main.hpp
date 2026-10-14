#pragma once

#include "delivery/Manager.hpp"
#include "ui/RandomizerFrame.hpp"
#include <iostream>
#include <wx/snglinst.h>
#include <wx/wx.h>

class MyApp : public wxApp {
  private:
    wxSingleInstanceChecker *checker = nullptr;

  public:
    virtual bool OnInit();
    virtual int OnExit();
};
