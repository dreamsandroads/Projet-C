#include<iostream>
#include<cmath>
#include<array>
#include<vector>
#include<algorithm> 
#include<iterator> 
#include "Projet_BS_EDP_class.h" // Fichier contenant la classe créée pour pricer un call/put : méthode implicite et méthode implicite_Cranston.
#include "Projet_BS_EDP_fonctions.h" // Fichier contenant les fonctions nécessaires pour calculer les sensibilités : delta, gamma, theta, vega et rho.

using namespace std;


int main()
{
        
    // Création d'un objet de la classe Pricing : 

    Pricing obj;

    // On peut choisir les paramètre pour le pricing de l'option call ou put (européenne) : 
    cout << "Paramètres utilisés : " << endl;
    cout << ""<< endl;
    cout << "Prix d'exercice : " << 0.5 << endl;
    cout << "Maturité maximale (en année): " << 1.0 << endl;
    cout << "Taux d'intérêt sans risque : " << 0.05 << endl;
    cout << "Volatilité de l'actif sous-jacent (constante) : " << 0.2 <<endl;
    cout << "Prix maximal de l'actif sous-jacent : " << 1.0 << endl;
    cout << "Pas temporel (maturité) : " << 20 << endl;
    cout << "Pas spatial (prix actif sous-jacent) : " << 20 << endl;
    cout << ""<< endl;


    // Pricing d'un call par la méthode implicite : 

    // On récupère l'évolution des prix de l'actif sous-jacent, de la maturité et du prix du call.
    // On stocke ces trois vecteurs dans la variable resultat_call_imp :  

    auto retc = obj.compute_implicite_call(0.5, 1.0, 0.05, 0.2, 1, 20, 20); // On utilise notre fonction de la class Pricing. 
    vector<vector<double>> resultat_call_imp{{retc.first, retc.second, retc.third}};

    // Affichage des résultats : (une ligne : prix de l'actif au moment de l'évaluation du prix de l'option, maturité de l'option, prix de l'option)

    cout << "Prix de l'actif sous-jacent | Maturité | Prix du call :" << endl;
    for (int i = 0; i < resultat_call_imp[1].size(); i++) {

            cout << resultat_call_imp[0][i] << " | " << resultat_call_imp[1][i] << " | " << resultat_call_imp[2][i] << endl;}



    // Pricing d'un put par la méthode implicite :  (On suit le même schéma que précédemment)

    auto retp = obj.compute_implicite_put(0.5, 1.0, 0.05, 0.2, 1, 20, 20); 
    vector<vector<double>> resultat_put_imp{{retp.first, retp.second, retp.third}};

    cout << "" << endl;
    cout << "Prix de l'actif sous-jacent | Maturité | Prix du put :" << endl;
    for (int i = 0; i < resultat_put_imp[1].size(); i++) {

            cout << resultat_put_imp[0][i] << " | " << resultat_put_imp[1][i] << " | " << resultat_put_imp[2][i] << endl;}


    ////////////////
    // On peut faire la même chose pour la méthode implicite_Cranston : 
    ///////////////

    // Implémentation des sensibilités avec l'équation de Black-Sholes-Merton : on utilise les fonctions du fichier : Projet_BS_EDP_fonctions.h

    // On donne les paramètres de notre option : 

    double S = 100.0;  // Prix de l'actif.
    double K = 100.0;  // Prix d'exercice.
    double r = 0.05;   // Taux d'intérêt sans risque.
    double v = 0.2;    // Volatilité de l'actif sous-jacent. 
    double T = 1.0;    // Maturité.           

    // Pour un call : 
    cout << ""<< endl;
    cout << "Sensibilité d'un call de paramètres : " << "S = " << S << "; K = " << K << "; r = " << r << "; v = " << v << "; T = " << T << " :" << endl;
    cout << "Sensibilité delta : " << delta_call(S, K,  r,  v,  T) << endl;
    cout << "Sensibilité gamma : " << gamma_call(S, K,  r,  v,  T) << endl;
    cout << "Sensibilité theta : " << theta_call(S, K,  r,  v,  T) << endl;
    cout << "Sensibilité vega : " << vega_call(S, K,  r,  v,  T) << endl;
    cout << "Sensibilité rho : " << rho_call(S, K,  r,  v,  T) << endl;

    // Pour un put : 

    cout << ""<< endl;
    cout << "Sensibilité d'un put de paramètres : " << "S = " << S << "; K = " << K << "; r = " << r << "; v = " << v << "; T = " << T << " :" << endl;
    cout << "Sensibilité delta : " << delta_put(S, K,  r,  v,  T) << endl;
    cout << "Sensibilité gamma : " << gamma_put(S, K,  r,  v,  T) << endl;
    cout << "Sensibilité theta : " << theta_put(S, K,  r,  v,  T) << endl;
    cout << "Sensibilité vega : " << vega_put(S, K,  r,  v,  T) << endl;
    cout << "Sensibilité rho : " << rho_put(S, K,  r,  v,  T) << endl;

    return 0;
}