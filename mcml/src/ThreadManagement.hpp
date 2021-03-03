#pragma once

#include "General.hpp"
#include "Tissue.hpp"
#include "DataContainer.hpp"
#include "Photon.hpp"

#include <thread>
#include <atomic>
#include <mutex>

class ProgressBar {
    std::atomic<int> counter;

public:
    int Total;

    ProgressBar() {}
    ProgressBar(int Total) : Total(Total) { counter.store(0);}

    float get_percentage() { return 100*static_cast<float>(counter.load())/static_cast<float>(Total);}
    int current() {return counter.load();}

    ProgressBar& operator++() {
        counter.fetch_add(1);
        return *this;
    }
};

template<class T>
class ThreadParams {
public:
    int NP;
    T chance;
    T increase;
    T treshold;
    bool debug;
    int id;

    ThreadParams() {}
    ThreadParams(int NP, T chance, T treshold, bool debug, int id)
                : NP(NP), chance(chance), treshold(treshold), debug(debug), id(id), increase(1/chance) {}

};

template<class T>
void compute(Tissue<T>& tissue, DataContainer<T>& data, ThreadParams<T> params, ProgressBar& counter, std::mutex& m) {
    for(int i = 0; i < params.NP; ++i) {
        Photon<double> p(Vector3<double>(/*random<double>(-0.1, 0.1), random<double>(-0.1, 0.1)*/0,0, 0), Vector3<double>(0, 0, 1), 1);
        bool alive = true;
        while(alive){
            double l = tissue.path_length();
            bool reflect = p.position.e[2] + p.direction.e[2]*l < data.Z0;
            bool transit = p.position.e[2] + p.direction.e[2]*l > data.Z1;
            if(params.debug)
                std::cerr << " hop";

            if(reflect || transit) {
                double l1;
                if(reflect)
                    l1 = fabs(p.position.e[2]/p.direction.e[2]);
                else
                    l1 = fabs((data.Z1 - p.position.e[2])/p.direction.e[2]);

                p.move(l1);
                double R;

                R = tissue.Frenel_refraction(p, params.debug);
                //reflect counter <---------------------------------- NEED TO CODE
                double r = sqrt(p.position.e[0]*p.position.e[0] + p.position.e[1]*p.position.e[1]);

                if(params.debug)
                    std::cerr << " reflect";

                //m.lock();
                if(reflect)
                    data.add_reflect(r, (1 - R)*p.weight);
                if(transit)
                    data.add_transit(r, (1 - R)*p.weight);
                //m.unlock();

                if(params.debug)
                    std::cerr << " R=" << R;
                if(std::isnan(R)) {
                    std::cerr << "\n R = NaN";
                    exit(-1);
                }
                p.weight *= R;
                if(params.debug){
                    std::cerr << " z=" << p.position.e[2];
                    std::cerr << " uz before " << p.direction.e[2];
                }
                p.direction.e[2] = - p.direction.e[2];
                if(params.debug)
                    std::cerr << " uz after " << p.direction.e[2];
                p.move(l-l1);
                if(params.debug)
                    std::cerr << " z=" << p.position.e[2];
            }
            else
                p.move(l);
            if(params.debug)
                std::cerr << " drop";
            //adsorbtion counter <----------------------------------- NEED TO CODE

            double r = sqrt(p.position.e[0]*p.position.e[0] + p.position.e[1]*p.position.e[1]);
            double z = p.position.e[2];

            //m.lock();
            data.add_medium(z, r, p.weight*tissue.att);
            //m.unlock();

            p.weight *= (1 - tissue.att);
            if(params.debug)
                std::cerr << " w= " << p.weight;
            if(p.weight < params.treshold) {
                if(fast_random<double>() < params.chance)
                    p.weight *= params.increase;
                else
                    alive = false;
            }

            if(alive){
                if(params.debug)
                    std::cerr << " spin:";
                tissue.scatter(p, params.debug);
                if(params.debug) {
                    std::cerr << " z=" << p.position.e[2] << " uz=" << p.direction.e[2] << "\n-------------------------------\n";
                }
            }

        }

        ++counter;
    }

}
