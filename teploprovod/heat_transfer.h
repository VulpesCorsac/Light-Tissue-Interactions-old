// Добавить стекла
// 
#include "../eigen/Eigen/Dense"

#include<iostream>
#include<cmath>
#include<fstream>
#include<cstdlib>

//функция для коэффициента теплопроводности от Т
template<typename T>
T lambda(T Temp, const T gamma) { 
    return  0.55 + gamma * Temp; 
}

//функция для коэффициента теплопроводности от G и от Т
template<typename T>
T lambda_G(T G, T Temp, const T gamma_G) { 
    return 0.55 + 5 * gamma_G * (1 - G) + gamma_G * Temp; 
}

//функция для константы скорости реакции от темпы:
template<typename T>
T K(T Temp) {
    return pow(10,30) * exp(-(2*pow(10,5)) / (8.31 * (Temp + 273)));
}


template < typename T, size_t Nz, size_t Nr, bool detector>
Matrix<T, Eigen::Dynamic, Eigen::Dynamic> HeatTransfer(T T0, T t_end, T kap_b, T kap_s, T Te, T time, T gamma, T gamma_G, T eps, Matrix<T,Dynamic,Dynamic>& HeatSourceMatrix) {
    using namespace std;
    using namespace Eigen;
    t_end += time; 
    //коэффициенты прогонки:
    T ai, bi, ci, fi;
    //константы
    const T lam = 0.55, ro = 1000, c = 3100;//как у яйца
    const T lam_s = 0.3, ro_s = 2500, c_s = 600; //параметры стекла
    const T L = 0.001, L_s = 0.0002, R = 0.01; //начальная темпа, L - длина образца по оси z,  L_s - длина стекла по оси z, R - радиус, температуры на границах.
    constexpr int Nz_s = 10;
    const T hr = R / ((Nr - 1)), hz = (L + 2 * L_s) / (Nz + 2 * Nz_s) , dt = 0.1, tau = 0.05;
    T q1, q2, q3, q4; //тепловые потоки 

    static Matrix<T,Dynamic,Dynamic> Temp;  //поле температуры
    Matrix<T,Dynamic,Dynamic> Tn = Matrix<T, Nz + 2 * Nz_s, Nr>::Zero();; //поле температуры на предыдущем временном слое 
    Matrix<T,Dynamic,Dynamic> Ts = Matrix<T, Nz + 2 * Nz_s, Nr>::Zero();; //поле температуры на предыдущей иттерации
    static Matrix<T,Dynamic,Dynamic> G; //поле степени деградации ткани G
    Matrix<T,Dynamic,Dynamic> Gn = Matrix<T, Nz + 2 * Nz_s, Nr>::Zero();; //поле G на предыдущем временном слое 
    ofstream f;
    ofstream g;   
    f.open("Results/res2dcyl_A_t:_" + to_string(int(time + 1)) + "_gamma_G:_" + to_string(gamma_G) + "_eps:_" + to_string(eps) + ".xls");
    g.open("Results/CoagMatrix_t:_" + to_string(int(time + 1)) + ".csv");
    //прогоночные коэффициенты:
    T alfa[(Nr - 1) + 1];    
    T beta[(Nr - 1) + 1]; 
    T max1 = 0, max2 = 0; // разницы для метода простой иттерации

    //поле темпы и G0 в начальный момент времени:
    if (time == 0) {
        Temp = Matrix<T, (Nz + 2 * Nz_s), Nr>::Constant((Nz + 2 * Nz_s),Nr,T0); 
        G = Matrix<T, Nz, Nr>::Constant(Nz,Nr,1);
    }


    //Цикл по времени
    while (time < t_end) {
        time += 0.05;
        //________________________________________Решаем в направлении z:_____________________________________________________

        for (int j = 0; j <= (Nr - 1); j ++) {
            
            alfa[0] = lam / (lam + hz * kap_s);
            beta[0] = (hz * kap_s / (lam + hz * kap_s)) * Te;

            for (int i = 0; i <= ((Nz + 2 * Nz_s) - 1); i ++) {
                Tn(i, j) = Temp(i, j);              //сохраняем темпу на предыдущем временом слое
                if ((i >= Nz_s) && (i <= (Nz - 1) + Nz_s)) {
                    Gn(i - Nz_s, j) = G(i - Nz_s, j);         //сохраняем поле G на предыдущем временом слое
                } 
            }

            //цикл с постусловием
            do {
                //запомним поле Temp на предыдущем шаге иттерации
                for (int i = 0; i <= ((Nz + 2 * Nz_s) - 1); i++) {
                    Ts(i, j) = Temp(i, j); 
                }
                if (Nz_s != 0) {
                     for (int i = 1; i < Nz_s - 1; i ++) {       //считаем коэфы в методе прогонки
                        ai = lam_s / pow(hz,2);
                        bi = 2.0 * lam_s / pow(hz,2) + ro_s * c_s / tau;
                        ci = lam_s / pow(hz,2);
                        fi = -ro_s * c_s * Temp(i, j) / tau;
                        alfa[i] = ai / (bi - ci * alfa[i - 1]);
                        beta[i] = (ci * beta[i - 1] - fi) / (bi - ci * alfa[i - 1]); 
                    }

                    //прогоночные коэфы для первой точки - границы раздела стелка и ткани

                    alfa[Nz_s - 1] = lambda_G(G(0, j), Temp(Nz_s - 1, j), gamma_G) / (lambda_G(G(0, j), Temp(Nz_s - 1, j), gamma_G) + lam_s * (1 - alfa[Nz_s - 2]));
                    beta[Nz_s - 1] = (lam_s * beta[Nz_s - 2]) / (lambda_G(G(0, j), Temp(Nz_s - 1, j), gamma_G) + lam_s * (1 - alfa[Nz_s - 2]));
                    
                    //прогоночные коэфы для второй точки - границы раздела стекла и ткани
                    alfa[(Nz - 1) + Nz_s + 1] = lam_s / (lam_s + lambda_G(G((Nz - 1), j), Temp((Nz - 1) + Nz_s + 1, j), gamma_G) * (1 - alfa[(Nz - 1) + Nz_s]));
                    beta[(Nz - 1) + Nz_s + 1] = (lambda_G(G((Nz - 1), j), Temp((Nz - 1) + Nz_s + 1, j), gamma_G) * beta[(Nz - 1) + Nz_s]) /
                                                             (lam_s + lambda_G(G(Nz - 1, j), Temp((Nz - 1) + Nz_s + 1, j), gamma_G) * (1 - alfa[(Nz - 1) + Nz_s]));
                    
                    for (int i = (Nz - 1) + Nz_s + 2; i < (Nz - 1) + 2 * Nz_s; i ++) {
                        ai = lam_s / pow(hz,2);
                        bi = 2.0 * lam_s/pow(hz,2) + ro_s * c_s / tau;
                        ci = lam_s / pow(hz,2);
                        fi = -ro_s * c_s * Temp(i, j) / tau;
                        alfa[i] = ai / (bi - ci * alfa[i - 1]);
                        beta[i] = (ci * beta[i - 1] - fi) / (bi - ci * alfa[i - 1]); 
                    }
                    //гр. усл 3 (для стекла)
                    Temp((Nz - 1) + 2 * Nz_s, j) = (lam_s * beta[(Nz - 1) + 2 * Nz_s - 1] + hz * kap_b * Te) / (hz * kap_b + lam_s * (1 - alfa[(Nz - 1) + 2 * Nz_s - 1]));
                    
                }
    
                for (int i = Nz_s; i <= (Nz - 1) + Nz_s; i ++) {
                    ai = 0.5 * (lambda_G(G(i - Nz_s, j), Temp(i - Nz_s, j), gamma_G) + lambda_G(G(i - Nz_s, j), Temp(i - Nz_s, j), gamma_G)) / pow(hz,2);
                    ci = 0.5 * (lambda_G(G(i - Nz_s, j), Temp(i - Nz_s, j), gamma_G) + lambda_G(G(i - Nz_s, j), Temp(i - Nz_s, j), gamma_G)) / pow(hz,2);
                    bi = ai + ci + ro * c / tau;
                    fi = -ro * c * Tn(i, j) / tau - HeatSourceMatrix(i - Nz_s, j);
                    alfa[i] = ai / (bi - ci * alfa[i-1]);
                    beta[i] = (ci * beta[i - 1] - fi) / (bi - ci * alfa[i - 1]); 
                }
                if (Nz_s == 0){
                    Temp((Nz - 1), j) = (lam_s * beta[(Nz - 1) - 1] + hz * kap_b * Te) / (hz * kap_b + lam_s * (1 - alfa[(Nz - 1) - 1]));

                }

                // Определяем поле температуры и G на промежуточном временном слое(прошли по оси z):
                for (int i = (Nz - 1) + 2 * Nz_s - 1; i >= 0; i --) {
                    Temp(i, j) = alfa[i] * Temp(i + 1, j) + beta[i];
                }

                for (int i = (Nz - 1); i >= 0; i --) {
                    if (G(i, j) < 0.01) {
                        G(i, j) = 0;
                    }
                    else { 
                        G(i, j) = Gn(i, j) * (1 - K(Temp(i + Nz_s, j)) * tau);//определяем поле G
                    }
                }
                

                //определяем максимумы для метода простой иттерации
                max1 = 0;
                max2 = 0;
                for (int i = 0; i <= (Nz - 1) + 2 * Nz_s; i++) {
                    if (max1 < abs (Temp(i, j) - Ts(i, j))) {
                        max1 = abs (Temp(i, j) - Ts(i, j));
                    }
                }
                for (int i = 0; i <= (Nz - 1) + 2 * Nz_s; i++) {
                    if (max2 < abs (Temp(i, j))) {
                        max2 = abs (Temp(i, j));
                    }
                }
            }   
            while ((max1 / max2) > eps);

        }
/*        g << "После прохода по z: " << "\n";
        f << "После прохода по z: " << "\n";
    for (int i = 0; i <= (Nz - 1); i ++) {
          for (int j = 0; j <= (Nr - 1); j++) {
              g << G(i,j) << "\t";
              f << Temp(i,j) << "\t";
          }
          g << endl;
          f << endl;

      }*/
      


        //_________________________________________Решаем в направлении r______________________________________________

        
       time += 0.05;//добавляем время, т.к. находимся на промежуточном слое по времени
        
        for (int i = 0; i <= (Nz - 1) + 2 * Nz_s; i ++) {
            // запомним поле Temp и G на предыдущем временном слое
            for (int j = 0; j <= (Nr - 1); j++) {
                Tn(i, j) = Temp(i, j); 
                if ((i >= Nz_s) && (i <= (Nz - 1) + Nz_s)) {
                    Gn(i - Nz_s, j) = G(i - Nz_s, j); 
                }
            }

            if ((i >= Nz_s) && (i <= Nz_s + (Nz - 1))) { 
                alfa[0]= 1.0;
                beta[0]= pow(hr,2) * HeatSourceMatrix(i - Nz_s, 0) / lambda_G(G(i - Nz_s, 0), Temp(i, 0), gamma_G);
            }

            if ((i < Nz_s) || (i > Nz_s + (Nz - 1))) { 
                alfa[0]= 1;
                beta[0]= 0; //поток равен нулю, т.к середина цилиндра
            }    

            //цикл с постусловием
            do {
                //запомним поле Temp на предыдущем шаге иттерации
                for (int j = 0; j <= (Nr - 1); j++) {
                    Ts(i, j) = Temp(i, j); 
                }

                for (int j = 1; j < (Nr - 1); j ++) {

                    if ((i >= Nz_s) && (i <= Nz_s + (Nz - 1))) { 
                        ai = 0.5 * (lambda_G(G(i - Nz_s, j), Temp(i, j), gamma_G) + lambda_G(G(i - Nz_s, j + 1), Temp(i, j + 1), gamma_G)) / pow(hz,2);
                        ci = 0.5 * (lambda_G(G(i - Nz_s, j - 1), Temp(i, j - 1), gamma_G) + lambda_G(G(i - Nz_s, j), Temp(i, j), gamma_G)) / pow(hz,2);
                        bi = ai + ci + ro * c / tau;
                        fi = -ro * c * Tn(i, j) / tau - HeatSourceMatrix(i - Nz_s, j);
                        alfa[j] = ai / (bi - ci * alfa[j - 1]);
                        beta[j] = (ci * beta[j - 1] - fi) / (bi - ci * alfa[j - 1]); 
                    }

                    if ((i < Nz_s) || (i > Nz_s + (Nz - 1))) {
                        ai = lam_s * (2 * j + 1) / (2 * j * pow(hr,2));
                        bi = 2.0 * lam_s / pow(hr,2) + ro_s * c_s / tau;
                        ci = lam_s * (2 * j - 1)/(2 * j * pow(hr,2));
                        fi = -ro_s * c_s * Temp(i, j) / tau;
                        alfa[j] = ai / (bi - ci * alfa[j - 1]);
                        beta[j] = (ci * beta[j - 1] - fi) / (bi - ci * alfa[j - 1]);
                    }
                }


                if ((i >= Nz_s) && (i <= Nz_s + (Nz - 1))) { 
                    // гр усл. 3
                    Temp(i, (Nr - 1))= (lambda_G(G(i - Nz_s, (Nr - 1)), Temp(i, (Nr - 1)), gamma_G) * beta[(Nr - 1) - 1] + hr * kap_b * Te + 0.25 * HeatSourceMatrix(i - Nz_s, (Nr - 1))* pow(hz,2)) / 
                                                                    (hr * kap_b + lambda_G(G(i - Nz_s, (Nr - 1)), Temp(i, (Nr - 1)), gamma_G) * (1 - alfa[(Nr - 1) - 1]));    
                }

                if ((i < Nz_s) || (i > Nz_s + (Nz - 1))) {
                    Temp(i, (Nr - 1))= (lam_s * beta[(Nr - 1) - 1] + hr * kap_s * Te) / (hr * kap_s + lam_s * (1 - alfa[(Nr - 1) - 1]));    
                }
                
                if ((i >= Nz_s) && (i <= (Nz - 1) + Nz_s)) {
                    G(i - Nz_s, (Nr - 1))= Gn(i - Nz_s, (Nr - 1)) * (1 - K(Temp(i, (Nr - 1))) * tau);
                    if (G(i - Nz_s, (Nr - 1))< 0.01) {
                        G(i - Nz_s, (Nr - 1))= 0;
                    }
                }

                // Определяем поле температуры и G на промежуточном временном слое(прошли по оси r):
                for (int j = (Nr - 1) - 1; j >= 0; j --) {
                    Temp(i, j) = alfa[j] * Temp(i, j + 1) + beta[j];
                    
                    if ((i >= Nz_s) && (i <= (Nz - 1) + Nz_s)) {
                        if (G(i - Nz_s, j) < 0.01) {
                            G(i - Nz_s, j) = 0;
                        }
                        else { 
                            G(i - Nz_s, j) = Gn(i - Nz_s, j) * (1 - K(Temp(i, j)) * tau);//определяем поле G
                        }
                    }
                }

                //определяем максимумы
                max1 = 0;
                max2 = 0;
                for (int j = 0; j <= (Nr - 1); j ++) {
                    if (max1 < abs (Temp(i, j) - Ts(i, j))) {
                        max1 = abs (Temp(i, j) - Ts(i, j));
                    }
                }
                for (int j = 0; j <= (Nr - 1); j++) {
                    if (max2 < abs (Temp(i, j))) {
                        max2 = abs (Temp(i, j));
                    }
                }
            }   
            while ((max1 / max2) > eps);
        }
    }//конец цикла по времени


    for (int i = 0; i <= (Nz - 1) + 2 * Nz_s; i ++) {
        for (int j = 0; j <= (Nr - 1); j++) {
            f << Temp(i,j) << "\t";
        }
        f << endl;
    }
    cout << "Вывел матрицу T\n";
    for (int i = 0; i <= (Nz - 1); i ++) {
        for (int j = 0; j <= (Nr - 1); j++) {
            g << G(i,j) << "\t";
        }
        g << endl;
    }
    cout << "Вывел матрицу G\n";

    f.close();
    g.close();
return G;
}