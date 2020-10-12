
#include "src/Systems/Systems.h"
#include "LamiaMain.h"

bool LamiaMain(float dt)
{
  LamiaSystems::LamiaSystem()->InputSystem()->Update(dt);

  return true;
}
