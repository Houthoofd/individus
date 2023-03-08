#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

double GenerateNumber(int decimalPrecision, double min, double max) {
    std::random_device rd; // Objet pour générer des nombres aléatoires
    std::mt19937 gen(rd()); // Générateur de nombres aléatoires
    std::uniform_real_distribution<double> dis(min, max); // Distribution uniforme entre min et max
    double r = dis(gen);
    double factor = pow(10, decimalPrecision);
    double nombreArrondi = round(r * factor) / factor;
    return nombreArrondi;
}

class Individu {
public:
    int id;
    double score;
    double attack;
    double defense;
    double counter_attack;
    double agility;

    Individu(int index, double attack, double defense, double counter_attack, double agility) {
        this->id = index;
        this->attack = attack;
        this->defense = defense;
        this->counter_attack = counter_attack;
        this->agility = agility;
        this->score = 0; // Initialise le score à 0
    }
};

class Generation {
public:
    int nbre_generation = 10;
    int nbre_individus = 10;
    double score_generation = 0;
    vector<Individu> individus; // Stocke les individus de la génération

    Generation() {
      // Initialise les individus de la génération
      vector<vector<double>> attributs_individus; // Vecteur stockant les attributs des individus
      for (int j = 0; j < nbre_individus; j++) {
        double attack, defense, counter_attack, agility;
        // Génère aléatoirement les attributs de l'individu
        do {
          attack = GenerateNumber(3, 0, 1);
          defense = GenerateNumber(3, 0, 1);
          counter_attack = GenerateNumber(3, 0, 1);
          agility = GenerateNumber(3, 0, 1);
        } while (find(attributs_individus.begin(), attributs_individus.end(), vector<double>{attack, defense, counter_attack, agility}) != attributs_individus.end()); // Vérifie si les attributs ne sont pas déjà présents dans le vecteur
          attributs_individus.push_back(vector<double>{attack, defense, counter_attack, agility}); // Ajoute les attributs de l'individu dans le vecteur
          this->individus.push_back(Individu(j, attack, defense, counter_attack, agility));
      }
    }

    // Met à jour le score de chaque individu de la génération
    void EvaluerIndividus() {
        for (int j = 0; j < nbre_individus; j++) {
            this->individus[j].score = this->individus[j].attack + this->individus[j].defense + this->individus[j].counter_attack + this->individus[j].agility;
        }
    }

    // Retourne le meilleur individu de la génération
    Individu GetMeilleurIndividu() {
        Individu meilleurIndividu = this->individus[0];
        for (int j = 1; j < nbre_individus; j++) {
            if (this->individus[j].score > meilleurIndividu.score) {
                meilleurIndividu = this->individus[j];
            }
        }
        return meilleurIndividu;
    }

    // Retourne les deux meilleurs individus de la génération
    vector<Individu> GetMeilleursIndividus() {
    // Tri les individus par ordre décroissant de score
    sort(this->individus.begin(), this->individus.end(), [](Individu a, Individu b) {
        return a.score > b.score;
    });

    // Retourne les deux premiers éléments de la liste triée
    vector<Individu> meilleursIndividus;
    meilleursIndividus.push_back(this->individus[0]);
    meilleursIndividus.push_back(this->individus[1]);
    return meilleursIndividus;
  }

  // Croise deux individus parents pour créer deux nouveaux individus enfants
  void CroiserIndividus(Individu parent1, Individu parent2, Individu& enfant1, Individu& enfant2) {
      // Tire un nombre aléatoire pour chaque caractéristique et échange si le nombre est inférieur à 0.5
      double random = GenerateNumber(3, 0, 1);
      if (random < 0.5) {
          enfant1.attack = parent2.attack;
          enfant2.attack = parent1.attack;
      }
      random = GenerateNumber(3, 0, 1);
      if (random < 0.5) {
          enfant1.defense = parent2.defense;
          enfant2.defense = parent1.defense;
      }
      random = GenerateNumber(3, 0, 1);
      if (random < 0.5) {
          enfant1.counter_attack = parent2.counter_attack;
          enfant2.counter_attack = parent1.counter_attack;
      }
      random = GenerateNumber(3, 0, 1);
      if (random < 0.5) {
          enfant1.agility = parent2.agility;
          enfant2.agility = parent1.agility;
      }
  }

  // Génère une nouvelle génération en croisant les individus de la génération précédente
  Generation GenererNouvelleGeneration(Generation generationPrecedente) {
      Generation nouvelleGeneration;
      for (int i = 0; i < generationPrecedente.nbre_individus; i += 2) {
          // Sélectionne deux individus parents aléatoirement
          int indexParent1 = rand() % generationPrecedente.nbre_individus;
          int indexParent2 = rand() % generationPrecedente.nbre_individus;
          Individu parent1 = generationPrecedente.individus[indexParent1];
          Individu parent2 = generationPrecedente.individus[indexParent2];

          // Croise les deux parents pour créer deux nouveaux enfants
          Individu enfant1(parent1.id + generationPrecedente.nbre_individus, parent1.attack, parent1.defense, parent1.counter_attack, parent1.agility);
          Individu enfant2(parent2.id + generationPrecedente.nbre_individus, parent2.attack, parent2.defense, parent2.counter_attack, parent2.agility);
      }
      EvaluerIndividus();
      return nouvelleGeneration;
  }

};

int main() {
    srand(time(NULL)); // Initialise la graine du générateur de nombres aléatoires

    vector<Generation> generations; // Stocke les générations

    int nbre_generations = 100;
    for (int i = 0; i < nbre_generations; i++) {
      generations.push_back(Generation());
      generations[i].EvaluerIndividus(); // Évalue les individus de la génération

      vector<Individu> meilleursIndividus = generations[i].GetMeilleursIndividus();
      cout << "---------------------------------------------------------" << endl;
      cout << "Meilleurs individus de la generation " << i+1 << ":" << endl;
      for (int j = 0; j < meilleursIndividus.size(); j++) {
          Individu meilleurIndividu = meilleursIndividus[j];
          cout << "id=" << meilleurIndividu.id << ", score=" << meilleurIndividu.score << ", attack=" << meilleurIndividu.attack << ", defense=" << meilleurIndividu.defense << ", counter_attack=" << meilleurIndividu.counter_attack << ", agility=" << meilleurIndividu.agility << endl;
      }
      cout << "---------------------------------------------------------" << endl;
  }
  return 0;
}
