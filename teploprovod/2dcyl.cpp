//Задача с матрицей потока полученной от Тани
#include<iostream>
#include<cmath>
#include<fstream>
#include<cstdlib>

using namespace std;

//функция для коэффициента теплопроводности от Т
double lambda(double T, double gamma) { 
    double lam = 0.55 + gamma * T; 
    return lam;
}

//функция для коэффициента теплопроводности от G
double lambda_G(double G, double gamma_G) { 
    double lam = 0.55 + gamma_G * (1 - G); 
    return lam;
}

//функция для константы скорости реакции от темпы:
double K(double T) {
    T = T + 273;
    double k = pow(10,30) * exp(-(2*pow(10,5)) / (8.31 * T));
    return k;
}

int main() {
    ofstream f; // файл для поля T
    ofstream g; // файл для поля G
    int Nr, Nz, Nz_s, x; //сетка (Nz_s - разбиение для стекла)
    //коэффициенты прогонки:
    double ai, bi, ci, fi;
    //константы
    double lam = 0.55, ro = 1000, c = 3100, a;//как у яйца
    double lam_s = 0.3, ro_s = 2500, c_s = 600, a_s; //параметры стекла
    double T0, L = 0.01, L_s = 0.005, R = 0.01, Tb, Tt, Tr; //начальная темпа, L - длина образца по оси z,  L_s - длина стекла по оси z, R - радиус, температуры на границах.
    double hr, hz, dt = 0.1, tau = 0.05, t_end, time = 0; 
    double q1, q2, q3, q4; //тепловые потоки
    double kap1, kap2, kap3, kap4, kap_b, kap_s;//коэфы теплопередачи
    double Te, Te2, Te3, Te4; // температуры снаружи
    double eps; // точность для метода простой иттерации
    double gamma, gamma_G; // коэф пропорциональности (lam = gamma * T)
   
    // задаём параметры
    cout << "Начальная температура: \n";
    cin >> T0;  
    cout << "Время нагрева: \n";
    cin >> t_end; 
    cout << "Введите коэффициент теплопередачи белка kap_b и стекла kap_s\n";
    cin >> kap_b >> kap_s;
    cout << "Введите температуру снаружи Te:\n";
    cin >> Te;
    cout << "Введите Nz и Nr и Nz_s:\n";
    cin >> Nz >> Nr >> Nz_s;
    cout << "Введите значение пропорциональности gamma_G (lam = gamma_G * T):\n";
    cin >> gamma_G;
    cout << "Введите точность eps: \n";
    cin >> eps;

    f.open("Results/res2dcyl_A_t:_" + to_string(int(t_end)) + "_gamma_G:_" + to_string(gamma_G) + "_eps:_" + to_string(eps) + ".xls");
    g.open("Results/G_t:_" + to_string(int(t_end)) + "_gamma_G:_" + to_string(gamma_G) + "_Te:_" + to_string(int(Te)) + "_eps:_" + to_string(eps) + "_kap_s:_" + to_string(int(kap_s)) + "_kap_b:_" + to_string(int(kap_b)) + ".xls");

    Nr -= 1 ; // (-1 для ровной сетки)
    Nz -= 1; // (-1 для ровной сетки)
    Nz_s += 1; // (+1 из-за двух границ стекло-ткань) 
    x = Nz + 2 * Nz_s ;//общее разбиение (стекла+ткань)
    hr = R / Nr, hz = (L + 2 * L_s) / x; //определяем шаг сетки    

    double T[Nr + 1][x + 1];  //поле температуры
    double Tn[Nr + 1][x + 1]; //поле температуры на предыдущем временном слое 
    double Ts[Nr + 1][x + 1]; //поле температуры на предыдущей иттерации
    double G[Nr + 1][x + 1]; //поле степени деградации ткани G
    double Gn[Nr + 1][x + 1]; //поле G на предыдущем временном слое 

    //Обьявление матрицы Источников
    double **Q = new double*[Nr + 1];
    for (int i = 0; i <= Nr; i ++)
    {
        Q[i] = new double[Nz + 1];
    }
 
    //прогоночные коэффициенты:
    double alfa[max(x, Nr) + 1];    
    double beta[max(x, Nr) + 1];
    double max1 = 0, max2 = 0; // разницы для mode 6

    //Поле G0
    for (int i = 0; i <= Nr; i ++) {
        for (int j = 0; j <= x; j ++) { 
            if ((j < Nz_s) || (j > Nz + Nz_s)) { 
                G[i][j] = -1;
            }
            if ((j >= Nz_s) && (j <= Nz + Nz_s)) {
                G[i][j] = 1;
            }
        }
    }

    //поле темпы в начальный момент времени:
    for (int i = 0; i <= Nr; i ++) {
        for (int j = 0; j <= x; j ++) {
            T[i][j] = T0;
        }
    }

    //Заполнение матрицы Источников (из файла)
    string path = "initial/A.txt";
    ifstream Qin;
    Qin.open(path);
    if (!Qin.is_open())
    {
        cout << "Ошибка открытия файла!" << endl;
    }
    else {
        for (int j = Nz; j >= 0; j --) {
            for (int i = 0; i <= Nr; i ++) {
                Qin >> Q[i][j]; 
            }
        }
    }

    //Цикл по времени
    while (time < t_end) {
        time += tau;
        //________________________________________Решаем в направлении z:_____________________________________________________

        for (int i = 0; i <= Nr; i ++) {
            alfa[0] = 1;
            beta[0] = 0;
            for (int j = 0; j <= x; j++) {
                Tn[i][j] = T[i][j]; //сохраняем темпу на предыдущем временом слое
                if ((j >= Nz_s) && (j <= Nz + Nz_s)) {
                    Gn[i][j] = G[i][j]; //сохраняем поле G на предыдущем временом слое
                } 
            }

            //цикл с постусловием
            do {
                //запомним поле T на предыдущем шаге иттерации
                for (int j = 0; j <= x; j++) {
                    Ts[i][j] = T[i][j]; 
                }

                for (int j = 1; j < Nz_s - 1; j ++) {//считаем коэфы в методе прогонки
                    ai = lam_s / pow(hz,2);
                    bi = 2.0 * lam_s / pow(hz,2) + ro_s * c_s / tau;
                    ci = lam_s / pow(hz,2);
                    fi = -ro_s * c_s * T[i][j] / tau;
                    alfa[j] = ai / (bi - ci * alfa[j - 1]);
                    beta[j] = (ci * beta[j - 1] - fi) / (bi - ci * alfa[j - 1]); 
                }

                //прогоночные коэфы для первой точки - границы раздела стелка и ткани
                alfa[Nz_s - 1] = lambda_G(G[i][Nz_s],gamma_G) / (lambda_G(G[i][Nz_s],gamma_G) + lam_s * (1 - alfa[Nz_s - 2]));
                   beta[Nz_s - 1] = (lam_s * beta[Nz_s - 2]) / (lambda_G(G[i][Nz_s],gamma_G) + lam_s * (1 - alfa[Nz_s - 2]));
    
                for (int j = Nz_s; j <= Nz + Nz_s; j ++) {
                    ai = 0.5 * (lambda_G(G[i][j],gamma_G) + lambda_G(G[i][j+1],gamma_G)) / pow(hz,2);
                    ci = 0.5 * (lambda_G(G[i][j-1],gamma_G) + lambda_G(G[i][j],gamma_G)) / pow(hz,2);
                    bi = ai + ci + ro * c / tau;
                    fi = -ro * c * Tn[i][j] / tau - Q[i][j - Nz_s];
                    alfa[j] = ai / (bi - ci * alfa[j-1]);
                    beta[j] = (ci * beta[j - 1] - fi) / (bi - ci * alfa[j - 1]); 
                }
            
                //прогоночные коэфы для второй точки - границы раздела стекла и ткани
                alfa[Nz + Nz_s + 1] = lam_s / (lam_s + lambda_G(G[i][Nz + Nz_s],gamma_G) * (1 - alfa[Nz + Nz_s]));
                beta[Nz + Nz_s + 1] = (lambda_G(G[i][Nz + Nz_s],gamma_G) * beta[Nz + Nz_s]) /
                                                         (lam_s + lambda_G(G[i][Nz + Nz_s],gamma_G) * (1 - alfa[Nz + Nz_s]));
                
                for (int j = Nz + Nz_s + 2; j < x; j ++) {
                    ai = lam_s / pow(hz,2);
                    bi = 2.0 * lam_s/pow(hz,2) + ro_s * c_s / tau;
                    ci = lam_s / pow(hz,2);
                    fi = -ro_s * c_s * T[i][j] / tau;
                    alfa[j] = ai / (bi - ci * alfa[j - 1]);
                    beta[j] = (ci * beta[j - 1] - fi) / (bi - ci * alfa[j - 1]); 
                }

                //гр. усл 3 (стекло)
                T[i][x] = (lam_s * beta[x - 1] + hz * kap_s * Te) / (hz * kap_s + lam_s * (1 - alfa[x - 1]));
                
                // Определяем поле температуры и G на промежуточном временном слое(прошли по оси z):
                for (int j = x - 1; j >= 0; j --) {
                    T[i][j] = alfa[j] * T[i][j + 1] + beta[j];
                    if ((j >= Nz_s) && (j <= Nz + Nz_s)) {
                        if (G[i][j] < 0.01) {
                            G[i][j] = 0;
                        }
                        else { 
                            G[i][j] = Gn[i][j] * (1 - K(T[i][j]) * tau);//определяем поле G
                        }
                    }
                }

                //определяем максимумы для метода простой иттерации
                max1 = 0;
                max2 = 0;
                for (int j = 0; j <= x; j++) {
                    if (max1 < abs (T[i][j] - Ts[i][j])) {
                        max1 = abs (T[i][j] - Ts[i][j]);
                    }
                }
                for (int j = 0; j <= x; j++) {
                    if (max2 < abs (T[i][j])) {
                        max2 = abs (T[i][j]);
                    }
                }
            }   
            while ((max1 / max2) > eps);
        }

        /*  //Вывод в файл на промежуточном слое (для дебага)
        f << " time = " << time << "поле Темпы после прохода по z " << endl;
        g << " time = " << time << "поле G после прохода по z" << endl;
        for (int j = x; j >= 0; j --) {
            for (int i = 0; i <= Nr; i++) {
                f << T[i][j] << "\t";
                g << G[i][j] << "\t";
            }
            f << endl;
            g << endl;
        }   
        f << endl;
        g << endl;*/
        
        //_________________________________________Решаем в направлении r______________________________________________
        
        time += tau;//добавляем время, т.к. находимся на промежуточном слое по времени
        
        for (int j = 0; j <= x; j ++) {
            // запомним поле T и G на предыдущем временном слое
            for (int i = 0; i <= Nr; i++) {
                Tn[i][j] = T[i][j]; 
                if ((j >= Nz_s) && (j <= Nz + Nz_s)) {
                    Gn[i][j] = G[i][j]; 
                }
            }

            if ((j >= Nz_s) && (j <= Nz_s + Nz)) { 
                alfa[0] = 1.0;
                beta[0] = pow(hr,2) * Q[0][j - Nz_s] / lambda_G(G[0][j],gamma_G);
            }

            if ((j < Nz_s) || (j > Nz_s + Nz)) { 
                alfa[0] = 1;
                beta[0] = 0; //поток равен нулю, т.к середина цилиндра
            }    

            //цикл с постусловием
            do {
                //запомним поле T на предыдущем шаге иттерации
                for (int i = 0; i <= Nr; i++) {
                    Ts[i][j] = T[i][j]; 
                }

                for (int i = 1; i < Nr; i ++) {

                    if ((j >= Nz_s) && (j <= Nz_s + Nz)) { 
                        ai = 0.5 * (lambda_G(G[i][j],gamma_G) + lambda_G(G[i+1][j],gamma_G)) / pow(hz,2);
                        ci = 0.5 * (lambda_G(G[i-1][j],gamma_G) + lambda_G(G[i][j],gamma_G)) / pow(hz,2);
                        bi = ai + ci + ro * c / tau;
                        fi = -ro * c * Tn[i][j] / tau - Q[i][j - Nz_s];
                        alfa[i] = ai / (bi - ci * alfa[i - 1]);
                        beta[i] = (ci * beta[i - 1] - fi) / (bi - ci * alfa[i - 1]); 
                    }

                    if ((j < Nz_s) || (j > Nz_s + Nz)) {
                        ai = lam_s * (2 * i + 1) / (2 * i * pow(hr,2));
                        bi = 2.0 * lam_s / pow(hr,2) + ro_s * c_s / tau;
                        ci = lam_s * (2 * i - 1)/(2 * i * pow(hr,2));
                        fi = -ro_s * c_s * T[i][j] / tau;
                        alfa[i] = ai / (bi - ci * alfa[i - 1]);
                        beta[i] = (ci * beta[i - 1] - fi) / (bi - ci * alfa[i - 1]);
                    }
                }

                if ((j >= Nz_s) && (j <= Nz_s + Nz)) { 
                    // гр усл. 3
                    T[Nr][j] = (lambda_G(G[Nr][j], gamma_G) * beta[Nr - 1] + hr * kap_b * Te + 0.25 * Q[Nr][j] * pow(hz,2)) / 
                                                                    (hr * kap_b + lambda_G(G[Nr][j], gamma_G) * (1 - alfa[Nr - 1]));    
                }

                if ((j < Nz_s) || (j > Nz_s + Nz)) {
                    T[Nr][j] = (lam_s * beta[Nr - 1] + hr * kap_s * Te) / (hr * kap_s + lam_s * (1 - alfa[Nr - 1]));    
                }
                
                if ((j >= Nz_s) && (j <= Nz + Nz_s)) {
                    G[Nr][j] = Gn[Nr][j] * (1 - K(T[Nr][j])*tau);
                    if (G[Nr][j] < 0.01) {
                        G[Nr][j] = 0;
                    }
                }

                // Определяем поле температуры и G на промежуточном временном слое(прошли по оси r):
                for (int i = Nr - 1; i >= 0; i --) {
                    T[i][j] = alfa[i] * T[i + 1][j] + beta[i];
                    
                    if ((j >= Nz_s) && (j <= Nz + Nz_s)) {
                        if (G[i][j] < 0.01) {
                            G[i][j] = 0;
                        }
                        else { 
                            G[i][j] = Gn[i][j] * (1 - K(T[i][j]) * tau);//определяем поле G
                        }
                    }
                }

                //определяем максимумы
                max1 = 0;
                max2 = 0;
                for (int i = 0; i <= Nr; i ++) {
                    if (max1 < abs (T[i][j] - Ts[i][j])) {
                        max1 = abs (T[i][j] - Ts[i][j]);
                    }
                }
                for (int i = 0; i <= Nr; i++) {
                    if (max2 < abs (T[i][j])) {
                        max2 = abs (T[i][j]);
                    }
                }
            }   
            while ((max1 / max2) > eps);
        }

    /*  //Вывод в файл на промежуточном слое (для дебага)
        f << " time = " << time << "поле Темпы после прохода по r " << endl;
        g << " time = " << time << "поле G после прохода по r" << endl;
        for (int j = x; j >= 0; j --) {
            for (int i = 0; i <= Nr; i++) {
                f << T[i][j] << "\t";
                g << G[i][j] << "\t";
            }
            f << endl;
            g << endl;
        }   
        f << endl;
        g << endl;
    */
    }//конец цикла по времени

    // Вывод в файл
    for (int j = x; j >= 0; j --) {
        for (int i = 0; i <= Nr; i++) {
            f << T[i][j] << "\t";
        }
     f << endl;
    }
    f << endl;

    for (int j = x; j >= 0; j --) {
        for (int i = 0; i <= Nr; i++) {
            g << G[i][j] << "\t";
        }
        g << endl;
    }
    g << endl;
    
    f.close();
    g.close();
    return 0;
}