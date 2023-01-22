#include<iostream>
#include<cmath>
#include<array>
#include<vector>
#include<algorithm> 
#include<iterator>  

using namespace std;

 

// Création de la class permettant de pricer une option européenne : call et put / méthode implicite et méthode implicite_Cranston. 
 

class Pricing{

    // Création des paramètres : 


    
    public:


    // Création d'un nouveau type pour pouvoir renvoyer simultanément 3 vecteurs : prix de l'actif sous-jacent, maturité, prix du call ou put. 


    struct IntPair {
    vector<double> first, second, third;
    } 
    typedef IntPair;


   // Création de la fonction de class permettant de pricer un call par la méthode implicite : 

    IntPair compute_implicite_call(double K, double T, double r, double sigma, double Smax, int Nt, int Ms){
            
            double dt = T/(double)(Nt); // Définition du pas temporel. 
            double dx = Smax/(double)(Ms); // Définition du pas spacial. 

            IntPair ret;

            double previous[Ms];
            double xvalues[Ms];
            for(int x = 0; x < Ms; x++){
                xvalues[x] = x*dx;
                previous[x] = max(xvalues[x]-K,0.0);
            }

            double result[Ms];
            vector<double> underlying;
            vector<double> maturity;
            vector<double> call_price;
            double prev_t = 0;

            for(int ti = 0; ti < Nt; ti++){  // On calcule le prix du call pour chaque maturité. 
                double cur_t = prev_t + dt;
                result[0] = 0;
                result[Ms-1] = xvalues[Ms-1]-K*exp(-r*dt*(T-prev_t));

                for(int xi = 1; xi < Ms-1; xi++){   // On calcule le prix du call pour chaque prix de l'actif sous-jacent. 
                    double coef_a = 0.5*xi*xi*sigma*sigma*dt;
                    double coef_b = 0.5*xi*xi*sigma*sigma*dt-r*xi*dt;
                    double coef_c = 1+r*xi*dt-xi*xi*sigma*sigma*dt-r*dt;
                    result[xi] = coef_a*previous[xi+1]+coef_b*previous[xi-1]+coef_c*previous[xi];
                }

                for (int xi = 1; xi < Ms; xi++) {
                    underlying.push_back(xvalues[xi]); // Prix de l'actif sous-jacent. 
                    maturity.push_back(prev_t);  // Maturité de l'option. 
                    call_price.push_back(result[xi]); // Prix du call (option considérée ici). 

                    previous[xi] = result[xi];
                } 
            
                prev_t = cur_t;

            }

            ret.first = underlying;
            ret.second = maturity;
            ret.third = call_price;
            return ret; // Variable contenant les 3 vecteurs : 1. Prix de l'actif sous-jacent, 2. Maturité du call, 3. Prix du call. 
        }

    // Création de la fonction de class permettant de pricer un put par la méthode implicite : 

        IntPair compute_implicite_put(double K, double T, double r, double sigma, double Smax, int Nt, int Ms){
            
            double dt = T/(double)(Nt); // Définition du pas temporel. 
            double dx = Smax/(double)(Ms); // Définition du pas spacial. 

            IntPair ret;

            double previous[Ms];
            double xvalues[Ms];
            for(int x = 0; x < Ms; x++){
                xvalues[x] = x*dx;
                previous[x] = max(K - xvalues[x],0.0);
            }

            double result[Ms];
            vector<double> underlying;
            vector<double> maturity;
            vector<double> put_price;
            double prev_t = 0;

            for(int ti = 0; ti < Nt; ti++){ // On calcule le prix du put pour chaque maturité. 
                double cur_t = prev_t + dt;
                result[0] = 0;
                result[Ms-1] = K*exp(-r*dt*(T-prev_t)) - xvalues[Ms-1];

                for(int xi = 1; xi < Ms-1; xi++){ // On calcule le prix du put pour chaque prix de l'actif sous-jacent. 
                    double coef_a = 0.5*xi*xi*sigma*sigma*dt;
                    double coef_b = 0.5*xi*xi*sigma*sigma*dt-r*xi*dt;
                    double coef_c = 1+r*xi*dt-xi*xi*sigma*sigma*dt-r*dt;
                    result[xi] = coef_a*previous[xi+1]+coef_b*previous[xi-1]+coef_c*previous[xi];
                }

                for (int xi = 0; xi < Ms; xi++) {
                    underlying.push_back(xvalues[xi]);  // Prix de l'actif sous-jacent.
                    maturity.push_back(prev_t); // Maturité de l'option. 
                    put_price.push_back(result[xi]); // Prix du put (option considérée ici). 

                    previous[xi] = result[xi];
                }

                prev_t = cur_t;

            }

            ret.first = underlying;
            ret.second = maturity;
            ret.third = put_price;
            return ret;   // Variable contenant les 3 vecteurs : 1. Prix de l'actif sous-jacent, 2. Maturité du put, 3. Prix du put. 
        }

    // Création de la fonction de class permettant de pricer un call par la méthode implicite_Crank_Nicolson :
    // Différence dans le code avec la méthode implicite : les coefficients a, b et c.  

        IntPair compute_implicite_Crank_Nicolson_call(double K, double T, double r, double sigma, double Smax, int Nt, int Ms){
            
            double dt = T/(double)(Nt); // Définition du pas temporel. 
            double dx = Smax/(double)(Ms); // Définition du pas spacial. 

            IntPair ret;

            double previous[Ms];
            double xvalues[Ms];
            for(int x = 0; x < Ms; x++){
                xvalues[x] = x*dx;
                previous[x] = max(xvalues[x]-K,0.0);
            }

            double result[Ms];
            vector<double> underlying;
            vector<double> maturity;
            vector<double> call_price;
            double prev_t = 0;

            for(int ti = 0; ti < Nt; ti++){  // On calcule le prix du call pour chaque maturité. 
                double cur_t = prev_t + dt;
                result[0] = 0;
                result[Ms-1] = xvalues[Ms-1]-K*exp(-r*dt*(T-prev_t));

                for(int xi = 1; xi < Ms-1; xi++){   // On calcule le prix du call pour chaque prix de l'actif sous-jacent. 
                    double coef_a = 0.5*xi*xi*dt*sigma*sigma+0.5*xi*dt;
                    double coef_b = 0.5*xi*xi*dt*sigma*sigma-0.5*xi*dt;
                    double coef_c = 1-xi*xi*sigma*sigma*dt-r*dt;
                    result[xi] = coef_a*previous[xi+1]+coef_b*previous[xi-1]+coef_c*previous[xi];
                }

                for (int xi = 1; xi < Ms; xi++) {
                    underlying.push_back(xvalues[xi]); // Prix de l'actif sous-jacent. 
                    maturity.push_back(prev_t);  // Maturité de l'option. 
                    call_price.push_back(result[xi]); // Prix du call (option considérée ici). 

                    previous[xi] = result[xi];
                } 
            
                prev_t = cur_t;

            }

            ret.first = underlying;
            ret.second = maturity;
            ret.third = call_price;
            return ret; // Variable contenant les 3 vecteurs : 1. Prix de l'actif sous-jacent, 2. Maturité du call, 3. Prix du call. 
        }


    // Création de la fonction de class permettant de pricer un put par la méthode implicite_Crank_Nicolson : 
    // Différence dans le code avec la méthode implicite : les coéfficients a, b et c. 

         IntPair compute_implicite_Crank_Nicolson_put(double K, double T, double r, double sigma, double Smax, int Nt, int Ms){
            
            double dt = T/(double)(Nt); // Définition du pas temporel. 
            double dx = Smax/(double)(Ms); // Définition du pas spacial. 

            IntPair ret;

            double previous[Ms];
            double xvalues[Ms];
            for(int x = 0; x < Ms; x++){
                xvalues[x] = x*dx;
                previous[x] = max(K - xvalues[x],0.0);
            }

            double result[Ms];
            vector<double> underlying;
            vector<double> maturity;
            vector<double> put_price;
            double prev_t = 0;

            for(int ti = 0; ti < Nt; ti++){ // On calcule le prix du put pour chaque maturité. 
                double cur_t = prev_t + dt;
                result[0] = 0;
                result[Ms-1] = K*exp(-r*dt*(T-prev_t)) - xvalues[Ms-1];

                for(int xi = 1; xi < Ms-1; xi++){ // On calcule le prix du put pour chaque prix de l'actif sous-jacent. 
                    double coef_a = 0.5*xi*xi*dt*sigma*sigma+0.5*xi*dt;
                    double coef_b = 0.5*xi*xi*dt*sigma*sigma-0.5*xi*dt;
                    double coef_c = 1-xi*xi*sigma*sigma*dt-r*dt;
                    result[xi] = coef_a*previous[xi+1]+coef_b*previous[xi-1]+coef_c*previous[xi];
                }

                for (int xi = 0; xi < Ms; xi++) {
                    underlying.push_back(xvalues[xi]);  // Prix de l'actif sous-jacent.
                    maturity.push_back(prev_t); // Maturité de l'option. 
                    put_price.push_back(result[xi]); // Prix du put (option considérée ici). 

                    previous[xi] = result[xi];
                }

                prev_t = cur_t;

            }

            ret.first = underlying;
            ret.second = maturity;
            ret.third = put_price;
            return ret;   // Variable contenant les 3 vecteurs : 1. Prix de l'actif sous-jacent, 2. Maturité du put, 3. Prix du put. 
        }



    };