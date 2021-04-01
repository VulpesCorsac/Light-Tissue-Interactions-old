#pragma once

#include "Tissue.h"
#include "Vector3.h"
#include "DataContainer.h"
#include "Photon.h"
#include "Frensel.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <exception>


class ProgressBar {
public:
    ProgressBar(int Total) noexcept
        : Total(Total) {
        counter.store(0);
    }

    float get_percentage() {
        return 100 * static_cast<float>(counter.load()) / static_cast<float>(Total);
    }

    int current() {
        return counter.load();
    }

    ProgressBar& operator++ () {
        counter.fetch_add(1);
        return *this;
    }

public:
    const int Total;

protected:
    std::atomic<int> counter;
};

template < typename T >
class ThreadParams {
public:
    int NP;
    T chance;
    T increase;
    T treshold;
    bool debug;
    int id;

    ThreadParams(int NP, T chance, T treshold, bool debug, int id) noexcept
        : NP(NP)
        , chance(chance)
        , increase(1/chance)
        , treshold(treshold)
        , debug(debug)
        , id(id) {}
};

template < typename T >
void compute(LayersWrapper<T>& L, DataContainer<T>& data, ThreadParams<T> params, ProgressBar& counter) {
    Vector3<T> origin(0, 0, 0);
    Vector3<T> direction(0, 0, 1);

    for(int i = 0; i < params.NP; i++) {
        hop_drop_spin(Photon<T>(origin, direction, 1), L, data, params);
        ++counter;
    }
}

template < typename T >
void compute(Tissue<T>& tissue, DataContainer<T>& data, ThreadParams<T> params, ProgressBar& counter, std::mutex& m) {
    for (int i = 0; i < params.NP; ++i) {
        Photon<T> p(Vector3<T>(0, 0, 0), Vector3<T>(0, 0, 1), 1);
        bool alive = true;
        while (alive) {
            auto l = tissue.path_length();
            bool reflect = p.position.e[2] + p.direction.e[2]*l < data.Z0;
            bool transit = p.position.e[2] + p.direction.e[2]*l > data.Z1;
            if (params.debug)
                std::cerr << " hop";

            if (reflect || transit) {
                T l1;
                if (reflect)
                    l1 = fabs(p.position.e[2]/p.direction.e[2]);
                else
                    l1 = fabs((data.Z1 - p.position.e[2])/p.direction.e[2]);

                p.move(l1);
                T R;

                R = Frenel_refraction<T>(p.direction, tissue.n, 1, params.debug);
                //reflect counter <---------------------------------- NEED TO CODE
                T r = sqrt(p.position.e[0]*p.position.e[0] + p.position.e[1]*p.position.e[1]);

                if (params.debug)
                    std::cerr << " reflect";

                //m.lock();
                if (reflect)
                    data.add_reflect(r, (1 - R)*p.weight);
                if (transit)
                    data.add_transit(r, (1 - R)*p.weight);
                //m.unlock();

                if (params.debug)
                    std::cerr << " R=" << R;
                if (std::isnan(R)) {
                    std::cerr << "\n R = NaN";
                    exit(-1);
                }
                p.weight *= R;
                if (params.debug) {
                    std::cerr << " z=" << p.position.e[2];
                    std::cerr << " uz before " << p.direction.e[2];
                }
                p.direction.e[2] = - p.direction.e[2];
                if (params.debug)
                    std::cerr << " uz after " << p.direction.e[2];
                p.move(l-l1);
                if (params.debug)
                    std::cerr << " z=" << p.position.e[2];
            } else
                p.move(l);
            if (params.debug)
                std::cerr << " drop";
            //adsorbtion counter <----------------------------------- NEED TO CODE

            T r = sqrt(p.position.e[0]*p.position.e[0] + p.position.e[1]*p.position.e[1]);
            T z = p.position.e[2];

            //m.lock();
            data.add_medium(z, r, p.weight*tissue.att);
            //m.unlock();

            p.weight *= (1 - tissue.att);
            if (params.debug)
                std::cerr << " w= " << p.weight;
            if (p.weight < params.treshold) {
                if (fast_random<T>() < params.chance)
                    p.weight *= params.increase;
                else
                    alive = false;
            }

            if (alive) {
                if (params.debug)
                    std::cerr << " spin:";
                tissue.scatter(p, params.debug);
                if (params.debug)
                    std::cerr << " z=" << p.position.e[2] << " uz=" << p.direction.e[2] << "\n-------------------------------\n";
            }

        }

        ++counter;
    }
}


template < typename T >
void set_up_threads(int number_of_threads, Tissue<T>& tissue, DataContainer<T>& data, ThreadParams<T> params, bool print) {
    ProgressBar counter(params.NP);
    std::vector<std::thread> threads;
    std::mutex m;

    for (int i = 0; i < number_of_threads; ++i) {
        ThreadParams<T> tmp(params.NP/number_of_threads, params.chance, params.treshold, params.debug, i);
        threads.emplace_back(std::thread(compute<T>, std::ref(tissue), std::ref(data), tmp, std::ref(counter), std::ref(m)));
    }

    while (counter.current() < counter.Total && print) {
        std::cerr << "\rSampling photons, done " << counter.get_percentage() << "\%";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    for (auto& t : threads)
        t.join();
}

template<typename T>
void hop_drop_spin(Photon<T>& p, LayersWrapper<T>& L, DataContainer<T>& data, ThreadParams<T> params) {

    T MICRO_STEP = 0.001 // used for leaving glass layer

    while(p.alive) {
        int idx = L.get_tissue_idx(p.position.e[2]);
        T down_edje = L.structure[idx - 1];
        T up_edje = L.structure[idx];
        T l;

        if(L.layers[idx].is_glass){ //--------------------------------------Operating propagation in glass
            T n = 1;
            bool outer = false;

            if(p.direction.e[2] > 0) {
                l = (up_edje - p.position.e[2])/p.direction.e[2];
                if(idx < L.structure.size())
                    n = L.layers[idx + 1].n;
                else
                    outer = true;
            }
            else {
                l = (down_edje - p.position.e[2])/p.direction.e[2];
                if(idx > 1)
                    n = L.layers[idx - 1].n;
                else
                    outer = true;
            }
            p.move(l);

            T R = Frenel_refraction<T>(p.direction, L.layers[idx].n, n, params.debug);
            if(std::isnan(R))
                throw std::logic_error("R is NaN, occurred in is_glass part");

            if(outer) {
                T r = sqrt(p.position.e[0]*p.position.e[0] + p.position.e[1]*p.position.e[1]);

                if (p.direction < 0)
                    data.add_reflect(r, (1 - R)*p.weight);
                else
                    data.add_transit(r, (1 - R)*p.weight);

                p.weight *= R;

                p.direction.e[2] = -p.direction.e[2];
            } else {
                if(random<T>(0, 1) <= R)
                    p.direction.e[2] = -p.direction.e[2];
                else
                    p.move(MICRO_STEP);
            }


        } else { //--------------------------------------------------------Operating propagation in tissue

            //=========================================== HOP ============================================

            l = L.layers[idx].path_length();

            bool up_cross = p.position.e[2] + p.direction.e[2]*l > up_edje;
            bool down_cross = p.position.e[2] + p.direction.e[2]*l < down_edje;

            if(down_cross || up_cross) {
                T partial_l;
                T n = 1;
                bool outer = false;
                T resize_l = 1;

                if(up_cross) {
                    partial_l = (up_edje - p.position.e[2])/p.direction.e[2];

                    if(idx < L.structure.size()) // photon tries to escape through up side
                        n = L.layers[idx + 1].n;
                    else
                        outer = true;
                } else {
                    partial_l = (down_edje - p.position.e[2])/p.direction.e[2];

                    if(idx > 1) // photon tries to escape through down side
                        n = L.layers[idx - 1].n;
                    else
                        outer = true;
                }

                T R = Frenel_refraction<T>(p.direction, L.layers[idx].n, n, params.debug);
                if(std::isnan(R))
                    throw std::logic_error("R is NaN, occurred in tissue part");

                if(outer) {
                    T r = sqrt(p.position.e[0]*p.position.e[0] + p.position.e[1]*p.position.e[1]);

                    if (p.direction < 0)
                        data.add_reflect(r, (1 - R)*p.weight);
                    else
                        data.add_transit(r, (1 - R)*p.weight);

                    p.weight *= R;

                    p.direction.e[2] = -p.direction.e[2];
                } else {
                    if(random<T>(0, 1) <= R)
                        p.direction.e[2] = -p.direction.e[2];
                    else {
                        T tmp_mpfl = L.layers[idx].mfpl;
                        if(up_cross){
                            idx++;
                        } else {
                            idx--;
                        }
                        resize_l = L.layers[idx].mfpl/tmp_mpfl;
                    }
                }

                p.move((l-partial_l)*resize_l);
            }

            //=============================================== DROP ======================================

            T r = sqrt(p.position.e[0]*p.position.e[0] + p.position.e[1]*p.position.e[1]);
            T z = p.position.e[2];

            data.add_medium(z, r, p.weight*L.layers[idx].att);

            p.weight *= (1 - L.layers[idx].att);

            if (p.weight < params.treshold) {
                if (random<T>(0, 1) < params.chance)
                    p.weight *= params.increase;
                else
                    p.alive = false;
            }

            //=============================================== SPIN ======================================

            if(p.alive)
                L.layers[idx].scatter(p, params.debug);

        }
    }
}
