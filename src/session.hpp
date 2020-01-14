//
// Created by ZodiacTiger on 2019-07-15.
//

#ifndef BLADERF_EVO_SESSION_HPP
#define BLADERF_EVO_SESSION_HPP

#include <device.hpp>


 class Session : public bladeRF::device
     {
  public:
       int radio_state = 1;
       long long unsigned time_stamp;
        Session (int radioState);
        ~Session ();
  private:
 };

#endif //BLADERF_EVO_SESSION_HPP
