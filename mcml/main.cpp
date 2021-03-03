#include<iostream>

#include "src/Photon.hpp"
#include "src/Vector3.hpp"
#include "src/Tissue.hpp"
#include "src/General.hpp"


int main() {
    //test script with image

                        //w  n    l    g    att
    Tissue<double> tissue(5, 1.5, 0.5, 0.5, 0.1);
    double z0 = 0;
    double z1 = 5;
    double r = 5;
    double dr = 0.01;
    double dz = 0.01;

    int nR = r/dr; //unesessary cast
    double REFLECT[nR]; //={0} or memset
    double TRANSIT[nR];
    int nZ = static_cast<int>((z1-z0)/dz);
    double MEDIUM[nZ][nR];

    for(int i = 0; i < nR; i++){
        REFLECT[i] = 0;
        TRANSIT[i] = 0;
        for(int j = 0; j < nZ; j++)
            MEDIUM[j][i] = 0;
    }


    double num = 10000;
    double treshold = 0.01;
    double chance = 0.1; //check to be greater than treshold
    double increase = 1/chance;

    bool debug;

    for(int i = 0; i < num; ++i) {
        debug = false;

        std::cerr << "\rSampling photons, done: " << i;
        Photon<double> p(Vector3<double>(/*random<double>(-0.1, 0.1), random<double>(-0.1, 0.1)*/0,0, 0), Vector3<double>(0, 0, 1), 1);
        bool alive = true;
        while(alive){
            double l = tissue.path_length();
            bool reflect = p.position.e[2] + p.direction.e[2]*l < z0;
            bool transit = p.position.e[2] + p.direction.e[2]*l > z1;
            if(debug)
                std::cerr << " hop";

            if(reflect || transit) {
                double l1;
                if(reflect)
                    l1 = fabs(p.position.e[2]/p.direction.e[2]);
                else
                    l1 = fabs((z1 - p.position.e[2])/p.direction.e[2]);

                p.move(l1);
                double R;

                R = tissue.Frenel_refraction(p, debug);
                //reflect counter <---------------------------------- NEED TO CODE
                double r = sqrt(p.position.e[0]*p.position.e[0] + p.position.e[1]*p.position.e[1]);
                int ir = static_cast<int>(r/dr);
                if(debug)
                    std::cerr << " reflect";
                if(ir < nR) {
                    if(reflect)
                        REFLECT[ir] += (1 - R)*p.weight;
                    if(transit)
                        TRANSIT[ir] += (1 - R)*p.weight;
                }
                if(debug)
                    std::cerr << " R=" << R;
                if(std::isnan(R)) {
                    std::cerr << "\n R = NaN";
                    exit(-1);
                }
                p.weight *= R;
                if(debug){
                    std::cerr << " z=" << p.position.e[2];
                    std::cerr << " uz before " << p.direction.e[2];
                }
                p.direction.e[2] = - p.direction.e[2];
                if(debug)
                    std::cerr << " uz after " << p.direction.e[2];
                p.move(l-l1);
                if(debug)
                    std::cerr << " z=" << p.position.e[2];
            }
            else
                p.move(l);
            if(debug)
                std::cerr << " drop";
            //adsorbtion counter <----------------------------------- NEED TO CODE

            double r = sqrt(p.position.e[0]*p.position.e[0] + p.position.e[1]*p.position.e[1]);
            int ir = static_cast<int>(r/dr);
            double z = p.position.e[2];
            int iz = static_cast<int>(z/dz);

            if(iz < 0) {
                std::cerr << "\n iz < 0 \n" << p.direction.e[2];
                exit(-1);
            }

            if(iz < nZ && ir < nR)
                MEDIUM[iz][ir] += p.weight*tissue.att;

            p.weight *= (1 - tissue.att);
            if(debug)
                std::cerr << " w= " << p.weight;
            if(p.weight < treshold) {
                if(fast_random<double>() < chance)
                    p.weight *= increase;
                else
                    alive = false;
            }

            if(alive){
                if(debug)
                    std::cerr << " spin:";
                tissue.scatter(p, debug);
                if(debug) {
                    std::cerr << " z=" << p.position.e[2] << " uz=" << p.direction.e[2] << "\n-------------------------------\n";
                }
            }

        }
    }

    double max_weight = 0;
    for(int i = 0; i < nR; i++) {
        if(max_weight < REFLECT[i])
            max_weight = REFLECT[i];
        if(max_weight < TRANSIT[i])
            max_weight = TRANSIT[i];

        for(int j = 0; j < nZ; j++){
            if(max_weight < MEDIUM[j][i])
                max_weight = MEDIUM[j][i];
        }
    }

    for(int i = 0; i < nR; i++) {
        REFLECT[i] /= max_weight;
        TRANSIT[i] /= max_weight;
        for(int j = 0; j < nZ; j++)
            MEDIUM[j][i] /= max_weight;
    }

    std::cerr << "\nWriting in output";

    std::cout << "P3\n" << nR << ' ' << nZ << "\n255\n";
    for(int h = nZ - 1; h >= 0; --h) {
        for(int w = 0; w < nR; ++w){
            int grayvalue = static_cast<int>(255.999*sqrt(MEDIUM[h][w])); // gamma corrected
            std::cout << grayvalue << ' ' << grayvalue << ' ' << grayvalue << '\n';
        }
    }


    return 0;
}
