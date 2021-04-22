#pragma once

#include "Photon.h"
#include "Medium.h"
#include "Fresnel.h"
#include "../Utils/Random.h"

#include <cmath>
#include <tgmath.h>

template <typename T>
class MonteCarlo
{
    public:
        MonteCarlo() = default;
        MonteCarlo(const Medium<T>& new_medium, const int& new_Np, const int& new_threads, const T& new_dz, const T& new_dr, const T& new_chance, const T& new_thr);
        ~MonteCarlo() = default;

        void Normalize();

    protected:
        int Nphotons = 1e6;
        int threads = 1;

        T dz = 1e-3;
        T dr = 1e-3;
        T threshold = 1e-3;
        T chance = 0.1;

        T nVac = 1.0;


        Matrix<T, Dynamic, Dynamic> A = Matrix<T, 500, 2000>::Constant(0.0);
        Matrix<T, 1, Dynamic> RR = Matrix<T, 1, 2000>::Constant(0.0);
        Matrix<T, 1, Dynamic> TT = Matrix<T, 1, 2000>::Constant(0.0);

        Photon<T> photon;
        const Medium<T>& tissue;

        void Launch(const Vector3D<T>& startCoord, const Vector3D<T>& startDir); //TODO: different light sources
        void Hop();
        void CheckBoundaries();
        void Drop();
        void Spin();
        void Terminate();
        void Simulation(const int& debug);

        T Volume(const T& ir);
        T Area(const T& ir);

};

template <typename T>
MonteCarlo<T>::MonteCarlo(const Medium<T>& new_medium, const int& new_Np, const int& new_threads, const T& new_dz, const T& new_dr, const T& new_chance, const T& new_thr): tissue(new_medium) {
    this->Nphotons = new_Np;
    this->threads = new_threads;
    this->dz = new_dz;
    this->dr = new_dr;
    this->chance = new_chance;
    this->threshold = new_thr;
    this->nVac = 1.0;
}

template <typename T>
void MonteCarlo<T>::Launch(const Vector3D<T>& startCoord, const Vector3D<T>& startDir) {
 //   std::cout << "LAUNCH" << std::endl;
    this->photon = Photon<T>(startCoord, startDir, 1.0, 0.0);
    this->photon.alive=1;
}

template <typename T>
void MonteCarlo<T>::Hop() {
    T RND = random(0.0, 1.0);
    T s = - std::log(RND)/tissue.getMt();
    this->photon.coordinate += s * this->photon.direction;
    this->photon.lastStep = s;
    this->photon.pathLength += s;
}

template <typename T>
void MonteCarlo<T>::CheckBoundaries() {
    if (this->photon.coordinate.z < 0) { // trying to escape -- front
        T s1 = std::abs(this->photon.coordinate.z / this->photon.direction.z);
   /*     this->photon.coordinate -= this->photon.lastStep * this->photon.direction;
        this->photon.pathLength -= this->photon.lastStep; // maybe i'll get rid of it
        this->photon.coordinate += s1 * this->photon.direction;*/
        this->photon.coordinate -= s1*this->photon.direction;

        T n1 = tissue.getN();
        T n2 = this->nVac;
        T cos1 = std::abs(this->photon.direction.z);

        T Ri = FresnelR(n1, n2, cos1);
        T r = std::sqrt(sqr(this->photon.coordinate.x) + sqr(this->photon.coordinate.y));
        int ir = std::floor(r/this->dr); // +1?

    /*    if (ir > Nr)
            std::cout << "ACHTUNG!!! ir exceeds Nr during boundary check" << std::endl;*/

        this->RR(ir) += (1 - Ri) * this->photon.weight;
        this->photon.weight *= Ri;
        this->photon.direction.z = -this->photon.direction.z;

        this->photon.coordinate = (this->photon.lastStep - s1) * this->photon.direction;
        this->photon.lastStep -= s1;
    } else if (this->photon.coordinate.z > tissue.getD()) { // trying to escape -- rear
        T s1 = std::abs((this->photon.coordinate.z-tissue.getD()) / this->photon.direction.z);
  /*      this->photon.coordinate -= this->photon.lastStep * this->photon.direction;
        this->photon.coordinate += s1 * this->photon.direction;*/
        this->photon.coordinate -= s1*this->photon.direction;

        T n1 = tissue.getN();
        T n2 = this->nVac;
        T cos1 = std::abs(this->photon.direction.z);

        T Ri = FresnelR(n1, n2, cos1);
        T r = std::sqrt(sqr(this->photon.coordinate.x) + sqr(this->photon.coordinate.y));
        int ir = std::floor(r/this->dr); // +1?

    /*    if (ir > Nr)
            std::cout << "ACHTUNG!!! ir exceeds Nr during boundary check" << std::endl;*/

        this->TT(ir) += (1 - Ri) * this->photon.weight;
        this->photon.weight *= Ri;
        this->photon.direction.z = -this->photon.direction.z;

        this->photon.coordinate += (this->photon.lastStep - s1) * this->photon.direction;
        this->photon.lastStep -= s1;
    }
}

template <typename T>
void MonteCarlo<T>::Drop() {
    T r = std::sqrt(sqr(this->photon.coordinate.x) + sqr(this->photon.coordinate.y));
    int ir = std::floor(r/this->dr);
    int iz = std::floor(this->photon.coordinate.z/this->dz);
    if (iz < 0)
        iz = 0;

/*    if (ir >= Nr)
        std::cout << "ACHTUNG!!! ir exceeds Nr during drop" << std::endl;
    if (iz >= Nz)
        std::cout << "ACHTUNG!!! iz exceeds Nz during drop" << std::endl;*/

    this->A(iz, ir) += this->photon.weight * (tissue.getMa()/tissue.getMt());
    this->photon.weight *= (tissue.getMs()/tissue.getMt());
}

template <typename T>
void MonteCarlo<T>::Spin() {
    T g = tissue.getG();
    T RND = random(0.0, 1.0);
    T cosHG = (1 + sqr(g) - sqr((1 - sqr(g))/(1 - g + 2*g*RND)))/(2*g);
    if (g == 0)
        cosHG = 2*RND - 1;
    else if (g == 1)
        cosHG = 1.0;
    T phi = 2*M_PI*RND; //radians

    T ux = this->photon.direction.x;
    T uy = this->photon.direction.y;
    T uz = this->photon.direction.z;
    T sinHG = std::sqrt(1 - sqr(cosHG));
    T temp = std::sqrt(1 - sqr(uz));

    T uxx = sinHG * (ux*uz*std::cos(phi) - uy*std::sin(phi))/temp + ux*cosHG;
    T uyy = sinHG * (uy*uz*std::cos(phi) + ux*std::sin(phi))/temp + uy*cosHG;
    T uzz = -sinHG * std::cos(phi) * temp + uz*cosHG;

    if (std::abs(uz) - 1.0 < 1e-3){

        uxx = sinHG * std::cos(phi);
        uyy = sinHG * std::sin(phi);
        if (uz > 0 || uz == 0)
            uzz = cosHG;
        else
            uzz = -cosHG;
    }

    this->photon.direction.x = uxx;
    this->photon.direction.y = uyy;
    this->photon.direction.z = uzz;
}

template <typename T>
void MonteCarlo<T>::Terminate() {
    if (this->photon.weight < this->threshold) {
        T RND = random(0.0, 1.0);
        if (RND < this->chance || RND == this->chance){
            this->photon.weight /= this->chance;
        } else
            this->photon.alive = 0;
    }
}


template <typename T>
void MonteCarlo<T>::Simulation(const int& debug) {
    Vector3D<T> startCoord, startDir;
    startCoord = Vector3D<T>(0,0,0);
    startDir = Vector3D<T>(0,0,1);
    Launch(startCoord, startDir);
    this->photon.number = debug;
    while(this->photon.alive == 1) {
            Hop();
            CheckBoundaries();
            Drop();
            Spin();
            Terminate();
    }
}

template <typename T>
T MonteCarlo<T>::Volume(const T& ir) {
    return 2*M_PI*(ir - 0.5)*sqr(this->dr)*this->dz;
}

template <typename T>
T MonteCarlo<T>::Area(const T& ir) {
    return 2*M_PI*(ir - 0.5)*sqr(this->dr);
}

template <typename T>
void MonteCarlo<T>::Normalize() {
    for (int i = 0; i < this->Nphotons; i++) {
        Simulation(i);
    }

 /*   std::cout << "R matrix" << std::endl;
    std::cout << this->RR << std::endl;
    std::cout << "T matrix" << std::endl;
    std::cout << this->TT << std::endl;
    std::cout << "A matrix" << std::endl;
    std::cout << this->A << std::endl;*/

    std::cout << this->RR.sum()/this->Nphotons << std::endl;
    std::cout << this->TT.sum()/this->Nphotons << std::endl;
/*    for (int i = 0; i < Nz; i++){
        for (int j = 0; j < Nr; j++)
            this->A(i,j)/=(Volume(j+1)*this->Nphotons);
    }

    for(int j = 0; j < Nr; j++){
        this->RR(j)/=(Area(j+1)*this->Nphotons);
        this->TT(j)/=(Area(j+1)*this->Nphotons);
    }*/

}
