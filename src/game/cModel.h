#pragma once

#include "cModelBase.h"

class cModel : public cModelBase
{
public:
  int field_370;
  int field_374;
  int field_378;
  int field_37C;
  int field_380;
  int field_384;
  int field_388;
  int field_38C;
  int field_390;
  int field_394;
  int field_398;
  int field_39C;
  int field_3A0;
  int field_3A4;
  int field_3A8;
  int field_3AC;
  int field_3B0;
  int field_3B4;
  int field_3B8;
  int field_3BC;
  int field_3C0;
  int field_3C4;
  int field_3C8;
  int field_3CC;
  int field_3D0;
  int field_3D4;
  int field_3D8;
  int field_3DC;
  int field_3E0;
  int field_3E4;
  int field_3E8;
  int field_3EC;
  int field_3F0;
  int field_3F4;
  int field_3F8;
  int field_3FC;
  int field_400;
  int field_404;
  int field_408;
  int field_40C;
  int field_410;
  int field_414;
  int field_418;
  int field_41C;
  int field_420;
  int field_424;
  int field_428;
  int field_42C;
  int field_430;
  int field_434;
  int field_438;
  int field_43C;
  int field_440;
  int field_444;
  int field_448;
  short field_44C;
  char field_44E;
  char field_44F;
  int field_450;
  int field_454;
  int field_458;
  float field_45C;
  float field_460;
  int field_464;
  int field_468;
  Hw::cTexture* field_46C;
  int field_470;
  int field_474;
  int field_478;
  int field_47C;
  int field_480;
  int field_484;
  int field_488;
  int field_48C;

  virtual ~cModel() override {};
};

VALIDATE_SIZE(cModel, 0x490);