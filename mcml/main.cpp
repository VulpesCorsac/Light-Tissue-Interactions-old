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

    int nR = static_cast<int>(r/dr);
    double REFLECT[nR];
    double TRANSIT[nR];
    int nZ = static_cast<int>((z1-z0)/dz);
    double MEDIUM[nZ][nR];

    for(int i = 0; i < nR; i++){
        REFLECT[i] = 0;
        TRANSIT[i] = 0;
        for(int j = 0; j < nZ; j++)
            MEDIUM[j][i] = 0;
    }


    double num = 450;
    double treshold = 0.01;
    double chance = 0.1; //check to be greater than treshold
    double increase = 1/chance;

    for(int i = 0; i < num; i++) {
        std::cerr << "\rSampling photons, done: " << i;
        Photon<double> p(Vector3<double>(random<double>(-0.1, 0.1), random<double>(-0.1, 0.1), 0), Vector3<double>(0, 0, 1), 1);
        bool alive = true;
        while(alive){
            double l = tissue.path_length();
            bool reflect = p.p.z() + p.u.z()*l < z0;
            bool transit = p.p.z() + p.u.z()*l > z1;
//            std::cerr << " hop";
            if(reflect || transit) {
                double l1;
                if(reflect)
                    l1 = fabs(p.p.z()/p.u.z());
                else
                    l1 = fabs((z1 - p.p.z())/p.u.z());

                p.move(l1);
                double R = tissue.Frenel_refraction(p);
                //reflect counter <---------------------------------- NEED TO CODE
                double r = sqrt(p.p.x()*p.p.x() + p.p.y()*p.p.y());
                int ir = static_cast<int>(r/dr);
//                std::cerr << " reflect";
                if(ir < nR) {
                    if(reflect)
                        REFLECT[ir] += (1 - R)*p.w;
                    if(transit)
                        TRANSIT[ir] += (1 - R)*p.w;
                }

//                std::cerr << " R=" << R;
                if(std::isnan(R)) {
                    std::cerr << "\n R = NaN";
                    exit(-1);
                }
                p.w *= R;
//                std::cerr << " z=" << p.p.z();
//                std::cerr << " uz before " << p.u.z();
                p.u.z() = - p.u.z();
//                std::cerr << " uz after " << p.u.z();
                p.move(l-l1);
//                std::cerr << " z=" << p.p.z();
            }
            else
                p.move(l);
//            std::cerr << " drop";
            //adsorbtion counter <----------------------------------- NEED TO CODE
            double r = sqrt(p.p.x()*p.p.x() + p.p.y()*p.p.y());
            int ir = static_cast<int>(r/dr);
            double z = p.p.z();
            int iz = static_cast<int>(z/dz);

            if(iz < 0) {
                std::cerr << "\n iz < 0 \n" << p.u.z();
                exit(-1);
            }

            if(iz < nZ && ir < nR)
                MEDIUM[iz][ir] += p.w*tissue.att;

            p.w *= (1 - tissue.att);
//            std::cerr << " w= " << p.w;
            if(p.w < treshold) {
                if(fast_random<double>() < chance)
                    p.w *= increase;
                else
                    alive = false;
            }

            if(alive){
//                std::cerr << " spin \n";
                p.u += tissue.scatter();
                p.u = p.u.unit();
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
