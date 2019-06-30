#include "stdafx.h"

#include "../xrLC_Light/net_task_menager.h"




void net_light ()
{
  


  create_net_task_menager();
  get_net_task_menager()->run();
  destroy_net_task_menager();

}