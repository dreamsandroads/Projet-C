#include<iostream>
#include<cmath>
#include<array>
#include<vector>
#include<algorithm> 
#include<iterator> 

using namespace std;




// Les fonctions suivantes permettent de calculer les sensibilités d'un call et d'un put : delta, gamma, theta, vega, rho. 
// Nous avons directement utilisé la formule explicite du modèle de Black-Sholes-Merton : résoudre une EDP dans ce cas n'était pas le plus adapté. 

// Fonction densité d'une loi normale centrée réduite. 

double norm_d(const double& c) {
    return (1.0/(pow(2*M_PI,0.5)))*exp(-0.5*c*c);
}


// Approximation de la fonction de répartition d'une loi normale centrée réduite :
// Il s'agit d'une fonction récursive :  

double norm_rep(const double& c) {
    double p = 1.0/(1.0 + 0.2316419*c);
    double p_sum = p*(0.319381530 + p*(-0.356563782 + p*(1.781477937 + p*(-1.821255978 + 1.330274429*p))));

    if (c >= 0.0) {
        return (1.0 - (1.0/(pow(2*M_PI,0.5)))*exp(-0.5*c*c) * p_sum);
    } else {
        return 1.0 - norm_rep(-c);
    }
}

// Implémentation de d1 et d2 : spécifier i = 1 ou 2 pour choisir entre calculer d1 et d2. 

double d_j(const int& i, const double& S, const double& K, const double& r, const double& v, const double& T) {
    return (log(S/K) + (r + (pow(-1,i-1))*0.5*v*v)*T)/(v*(pow(T,0.5)));
}

// Sensibilité Delta d'un call : 

double delta_call(double S, double K, double r, double v, double T){
    return norm_rep(d_j(1, S, K, r, v, T));
}

// Sensibilité Gamma d'un call : 

double gamma_call(double S, double K, double r, double v, double T){
    return norm_d(d_j(1, S, K, r, v, T))/(S*v*pow(T,0.5));
}

// Sensibilité Theta d'un call :

double theta_call(double S, double K, double r, double v, double T){
    return -(norm_d(d_j(1, S, K, r, v, T))*S*v)/2*pow(T,0.5) - r*K*exp(-r*T)*norm_rep(d_j(2, S, K, r, v, T));
}

// Sensibilité Vega d'un call : 

double vega_call(double S, double K, double r, double v, double T){
    return norm_d(d_j(1, S, K, r, v, T))*S*pow(T,0.5);
}

// Sensibilité Rho d'un call  : 

double rho_call(double S, double K, double r, double v, double T){
    return norm_rep(d_j(2, S, K, r, v, T))*T*K*exp(-r*T);
}

// Sensibilité Delta d'un put : 

double delta_put(double S, double K, double r, double v, double T){
    return norm_rep(d_j(1, S, K, r, v, T)) - 1 ;
}

// Sensibilité Gamma d'un put : 

double gamma_put(double S, double K, double r, double v, double T){
    return norm_d(d_j(1, S, K, r, v, T))/(S*v*pow(T,0.5));
}

// Sensibilité Theta d'un put :

double theta_put(double S, double K, double r, double v, double T){
    return -(norm_d(d_j(1, S, K, r, v, T))*S*v)/2*pow(T,0.5) + r*K*exp(-r*T)*norm_rep(-d_j(2, S, K, r, v, T));
}

// Sensibilité Vega d'un put : 

double vega_put(double S, double K, double r, double v, double T){
    return norm_d(d_j(1, S, K, r, v, T))*S*pow(T,0.5);
}

// Sensibilité Rho d'un put  : 

double rho_put(double S, double K, double r, double v, double T){
    return -norm_rep(-d_j(2, S, K, r, v, T))*T*K*exp(-r*T);
}